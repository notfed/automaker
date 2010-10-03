/* 
%use critbit0_contains;
%use critbit0_insert;
%use critbit0_delete;
%use open_read;
%use buffer_putsalign;
%use buffer_putflush;
%use getln;
*/
#include "critbit0.h"
#include "str0.h"
#include "buffer.h"
#include "stralloc.h"
#include "getln.h"

critbit0_tree modules;
limitmalloc_pool pool = { 1024 };
stralloc line = {0};

int dependon(str0 m)
{
    int fd;
    int match;
    str0 newmod;
    if(!critbit0_contains(&modules,m)) continue;
    if(!critbit0_insert(&modules,&pool,m)) return 111;
    buffer_putsalign(m); buffer_PUTC('\n');
    fd = open_read(m);
    if(fd<0) return 111;

    do {
      rc = getln(buffer_0,&line,&match,'\n');
      if(rc<0) break;
      if(!str_start(line.s,"/*")) continue;
        
      rc = getln(buffer_0,&line,&match,'\n');
      if(rc<0) break;
      if(!str_start(line.s,"%use ")) continue;
      if(line.s[line.len-1]!=';') break;
      line.s[line.len-1] = '.';
      stralloc_append(&line,"c");
      dependon(line.s+5);

    } while(match);
    
    close(fd);
    return 0;
}
int main(int argc, char*argv[])
{
    int i,rc;
    str0 m;
    if(argc<=1) return 100;
    for(i=0;i<argc;i++)
    {
        if((rc=dependon(argv[i]))!=0) return rc;
    }
    buffer_flush(buffer_1);
    return 0;
}
