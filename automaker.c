/* 
%use buffer_0;
%use buffer_1;
%use buffer_2;
%use buffer_init;
%use buffer_put;
%use buffer_get;
%use buffer_read;
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
#include "exit.h"
#include "str.h"

#define puts(s) buffer_putsalign(buffer_1,(s))
#define putflush() buffer_flush(buffer_1)
#define FATAL "automaker: error: "

static critbit0_tree modules;
static critbit0_tree nextup;
static critbit0_tree allmodules;
static critbit0_tree executables;
static limitmalloc_pool pool = { 65536 };
static stralloc line = {0}; 
static stralloc modc = {0}; 
static char buffer_f_space[BUFFER_INSIZE];
static buffer buffer_f;

static void cleanup()
{
    critbit0_clear(&executables,&pool);
    critbit0_clear(&modules,&pool);
    critbit0_clear(&allmodules,&pool);
    critbit0_clear(&nextup,&pool);
}

static void err_readfailed(str0 dep)
{
    strerr_die4sys(111,FATAL,"failed to open '",dep,"': ");
}
static void err_memsoft()
{
    strerr_die2x(111,FATAL,"memory limit exceeded");
}
static void err_memhard()
{
    _exit(111);
}
static void forceclose(int fd)
{
    if(close(fd)==-1) 
    {
      strerr_die2sys(111,FATAL,"failed to close file descriptor: ");
    }
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
    if(!critbit0_insert(&modules,&pool,&m)) err_memsoft();
    if(!critbit0_insert(&allmodules,&pool,&m)) err_memsoft();

    /* {m}.c is a new module */
    if(!stralloc_copys(&modc,m)) err_memhard();
    if(!stralloc_cats(&modc,".c")) err_memhard(); 
    if(!stralloc_0(&modc)) err_memhard();

    /* Open module source file */
    for(;;) {
        fd = open_read(modc.s);
        if(fd>=0) break;
        if(errno == error_intr)  { sleep(1); continue; }
        err_readfailed(modc.s);
    } 
    buffer_init(&buffer_f,buffer_unixread,fd,buffer_f_space,sizeof buffer_f_space);

    /* Read first line */
    rc = getln(&buffer_f,&line,&match,'\n');
    if(rc<0) { forceclose(fd); return 1; }

    /* Make sure first line is a comment start */
    if(!str_start(line.s,"/*")) { forceclose(fd); return 0; }

    for(;;) {
        
      /* Read next line */
      rc = getln(&buffer_f,&line,&match,'\n');
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
        if(!critbit0_insert(&nextup,&pool,&newmod)) err_memsoft();
    } 

    /* Done reading this file */
    forceclose(fd);
    return 0;
}

/* moredepends */
static int newcount;
static str0 moredepends_arg;
int moredepends_callback(void)
{
    ++newcount;
    if(dependon(moredepends_arg)!=0) return 0;
    str0_free(&moredepends_arg,&pool);
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
      if(critbit0_allprefixed(&nextup, &pool, &moredepends_arg, &empty, moredepends_callback)!=1) 
          err_memsoft();
    } while(newcount!=oldcount);
    return 0;
}

/* loadall */
static str0 loadall_arg;
static int loadall_callback(void)
{
    puts(" "); puts(loadall_arg); puts(".o");
    str0_free(&loadall_arg,&pool);
    return 1;
}
static int loadall(str0 modname)
{
    str0 empty = "";

    /* First line */
    puts(modname); puts(" : load "); 
    puts(modname); puts(".o");
    if(critbit0_allprefixed(&nextup, &pool, &loadall_arg, &empty, loadall_callback)!=1)
          err_memsoft();
    puts("\n");

    /* Second line */
    puts("	./load "); puts(modname); 
    if(critbit0_allprefixed(&nextup, &pool, &loadall_arg, &empty, loadall_callback)!=1)
          err_memsoft();
    puts("\n");

    return 0;
}

/* compileall */
static str0 compileall_arg;
static int compileall_callback(void)
{
    puts(compileall_arg); puts(".o : compile "); puts(compileall_arg); puts(".c\n");
    puts("	./compile "); puts(compileall_arg); puts(".c\n");
    str0_free(&compileall_arg,&pool);
    return 1;
}
static int compileall()
{
    str0 empty = "";
    if(critbit0_allprefixed(&allmodules, &pool, &compileall_arg, &empty, compileall_callback)!=1) 
          err_memsoft();
    return 0;
}

/* itall */
static str0 itall_arg;
static int itall_callback(void)
{
    puts(" "); puts(itall_arg);
    str0_free(&itall_arg,&pool);
    return 1;
}
static int itall()
{
    str0 empty = "";
    puts("it :");
    if(critbit0_allprefixed(&executables, &pool, &itall_arg, &empty, itall_callback)!=1) 
          err_memsoft();
    puts("\n");
    return 0;
}

/* cleanall */
static str0 cleanall_arg;
static int cleanall_callback(void)
{
    puts(" ");
    puts(cleanall_arg);
    str0_free(&cleanall_arg,&pool);
    return 1;
}
static int cleanall()
{
    str0 empty = "";
    puts("clean : \n	rm -f *.o");
    if(critbit0_allprefixed(&executables, &pool, &cleanall_arg, &empty, cleanall_callback)!=1)
          err_memsoft();
    puts("\n");
    return 0;
}

int main(int argc, char*argv[])
{
    int i,len,rc;
    char *p;
    if(argc<=1) {
        strerr_die1x(100,"automaker: usage: automaker [files ...]");
    }

    puts("default : it\n");
    puts("load :\n" 
        "	echo '#!/bin/sh' > load\n"
        "	echo 'main=\"$$1\"; shift' >> load\n"
        "	echo 'exec cc -o \"$$main\" \"$$main\".o $${1+\"$$@\"}' >> load\n"
        "	chmod +x load\n");
    puts("compile :\n"
        "	echo '#!/bin/sh' > compile\n"
        "	echo 'exec cc -c $${1+\"$$@\"}' >> compile\n"
        "	chmod +x compile\n");

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
        }
        if(!critbit0_contains(&executables,&p))
            if(!critbit0_insert(&executables,&pool,&p)) return 1;

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
    cleanup();

    putflush();

    return 0;
}
