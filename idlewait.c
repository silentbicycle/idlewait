/* 
 * Copyright (c) 2010 Scott Vokes <vokes.s@gmail.com>
 *  
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <unistd.h>

#define DEF_PAUSE	1
#define DEBUG		0

static double p_avg();
static void wait_until_idle(int, double, int);


static double p_avg() {
        double avg[1];
        if (getloadavg(avg, 1) == -1) {
                printf("got error\n");
                exit(1);
        }
        return avg[0];
}


static void wait_until_idle(int wait, double load, int maxwait) {
        int waited = 0;
        double cur_load;

        if (DEBUG) printf("maxwait: %d\n", maxwait);

        while ((cur_load = p_avg()) >= load) {
                sleep(wait);
                waited += wait;
                if (DEBUG) printf("load: %2.2f\twaited: %d\n",
                    cur_load, waited);
                if (maxwait > 0 && waited >= maxwait) break;
        }
}


static void print_usage() {
        printf("idlewait: Sleep until system load is less than the first argument.\n");
        printf("Usage: idlewait load [max_wait_in_seconds]\n");
}


int main(int argc, char* argv[]) {
        int pause = DEF_PAUSE;
        int maxwait = 0;        /* forever */
        double load;

        if (argc < 2 || argc > 3) {
                print_usage();
                exit(1);
        } else if (argc == 3) {
                maxwait = atoi(argv[2]);
        }

        load = strtod(argv[1], NULL);
        if (load <= 0) {
                printf("Invalid load: %s\n", argv[1]);
                printf("(Load must be a decimal, e.g. 1.4.)\n", argv[1]);
                exit(1);
        }

        wait_until_idle(pause, load, maxwait);
        return 0;
}
