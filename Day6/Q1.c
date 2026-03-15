#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

struct message {
	long int type;
	int number;
};

int isPalindrome(int num) {
	int original = num;
	int reversed = 0;
	while(num != 0) {
		int digit = num % 10;
		reversed = reversed * 10 + digit;
		num /= 10;
	}

	return (original == reversed);
}

int main() {
	int msgid;
	struct message msg;
	
	msgid = msgget((key_t)1234, 0666 } IPC_CREAT);
	if(msg == -1) {
		printf("Message queue creation failed\n");
		exit(1);
	}

	pid_t pid = fork();
	
	if(pid > 0) { // Parent Process
		msg.type = 1;
		printf("Parent: Enter a number");
		scanf("%d", &msg.number);
		
		msgsnd(msgid, &msg, sizeof(msg.number), 0);
		printf("Parent: Number sent to child\n");
		wait(NULL);
	}

	else { // Child Process
		msgrcv(msgid, &msg, sizeof(msg.number), 1, 0);
		printf("Child: Recieved number = %d\n", msg.number);
	
		if(isPalindrome(msg.number)) printf("Child: Number is Palindrome\n");
		else printf("Child: Number not a palindrome\n");
	}

	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
