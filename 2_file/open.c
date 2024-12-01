#include <stdlib.h>
#include <fcntl.h>

#define PERMS 0644
char *workfile = "junk";

int main(){
	int filedes;

	if((filedes = open(workfile, O_RDWR | O_CREAT, PERMS))==-1){
		printf("Couldn't open %s\n", workfile);
		exit(1);
	}

	exit(0);
}
