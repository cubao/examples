#include "mongoose.h"

void ev_handler(struct mg_connection *nc, int ev, void *p)
{
    struct mbuf *io = &nc->recv_mbuf;
    (void)p;

    switch (ev) {
    case MG_EV_RECV:
        mg_send(nc, io->buf, io->len); // Echo message back
        mbuf_remove(io, io->len);      // Discard message from recv buffer
        break;
    default:
        break;
    }
}

int main(void)
{
    struct mg_mgr mgr;
    const char *port1 = "1234", *port2 = "127.0.0.1:17000";

    mg_mgr_init(&mgr, NULL);
    mg_bind(&mgr, port1, ev_handler);
    mg_bind(&mgr, port2, ev_handler);

    printf("Starting echo mgr on ports %s, %s\n", port1, port2);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}
