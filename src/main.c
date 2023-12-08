#define _GNU_SOURCE		/* needed for getline in c89??? */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void displayPrompt();

size_t max_size = 64;
int running = 1;

char *hostname;
char *username;

void interruptHandler(int signum) {
	running = 0;
}

int main()
{
	int i;
	size_t n = 10;
	char *command = NULL;
	char *new_command[10];
	pid_t pid = getpid();

	signal(SIGINT, interruptHandler);

	hostname = (char *)malloc(max_size);		/* would sometimes crash other times not if not for malloc */
	if (gethostname(hostname, max_size) == -1)
		printf("Couldn't get hostname");
	if ((username = getlogin()) == NULL)
		printf("Couldn't get username");

	while (running) {
		if (pid == 0) {
			execve(new_command[0], new_command, NULL);
			return 0;
		} else {
			displayPrompt();
			getline(&command, &n, stdin);

			/* break up command */
			new_command[0] = strtok(command, " ");
			/* put in char *new_command[] */
			for (i = 1; (new_command[i] = strtok(NULL, " ")); i++)
				new_command[i][strcspn(new_command[i], "\n")] = '\0';			/* remove newline from EOL */

			pid = fork();
			wait(NULL);
		}
	}
	free(hostname);
	return 0;
}

void displayPrompt() {
	char *cwd;

	if ((cwd = getcwd(NULL, 128)) == NULL)
		printf("Couldn't get current directory");

	printf("%s@%s %s$ ", hostname, username, cwd);
}
