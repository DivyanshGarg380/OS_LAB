#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int at;
    int bt;
    int deadline;
    int ct;
    int tat;
    int wt;
};

void edf(struct Process p[], int n){
    int time = 0;
    float avgwt = 0, avgtat = 0;
    int completed = 0;
    int rem[10];

    for(int i = 0; i < n; i++) rem[i] = p[i].bt;

    printf("\nGantt Chart:\n");

    while(completed < n) {
        int idx = -1;
        int mini = 1e6;

        // choose earlist deadline process
        for(int i = 0; i < n; i++) {
            if(p[i].at <= time && rem[i] > 0) {
                if(p[i].deadline < mini) {
                    mini = p[i].deadline;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            time++;
            continue;
        }

        printf("|P%d", p[idx].pid);
        rem[idx]--;
        time++;

        if(rem[idx] == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;

            avgwt += p[idx].wt;
            avgtat += p[idx].tat;
            completed++;
        }

    }
    
    printf("|\n\n");

    for(int i=0;i<n;i++)
    {
        printf("P%d WT=%d TAT=%d\n", p[i].pid, p[i].wt, p[i].tat);
    }

    printf("\nAverage WT = %.2f\n", avgwt/n);
    printf("Average TAT = %.2f\n", avgtat/n);
}

int main()
{
    struct Process p[4] = {
        {1,0,5,12},
        {2,1,3,7},
        {3,2,4,10},
        {4,3,2,6}
    };

    edf(p,4);
}