#include <unistd.h>

int main(){
	printf("Executing execl.\n");

	execl("/bin/ls", "ls", "-l", (char*)0);
	perror("이게 왜 됐노..;;");
	exit(1);
}
