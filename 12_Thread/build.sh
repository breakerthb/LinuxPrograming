#!/usr/sh

cc -D_REENTRANT demo_thread1.c -o run_thread1 -lpthread

cc -D_REENTRANT demo_thread2.c -o run_thread2 -lpthread

cc -D_REENTRANT demo_thread3.c -o run_thread3 -lpthread

cc -D_REENTRANT demo_thread4.c -o run_thread4 -lpthread



cc -D_REENTRANT demo_thread7.c -o run_thread7 -lpthread

cc -D_REENTRANT demo_thread8.c -o run_thread8 -lpthread

cc -D_REENTRANT demo_thread8a.c -o run_thread8a -lpthread