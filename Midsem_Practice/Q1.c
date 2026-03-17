/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

/*
    Parent reads: PID, AT, BT, Priority
    Use: fork(), exec()
    Child executes: premPriority
    Parent uses: wait()
    Display process table
*/


// Parent Process
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct Process {
    int pid, at, bt, pr;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], pr[n];
    for(int i = 0; i < n; ++i) {
        printf("Enter PID AT BT Priority: ");
        scanf("%d %d %d %d", &pid[i], &at[i], &bt[i], &pr[i]);
    }
    pid_t p = fork();
    switch(p) {
        case -1:
            printf("Fork failed\n");
            break;

        case 0:
            execl("./premPriority", "premPriority", NULL);
            perror("exec failed");
            break;

        default:
            wait(NULL);
            printf("Parent displaying data\n");
            for(int i = 0; i < n; i++) {
                printf("%d %d %d %d\n", pid[i], at[i], bt[i], pr[i]);
            }
    }
    return 0;
}

// PremPriority.c
#include <stdio.h>
int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], pr[n];
    int ct[n], tat[n], wt[n], rt[n];

    for(int i = 0; i < n; ++i){
        printf("Enter PID AT BT Priority: ");
        scanf("%d %d %d %d", &pid[i], &at[i], &bt[i], &pr[i]);
        rt[i] = bt[i];
    }

    int completed = 0, time = 0, mini , idx;
    while(completed != n) {
        mini = 9999;
        idx = -1;

        for(int i = 0; i < n; ++i) {
            if(at[i] <= time && rt[i] > 0 && pr[i] < mini) {
                mini = pr[i];
                idx = i;
            }
        }

        if(idx != -1) {
            rt[idx]--;
            time++;

            if(rt[idx] == 0) {
                ct[idx] = time;
                completed++;
            }
        } else {
            time++;
        }
    }

    for(int i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    printf("\nPID AT BT PR CT TAT WT\n");
    for(int i = 0; i < n; i++) {
        printf("%d %d %d %d %d %d %d\n",
            pid[i], at[i], bt[i], pr[i],
            ct[i], tat[i], wt[i]);
    }

    return 0;
}