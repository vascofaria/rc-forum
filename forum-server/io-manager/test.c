#include <stdio.h>

int main(int argc, char const *argv[])
{
	char command[3];
	char *msg = "reg 12345 ola xd";

	sscanf(msg, "%s", command);
	printf("%s\n", command);
	printf("%s\n", (msg + 4 *sizeof(char)));

	return 0;
}