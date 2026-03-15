#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int at;
    int bt;
    int pr;
    int ct;
    int tat;
    int wt;
};

void fcfs(struct Process p[], int n) {
    int time = 0;
    float avgwt = 0, avgtat = 0;

    printf("\nGantt Chart:\n");
    for(int i = 0; i < n; ++i) {
        if(time < p[i].at) time = p[i].at;

        printf("| P%d ", p[i].pid);

        time += p[i].bt;
        p[i].ct = time;

        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        avgwt += p[i].wt;
        avgtat += p[i].tat;

    }

    printf("|\n\n");
    for(int i = 0; i < n; ++i) {
        printf("P%d WT=%d TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);
    }
    printf("Average WT = %.2f\n", avgwt/n);
    printf("Average TAT = %.2f\n", avgtat/n);
}

// Priority non premmptive scheduling
void priority(struct Process p[], int n) {
    int done[10] = {0};
    int completed = 0, time = 0;
    float avgwt = 0, avgtat = 0;

    printf("\nGantt Chart:\n");

    while(completed < n) {
        int idx = -1;
        int best = -1;

        for(int i = 0; i < n; ++i) {
            if(!done[i] && p[i].at <= time) {
                if(p[i].pr > best) {
                    best = p[i].pr;
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

        done[idx] = 1;
        completed++;
    }

    printf("|\n\n");

    for(int i=0;i<n;i++)
        printf("P%d WT=%d TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);

    printf("Average WT=%.2f\n",avgwt/n);
    printf("Average TAT=%.2f\n",avgtat/n);
}

// Round Robin
void roundRobin(struct Process p[], int n){
    int time = 0, done = 0;
    int rem[10];
    float avgwt = 0, avgtat = 0;
    int quantum = 10;

    for(int i = 0; i < n; i++) rem[i] = p[i].bt;
    printf("\nGantt Chart:\n");

    while(done < n) {
        int executed = 0;
        for(int i = 0; i < n; ++i) {
            if(rem[i] > 0 && p[i].at <= time) {
                executed = 1;
                printf("| P%d ",p[i].pid);

                if(rem[i] > quantum) {
                    time += quantum;
                    rem[i] -= quantum;
                }

                else {
                    time += rem[i];
                    rem[i] = 0;

                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt  = p[i].tat - p[i].bt;

                    avgwt += p[i].wt;
                    avgtat += p[i].tat;

                    done++;
                }

            }
        }

        if(!executed) time++;
    }

    printf("|\n\n");

    for(int i=0;i<n;i++)
        printf("P%d WT=%d TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);

    printf("Average WT=%.2f\n",avgwt/n);
    printf("Average TAT=%.2f\n",avgtat/n);
}

// Shortest Time remaining first SRTF
void srtf(struct Process p[], int n) {
    int time = 0;
    int completed = 0;
    int rem[10];
    float avgtat = 0, avgwt = 0;

    for(int i = 0; i < n; ++i) rem[i] = p[i].bt;

    printf("\nGantt Chart:\n");

    while(completed < n) {
        int idx = -1;
        int mini = 1e5;

        for(int i = 0; i < n; ++i) {
            if(p[i].at <= time && rem[i] > 0) {
                if(rem[i] < mini) {
                    mini = rem[i];
                    idx = i;
                }
            }
        }

        if(idx == -1){
            time++;
            continue;
        }

        printf("| P%d ",p[idx].pid);

        rem[idx]--;
        time++;

        if(rem[idx] == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            avgwt += p[idx].wt;
            avgtat += p[idx].tat;

            completed++;
        }
    }

    printf("|\n\n");

    for(int i=0;i<n;i++)
    {
        printf("P%d  WT=%d  TAT=%d\n",p[i].pid,p[i].wt,p[i].tat);
    }

    printf("\nAverage WT = %.2f\n",avgwt/n);
    printf("Average TAT = %.2f\n",avgtat/n);

}

int main()
{
    int choice;

    struct Process p[4] = {
        {1,0,60,3},
        {2,3,30,2},
        {3,4,40,1},
        {4,9,10,4}
    };

    printf("1.FCFS\n");
    fcfs(p,4);
    printf("2.Round Robin\n");
    roundRobin(p,4);
    printf("3.Priority\n");
    priority(p,4);
    printf("4.SRTF\n");
    srtf(p,4);

    return 0;
}