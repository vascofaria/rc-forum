#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DATA_SIZE 1111111

int main(int argc, char const *argv[])
{
	int    fp, fp1;
	long int size;
	unsigned char    data[DATA_SIZE];
	char* 	 file_name = argv[1];

	fp  = open("a.png", O_RDONLY);
	fp1 = open("b.png", O_WRONLY);


	size = read(fp, data, DATA_SIZE);
	write(fp1, data, size);

	return 0;
}