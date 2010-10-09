/* 
%use buffer_0;
%use buffer_1;
%use buffer_2;
%use buffer_init;
%use buffer_put;
%use buffer_get;
%use critbit0_insert;
%use critbit0_contains;
%use critbit0_allprefixed;
%use critbit0_clear;
%use getln;
%use open_read;
%use str_start;
%use stralloc_copys;
%use stralloc_cats;
%use stralloc_append;
%use strerr_sys;
%use strerr_die;
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


static critbit0_tree modules;
static critbit0_tree nextup;
static critbit0_tree allmodules;
static critbit0_tree executables;
static limitmalloc_pool pool = { 4096 };
static stralloc line = {0};

/* File read buffer */
static int buffer_f_read(int fd,char *buf,int len);
static char buffer_f_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(buffer_f_read,-1,buffer_f_space,sizeof buffer_f_space);
static buffer *buffer_f = &it;
static int buffer_f_read(int fd, char *buf,int len)
{
  if (buffer_flush(buffer_f) == -1) return -1;
  return read(fd,buf,len);
}

#define puts(s) buffer_putsalign(buffer_1,(s))
#define putflush() buffer_flush(buffer_1)
#define put2s(s) buffer_putsalign(buffer_2,(s))
#define put2flush() buffer_flush(buffer_2)
#define oops() strerr_die1sys(errno,"oops: an error occured: ")

/* {m}.c */
static stralloc modc = {0};

static void err_readfailed(str0 dep)
{
    put2s("automaker: error: could not open '");
    put2s(dep);
    put2s("'\n");
    put2flush();
    _exit(111);
}

/* Read the file {m}.c and read all of its dependencies into the tree */
static int dependon(str0 m)
{
    int fd;
    int rc;
    int match;
    str0 newmod;

    /* No use in reading a module twice */
    if(critbit0_contains(&modules,&m)) return 0;

    /* Add the module to the tree */
    if(!critbit0_insert(&modules,0,&m)) return 1;
    if(!critbit0_insert(&allmodules,0,&m)) return 1;

    /* {m}.c is a new module */
    stralloc_copys(&modc,m);
    stralloc_cats(&modc,".c"); 
    stralloc_0(&modc);

    /* Open module source file */
    for(;;) {
        fd = open_read(modc.s);
        if(fd>=0) break;
        if(errno == error_intr)  { sleep(1); continue; }
        err_readfailed(modc.s);
    } 
    buffer_init(buffer_f,buffer_f_read,fd,buffer_f_space,sizeof buffer_f_space);

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
static int newcount;
static str0 moredepends_arg;
int moredepends_callback(void)
{
    ++newcount;
    if(dependon(moredepends_arg)!=0) return 0;
    return 1;
}
static int moredepends()
{
    int oldcount;
    str0 empty = "";
    oldcount = 0;
    newcount = 0;
    do {
      oldcount = newcount;
      newcount = 0;
      if(critbit0_allprefixed(&nextup, 0, &moredepends_arg, &empty, moredepends_callback)<0) oops();
    } while(newcount!=oldcount);
    return 0;
}

/* loadall */
static str0 loadall_arg;
static int loadall_callback(void)
{
    puts(" "); puts(loadall_arg); puts(".o");
    return 1;
}
static int loadall(str0 modname)
{
    str0 empty = "";

    /* First line */
    puts(modname); puts(" : load "); 
    puts(modname); puts(".o");
    if(critbit0_allprefixed(&nextup, 0, &loadall_arg, &empty, loadall_callback)<0) oops();
    puts("\n");

    /* Second line */
    puts("	./load "); puts(modname); 
    if(critbit0_allprefixed(&nextup, 0, &loadall_arg, &empty, loadall_callback)<0) oops();
    puts("\n");

    return 0;
}

/* compileall */
static str0 compileall_arg;
static int compileall_callback(void)
{
    puts(compileall_arg); puts(".o : compile "); puts(compileall_arg); puts(".c\n");
    puts("	./compile "); puts(compileall_arg); puts(".c\n");
    return 1;
}
static int compileall()
{
    str0 empty = "";
    if(critbit0_allprefixed(&allmodules, 0, &compileall_arg, &empty, compileall_callback)<0) oops();
    return 0;
}

/* itall */
static str0 itall_arg;
static int itall_callback(void)
{
    puts(" "); puts(itall_arg);
    return 1;
}
static int itall()
{
    str0 empty = "";
    puts("it :");
    if(critbit0_allprefixed(&executables, 0, &itall_arg, &empty, itall_callback)<0) oops();
    puts("\n");
    return 0;
}

/* cleanall */
static str0 cleanall_arg;
static int cleanall_callback(void)
{
    puts(" ");
    puts(cleanall_arg);
    return 1;
}
static int cleanall()
{
    str0 empty = "";
    puts("clean : \n	rm -f *.o");
    if(critbit0_allprefixed(&executables, 0, &cleanall_arg, &empty, cleanall_callback)<0) oops();
    puts("\n");
    return 0;
}

int main(int argc, char*argv[])
{
    int i,len,rc;
    char *p;
    if(argc<=1) {
        put2s("automaker: usage: automaker [files ...]\n");
        put2flush();
        return 100;
    }
    puts("default : it\n");
    for(i=1;i<argc;i++)
    {
        /* grow a new tree */
        critbit0_clear(&modules,&pool);
        critbit0_clear(&nextup,&pool);

        /* chop off .c suffix */
        p = argv[i];
        len = str0_length(&p);
        if((len > 2 
          && p[len-2] == '.' 
          && p[len-1] == 'c')) 
        {
          p[len-2] = 0;
          if(!critbit0_contains(&executables,&p))
            if(!critbit0_insert(&executables,&pool,&p)) return 1;
        }

        /* add module to tree along with its dependents */
        if((rc=dependon(argv[i]))!=0) return rc;

        /* recursively get more dependencies */
        if((rc=moredepends())!=0) return rc;

        /* list all executable modules to load them */
        if((rc=loadall(argv[i]))!=0) return rc;

    }



    /* list all modules to compile them */
    if((rc=compileall())!=0) return rc;

    /* make it */
    if((rc=itall())!=0) return rc;

    /* make clean */
    if((rc=cleanall())!=0) return rc;

    /* Cleanup time */
    critbit0_clear(&executables,&pool);
    critbit0_clear(&modules,&pool);
    critbit0_clear(&allmodules,&pool);
    critbit0_clear(&nextup,&pool);

    putflush();

    return 0;
}
