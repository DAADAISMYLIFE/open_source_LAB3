#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	// 1. execl 수행할 프로세스 생성 fork
	// 2. 매개변수 여러개 받을 수 있는 execvp로 명령 수행
	
	pid_t pid;

	pid = fork();

	// 자식 프로세스가 명령어 실행
	if(pid == 0){
		execvp(argv[1], &argv[1]);
		perror("명령이 실행되지 않아버렸다.");
	}
	else if(pid > 0){
		// 명령어 수행을 기다림  
		wait((int *)0);
		exit(0);
	}
	else{
		perror("fork error");
	}

}
