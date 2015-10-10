#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <inttypes.h>

#include <evhtp.h>

#include <signal.h>

#include <event2/bufferevent.h>

#include <sys/time.h>

static void
request_cb(evhtp_request_t * req, void * arg) {
    printf("hi %zu\n", evbuffer_get_length(req->buffer_in));
    evbase_t* base = (evbase_t*)(arg);
    event_base_loopbreak(base);
}

evbase_t           * evbase;

void SigHandler(int sig) {
  printf("in signal..\n");
  event_base_loopbreak(evbase);
}

int
main(int argc, char ** argv) {
    evhtp_connection_t * conn;

    signal(SIGINT, SigHandler);

    evbase  = event_base_new();
    conn    = evhtp_connection_new(evbase, "127.0.0.1", 8384);

    const int len = 6;
    evhtp_request_t* requests[len];
    int j = 0;
    while (j++ < 10) {
       for (int i = 0; i < len; ++i) {
           requests[i] = evhtp_request_new(request_cb,
                                           evbase);
           evhtp_set_hook(&requests[i]->hooks,
                          evhtp_hook_on_read,
                          print_data,
                          evbase);
           evhtp_make_request(conn,
                              requests[i],
                              htp_method_GET,
                              "/spl?breed_id=9894");
           printf("waiting....\n");
           event_base_loop(evbase, 0);
       }
       for (int k = 0; k < len; ++k) {
         evhtp_request_free(requests[k]);
       }
    }

    event_base_free(evbase);
    evhtp_connection_free(conn);
    printf("Done!\n");

    return 0;
}
