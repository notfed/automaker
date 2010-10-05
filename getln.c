/*
%use getln2;
%use stralloc_catb;
*/
#include "buffer.h"
#include "byte.h"
#include "stralloc.h"
#include "getln.h"

int getln(bb,sa,match,sep)
register buffer *bb;
register stralloc *sa;
int *match;
int sep;
{
  char *cont;
  unsigned int clen;

  if (getln2(bb,sa,&cont,&clen,sep) == -1) return -1;
  if (!clen) { *match = 0; return 0; }
  if (!stralloc_catb(sa,cont,clen)) return -1;
  *match = 1;
  return 0;
}
