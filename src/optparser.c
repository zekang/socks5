#include <string.h>
#include <stdlib.h>

#include "optparser.h"
#include "socks5.h"
#include "logger.h"

extern struct socks5_server g_server;

int socks5_server_parse(int argc, char **argv) {

#define OPTION_USERNAME_IDX 1
#define OPTION_PASSWORD_IDX 2

    int option_index = 0;
    struct option long_options[] = {
        { "username",    required_argument,  &option_index,  OPTION_USERNAME_IDX },
        { "password",    required_argument,  &option_index,  OPTION_PASSWORD_IDX },
        { "port",        required_argument,  NULL,           'p'                 },
        { "daemon",      required_argument,  NULL,           'd'                 },
        { 0,             0,                  NULL,           0                   }
    };

    int c;

    while (1) {
        c = getopt_long (argc, argv, "dp:", long_options, NULL);

        // end of the options.
        if (-1 == c) {
            break;
        }

        switch (c) {
            case 'd':
                logger_debug("run as daemon\n");
                g_server.daemon = true;
                break;
            case 'p':
                g_server.port = atoi(optarg);
                logger_debug("port: [%d]\n", g_server.port);
                break;
            case 0: {
                switch (option_index) {
                    case OPTION_USERNAME_IDX: {
                        int ulen = strlen(optarg);
                        int maxulen = SOCKS5_AUTH_USERNAMEPASSWORD_MAX_LEN;
                        if (ulen >= maxulen) {
                            logger_error("username exceed length [%d]\n", maxulen);
                            return -1;
                        }
                        stpcpy(g_server.username, optarg);
                        logger_debug("username: [%s]\n", g_server.username);
                        break;
                    }
                    case OPTION_PASSWORD_IDX: {
                        int plen = strlen(optarg);
                        int maxplen = SOCKS5_AUTH_USERNAMEPASSWORD_MAX_LEN;
                        if (plen >= maxplen) {
                            logger_error("password exceed length [%d]\n", maxplen);
                            return -1;
                        }
                        logger_debug("password: [%s]\n", g_server.password);
                        stpcpy(g_server.password, optarg);
                        break;
                    }
                    default:
                        return -1;
                }
                break;
            }
            default:
                return -1;
        }
    }

    return 0;
}
