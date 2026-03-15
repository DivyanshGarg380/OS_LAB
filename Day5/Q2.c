#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
};

void mlfq(struct Process p[], int n) {
    int time = 0;
    int q1 = 4;
    int q2 = 8;

    int rem[10];
    for(int i = 0; i < n; ++i) rem[i] = p[i].bt;
    
    printf("\nGantt Chart:\n");

    // Queue 1
    for(int i = 0; i < n; ++i) {
        if(rem[i] > 0) {
            printf("| P%d ", p[i].pid);

            if(rem[i] > q1) {
                time += q1;
                rem[i] -= q1;
            } else {
                time += rem[i];
                rem[i] = 0;
                p[i].ct = time;
            }
        }
    }

    // Queue 2
    for(int i = 0; i < n; ++i) {
        if(rem[i] > 0) {
            printf("| P%d ",p[i].pid);

            if(rem[i] > q2) {
                time += q2;
                rem[i] -= q2;
            } else {
                time += rem[i];
                rem[i] = 0;

                p[i].ct = time;
            }
        }
    }

    // FCFS
    for(int i = 0; i < n; ++i){
        if(rem[i] > 0) {
            printf("| P%d ",p[i].pid);
            time += rem[i];
            rem[i] = 0;

            p[i].ct = time;
        }
    }

    printf("|\n\n");

    float avgwt = 0;
    float avgtat = 0;

    for(int i=0;i<n;i++)
    {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt  = p[i].tat - p[i].bt;

        avgwt += p[i].wt;
        avgtat += p[i].tat;

        printf("P%d WT=%d TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);
    }

    printf("\nAverage WT = %.2f\n",avgwt/n);
    printf("Average TAT = %.2f\n",avgtat/n);
}

int main()
{
    struct Process p[4] =
    {
        {1,0,20},
        {2,1,15},
        {3,2,10},
        {4,3,5}
    };

    mlfq(p,4);
}