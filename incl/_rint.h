#ifndef RINT_H
#define RINT_H

#include <unistd.h>

int sig_init(void);
ssize_t sig_write(int value);
ssize_t sig_read(int *value);

#endif