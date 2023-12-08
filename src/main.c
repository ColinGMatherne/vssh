#define _GNU_SOURCE		/* needed for getline in c89 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void displayPrompt();

int main()
{
	int i;
	size_t n = 10;
	char *command = NULL;
	char *new_command[10];
	pid_t pid = getpid();

	while (1) {
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
				new_command[i][strcspn(new_command[i], "\n")] = '\0';

			pid = fork();
			wait(NULL);
		}
	}
	return 0;
}

void displayPrompt() {
	size_t max_size = 64;
	char *hostname = (char *)malloc(max_size);
	char *username;
	char *cwd = (char *)malloc(max_size);

	if (gethostname(hostname, max_size) == -1)
		printf("Couldn't get hostname");
	username = getlogin();
	cwd = getcwd(NULL, 128);

	printf("%s@%s %s$ ", hostname, username, cwd);
	free(hostname);
	free(cwd);
}
