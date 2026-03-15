#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

struct message {
	long type;
	int type;
	int hour;
	int minute;
};

int main() {
	int msgid;
	struct message msg;
	
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1) {
		printf("Queue creation failed\n");
		return 1;
	}

	pid_t pid = fork();
	
	if(pid > 0) {
		msg.type = 1;
		for(int i = 0; i < 6; i++) {
			printf("Enter number of users: ");
			scanf("%d", &msg.users);

			printf("Enter hour: ");
			scanf("%d", &msg.hour);
		
			printf("Enter minute: ");
			scanf("%d", &msg.minute);

			msgsnd(msgid, &msg, sizeof(msg), 0);
		}

		wait(NULL);
	}

	else {
		int total_users = 0;
		for(int i = 0; i < 6; ++i) {
			msgrcv(msgid, &msg, sizeof(msg), 1, 0);

			printf("Received: %d users at %d:%d\n", msg.users, msg.hour, msg.minute);
			
			total_users += msg.users;
		}
		
		float avg = total_users / 6.0;
		printf("\nAverage user in this hour: %.2f\n", avg);
	}

	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
