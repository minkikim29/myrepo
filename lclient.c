/*
 * Name: Emma Mirică
 * Project: TWAMP Protocol
 * Class: OSS
 * Email: emma.mirica@cti.pub.ro
 *
 * Source: server.c
 * Note: contains the TWAMP server implementation
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/prctl.h>

#if defined(CONFIG_DASAN_SWITCH)
#include "lib.h"
#include "dasan/ds_lib.h"
#include "nsm_message.h"
#include "nsm_client.h" 
#if defined(HAVE_TWAMPLIGHTD_SENDER)
#include "twamplightd.h"
#include "sle_twamplight.h"
#include "twamplight.h"

/* TWAMP timestamp is NTP time (RFC1305).
 * Should be in network byte order!      */

#define PORTBASE_SEND    30000
#define PORTBASE_RECV    20000
#define TEST_SESSIONS    1
#define TEST_MESSAGES    0
#define TEST_INTERVAL    0
#define TEST_TIMEOUT     3
#define TIMEOUT          2 /* SECONDS */

#define TEST_CONTINUOUS_LOSS_THRES      0
#define TEST_MINUTE_LOSS_AVG     	0
#define TEST_MINUTE_LOSS_THRES     	0

#define PRIu64 __PRI64_PREFIX "u"

struct twamp_test_info {
    int testfd;
    int testport;
    uint16_t port;
};

static __thread int g_set_signal = 0;

/* debug */
//#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define TWAMP_PRINTD printd
#define TWAMP_FPRINTF fprintf
#else
#define TWAMP_PRINTF
#define TWAMP_FPRINTF
#endif

extern pal_pthread_key_t g_twamplight_sender_thread_key;
/* This is a rountine to make a process be a daemon. This routine is quoted from ZebOS library 
 * 
 * Return values:
 * -1: failed to be a daemon
 *  0: success
 */
static int daemonize(void)
{
	pid_t pid;
	int fd;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return -1;
	}
	else if (pid != 0)
	{
		exit(0);
	}

	pid = setsid();
	if (pid < -1)
	{
		perror("setsid");
		return -1;
	}

	chdir("/");

	fd = open("/dev/null", O_RDWR, 0);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		if (fd > 2)
			close(fd);
	}

	umask(0027);

	return 0;
}

/* Prints the help of the TWAMP server */
static void usage(char *progname)
{
    TWAMP_FPRINTF(stderr, "Usage: %s [options]\n", progname);
    TWAMP_FPRINTF(stderr, "\nWhere \"options\" are:\n\n");

    TWAMP_FPRINTF(stderr,
            "	-a authmode		Default is Unauthenticated\n"
            "	-p port_min		Port range for Test receivers based on port_min\n"
            "	-h         		Print this help message and exits\n");
    return;
}

/*
 * Configure required stuff for this thread
 */
int
twamp_lclient_thread_config(struct twamplight_sender_vrf *twamp_vrf)
{
	char thread_name[256];

	if(twamp_vrf == NULL)
		return -1;
#ifdef HAVE_VRF_NS
	/* Bind thread net name space to VRF */
	if(twamp_vrf->fib_id != 0)
	{
		if (pal_set_ns (twamp_vrf->fib_id) != 0)
			return -2;
	}
#endif

	/* Set thread name */
	pal_snprintf(thread_name, sizeof(thread_name)-1, "twamp-light-sender_%s_%d", twamp_vrf->vrf_name, twamp_vrf->session_id);
	prctl(PR_SET_NAME, (unsigned long)thread_name, 0, 0, 0);

	/* Set Thread local storage */
	pthread_setspecific(g_twamplight_sender_thread_key, twamp_vrf);
	return 0;
}

void print_metrics2(int session_id, uint32_t msg_count, uint16_t port, const TWAMPTimestamp recv_resp_time, const ReflectorUPacket *pack) {

#if !defined(CONFIG_DASAN_COMMON)
    /* Get Time of the received TWAMP-Test response message */
    TWAMPTimestamp recv_resp_time = get_timestamp();
#endif

    /* Print different metrics */

#ifdef DEBUG_ENABLE
    /* Compute round-trip */
    TWAMP_PRINTD( "Round-trip time for TWAMP-Test session %d packet %d for port %hd is %lu [usec]\n",
	    session_id, msg_count, port, get_time_difference32(&recv_resp_time, &pack->sender_time));
    TWAMP_PRINTD( "Receive time - Send time for TWAMP-Test"
	    " Session %d packet %d for port %d is %lu [usec]\n", session_id, msg_count, port,
	    get_time_difference32(&pack->receive_time, &pack->sender_time));
    TWAMP_PRINTD( "Reflect time - Send time for TWAMP-Test"
	    " Session %d packet %d for port %d is %lu [usec]\n", session_id, msg_count, port,
	    get_time_difference32(&pack->time, &pack->sender_time));
#endif

}

static uint32_t twamplight_get_timestamp_usec(void)
{
    struct timeval tv;        
    gettimeofday(&tv, NULL);  

    return tv.tv_sec * 1000000 + tv.tv_usec;
}

static void
twamplight_sender_srcip_check_update(struct twamplight_sender_vrf *twamplight_sender_vrf, int valid)
{
    twamplight_sender_vrf->srcip_valid = valid;
}
#ifdef ISIS_VARIATION
static void
twamplight_sender_history_update(struct twamplight_sender_vrf *twamplight_sender_vrf, uint32_t nSent, uint32_t notRecvd, uint32_t ml_over_percent, uint32_t cl_loss_count, char *vrfname, uint32_t avg_var, uint32_t minrtt, uint32_t maxrtt, uint32_t avgrtt)
{
    twamplight_sender_vrf->sent = nSent;
    twamplight_sender_vrf->notRecvd = notRecvd;

#ifdef ISIS_VARIATION
    twamplight_sender_vrf->avg_var = avg_var;
#endif

    twamplight_sender_vrf->ml_over_percent = ml_over_percent;
    twamplight_sender_vrf->cl_loss_count = cl_loss_count;

    twamplight_sender_vrf->minrtt = minrtt;
    twamplight_sender_vrf->maxrtt = maxrtt;
    twamplight_sender_vrf->avgrtt = avgrtt;
}
#else
static void
twamplight_sender_history_update(struct twamplight_sender_vrf *twamplight_sender_vrf, uint32_t nSent, uint32_t notRecvd, uint32_t ml_over_percent, uint32_t cl_loss_count, char *vrfname, uint32_t minrtt, uint32_t maxrtt, uint32_t avgrtt)
{
    twamplight_sender_vrf->sent = nSent;
    twamplight_sender_vrf->notRecvd = notRecvd;

    twamplight_sender_vrf->ml_over_percent = ml_over_percent;
    twamplight_sender_vrf->cl_loss_count = cl_loss_count;

    twamplight_sender_vrf->minrtt = minrtt;
    twamplight_sender_vrf->maxrtt = maxrtt;
    twamplight_sender_vrf->avgrtt = avgrtt;
}
#endif
static void
twamplight_sender_rtt_history_update(struct twamplight_sender_vrf *info, uint32_t minrtt, uint32_t maxrtt, uint32_t avgrtt)
{

    int count = 0;
    twamplight_sender_rtt_history_t *new = NULL, *find = NULL;

    if(info == NULL)
	return;

    if(info->rtt_history_tree == NULL)
	return;

    count = avl_GetCount(info->rtt_history_tree);
    if(count >= 10)
    {
	find = avl_First(info->rtt_history_tree, NULL);
	if(find)
	{
	    avl_Delete(info->rtt_history_tree, find);
	}
    }

    new = XMALLOC(MTYPE_TMP, sizeof(twamplight_sender_rtt_history_t));
    if(NULL == new)
    {
	return;
    }

    new->session_id = info->session_id;
    strcpy(new->remote_ip, info->remote_ip);
    time(&new->last_pkt_uptime);
    new->minrtt = minrtt;
    new->maxrtt = maxrtt;
    new->avgrtt = avgrtt;

    avl_Insert(info->rtt_history_tree, new);

    info->is_history = 1;

}

static void twamplight_sender_rtt_clear(struct twamplight_sender_vrf *twamp_vrf)
{
  twamp_vrf->minrtt = 0;
  twamp_vrf->maxrtt = 0;
  twamp_vrf->avgrtt = 0;
}

static int32_t
_delete_twamplight_sender_rtt_history(void *data, void *param)
{
    XFREE(MTYPE_TMP, data);
    data = NULL;

    return 0;
}

static int32_t
_twamplight_sender_rtt_history_cmp(void *a, void *b)
{
    twamplight_sender_rtt_history_t *dataA = (twamplight_sender_rtt_history_t *)a;
    twamplight_sender_rtt_history_t *dataB = (twamplight_sender_rtt_history_t *)b;

    if(dataA->session_id > dataB->session_id)
    {
	return 1;
    }
    else if(dataA->session_id < dataB->session_id)
    {
	return -1;
    }

    if(dataA->last_pkt_uptime > dataB->last_pkt_uptime)
    {
	return 1;
    }
    else if(dataA->last_pkt_uptime < dataB->last_pkt_uptime)
    {
	return -1;
    }

    return 0;
}

int twamplight_sender_main(void *arg)
{
    char *progname = NULL;
    struct sockaddr_in serv_addr;
    struct hostent *server = NULL;
    struct hostent *client = NULL;
    char *argv[] = { "twampd-light-sender" };
    struct twamplight_sender_vrf *twamp_vrf = (struct twamplight_sender_vrf *)arg;
    int rv;
    uint32_t totalrtt = 0, maxrtt = 0, minrtt = 0, avgrtt = 0;
#ifdef ISIS_VARIATION
    uint32_t prev_rtt = 0, var = 0, prev_var = 0, avg_var = 0, total_var = 0;
#endif
    /* twamp monitoring continuous loss */
    int continuous_loss = 0;
    int over_c_threshold = 0;
    /* twamp monitoring minute loss */
    uint32_t session_start_usec = twamplight_get_timestamp_usec();
    int loss_percent = 0;
    int loss_count = 0;
    struct twamp_test_info *twamp_test = malloc(sizeof(struct twamp_test_info));
    struct sockaddr_in local_addr;
    socklen_t server_len;

    int server_port = SERVER_PORT;
    int client_port = SERVER_PORT;
    char server_ip[32];
    char client_ip[32];

    uint32_t test_sessions_msg = TEST_MESSAGES;
    uint32_t test_sessions_interval = TEST_INTERVAL;
    uint32_t test_sessions_timeout = TEST_TIMEOUT;

    uint32_t continuous_loss_threshold = TEST_CONTINUOUS_LOSS_THRES;
    uint32_t minute_loss_average_time = TEST_MINUTE_LOSS_AVG;
    uint32_t minute_loss_threshold = TEST_MINUTE_LOSS_THRES;
    int32_t test_session_msg_sent = 0;
    int32_t test_session_msg_dont_recvd = 0;

    if(twamp_test == NULL)
    {
	zlog_debug (TWAMP_ZG, "Error on malloc\n");
	pal_pthread_exit(NULL);
    }

    twamplight_sender_rtt_clear(twamp_vrf);

    srand(time(NULL));
    /* Obtain the program name without the full path */
    progname = (progname = strrchr(argv[0], '/')) ? progname + 1 : *argv;

    /* Configure thread */
    if(twamp_lclient_thread_config(twamp_vrf) != 0){
	free(twamp_test);
	twamp_test = NULL;
	pal_pthread_exit(NULL);
    }

    /* Set from arguments */
    strcpy(server_ip, twamp_vrf->remote_ip);
    server_port = twamp_vrf->remote_port;
    strcpy(client_ip, twamp_vrf->local_ip);
    client_port = twamp_vrf->local_port;
    test_sessions_interval = twamp_vrf->interval_time;
    if(test_sessions_interval < 0)
    {
#ifdef DEBUG_ENABLE
	TWAMP_PRINTD("[%s %d] ERROR: interval time must be bigger than 0\n");
#endif
	free(twamp_test);
	twamp_test = NULL;
	pal_pthread_exit(NULL);
    }
    test_sessions_timeout = twamp_vrf->timeout;
    minute_loss_average_time = twamp_vrf->ml_av_time;
    minute_loss_threshold = twamp_vrf->ml_thres;
    continuous_loss_threshold = twamp_vrf->cl_count;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(server_port);

    test_session_msg_dont_recvd = test_session_msg_sent = 0;

    /* Set Socket */
    twamp_test->testfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (twamp_test->testfd < 0) {
	zlog_debug (TWAMP_ZG, "Error opening socket\n");
	free(twamp_test);
	twamp_test = NULL;

	twamplight_sender_srcip_check_update(twamp_vrf, 0);
    }
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(client_ip);

    /* Try to bind on an available port */
    /* local */
    twamp_test->testport = client_port;
    local_addr.sin_port = htons(twamp_test->testport);
    if (bind(twamp_test->testfd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr)) < 0)
    {
#ifdef DEBUG_ENABLE
	zlog_debug(TWAMP_ZG, "Session:%d Src port number %d is not available to bind. Error %s.\n", twamp_vrf->session_id, twamp_test->testport, strerror(errno));
#endif
	close(twamp_test->testfd);
	free(twamp_test);
	twamp_test = NULL;

	twamplight_sender_srcip_check_update(twamp_vrf, 0);
	pal_pthread_exit(NULL);
    }
    
    /* Remote */
    twamp_test->port = server_port;
    serv_addr.sin_port = htons(twamp_test->port);
    server_len = sizeof(serv_addr);

    /* timeout */
    if (test_sessions_timeout > 0)
    {
	struct timeval tv_timeo;

	memset(&tv_timeo, 0x0, sizeof(struct timeval));
	tv_timeo.tv_sec = test_sessions_timeout; /* timeout */
	setsockopt(twamp_test->testfd, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo));
    }

    /* init rtt history tree */
    if(twamp_vrf->rtt_history_tree)
    {
	avl_Destroy(twamp_vrf->rtt_history_tree, _delete_twamplight_sender_rtt_history, NULL);
	twamp_vrf->rtt_history_tree = NULL;
    }
    twamp_vrf->rtt_history_tree = avl_Create(_twamplight_sender_rtt_history_cmp, NULL);
    if(NULL == twamp_vrf->rtt_history_tree)
    {
#ifdef DEBUG_ENABLE
	TWAMP_PRINTD("[%s %d] ERROR: twamplight sender AVL tree creation fail\n");
#endif
	close(twamp_test->testfd);
	free(twamp_test);
	twamp_test = NULL;
	pal_pthread_exit(NULL);
    }

    /* After the TWAMP-Control connection has been established, the
     * Control-Client will negociate and set up some TWAMP-Test sessions */
    /* For each accepted TWAMP-Test session send test_sessions_msg
     * TWAMP-Test packets */
    
    while(1) {
	SenderUPacket pack;
	uint32_t session_current_usec = twamplight_get_timestamp_usec();
	ReflectorUPacket pack_reflect;

	g_set_signal = twamp_vrf->signal; 

	/* take interrupt */
	if (g_set_signal) {
	    int i, j, k;
	    char wbuf[128];

	    /* graceful shutdown */
	    if ((g_set_signal & SET_SHUTDOWN) == SET_SHUTDOWN) {
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("remove all resource\n");
#endif

		if(twamp_test)
		{
		    if(twamp_test->testfd > 0)
		    {
			close(twamp_test->testfd);
		    }
		    free(twamp_test);
		}
		pal_pthread_exit(NULL);
	    }
	}       

	test_sessions_msg++;

	memset(&pack, 0, sizeof(pack));
	pack.seq_number = htonl(test_sessions_msg);
	pack.time = get_timestamp();
	pack.error_estimate = htons(0x01);    // Multiplier = 1.
	//pack.error_estimate = htons(0x100);    // Multiplier = 1.

#ifdef DEBUG_ENABLE
	TWAMP_PRINTD("Sending TWAMP-Test message for port %d...\n", twamp_test->port);
#endif

	rv = sendto(twamp_test->testfd, &pack, sizeof(pack), 0,
		(struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (rv <= 0) {
#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("Error sending test packet\n");
#endif
	    twamplight_sender_srcip_check_update(twamp_vrf, 0);
	    if (test_sessions_interval > 0)
	    {
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] SLEEP test_sessions_interval:%d ms\n",__FUNCTION__,__LINE__,test_sessions_interval);
#endif
		usleep(test_sessions_interval*1000);
	    }
	    continue;
	}

	twamplight_sender_srcip_check_update(twamp_vrf, 1);

	test_session_msg_sent++;

	memset(&pack_reflect, 0, sizeof(pack_reflect));
	rv = recvfrom(twamp_test->testfd, &pack_reflect, sizeof(pack_reflect), 0,
		(struct sockaddr *)&serv_addr, &server_len);
	if (rv <= 0) {
#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("Error receiving test reply\n");
#endif
	    test_session_msg_dont_recvd++;
	    continuous_loss++;
	    if((continuous_loss > continuous_loss_threshold) && !over_c_threshold)
	    {
		/* syslog and trap */
		zlog_warn (TWAMP_ZG, "Session:%d TWAMP test packet(remote:%s) continuous loss is %d over than threshold %d\n", twamp_vrf->session_id, server_ip, continuous_loss, continuous_loss_threshold);
		twamplight_monitoring_continuous_loss_trap(twamp_vrf->session_id, continuous_loss, continuous_loss_threshold);
		over_c_threshold = 1;
	    }

	    loss_count++;

#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("[%s %d]  session_current_usec: %lu session_start_usec %lu  %lu minavg:%d\n", __FUNCTION__,__LINE__,session_current_usec ,session_start_usec, session_current_usec - session_start_usec, minute_loss_average_time);
#endif

	    if (totalrtt > 0)
		avgrtt = totalrtt / (test_session_msg_sent - test_session_msg_dont_recvd);


	    if( (session_current_usec - session_start_usec ) >= (minute_loss_average_time * 60000000) )
	    {
		session_start_usec = twamplight_get_timestamp_usec();

		loss_percent = (loss_count * 100 ) / test_session_msg_sent;

#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] loss_percent:%d  loss_count:%d\n",__FUNCTION__,__LINE__,loss_percent, loss_count);
#endif

		if(loss_percent >  minute_loss_threshold)
		{
		    /* minute-loss syslog & trap */
		    zlog_warn(TWAMP_ZG, "Session:%d TWAMP test packet(remote:%s) minute loss is %d %% over than %d %% threshold for last %d min\n", twamp_vrf->session_id, server_ip, loss_percent, minute_loss_threshold, minute_loss_average_time);

		    twamplight_monitoring_minute_loss_trap(twamp_vrf->session_id, loss_percent, minute_loss_average_time, minute_loss_threshold); 
		}

		if(loss_percent != 100)
		{
		    twamplight_sender_rtt_history_update(twamp_vrf, minrtt, maxrtt, avgrtt);
		}

		totalrtt = 0;
		loss_count = 0;
		test_session_msg_dont_recvd = test_session_msg_sent = 0;
	       	maxrtt = 0;
		minrtt = 0;
		avgrtt = 0;

#ifdef ISIS_VARIATION 
		prev_rtt = 0;
		total_var = 0;
#endif
	    }
#ifdef ISIS_VARIATION
	    twamplight_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, avg_var, minrtt, maxrtt, avgrtt);
#else
           twamplight_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, minrtt, maxrtt, avgrtt);
#endif
	    if (test_sessions_interval > 0)
	    {
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] SLEEP test_sessions_interval:%d ms\n",__FUNCTION__,__LINE__,test_sessions_interval);
#endif
		usleep(test_sessions_interval*1000);
	    }

	    continue;
	}
#ifdef DEBUG_ENABLE
	TWAMP_PRINTD("[%s %d] Received TWAMP-Test message response for port %d.\n",__FUNCTION__,__LINE__, twamp_test->port);
#endif

	{
	    TWAMPTimestamp recv_resp_time = get_timestamp();
	    uint32_t rtt;
#define min(a,b)      (((a) < (b)) ? (a) : (b))
#define max(a,b)      (((a) > (b)) ? (a) : (b))

	    /* Print the round-trip metrics */
#ifdef DEBUG_ENABLE
	    print_metrics2(twamp_vrf->session_id, test_sessions_msg, ntohs(twamp_test->port), recv_resp_time, &pack_reflect);
#endif
	    if((recv_resp_time.integer != 0) || (recv_resp_time.fractional != 0))
	    {
		rtt = get_time_difference32(&recv_resp_time, &pack_reflect.sender_time);
		if (minrtt == 0)
		    minrtt = rtt;
		else
		    minrtt = min(minrtt, rtt);

		if (maxrtt == 0)
		    maxrtt = rtt;
		else
		    maxrtt = max(maxrtt, rtt);

		totalrtt += rtt;
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] rtt: %lu min:%lu max:%lu avg:%lu total:%lu\n",__FUNCTION__,__LINE__, rtt, minrtt, maxrtt, avgrtt, totalrtt);
#endif

#ifdef ISIS_VARIATION 
		{
		    uint32_t small_rtt = 0, big_rtt = 0;
		    int msg_recvd = test_session_msg_sent - test_session_msg_dont_recvd;
		    small_rtt = min(rtt, prev_rtt);
		    big_rtt = max(rtt, prev_rtt);

		    var = big_rtt - small_rtt;
		    total_var += var;
#ifdef DEBUG_ENABLE
		    TWAMP_PRINTD("[%s %d] ISIS_VARIATION - rtt:%lu prevrtt:%lu big:%lu small:%lu var:%lu totalvar:%lu\n",__FUNCTION__,__LINE__,rtt, prev_rtt, big_rtt, small_rtt, var, total_var);
#endif
		    if(test_session_msg_sent > 1)
		    {
			avg_var = total_var / ( msg_recvd - 1 );
#ifdef DEBUG_ENABLE
			TWAMP_PRINTD("[%s %d] ISIS_VARIATION - avgvar:%lu var:%lu recvd:%d\n",__FUNCTION__,__LINE__,avg_var, var, msg_recvd-1);
#endif
		    }
		    prev_rtt = rtt;
		}
#endif
	    }

	    /* initialize continuous loss value */
	    continuous_loss = 0;
	    over_c_threshold = 0;
#if 0 /* To-Do */
	    if(over_c_threshold)
	    {
		/* send continuous-loss clear trap */
	    }
#endif

	    if (totalrtt > 0)
		avgrtt = totalrtt / (test_session_msg_sent - test_session_msg_dont_recvd);

#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("[%s %d] session_current_usec: %lu session_start_usec %lu - %lu \n",__FUNCTION__,__LINE__, session_current_usec ,session_start_usec, session_current_usec - session_start_usec);
#endif
	    if( (session_current_usec - session_start_usec ) >= (minute_loss_average_time * 60000000) )
	    {
		session_start_usec = twamplight_get_timestamp_usec();

		loss_percent = (loss_count * 100 ) / test_session_msg_sent;
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] loss_percent:%d  loss_count:%d\n",__FUNCTION__,__LINE__,loss_percent, loss_count);
#endif
		if(loss_percent >  minute_loss_threshold)
		{
		    /* minute-loss syslog & trap */
		    zlog_warn(TWAMP_ZG,"Session:%d TWAMP test packet(remote:%s) minute loss is %d over than %d threshold (%dmin)\n",twamp_vrf->session_id, server_ip,loss_percent, minute_loss_threshold, minute_loss_average_time);
		    twamplight_monitoring_minute_loss_trap(twamp_vrf->session_id, loss_percent, minute_loss_average_time, minute_loss_threshold); 
		}

		/* to isis */
#ifdef ISIS_VARIATION 
		{
		    struct nsm_msg_twamp_isis_varation msg;
		    memset(&msg, 0, sizeof(struct nsm_msg_twamp_isis_varation));
		    memcpy(msg.ip, client_ip, strlen(client_ip));                            
		    msg.minrtt = minrtt;
		    msg.maxrtt = maxrtt;
		    msg.avgrtt = avgrtt;
		    msg.avg_var = avg_var;
#ifdef DEBUG_ENABLE
		    TWAMP_PRINTD("[%s %d] Send isis ip:%s min:%lu max:%lu avg:%lu avg_var:%lu\n",__FUNCTION__,__LINE__,msg.ip, msg.minrtt, msg.maxrtt, msg.avgrtt, msg.avg_var);
#endif
		    nsm_client_send_twamp_isis_variation_update(TWAMP_ZG->nc, &msg);
		}
#endif

		twamplight_sender_rtt_history_update(twamp_vrf, minrtt, maxrtt, avgrtt);

		totalrtt = 0;
		loss_count = 0;
		test_session_msg_dont_recvd = test_session_msg_sent = 0;
	       	maxrtt = 0;
		minrtt = 0;
		avgrtt = 0;
#ifdef ISIS_VARIATION 
		prev_rtt = 0;
		total_var = 0;
#endif
	    }
#ifdef ISIS_VARIATION
	    twamplight_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, avg_var, minrtt, maxrtt, avgrtt);
#else
           twamplight_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, minrtt, maxrtt, avgrtt);
#endif
	}
	
	if (test_sessions_interval > 0)
	{
#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("[%s %d] SLEEP test_sessions_interval:%d ms\n",__FUNCTION__,__LINE__,test_sessions_interval);
#endif
	    usleep(test_sessions_interval*1000);
	}
    }
    close(twamp_test->testfd);
    free(twamp_test);
    twamp_test = NULL;


    if(twamp_vrf->rtt_history_tree)
    {
	avl_Destroy(twamp_vrf->rtt_history_tree, _delete_twamplight_sender_rtt_history, NULL);
	twamp_vrf->rtt_history_tree = NULL;
    }
#ifdef DEBUG_ENABLE
    TWAMP_PRINTD("[%s %d] END thread\n",__FUNCTION__,__LINE__); 
#endif

    return 0;
}
#endif /* HAVE_TWAMPLIGHTD_SENDER */
#endif /* CONFIG_DASAN_SWITCH */
