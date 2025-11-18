#include<unistd.h>

#include "../incl/_rint.h"

static int buf[2];

int sig_init(void) {
    return pipe(buf);
}

ssize_t sig_write(int desc) {
    return write(buf[1], &desc, sizeof(desc));
}

ssize_t sig_read(int *desc) {
    return read(buf[0], desc, sizeof(*desc));
}
