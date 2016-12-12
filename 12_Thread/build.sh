#!/usr/sh

cc -D_REENTRANT demo_thread1.c -o run_thread1 -lpthread

cc -D_REENTRANT demo_thread2.c -o run_thread2 -lpthread