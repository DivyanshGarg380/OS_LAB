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
    Hostel Room Allocation using Semaphores

    Develop a C/C++ program to simulate a Hostel Room Allocation system using Operating System concepts with the following requirements:

        a) The Parent process reads the number of students along with their Student ID and request (arrival) times.

        b) The system has only 5 rooms available. The program should take input for the number of students requesting rooms.
            • If rooms are available, allocate them to students.
            • If no vacancy is available, display an appropriate message and make the student wait until a room becomes free.
            • Use semaphores to manage room allocation.

        c) Create child processes that dynamically allocate rooms to students in the order of their arrival times.
            • Ensure proper synchronization using semaphores during allocation and release of rooms.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ROOMS 5
sem_t rooms;

int main() {
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    int id[n], at[n];
    for(int i = 0; i < n; ++i) {
        printf("Enter Student ID and Arrival Time: ");
        scanf("%d %d", &id[i], &at[i]);
    }

    sem_init(&rooms, 1, ROOMS);
    for(int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if(pid == 0) {
            sleep(at[i]);
            printf("Student %d arrived\n", id[i]);

            if(sem_trywait(&rooms) == -1) {
                printf("No rooms available for Student %d. Waiting...\n", id[i]);
                sem_wait(&rooms);
            }

            printf("Room allocated to Student %d\n", id[i]);
            sleep(2);
            printf("Student %d leaving, room freed\n", id[i]);
            sem_post(&rooms);
            exit(0);
        }
    }

    for(int i = 0; i < n; ++i) wait(NULL);
    sem_destroy(&rooms);
    return 0;
}