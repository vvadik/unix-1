#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int bsize = 4096;
u_char *block;
int n_block = 0, n_zero = 0;
int fds, fdd;

void push_nonzero() {
  write(fdd,block,n_block);
  n_block = 0;
}

void add_nonzero(u_char k) {
  block[n_block++] = k;
  if (n_block == bsize) push_nonzero();
}

void add_zero() {
  lseek(fdd,n_zero,SEEK_CUR);
  n_zero = 0;
}

void Usage() {
  fprintf(stdout,"Usage: [-b bsize] [src] dst\n");
  exit(1);
}

int main(int argc, char **argv) {
  char *dest;
  u_char k;
  int o;
  
  while ((o = getopt(argc, argv, "b:")) != -1) {
    switch (o) {
      case 'b':
        sscanf(optarg,"%d",&bsize);
        if ((bsize <= 0) || (bsize >= 1024*1024*64)) Usage();
        break;
      default:
        Usage();
        break;
    }
  }
  argc -= optind;
  argv += optind;

  if (argc == 1) {
    fds = fileno(stdin);
    dest = argv[0];
  }
  else if (argc == 2) {
    if ((fds = open(argv[0], O_RDONLY)) == -1) {
      fprintf(stdout, "src: %s\n", strerror(errno));
      Usage();
    }
    dest = argv[1];
  }
  else {
    Usage();
  }
  if ((fdd = open(dest,O_CREAT|O_TRUNC|O_RDWR,0666)) == -1) {
    fprintf(stdout, "dst: %s\n", strerror(errno));
    Usage();
  }
  
  block = (u_char *)malloc(bsize);
  while (read(fds,&k,1) == 1) {
    if (k) {
      if (n_zero) add_zero();
      add_nonzero(k);
    }
    else {
      if (n_block) push_nonzero();
      n_zero++;
    }
  }
  if (n_block) push_nonzero();
  if (n_zero) add_zero();
  close(fdd);
  close(fds);
  free(block);
  return 0;
}