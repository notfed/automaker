/* 
%use open_read;
%use getln;
%use critbit0_insert;
%use critbit0_contains;
%use buffer_put;
*/
#include "critbit0.h"
#include "str0.h"
#include "buffer.h"
#include "stralloc.h"
#include "getln.h"
#include "strerr.h"
#include "error.h"
#include "readwrite.h"
#include "open.h"
#include "str.h"


critbit0_tree modules;
critbit0_tree nextup;
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
#define put2s(s) buffer_putsalign(buffer_2,(s))
#define put2flush() buffer_flush(buffer_2)
#define check() buffer_putsflush(buffer_2,"[check]")
#define oops() strerr_die1sys(errno,"oops: an error occured: ")

/* {m}.c */
stralloc modc = {0};

/* number of modules in tree */
int count = 0;


void debug(const char *str)
{
    buffer_puts(buffer_1,"[");
    buffer_puts(buffer_1,str);
    buffer_putsflush(buffer_1,"]\n");
}

/* Read the file {m}.c and read all of its dependencies into the tree */
int dependon(str0 m)
{
    int fd;
    int rc;
    int match;
    str0 newmod;

    /* No use in reading a module twice */
    if(critbit0_contains(&modules,&m)) return 0;

    /* Add the module to the tree */
    if(!critbit0_insert(&modules,&pool,&m)) return 1;
    critbit0_clear(&nextup,&pool);
    count++;

    /* {m}.c is a new module */
    stralloc_copys(&modc,m);
    stralloc_cats(&modc,".c"); 
    stralloc_0(&modc);

    /* Open module source file */
    if((fd = open_read(modc.s))<0) return 1;
    buffer_f->fd = fd;

    /* Read first line */
    rc = getln(buffer_f,&line,&match,'\n');
    if(rc<0) { close(fd); return 1; }

    /* Make sure first line is a comment start */
    if(!str_start(line.s,"/*")) { close(fd); return 0; }

    for(;;) {
        
      /* Read next line */
      rc = getln(buffer_f,&line,&match,'\n');
      if(rc<0 || !match) break;

      /* If line is a comment ender, we're done with this file */
      if(str_start(line.s,"*/")) break;

      /* Make sure line is of format "%use MODULE;\n" */
      if(line.len<8) continue;
      if(!str_start(line.s,"%use ")) continue;
      if(line.s[line.len-2]!=';') continue;
 
      /* extract just the module name */
      line.s[line.len-2] = 0;
      newmod = line.s + 5;

      /* Put the name in the tree */
      if(!critbit0_contains(&nextup,&newmod))
        if(!critbit0_insert(&nextup,&pool,&newmod)) return 111;
    } 

    /* Done reading this file */
    close(fd);
    return 0;
}

/* moredepends */
str0 moredepends_arg;
int moredepends_callback(void)
{
    if(dependon(moredepends_arg)!=0) return 0;
    return 1;
}
int moredepends()
{
    str0 empty = "";
    if(critbit0_allprefixed(&nextup, &pool, &moredepends_arg, &empty, moredepends_callback)<0) oops();
    put2flush();
    return 0;
}

/* showresults */
str0 showresults_arg;
int showresults_callback(void)
{
    debug(showresults_arg);
    return 1;
}
int showresults()
{
    str0 empty = "";
    if(critbit0_allprefixed(&nextup, &pool, &showresults_arg, &empty, showresults_callback)<0) oops();
    put2flush();
    return 0;
}
int main(int argc, char*argv[])
{
    int i,len,rc;
    char *p;
    if(argc<=1) {
        put2s("automaker-list: usage: automaker-list [files ...]\n");
        put2flush();
        return 100;
    }
    puts("Listing modules: \n"); 
    putflush();
    for(i=1;i<argc;i++)
    {
        /* grow a new tree */
        critbit0_clear(&modules,&pool);
        critbit0_clear(&nextup,&pool);
        count = 0;

        /* chop off .c suffix */
        p = argv[i];
        len = str0_length(&p);
        if((len > 2 
          && p[len-2] == '.' 
          && p[len-1] == 'c')) 
        {
          p[len-2] = 0;
        }

        /* add module to tree along with its dependents */
        if((rc=dependon(argv[i]))!=0) return rc;

        /* recursively get more dependencies */
       // if((rc=moredepends())!=0) return rc;

        /* display all members of the tree */
        if((rc=showresults())!=0) return rc;
    }

    return 0;
}
