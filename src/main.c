#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void divideString(char *cmd, char *delim, int *argc, char **argv);

int main()
{
	while (1) {
		char *cmd = NULL; char *delim = " \n"; size_t n = 0;
		int argc; char **argv = NULL; pid_t pid = getpid();

		printf("> ");
		if (getline(&cmd, &n, stdin) == -1)
			return -1;

		divideString(cmd, delim, &argc, NULL);				/* need JUST argc to malloc argv */
		if ((argv = (char **)malloc(sizeof(char *) * argc)) == NULL) {
			perror("Could not allocate memoty");
			exit(EXIT_FAILURE);
		}
		divideString(cmd, delim, NULL, argv);

		pid = fork();
		if (pid == 0)
			if (execv(argv[0], argv) == -1) {
				perror("Could not execute command");
				exit(EXIT_FAILURE);
			}
		wait(&pid);

		free(cmd);
		free(argv);
	}

	return 0;
}

void divideString(char *cmd, char *delim, int *argc, char **argv) {
	char *token;
	int8_t i = 0;
	char *cmd_cpy = strdup(cmd);

	token = strtok(cmd_cpy, delim);									/* One Ring */
		if (argv != NULL) {
		for (i = 0; token; i++) {
			argv[i] = token;
			token = strtok(NULL, delim);							/* To Rule  */
		}
		argv[i] = NULL;
	} else {
		for (i = 0; token; i++) {
			token = strtok(NULL, delim);							/* Them All */
		}
	}

	if (argc != NULL)
		*argc = i;
	free(cmd_cpy);
}
