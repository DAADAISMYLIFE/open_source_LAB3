#define MSGSIZE 16

int main(){
	char buf[MSGSIZE];
	int p[2], i;
	int pid;
	
	// 파이프로 설정 
	if(pipe(p) == -1){
		perror("pipe call failed\n");
		exit(1);
	}

	pid = fork();

	if(pid == 0){
		close(p[0]); // 자식 프로세스 입구 닫음 = 쓰는 역할만 한다는 뜻 
		
		for(i = 0; i < 2; i++){
			sprintf(buf, "Hello, World #%d", i+1);

			write(p[1], buf, MSGSIZE);
		}
	}
	else if(pid > 0){
		close(p[1]); // 부모 프로세스 출구 닫음 = 받는 역할만 한다는 뜻
		for(i = 0; i < 2; i++){
			read(p[0], buf, MSGSIZE);
			printf("%s\n", buf);
		}
	}
	else
		perror("fork failde");
}
