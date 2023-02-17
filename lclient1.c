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
#include <errno.h>
#include "twamp.h"
#if defined(CONFIG_DASAN_COMMON)
#include "history.h"
#endif

#define PORTBASE_SEND    30000
#define PORTBASE_RECV    20000
#define TEST_SESSIONS    1
#define TEST_MESSAGES    1
#define TEST_INTERVAL    0
#define TEST_TIMEOUT     3
#define TIMEOUT          2 /* SECONDS */

struct twamp_test_info {
    int testfd;
    int testport;
    uint16_t port;
};

static enum Mode authmode = kModeUnauthenticated;
static int port_send = PORTBASE_SEND;
static int port_recv = PORTBASE_RECV;
static int server_port = SERVER_PORT;
static int client_port = SERVER_PORT;
static char server_ip[32];
static char client_ip[32];

static uint16_t test_sessions_no = TEST_SESSIONS;
static uint32_t test_sessions_msg = TEST_MESSAGES;
static uint32_t test_sessions_interval = TEST_INTERVAL;
static uint32_t test_sessions_timeout = TEST_TIMEOUT;
static uint16_t active_sessions = 0;
#if defined(CONFIG_DASAN_COMMON)
static int32_t user_taskid = 0;
static int32_t test_session_msg_sent = 0;
static int32_t test_session_msg_dont_recvd = 0;
static char vrfname[64] = "default";
#endif

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

/* The parse_options will check the command line arguments */
static int parse_options(struct hostent **server, struct hostent **client, char *progname, int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }
    int opt;

#if defined(CONFIG_DASAN_COMMON)
    while ((opt = getopt(argc, argv, "s:a:p:P:n:m:t:v:i:o:e:u:h")) != -1) {
#else
    while ((opt = getopt(argc, argv, "s:a:p:P:n:m:h")) != -1) {
#endif
        switch (opt) {
        case 's':
            /* Get the Server's IP */
            //*server = gethostbyname(optarg);
			//printf("server ip = %s(%s)\n", optarg, server);
			strcpy(server_ip, optarg);
            break;
        case 'a':
            /* For now it only supports unauthenticated mode */
            authmode = kModeUnauthenticated;
            break;
#if 0
        case 'p':
            port_send = atoi(optarg);
            /* The port must be a valid one */
            if (port_send < 1024 || port_send > 65535)
                return 1;
            break;
        case 'P':
            port_recv = atoi(optarg);
            /* The port must be a valid one */
            if (port_recv < 1024 || port_recv > 65535)
                return 1;
            break;
#else
        case 'p':
            server_port = atoi(optarg);
            /* The port must be a valid one */
            if ((server_port != SERVER_PORT) && (server_port < 1 || server_port > 65535))
                return 1;
            break;
#endif
        case 'n':
            test_sessions_no = atoi(optarg);
            /* Test sessions number must be a valid one */
            if (test_sessions_no < 0)
                return 1;
            break;
        case 'm': /* repeat */
            test_sessions_msg = atoi(optarg);
            /* Test messages per session must be a valid one */
            if (test_sessions_msg < 0)
                return 1;
            break;
#if defined(CONFIG_DASAN_COMMON)
		case 't':
			user_taskid = atoi(optarg);
			break;
		case 'v':
			memset(vrfname, 0x0, sizeof(vrfname));
			strncpy(vrfname, optarg, strlen(optarg));
			break;
		case 'i':
			//*client = gethostbyname(optarg);
			//printf("client ip = %s\n", optarg);
			strcpy(client_ip, optarg);
			break;
		case 'o':
			client_port = atoi(optarg);
			/* The port must be a valid one */
			if ((client_port != SERVER_PORT) && (client_port < 1 || client_port > 65535))
				return 1;
			break;
		case 'e': /* interval */
			test_sessions_interval = atoi(optarg);
			/* Test sessions interval must be a valid one */
			if (test_sessions_interval < 0)
				return 1;
			break;
		case 'u': /* timeout */
			test_sessions_timeout = atoi(optarg);
			/* Test sessions timeout must be a valid one */
			if (test_sessions_timeout < 0)
				return 1;
			break;
#endif
        case 'h':
        default:
            return 1;
        }
    }

    return 0;
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

int main(int argc, char *argv[])
{
    char *progname = NULL;
    srand(time(NULL));
    progname = (progname = strrchr(argv[0], '/')) ? progname + 1: *argv;

    struct sockaddr_in serv_addr;
    struct hostent *server = NULL;
    struct hostent *client = NULL;

#if defined(CONFIG_DASAN_COMMON)
    ReflectorUPacketDS *pack_reflect_array = NULL;
    size_t msize = 0;
#endif

    /* Sanity check */
    /*if (getuid() == 0) {
        printf( "%s should not be run as root\n", progname);
        exit(EXIT_FAILURE);
    }*/

    /* Check client options */
    if (parse_options(&server, &client, progname, argc, argv)) {
        usage(progname);
        exit(EXIT_FAILURE);
    }
#if 0
    if (server == NULL) {
        printf("Error, no such host\n");
        exit(EXIT_FAILURE);
    }
#endif

#if defined(CONFIG_DASAN_COMMON)
    if(0 == user_taskid)
    {
	user_taskid = rand();
    }
#endif

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    //memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(server_port);

    /* After the TWAMP-Control connection has been established, the
     * Control-Client will negociate and set up some TWAMP-Test sessions */

    struct twamp_test_info *twamp_test = malloc(test_sessions_no * sizeof(struct twamp_test_info));
    if (!twamp_test) {
        printf( "Error on malloc\n");
#if defined(CONFIG_DASAN_COMMON)
		history_lightclient_result(user_taskid, TWAMP_CLIENT_RESULT_FAIL);
#endif
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
#if 0
		if (client != NULL)
			memcpy(&local_addr.sin_addr.s_addr, client->h_addr, client->h_length);
#endif
		local_addr.sin_addr.s_addr = inet_addr(client_ip);

        /* Try to bind on an available port */
		twamp_test[active_sessions].testport = client_port;
		local_addr.sin_port = htons(twamp_test[active_sessions].testport);
		if (bind(twamp_test[active_sessions].testfd, (struct sockaddr *)&local_addr,
					sizeof(struct sockaddr)) < 0)
		{
            printf( "Src port number %d is not available to bind. Error %s.\n", twamp_test[active_sessions].testport, strerror(errno));
            continue;
        }

        twamp_test[active_sessions].port = server_port;
        active_sessions++;
    }


    /* For each accepted TWAMP-Test session send test_sessions_msg
     * TWAMP-Test packets */
	int rv;
	uint64_t totalrtt = 0, maxrtt = 0, minrtt = 0, rtt = 0, avgrtt = 0, stddev = 0;

    for (i = 0; i < active_sessions; i++) {
        uint32_t j;

#if defined(CONFIG_DASAN_COMMON)
		if (test_sessions_timeout > 0)
		{
			struct timeval tv_timeo;

			memset(&tv_timeo, 0x0, sizeof(struct timeval));
			tv_timeo.tv_sec = test_sessions_timeout; /* timeout */
			setsockopt(twamp_test[i].testfd, SOL_SOCKET, SO_RCVTIMEO, &tv_timeo, sizeof(tv_timeo));
		}

		test_session_msg_dont_recvd = test_session_msg_sent = 0;
#endif

		totalrtt = 0;
		maxrtt = 0;
		minrtt = 0;
		rtt = 0;
		avgrtt = 0;
	    stddev = 0;

        for (j = 0; j < test_sessions_msg; j++) {
            SenderUPacket pack;
            memset(&pack, 0, sizeof(pack));
            pack.seq_number = htonl(i * test_sessions_msg + j);
            pack.time = get_timestamp();
			pack.error_estimate = htons(0x01);    // Multiplier = 1.
            //pack.error_estimate = htons(0x100);    // Multiplier = 1.

            printf("Sending TWAMP-Test message %d for port %d...\n", j + 1, twamp_test[i].port);
            serv_addr.sin_port = htons(twamp_test[i].port);
            rv = sendto(twamp_test[i].testfd, &pack, sizeof(pack), 0,
                        (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            if (rv <= 0) {
                printf("Error sending test packet\n");
                continue;
            }

#if defined(CONFIG_DASAN_COMMON)
			test_session_msg_sent++;
#endif
			socklen_t len = sizeof(serv_addr);
			//serv_addr.sin_port = htons(twamp_test[i].port);
			ReflectorUPacket pack_reflect;
			memset(&pack_reflect, 0, sizeof(pack_reflect));
			rv = recvfrom(twamp_test[i].testfd, &pack_reflect, sizeof(pack_reflect), 0,
					(struct sockaddr *)&serv_addr, &len);
			if (rv <= 0) {
				printf("Error receiving test reply\n");
#if defined(CONFIG_DASAN_COMMON)
				test_session_msg_dont_recvd++;
#endif
				continue;
            }
            printf("Received TWAMP-Test message response %d for port %d.\n", j + 1, twamp_test[i].port);
#if defined(CONFIG_DASAN_COMMON)
			{
				TWAMPTimestamp recv_resp_time = get_timestamp();
				uint64_t rtt;

				/* Print the round-trip metrics */
				print_metrics(j + 1, ntohs(twamp_test[i].port), recv_resp_time, &pack_reflect);
				if((recv_resp_time.integer != 0) || (recv_resp_time.fractional != 0))
				{

#define min(a,b)      (((a) < (b)) ? (a) : (b))
#define max(a,b)      (((a) > (b)) ? (a) : (b))

					rtt = get_time_difference(&recv_resp_time, &pack_reflect.sender_time);
					if (minrtt == 0)
						minrtt = rtt;
					else
						minrtt = min(minrtt, rtt);

					if (maxrtt == 0)
						maxrtt = rtt;
					else
						maxrtt = max(maxrtt, rtt);

					totalrtt += rtt;
				}
			}
#else
            /* Print the round-trip metrics */
            print_metrics(j + 1, ntohs(twamp_test[i].port), &pack_reflect);
#endif
			if (test_sessions_interval > 0)
				sleep(test_sessions_interval);
        }

		if (totalrtt > 0)
			avgrtt = totalrtt / (test_session_msg_sent - test_session_msg_dont_recvd);
		dump_metrics_out_without_stddev(&serv_addr, test_session_msg_sent, test_session_msg_dont_recvd, minrtt, maxrtt, avgrtt, user_taskid, vrfname);
    }

    /* Close all TWAMP-Test sockets */
    for (i = 0; i < active_sessions; i++)
        close(twamp_test[i].testfd);
    free(twamp_test);
#if defined(CONFIG_DASAN_COMMON)
	if (test_session_msg_sent == test_session_msg_dont_recvd)
		history_lightclient_result(user_taskid, TWAMP_CLIENT_RESULT_FAIL);
	else
	    history_lightclient_result(user_taskid, TWAMP_CLIENT_RESULT_SUCCESS);
#endif

    return 0;
}
