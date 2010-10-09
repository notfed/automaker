#include "buffer.h"
void buffer_init(buffer *b,int (*op)(),int fd,char *y,unsigned int ylen)
{
    b->x  = y;
    b->p  = 0;
    b->n  = ylen;
    b->fd = fd;
    b->op = op;
}
