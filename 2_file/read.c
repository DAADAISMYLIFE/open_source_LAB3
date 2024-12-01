#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void){
        int fd;
        ssize_t n1, n2;
        char buf1[512], buf2[512];

        if((fd = open("foo", O_RDONLY))==-1){
                printf("파일이 없습니다.\n");
                exit(1);
        }

        n1 = read(fd, buf1, 512);
        n2 = read(fd, buf2, 512);

        printf("n1 : %s, n2 : %s\n", buf1, buf2);

        return 0;
}
