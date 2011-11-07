#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s /dev/tty[0-9]+ [COMMANDS ...]\n", argv[0]);
		fprintf(stderr, "If COMMANDS is blank, write only a new line.\n");
		fprintf(stderr, "You can obtain the tty device from a particular terminal via the 'tty' command.\n");
		fprintf(stderr, "Please note, you may have to run this program as root.\n");
		return 1;
	}
	int ttyfd;
	if ((ttyfd = open(argv[1], O_WRONLY)) == -1) {
		perror("open");
		return errno;
	}
	for (int i = 2; i < argc; ++i) {
		for (char *c = argv[i]; *c; ++c) {
			if (ioctl(ttyfd, TIOCSTI, c) == -1) {
				perror("ioctl");
				return errno;
			}
		}
		if (i != argc - 1 && ioctl(ttyfd, TIOCSTI, " ") == -1) {
			perror("ioctl");
			return errno;
		}
	}
	if (ioctl(ttyfd, TIOCSTI, "\n") == -1) {
		perror("ioctl");
		return errno;
	}
	return 0;
}
