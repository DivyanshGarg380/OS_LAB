#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int bt;
    int ct;
    int tat;
    int wt;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d",&n);
    struct Process p[n];

    for(int i=0;i<n;i++){
        p[i].pid=i+1;
        printf("Enter Burst Time for P%d: ",i+1);
        scanf("%d",&p[i].bt);
    }

    int time = 0;
    float avgtat = 0, avgwt = 0;
    printf("\nGantt Chart:\n");

    for(int i = 0; i < n; ++i){
        printf("| P%d ", p[i].pid);

        time += p[i].bt;
        p[i].ct = time;

        p[i].tat = p[i].ct;
        p[i].wt = p[i].tat - p[i].bt;

        avgwt += p[i].wt;
        avgtat += p[i].tat;
    }
    
    printf("|\n\n");

    for(int i=0;i<n;i++)
        printf("P%d WT=%d TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);

    printf("\nAverage WT=%.2f",avgwt/n);
    printf("\nAverage TAT=%.2f",avgtat/n);
}
