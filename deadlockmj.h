#ifndef DEADLOCKMJ_H
#define DEADLOCKMJ_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/wait.h>

int req_lt_available( int **req, int *avail, int pid, int num_res);
int deadlockmj(int *available, int m, int n, int **request, int **allocated,int *pids);
void copySHMData(int *available,int **request, int **allocated, int *pids);

FILE *fptr;

int available_[20];
int request_[18][20];
int allocated_[18][20];
int pids_[18];

int req_lt_available( int **req, int *avail, int pid, int num_res)
{
    int i;
    for(i =0;i<num_res;i++)
    {
        //printf("\nreq:%d avail:%d num_res : %d i : %d\n",request_[pid][num_res],available_[i],num_res,i);
        if(request_[pid][num_res] > available_[i])
        {
            //printf("\nreq:%d avail:%d num_res : %d PID : %d i : %d\n",request_[pid][num_res],available_[i],num_res,pids_[i],i);
            break;
        } else{
             allocated_[pid][i] = request_[pid][num_res];
             available_[i] -= request_[pid][num_res];
            // printf("\nreq:%d avail:%d num_res : %d PID : %d i : %d\n",request_[pid][num_res],available_[i],num_res,pids_[i],i);
        }
    }
    return (i==num_res);
}

int deadlockmj(int *available, int m, int n, int **request, int **allocated,int *pids)
{
    int i;
    int work[m];                //No.of Resources
    int finish[n];              //No.of processes

    for(i=0;i<m;work[i]=available_[i++]);
    for(i=0;i<n;finish[i++]=0);


    //printf("\n Request[5] : %d",request_[5][5]);
    int p=0;
    for(p;p<n;p++)    // For each process
    {
        if(finish[p]) continue;
        //printf("\n PID is : %d",pids_[p]);
        if(req_lt_available(request_,&work,p,m))
        {
            //printf("\n Finished PID %d\n",pids_[p]);
            finish[p] = 1;
            for(i=0;i<m;i++)
            {
                work[i] += allocated_[p][i];
            }
            p=-1;
        }   
    }
    
    for(p=0;p<n;p++)
    {
        if(!finish[p])
        {
            break;
        }
    }


    return (p != n);
}



#endif