#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int i = 0;
void p_handler (int), c_handler(int);

int main(){
	pid_t pid, ppid;
	struct sigaction act;

	pid = fork();
	if(pid == 0){	// 자식 프로세스의 경우
		act.sa_handler = c_handler;
		sigaction(SIGUSR1, &act, NULL);
		ppid = getppid(); //부모 프로세스 pid 얻음

		while(1){
			sleep(1);
			kill(ppid, SIGUSR1); // 부모 프로세스에 시그널 전송
			pause(); // 신호 올때까지 대기 
		}
	}
	else if(pid > 0){	// 부모 프로세스의 경우
		act.sa_handler = p_handler;
		sigaction(SIGUSR1, &act, NULL);
		while(1){
			pause(); // 신호 올때까지 대기
			sleep(1);
			kill(pid, SIGUSR1);	
		}
	}
	else{
		perror("Error");
	}
}


void p_handler(int signo){
	printf("Parent handler: call %d times.\n",++i);
}

void c_handler(int signo){
	printf("Child handler: call %d times.\n", ++i);
}
