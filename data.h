#ifndef DATA_H
#define DATA_H


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "config.h"

struct resDescriptor{
    
    uint pids[18];
    uint res_req[20];
    uint alloc[18][20];
    uint req[18][20];
    uint available[20];

};

struct Clock{
    uint sec;
    uint msec;
};

struct PCB{
    
	int pid;
	int req[20];
};

#endif