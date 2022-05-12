#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd;
	int length;
	char* mapped_mem;
	int pagesize = 0;
	pagesize=getpagesize();
	fd = open(argv[1],O_RDWR | O_CREATE, S_IRUSE | S_IWUSR);
	length = lseek(fd,0,SEEK_END);
	lseek(fd,(pagesize*2 - length%pagesize - 1),SEEK_END);
	write(fd, "-1",1);
	mapped_mem = mmap(NULL,length/pagesize+2,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	printf("\n input:");
	char buf[pagesize];
	fgets(buf,pagesize,stdin);
	int local = 0;
	scanf("%d",&local);
	memmove(mapped_mem+local+strlen(buf),mapped_mem+local,length-local);
	memcpy(mapped_mem+local,buf,strlen(buf)-1);
	munmap(mapped_mem,length/pagesize+2,MS_SYNC|MS_INVALIDATE);
	ftruncate(fd,length+strlen(buf));
	return 0;
}
