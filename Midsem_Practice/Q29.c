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
    Write a C program using System V Message Queues (msgsnd, msgrcv) such that:

    - Parent creates a message queue using key (key_t)1234.
    - Parent creates 2 child processes:
        - Child 1 (Producer)
            - Reads N integers from user
            - Sends each number to the queue with:
                - mtype = 1
        
        - Child 2 (Processor)
            - Receives numbers from queue (mtype = 1)
            - For each number:
                - If even → send back with mtype = 2
                - If odd → send back with mtype = 3
        
        - Parent
            - Receives processed messages:
                - mtype = 2 → even numbers
                - mtype = 3 → odd numbers
            - Computes:
                - Sum of even numbers
                - Sum of odd numbers
        
        - Termination Condition
        - After all numbers are processed:
            - Child 1 sends a special value -1
            - Child 2 forwards termination to parent
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX 100

struct msgbuf {
    long mtype;
    int data;
};

int main() {
    int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    pid_t pid1 = fork();
    if(pid1 == 0) {
        // producer  
        struct msgbuf msg;
        int n, x;
        printf("Enter number of elements: ");
        scanf("%d", &n);

        for(int i = 0; i < n; ++i) {
            scanf("%d", &x);
            msg.mtype = 1;
            msg.data = x;
            msgsnd(msgid, &msg, sizeof(int), 0);
        }

        msg.data = -1;
        msgsnd(msgid, &msg, sizeof(int), 0);
        exit(0);
    }

    pid_t pid2 = fork();
    if(pid2 == 0) {
        // consumer
        struct msgbuf msg;
        while(1) {
            msgrcv(msgid, &msg, sizeof(int), 1, 0);
            if(msg.data == -1) {
                msg.mtype = 2;
                msgsnd(msgid, &msg, sizeof(int), 0);
                msg.mtype = 3;
                msgsnd(msgid, &msg, sizeof(int), 0);
                break;
            }

            if(msg.data % 2 == 0) msg.mtype = 2;
            else msg.mtype = 3;
            msgsnd(msgid, &msg, sizeof(int), 0);
        }
        exit(0);
    }

    //parent
    struct msgbuf msg;
    int even = 0, odd = 0;
    int doneEven = 0, doneOdd = 0;

    while(!(doneEven && doneOdd)) {
        msgrcv(msgid, &msg, sizeof(int), 0, 0);
        if(msg.type == 1) continue;
        if(msg.data == -1) {
            if(msg.mtype == 2) doneEven = 1;
            if(msg.mtype == 3) doneOdd = 1;
            continue;
        }
        if(msg.mtype == 2) {
            even += msg.data;
        } else if(msg.mtype == 3) {
            odd += msg.data;
        }
    }

    printf("Even sum = %d\n", even);
    printf("Odd sum %d\n", odd);
    wait(NULL);
    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}