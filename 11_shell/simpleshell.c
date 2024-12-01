#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

pid_t pid;

// 명령어와 인자를 구분하는 함수
int getargs(char *cmd, char **argv);

// 시그널 처리 함수 
void handler(int signo);

int main(){
	char buf[256];
	char *argv[50];
	int narg;
	int is_background;
	int is_error;

	// 부모 프로세스(쉘)은 인터럽트 시그널 무시하고 자식 프로세스에게 시그널 전송
	struct sigaction p_act;
	p_act.sa_handler = handler;
	sigfillset(&(p_act.sa_mask));
	sigaction(SIGINT, &p_act, NULL);

	while(1){
		is_error = 0;
		// 사용자 명령 입력 
		printf("shell> ");
		gets(buf);
		clearerr(stdin);

		is_background = 0;
		// 명령어와 인자를 구분
		narg = getargs(buf, argv);
		if(narg == 0){
			continue;
		}
			
		// 1. exit 이면 탈출할 것
		if(strcmp(argv[0], "exit") == 0)
			exit(0);

		// 2. 마지막 인자가 &이면 백그라운드 작업해야함
		// 	- 인자하나 없얘야함.
		if(strcmp(argv[narg - 1], "&") == 0){
			argv[narg--] = NULL;
			is_background = 1;
		}


		// 3. 리다이렉션 및 파이프
		int i;
		int fd = -1;
		
		// 백업용 fd
		int fd_in = dup(0);
		int fd_out = dup(1);
		int fd_err = dup(2);
		
		for(i = 0; i < narg; i++){
			if(strcmp(argv[i], ">") == 0){	
				if((fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
					perror("open");
					exit(1);
				}
				if((dup2(fd, 1) == -1)){
					perror("dup2");
					exit(1);
				}
				argv[i] = NULL;
				break;
			}
			else if(strcmp(argv[i], ">>") == 0){
				if((fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1){
					perror("open");
					exit(1);
				}
				if((dup2(fd, 1) == -1)){
					perror("dup2");
					exit(1);
				}
				argv[i] = NULL;
				break;
			}	
			else if(strcmp(argv[i], "<") == 0){
				if((fd = open(argv[i + 1], O_RDONLY, 0644)) == -1){
					perror("open");
					is_error = 1;
					break;
				}
				if((dup2(fd, 0) == -1)){
					perror("dup2");
					exit(1);
				}
				argv[i] = NULL;
				break;
			}
		}
		
		// 혹시 이전에 에러나오면 명령어 수행하지 말기
		if(is_error == 1)
			continue;

		// 명령어 수행 과정 
		// 자식 프로세스 생성
		pid = fork();

		if(pid == 0){
			// 자식 프로세스는 명령어 수행
			// 명령어 수행
			execvp(argv[0], argv);
			perror("명령어 수행 불가");
			exit(1);
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
		
		// 리다이렉션 후 원래 파일 복구 및  파일 디스크립터 닫기
		if (fd != -1){
			
			if((dup2(fd_in, 0) == -1) || (dup2(fd_out, 1) == -1) || (dup2(fd_err, 2) == -1)){
                        	perror("dup2");
                        	exit(1);
                        }

			if((close(fd) == -1) || (close(fd_in) == -1) || (close(fd_out) == -1) || (close(fd_err) == -1)) {
    				perror("close");
    				exit(1);
			}
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

// 자식에게 신호 전달
void handler(int signo){
	if(pid > 0)
		kill(pid, signo);
}
