#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>    
#include<sys/sem.h>
#include<fcntl.h>
#include<errno.h>

#include "data.h"

void time_increment();
void advanceClock(int msec);
void initClock();
void clearPCB(int loc);
void initRD();
void tandk();
int createChildProc();
void initPCB();

#define MILLI 1000


int pids[18];

