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
    Write a C program where:

    - Parent
        - Creates message queue (key_t)1234
        - Inputs:
            - n processes
            - Arrival Time (AT) and Burst Time (BT)
            - Time quantum q
            - Sends all processes to child using message queue (mtype = 1)
    
    - Child (Scheduler Process)
        - Receives all processes
        - Implements Round Robin Scheduling WITH Arrival Time
        - Uses 1 thread (simpler, realistic)
    
    - Thread Task:
        - Maintain:
            - time
            - remaining[]
        - At each step:
            - Only schedule processes where AT <= time
            - Execute for q
        - Update:
            - Remaining time
            - Completion Time (CT)
    
    - After completion:
        - Compute:
            - TAT = CT - AT
            - WT = TAT - BT
    
        - Send results to parent (mtype = 2)
    
    - Parent
        - Receives results and prints output
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/wait.h>

#define MAX 10
struct msgbuf {
    long type;
    int pid;
    int at;
    int bt;
};

struct result {
    long type;
    int pid;
    int ct, tat, wt;
};

int n, q;
int at[MAX], bt[MAX], rem[MAX], ct[MAX];

void *rr_scheduler(void *arg) {
    int time = 0, completed = 0;
    while(completed < n) {
        int found = 0;
        for(int i = 0; i < n; ++i) {
            if(at[i] <= time && rem[i] > 0) {
                found = 1;
                int exec = (rem[i] > q) ? q : rem[i];
                rem[i] -= exec;
                time += exec;

                if(rem[i] == 0) {
                    ct[i] = time;
                    completed++;
                }
            }
        }

        if(!found) time++;
    }
    return NULL;
}

int main() {
    int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    printf("Enter n: ");
    scanf("%d", &n);

    struct msgbuf msg;

    printf("Enter AT and BT:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &msg.at, &msg.bt);
        msg.type = 1;
        msg.pid = i + 1;
        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
    }

    printf("Enter quantum: ");
    scanf("%d", &q);
    
    pid_t pid = fork();

    if(pid == 0) {
        for(int i = 0; i < n; ++i) {
            msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0);
            at[i] = msg.at;
            bt[i] = msg.bt;
            rem[i] = bt[i];
        }

        pthread_t t;
        pthread_create(&t, NULL, rr_scheduler, NULL);
        pthread_join(t, NULL);

        struct result res;
        for (int i = 0; i < n; i++) {
            res.mtype = 2;
            res.pid = i + 1;
            res.ct = ct[i];
            res.tat = ct[i] - at[i];
            res.wt = res.tat - bt[i];

            msgsnd(msgid, &res, sizeof(res) - sizeof(long), 0);
        }
        exit(0);
    }

    // parent
    struct result res;
    for(int i = 0; i < n; ++i) {
        msgrcv(msgid, &res, sizeof(res) - sizeof(long), 2, 0);
        printf("P%d → CT=%d TAT=%d WT=%d\n", res.pid, res.ct, res.tat, res.wt);
    }

    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);

}