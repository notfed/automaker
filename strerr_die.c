#include "buffer.h"
#include "exit.h"
#include "strerr.h"
#define puts2(s) buffer_puts(buffer_2,s);
void strerr_warn(const char *x1,const char *x2,const char *x3,const char *x4,const char *x5,const char *x6,const struct strerr *se)
{
  strerr_sysinit();
 
  if (x1) puts2(x1);
  if (x2) puts2(x2);
  if (x3) puts2(x3);
  if (x4) puts2(x4);
  if (x5) puts2(x5);
  if (x6) puts2(x6);
 
  while(se) {
    if (se->x) puts2(se->x);
    if (se->y) puts2(se->y);
    if (se->z) puts2(se->z);
    se = se->who;
  }
 
  puts2("\n");
  buffer_flush(buffer_2);
}

void strerr_die(int e,const char *x1,const char *x2,const char *x3,const char *x4,const char *x5,const char *x6,const struct strerr *se)
{
  strerr_warn(x1,x2,x3,x4,x5,x6,se);
  _exit(e);
}
