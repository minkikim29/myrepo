/*
 * Name: Emma Mirică
 * Project: TWAMP Protocol
 * Class: OSS
 * Email: emma.mirica@cti.pub.ro
 *
 * Source: client.c
 * Note: contains the TWAMP client implementation.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <netdb.h>
#include <sys/stat.h>
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
#include "twampd.h"
#include "twamp.h"

/* TWAMP timestamp is NTP time (RFC 1305).
 * Should be in network byte order!	 */
#define PORTBASE_SEND    30000
#define PORTBASE_RECV    20000
#define TEST_SESSIONS    1
#define TEST_MESSAGES    0
#define TEST_INTERVAL    0              
#define TEST_TIMEOUT     3              
#define TIMEOUT          2 /* SECONDS */

#define TEST_CONTINUOUS_LOSS_THRES      0
#define TEST_MINUTE_LOSS_AVG            0
#define TEST_MINUTE_LOSS_THRES          0

/* debug */
//#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define TWAMP_PRINTD printd
#define TWAMP_FPRINTF fprintf
#else
#define TWAMP_PRINTF
#define TWAMP_FPRINTF
#endif

static uint16_t test_sessions_no = TEST_SESSIONS;
static uint16_t active_sessions = 0;
static int port_send = PORTBASE_SEND;
static int port_recv = PORTBASE_RECV;

struct twamp_test_info {
    int testfd;
    int testport;
    uint16_t port;
};

static __thread int g_set_signal = 0;

extern pal_pthread_key_t g_twampfull_sender_thread_key;
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

/* The function that prints the help for this program */
static void usage(char *progname)
{
    printf( "Usage: %s [options]\n", progname);
    printf( "\n\nWhere \"options\" are:\n");
    printf( "   -s  server      The TWAMP server IP [Mandatory]\n"
            "   -a  authmode    Default Unauthenticated\n"
#if 0
            "   -p  port_sender The miminum Test port sender\n"
            "   -P  port_recv   The minimum Test port receiver\n"
#else
            "   -p  port        The Control port sender (862|<1024-65535>) (The default is port 862)\n"
#endif
            "   -n  test_sess   The number of Test sessions\n"
            "   -m  no_test_msg The number of Test packets per Test session\n"
           );
    return;
}

/* 
 * Configure required stuff for this thread
 */
int
twamp_client_thread_config(struct twampfull_sender_vrf *twamp_vrf)
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
    pal_snprintf(thread_name, sizeof(thread_name)-1, "twamp-full-sender_%s_%d", twamp_vrf->vrf_name, twamp_vrf->session_id);
    prctl(PR_SET_NAME, (unsigned long)thread_name, 0, 0, 0);

    /* Set Thread local storage */
    pthread_setspecific(g_twampfull_sender_thread_key, twamp_vrf);
    return 0;
}

static void _print_metrics(uint32_t j, uint16_t port, const ReflectorUPacket *pack) {               

    /* Get Time of the received TWAMP-Test response message */                              
    TWAMPTimestamp recv_resp_time = get_timestamp();                                        

    /* Print different metrics */                                                           

    /* Compute round-trip */                                                                
    printf( "Round-trip time for TWAMP-Test packet %d for port %hd is %" PRIu64 " [usec]\n",
	    j, port, get_time_difference(&recv_resp_time, &pack->sender_time));             
    printf( "Receive time - Send time for TWAMP-Test"                                       
	    " packet %d for port %d is %" PRIu64 " [usec]\n", j, port,                      
	    get_time_difference(&pack->receive_time, &pack->sender_time));                  
    printf( "Reflect time - Send time for TWAMP-Test"                                       
	    " packet %d for port %d is %" PRIu64 " [usec]\n", j, port,                      
	    get_time_difference(&pack->time, &pack->sender_time));                          

}

static uint32_t twampfull_get_timestamp_usec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000000 + tv.tv_usec;
}

static void
twampfull_sender_srcip_check_update(struct twampfull_sender_vrf *twampfull_sender_vrf, int valid)
{
    twampfull_sender_vrf->srcip_valid = valid;
}
/* This function sends StopSessions to stop all active Test sessions */
static int send_stop_session(int socket, int accept, int sessions)
{
    StopSessions stop;
    memset(&stop, 0, sizeof(stop));
    stop.Type = kStopSessions;
    stop.Accept = accept;
    stop.SessionsNo = htonl(sessions);
    return send(socket, &stop, sizeof(stop), 0);
}

static int send_start_sessions(int socket)
{
    StartSessions start;
    memset(&start, 0, sizeof(start));
    start.Type = kStartSessions;
    return send(socket, &start, sizeof(start), 0);
}

/* The function will return a significant message for a given code */
static char *get_accept_str(int code)
{
    switch (code) {
    case kOK:
        return "OK.";
    case kFailure:
        return "Failure, reason unspecified.";
    case kInternalError:
        return "Internal error.";
    case kAspectNotSupported:
        return "Some aspect of request is not supported.";
    case kPermanentResourceLimitation:
        return "Cannot perform request due to permanent resource limitations.";
    case kTemporaryResourceLimitation:
        return "Cannot perform request due to temporary resource limitations.";
    default:
        return "Undefined failure";
    }
}
static void
twampfull_sender_rtt_history_update(struct twampfull_sender_vrf *info, uint32_t minrtt, uint32_t maxrtt, uint32_t avgrtt)
{

    int count = 0;
    twampfull_sender_rtt_history_t *new = NULL, *find = NULL;

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

    new = XMALLOC(MTYPE_TMP, sizeof(twampfull_sender_rtt_history_t));
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

#ifdef ISIS_VARIATION
static void
twampfull_sender_history_update(struct twampfull_sender_vrf *twampfull_sender_vrf, uint32_t nSent, uint32_t notRecvd, uint32_t ml_over_percent, uint32_t cl_loss_count, char *vrfname, uint32_t avg_var, uint32_t minrtt, uint32_t maxrtt, uint32_t avgrtt)
{
    twampfull_sender_vrf->sent = nSent;
    twampfull_sender_vrf->notRecvd = notRecvd;

#ifdef ISIS_VARIATION
    twampfull_sender_vrf->avg_var = avg_var;
#endif

    twampfull_sender_vrf->ml_over_percent = ml_over_percent;
    twampfull_sender_vrf->cl_loss_count = cl_loss_count;

    twampfull_sender_vrf->minrtt = minrtt;
    twampfull_sender_vrf->maxrtt = maxrtt;
    twampfull_sender_vrf->avgrtt = avgrtt;
}
#else
static void
twampfull_sender_history_update(struct twampfull_sender_vrf *twampfull_sender_vrf, uint32_t nSent, uint32_t notRecvd, uint32_t ml_over_percent, uint32_t cl_loss_count, char *vrfname, uint32_t minrtt, uint32_t maxrtt, uint32_t avgrtt)
{
    twampfull_sender_vrf->sent = nSent;
    twampfull_sender_vrf->notRecvd = notRecvd;

    twampfull_sender_vrf->ml_over_percent = ml_over_percent;
    twampfull_sender_vrf->cl_loss_count = cl_loss_count;

    twampfull_sender_vrf->minrtt = minrtt;
    twampfull_sender_vrf->maxrtt = maxrtt;
    twampfull_sender_vrf->avgrtt = avgrtt;
}
#endif

static void twampfull_sender_rtt_clear(struct twampfull_sender_vrf *twamp_vrf)
{
    twamp_vrf->minrtt = 0;
    twamp_vrf->maxrtt = 0;
    twamp_vrf->avgrtt = 0;
}

static int32_t
_delete_twampfull_sender_rtt_history(void *data, void *param)
{
    XFREE(MTYPE_TMP, data);
    data = NULL;

    return 0;
}

static int32_t
_twampfull_sender_rtt_history_cmp(void *a, void *b)
{
    twampfull_sender_rtt_history_t *dataA = (twampfull_sender_rtt_history_t *)a;
    twampfull_sender_rtt_history_t *dataB = (twampfull_sender_rtt_history_t *)b;

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

int twampfull_sender_main(void *arg)
{
    char *progname = NULL;
    struct sockaddr_in serv_addr;
    struct hostent *server = NULL;
    struct hostent *client = NULL;
    char *argv[] = { "twampd-full-sender" };
    struct twampfull_sender_vrf *twamp_vrf = (struct twampfull_sender_vrf *)arg;
    int rv;
    uint32_t totalrtt = 0, maxrtt = 0, minrtt = 0, avgrtt = 0;

    /* twamp monitoring continuous loss */
    int continuous_loss = 0;
    int over_c_threshold = 0;

    /* twamp monitoring minute loss */                                          
    uint32_t session_start_usec = twampfull_get_timestamp_usec();              
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
    twampfull_sender_rtt_clear(twamp_vrf);

    srand(time(NULL));
    /* Obtain the program name without the full path */
    progname = (progname = strrchr(argv[0], '/')) ? progname + 1 : *argv;

    /* Configure thread */
    if(twamp_client_thread_config(twamp_vrf) != 0){
	free(twamp_test);
	twamp_test = NULL;
	pal_pthread_exit(NULL);
    }

    /* Set from arguments */
    strcpy(server_ip, twamp_vrf->remote_ip);
    server_port = twamp_vrf->remote_port;
    //strcpy(client_ip, twamp_vrf->local_port);
    //client_port = twamp_vrf->local_port;
    test_sessions_interval = twamp_vrf->interval_time;
    if(test_sessions_interval < 0)                                           
    {                                                                        
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
    twamp_test->testfd = socket(AF_INET, SOCK_STREAM, 0);
    if (twamp_test->testfd < 0) {
	zlog_debug (TWAMP_ZG, "Error opening socket\n");
	free(twamp_test);
	twamp_test = NULL;

	twampfull_sender_srcip_check_update(twamp_vrf, 0);
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(client_ip);

    printf("Connecting to server %s...\n", inet_ntoa(serv_addr.sin_addr));      
    if (connect(twamp_test->testfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
	printf("Error connecting\n");                                           
	exit(EXIT_FAILURE);                                                     
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
	avl_Destroy(twamp_vrf->rtt_history_tree, _delete_twampfull_sender_rtt_history, NULL);
	twamp_vrf->rtt_history_tree = NULL;
    }
    twamp_vrf->rtt_history_tree = avl_Create(_twampfull_sender_rtt_history_cmp, NULL);
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


    /* Receive Server Greeting and check Modes */
    ServerGreeting greet;
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
    memset(&greet, 0, sizeof(greet));
    rv = recv(twamp_test->testfd, &greet, sizeof(greet), 0);
    if (rv <= 0) {
	close(twamp_test->testfd);
	printf("Error receiving Server Greeting\n");
	exit(EXIT_FAILURE);
    }
    greet.Modes = ntohl(greet.Modes);
    if (greet.Modes == 0) {
	close(twamp_test->testfd);
	printf( "The server does not support any usable Mode\n");
	exit(EXIT_FAILURE);
    }
    printf("Received ServerGreeting.\n");

    /* Compute SetUpResponse */
    printf("Sending SetUpResponse...\n");
    SetUpResponse resp;
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
    memset(&resp, 0, sizeof(resp));
    resp.Mode = htonl(greet.Modes & kModeUnauthenticated);
    rv = send(twamp_test->testfd, &resp, sizeof(resp), 0);
    if (rv <= 0) {
	close(twamp_test->testfd);
	printf("Error sending Greeting Response\n");
	exit(EXIT_FAILURE);
    }

    /* Receive ServerStart message */
    ServerStart start;
    memset(&start, 0, sizeof(start));
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
    rv = recv(twamp_test->testfd, &start, sizeof(start), 0);
    if (rv <= 0) {
	close(twamp_test->testfd);
	printf("Error Receiving Server Start\n");
	exit(EXIT_FAILURE);
    }

    /* If Server did not accept our request */
    if (start.Accept != kOK) {
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	close(twamp_test->testfd);
	printf( "Request failed: %s\n", get_accept_str(start.Accept));
	exit(EXIT_FAILURE);
    }
    printf("Received ServerStart.\n");

    /* After the TWAMP-Control connection has been established, the
     * Control-Client will negociate and set up some TWAMP-Test sessions */

    if (!twamp_test) {
	printf( "Error on malloc\n");
	close(twamp_test->testfd);
	exit(EXIT_FAILURE);
    }
    uint16_t i;
    /* Set TWAMP-Test sessions */
    for (i = 0; i < test_sessions_no; i++) {

	/* Setup test socket */
	twamp_test[active_sessions].testfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (twamp_test[active_sessions].testfd < 0) {
	    printf("Error opening socket\n");
	    continue;
	}

	struct sockaddr_in local_addr;
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = INADDR_ANY;

	/* Try to bind on an available port */
	int check_time = CHECK_TIMES;
	while (check_time--) {
	    twamp_test[active_sessions].testport = port_send + rand() % 1000;
	    local_addr.sin_port = htons(twamp_test[active_sessions].testport);
	    if (!bind(twamp_test[active_sessions].testfd, (struct sockaddr *)&local_addr,
			sizeof(struct sockaddr)))
		break;
	}
	if (check_time == 0) {
	    printf( "Couldn't find a port to bind to for session %d\n", i + 1);
	    continue;
	}
	printf("Sending RequestTWSession for port %d...\n", twamp_test[active_sessions].testport);
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	RequestSession req;
	memset(&req, 0, sizeof(req));
	req.Type = kRequestTWSession;
	req.IPVN = 4;
	req.SenderPort = htons(twamp_test[active_sessions].testport);
	//req.SenderPort = htons(port_recv + rand() % 1000);//htons(twamp_test[active_sessions].testport);
	//req.ReceiverPort = htons(port_recv + rand() % 1000);                                            
	req.ReceiverPort = req.SenderPort;
	req.PaddingLength = htonl(27);     // As defined in RFC 6038#4.2
	TWAMPTimestamp timestamp = get_timestamp();
	timestamp.integer = htonl(ntohl(timestamp.integer) + 10);   // 10 seconds for start time
	req.StartTime = timestamp;
	struct timeval timeout;
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	timeval_to_timestamp(&timeout, &req.Timeout);

	/* Trying to send the RequestTWSession request for this TWAMP-Test */
	rv = sendto(twamp_test->testfd, &req, sizeof(req), 0, (struct sockarrd *)&serv_addr, sizeof(serv_addr));
	if (rv <= 0) {
	    printf( "[%d] ", twamp_test[active_sessions].testport);
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	    printf("Error sending RequestTWSession message\n");
	    close(twamp_test[active_sessions].testfd);
	    free(twamp_test);
	    close(twamp_test->testfd);
	    exit(EXIT_FAILURE);
	}
	/* See the Server's response */
	AcceptSession acc;
	memset(&acc, 0, sizeof(acc));
	rv = recv(twamp_test->testfd, &acc, sizeof(acc), 0);
	if (rv <= 0) {
	    printf( "[%d] ", twamp_test[active_sessions].testport);
	    printf("Error receiving Accept Session\n");
	    close(twamp_test[active_sessions].testfd);
	    free(twamp_test);
	    close(twamp_test->testfd);
	    exit(EXIT_FAILURE);
	}
	fprintf(stderr,"Received %d bytes from server\n", rv);
	/* See the Server response to this RequestTWSession message */
	if (acc.Accept != kOK) {
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	    close(twamp_test[active_sessions].testfd);
	    continue;
	}
	twamp_test[active_sessions].port = acc.Port;
	active_sessions++;
    }

    if (active_sessions) {
	printf("Sending StartSessions for all active ports ...\n");
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	/* If there are any accepted Test-Sessions then send
	 *          * the StartSessions message */
	rv = send_start_sessions(twamp_test->testfd);
	if (rv <= 0) {
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	    printf("Error sending StartSessions\n");
	    /* Close all TWAMP-Test sockets */
	    for (i = 0; i < active_sessions; i++)
		close(twamp_test[i].testfd);
	    free(twamp_test);
	    close(twamp_test->testfd);
	    exit(EXIT_FAILURE);
	}
    }
    /* See the Server's response */
    StartACK ack;
    memset(&ack, 0, sizeof(ack));
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
    rv = recv(twamp_test->testfd, &ack, sizeof(ack), 0);
    if (rv <= 0) {
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	printf( "[%d] ", twamp_test[active_sessions].testport);
	printf("Error receiving Accept Session\n");
	close(twamp_test[active_sessions].testfd);
	free(twamp_test);
	close(twamp_test->testfd);
	exit(EXIT_FAILURE);
    }

    /* For each accepted TWAMP-Test session send test_sessions_msg
     *  * TWAMP-Test packets */
    while(1) {
fprintf(stderr, "[%s:%d], %s \n",__FUNCTION__,__LINE__, strerror(errno));
	SenderUPacket pack;
	uint32_t session_current_usec = twampfull_get_timestamp_usec();
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
	    twampfull_sender_srcip_check_update(twamp_vrf, 0);
	    if (test_sessions_interval > 0)
	    {
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] SLEEP test_sessions_interval:%d ms\n",__FUNCTION__,__LINE__,test_sessions_interval);
#endif
		usleep(test_sessions_interval*1000);
	    }
	    continue;
	}

	twampfull_sender_srcip_check_update(twamp_vrf, 1);

	test_session_msg_sent++;

	memset(&pack_reflect, 0, sizeof(pack_reflect));
	rv = recvfrom(twamp_test->testfd, &pack_reflect, sizeof(pack_reflect), 0, (struct sockaddr *)&serv_addr, &server_len);
	if (rv <= 0) {
#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("Error receiving test reply\n");
#endif
	    test_session_msg_dont_recvd++;
	    continuous_loss++;
	    if((continuous_loss > continuous_loss_threshold) && !over_c_threshold)
	    {
		/* syslog and
		 * trap */
		zlog_warn (TWAMP_ZG, "Session:%d TWAMP test packet(remote:%s) continuous loss is %d over than threshold %d\n", twamp_vrf->session_id, server_ip, continuous_loss, continuous_loss_threshold);
		//twampfull_monitoring_continuous_loss_trap(twamp_vrf->session_id, continuous_loss, continuous_loss_threshold);
		over_c_threshold = 1;
	    }

	    loss_count++;
#ifdef DEBUG_ENABLE
	    TWAMP_PRINTD("[%s %d]  session_current_usec: %lu session_start_usec %lu  %lu minavg:%d\n", __FUNCTION__, __LINE__,session_current_usec ,session_start_usec, session_current_usec - session_start_usec, minute_loss_average_time);
#endif

	    if (totalrtt > 0)
		avgrtt = totalrtt / (test_session_msg_sent - test_session_msg_dont_recvd);


	    if( (session_current_usec - session_start_usec ) >= (minute_loss_average_time * 60000000) )
	    {
		session_start_usec = twampfull_get_timestamp_usec();

		loss_percent = (loss_count * 100 ) / test_session_msg_sent;

#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] loss_percent:%d  loss_count:%d\n",__FUNCTION__,__LINE__,loss_percent, loss_count);
#endif

		if(loss_percent >  minute_loss_threshold)
		{
		    /* minute-loss syslog & trap */
		    zlog_warn(TWAMP_ZG, "Session:%d TWAMP test packet(remote:%s) minute loss is %d %% over than %d %% threshold for last %d min\n", twamp_vrf->session_id, server_ip, loss_percent, minute_loss_threshold, minute_loss_average_time);

		    //twampfull_monitoring_minute_loss_trap(twamp_vrf->session_id, loss_percent, minute_loss_average_time, minute_loss_threshold);
		}

		if(loss_percent != 100)
		{
		    twampfull_sender_rtt_history_update(twamp_vrf, minrtt, maxrtt, avgrtt);
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
	    twampfull_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, avg_var, minrtt, maxrtt, avgrtt);
#else
	    twampfull_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, minrtt, maxrtt, avgrtt);
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
		session_start_usec = twampfull_get_timestamp_usec();

		loss_percent = (loss_count * 100 ) / test_session_msg_sent;
#ifdef DEBUG_ENABLE
		TWAMP_PRINTD("[%s %d] loss_percent:%d  loss_count:%d\n",__FUNCTION__,__LINE__,loss_percent, loss_count);
#endif
		if(loss_percent >  minute_loss_threshold)
		{
		    /* minute-loss syslog & trap */
		    zlog_warn(TWAMP_ZG,"Session:%d TWAMP test packet(remote:%s) minute loss is %d over than %d threshold (%dmin)\n",twamp_vrf->session_id, server_ip,loss_percent, minute_loss_threshold, minute_loss_average_time);
		    //twampfull_monitoring_minute_loss_trap(twamp_vrf->session_id, loss_percent, minute_loss_average_time, minute_loss_threshold);
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
		twampfull_sender_rtt_history_update(twamp_vrf, minrtt, maxrtt, avgrtt);

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
	    twampfull_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, avg_var, minrtt, maxrtt, avgrtt);
#else
	    twampfull_sender_history_update(twamp_vrf, test_session_msg_sent, test_session_msg_dont_recvd, loss_percent, continuous_loss, twamp_vrf->vrf_name, minrtt, maxrtt, avgrtt);
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
	avl_Destroy(twamp_vrf->rtt_history_tree, _delete_twampfull_sender_rtt_history, NULL);
	twamp_vrf->rtt_history_tree = NULL;
    }
#ifdef DEBUG_ENABLE
    TWAMP_PRINTD("[%s %d] END thread\n",__FUNCTION__,__LINE__);
#endif
    return 0;
}
