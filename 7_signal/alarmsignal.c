#include <stdio.h>
#include <signal.h>

void alarm_handler(int);

int alarm_flag = 0;

int main(){
	struct sigaction act;

	act.sa_handler = alarm_handler;
	sigaction(SIGALRM, &act, NULL);

	alarm(5);	// 5초 후 자신에게 시그널
	pause();	// 대기 
	if(alarm_flag)
		printf("Passed a 5 sescs.\n");

}

void alarm_handler(int sig){
	printf("Received a alarm signal.\n");
	alarm_flag = 1;
}
