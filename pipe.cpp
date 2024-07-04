#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main()
{
	int fds[2];
	char str[] = "who are you?";
	char buf[30];
	pipe(fds);
	pid_t pid = fork();
	if(pid>0)
		write(fds[1], str, sizeof(str));
	else
	{
		read(fds[0], buf, 30);
		puts(buf);
	}
	return 0;
}
