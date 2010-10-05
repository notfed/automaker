/*
%use stralloc_ready;
%use buffer_get;
*/
#include "buffer.h"
#include "stralloc.h"
#include "byte.h"
#include "getln.h"

int getln2(bb,sa,cont,clen,sep)
register buffer *bb;
register stralloc *sa;
/*@out@*/char **cont;
/*@out@*/unsigned int *clen;
int sep;
{
  register char *x;
  register unsigned int i;
  int n;
 
  if (!stralloc_ready(sa,0)) return -1;
  sa->len = 0;
 
  for (;;) {
    n = buffer_feed(bb);
    if (n < 0) return -1;
    if (n == 0) { *clen = 0; return 0; }
    x = buffer_PEEK(bb);
    i = byte_chr(x,n,sep);
    if (i < n) { buffer_SEEK(bb,*clen = i + 1); *cont = x; return 0; }
    if (!stralloc_readyplus(sa,n)) return -1;
    i = sa->len;
    sa->len = i + buffer_get(bb,sa->s + i,n);
  }
}
