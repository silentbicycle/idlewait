#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define DEF_SECS	1
#define DEBUG		0

static double p_avg();
static void p_loop(int, double, int, int);

static double p_avg() {
        double avg[1];
        if (getloadavg(avg, 1) == -1) {
                printf("got error\n");
                exit(1);
        }
        return avg[0];
}

static void p_loop(int wait, double load, int maxwait, int debug) {
        int waited = 0;
        double cur_load;
        assert(load > 0);

        if (debug)
                printf("wait: %d\tload: %f\tmaxwait: %d\n",
                    wait, load, maxwait);

        while ((cur_load = p_avg()) >= load) {
                sleep(wait);
                waited += wait;
                if (debug) printf("load: %f\twaited: %d\n",
                    cur_load, waited);
                if (maxwait > 0 && waited >= maxwait) break;
        }
}


static void print_usage() {
        printf("Sleep until system load is less than the first argument.\n");
        printf("Usage: procrast load [max_wait_in_seconds]\n");
}


int main(int argc, char* argv[]) {
        int secs = DEF_SECS;
        int maxwait = 0;        /* 0 = forever */
        int debug = DEBUG;
        double load;

        /* TODO use getopt */

        if (argc < 2 || argc > 3) {
                print_usage();
                exit(1);
        }

        if (argc == 3) {
                maxwait = atoi(argv[2]);
        }

        /* TODO use strtonum? OpenBSD-specific... */
        load = atof(argv[1]);
        if (load <= 0) {
                printf("Invalid load: %f\n", load);
                exit(1);
        }

        p_loop(secs, load, maxwait, debug);
        return 0;
}
