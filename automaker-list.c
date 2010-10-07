/* 
%use critbit0_insert;
%use critbit0_contains;
%use open_read;
%use buffer_put;
%use getln;
*/
#include "critbit0.h"
#include "str0.h"
#include "buffer.h"
#include "stralloc.h"
#include "getln.h"
#include "strerr.h"
#include "error.h"

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
#define putflush() buffer_flush(buffer_1)
#define check() buffer_putsflush(buffer_2,"[check]")
#define oops() strerr_die1sys(errno,"oops: an error occured: ")
stralloc nmod = {0};

int count = 0;
int dependon(str0 m)
{
    int fd;
    int rc;
    int match;
    str0 newmod;

    if(critbit0_contains(&modules,&m)) return 0;
    if(!critbit0_insert(&modules,&pool,&m)) return 111;

    stralloc_copys(&nmod,m);
    if(! (nmod.len > 2 
          && nmod.s[nmod.len-2] == '.' 
          && nmod.s[nmod.len-1] == 'c')) 
    {
      stralloc_cats(&nmod,".c"); 
    }
    stralloc_0(&nmod);

    if(count++ == 0) {
       puts("./load -o "); puts(nmod.s); puts("\n"); putflush();
    } else {
        puts(" "); puts(m); puts(".o"); putflush();
    }

    if((fd = open_read(nmod.s))<0) return 111;
    buffer_f->fd = fd;

    rc = getln(buffer_f,&line,&match,'\n');
    if(rc<0) { close(fd); return 111; }

    if(!str_start(line.s,"/*")) { close(fd); return 0; }

    do {
        
      rc = getln(buffer_f,&line,&match,'\n');
      if(rc<0) break;

      if(str_start(line.s,"*/")) break;
      if(line.len<=8) continue;
      if(!str_start(line.s,"%use ")) continue;

      if(line.s[line.len-2]!=';') continue;

      line.s[line.len-2] = 0;
      if(dependon(line.s+5)!=0) { oops(); break; }
      buffer_f->fd = fd;
    } while(match);
    puts("\n"); putflush();
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
