#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  int fd;
  u_char k = 1;
  if ((fd = open("fileA",O_CREAT|O_TRUNC|O_RDWR,0666)) > -1) {
    write(fd,&k,1);
    lseek(fd,10000,SEEK_SET);
    write(fd,&k,1);
    lseek(fd,4*1024*1024,SEEK_SET);
    write(fd,&k,1);
    close(fd);
  }
  return 0;
}