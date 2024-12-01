#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_READ 10

int main(int argc, char *argv[]){
	int src_fd;
	int des_fd;
	
	char buf[MAX_READ];
	ssize_t rcnt;
	ssize_t tot_rcnt = 0;

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	if (argc < 3){
		fprintf(stderr, "Usage : file_copy src_file des_file\n");
		exit(1);
	}

	if ((src_fd = open(argv[1], O_RDONLY))==-1){
		perror("src open");
		exit(1);
	}

	if ((des_fd = creat(argv[2],mode))==-1){
		perror("des open");
		exit(1);
	}

	while((rcnt = read(src_fd, buf, MAX_READ)) > 0){
		tot_rcnt += write(des_fd, buf, rcnt);

	}

	if(rcnt < 0){
		perror("read");
		exit(1);
	}

	printf("total write count = %ld\n", tot_rcnt);
	close(src_fd);
	close(des_fd);


}
