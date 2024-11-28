#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// 명령어와 인자를 구분하는 함수
int getargs(char *cmd, char **argv);

int main(){
	char buf[256];
	char *argv[50];
	int narg;
	pid_t pid;
	int is_background;

	while(1){
		// 사용자 명령 입력 
		printf("shell> ");
		gets(buf);
		clearerr(stdin);

		is_background = 0;
		// 명령어와 인자를 구분
		narg = getargs(buf, argv);
			
		// 1. exit 이면 탈출할 것
		if(strcmp(argv[0], "exit") == 0)
			exit(0);

		// 2. 마지막 인자가 &이면 백그라운드 작업해야함
		// 	- 인자하나 없얘야함.
		if(strcmp(argv[narg - 1], "&") == 0){
			argv[narg-1] = NULL;
			is_background = 1;
		}
		// 자식 프로세스 생성
		pid = fork();

		if(pid == 0){
			// 자식 프로세스는 명령어 수행
			// 명령어 수행
			execvp(argv[0], argv);
		}
		else if(pid > 0){

			// 백그라운드 작업이 아닐 경우 
			// 부모 프로세스는 자식의 동작을 대기
			if(is_background == 0)
				wait((int *)0);
			else
				printf("pid : %d\n", pid);
		}
		else{
			perror("fork failed");
		}
	}
}

int getargs(char *cmd, char **argv){
	int narg = 0;

	while(*cmd){
		if(*cmd == ' ' || *cmd == '\t')
			*cmd++ = '\0';
		else{
			argv[narg++] = cmd++;
			while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
				cmd++;
		}
	}

	argv[narg] = NULL;
	return narg;
}
