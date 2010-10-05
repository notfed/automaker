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

/* File read buffer */
int buffer_f_read(int fd,char *buf,int len);
char buffer_f_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(buffer_f_read,-1,buffer_f_space,sizeof buffer_f_space);
buffer *buffer_f = &it;
int buffer_f_read(int fd, char *buf,int len)
{
  if (buffer_flush(buffer_f) == -1) return -1;
  return read(fd,buf,len);
}

#define puts(s) buffer_putsalign(buffer_1,(s))
#define putc(c) buffer_PUTC(buffer_1,c)
#define putflush() buffer_flush(buffer_1)
int dependon(str0 m)
{
    int fd;
    int rc;
    int match;
    str0 newmod;

    if(critbit0_contains(&modules,&m)) return 0;
    if(!critbit0_insert(&modules,&pool,&m)) return 111;

    buffer_putsflush(buffer_2,"depend\n");

    puts(m); putc("\n");

    if((fd = open_read(m))<0) return 111;
    buffer_f->fd = fd;

    do {
      rc = getln(buffer_f,&line,&match,'\n');
      buffer_putsflush(buffer_2,"[check]\n");
      
      if(rc<0) break;
      if(!str_start(line.s,"/*")) continue;
        
      rc = getln(buffer_f,&line,&match,'\n');
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
    if(argc<=1) {
        buffer_putsflush(buffer_2,
             "automaker-list: usage: automaker-list [files ...]\n");
        return 100;
    }
    puts("Listing modules: \n"); putflush();
    for(i=1;i<argc;i++)
    {
        if((rc=dependon(argv[i]))!=0) return rc;
    }
    putflush();
    return 0;
}
