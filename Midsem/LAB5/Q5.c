#include <stdio.h>

struct Process {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int done;
};

int main () {
    int n;
    printf("Enter number of processes: ");
    scanf("%d",&n);

    struct Process p[n];

    for(int i=0;i<n;i++)
    {
        p[i].pid=i+1;
        p[i].done=0;

        printf("\nProcess %d\n",i+1);
        printf("Arrival Time: ");
        scanf("%d",&p[i].at);

        printf("Burst Time: ");
        scanf("%d",&p[i].bt);
    }

    int time = 0, completed = 0;
    float avgtat = 0, avgwt = 0;

    printf("\nGantt Chart:\n");

    while(completed < n) {
        int idx = -1;
        int mini = 1e5;
        for(int i = 0; i < n; i++) {
            if(p[i].at <= time && p[i].done == 0) {
                if(p[i].bt < mini) {
                    mini = p[i].bt;
                    idx = i;
                }
            }
        }


        if(idx == -1) {
            time++;
            continue;
        }

        printf("| P%d ",p[idx].pid);

        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;

        avgwt += p[idx].wt;
        avgtat += p[idx].tat;

        p[idx].done = 1;
        completed++;
    }

    printf("|\n\n");

    for(int i=0;i<n;i++)
        printf("P%d WT=%d TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);

    printf("\nAverage WT=%.2f",avgwt/n);
    printf("\nAverage TAT=%.2f",avgtat/n);

}