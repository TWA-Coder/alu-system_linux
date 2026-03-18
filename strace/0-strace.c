#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * main - traces a process and prints system call numbers as they're called
 * @argc: argument count
 * @argv: argument array
 * @envp: environment parameters
 * Return: 0 on success | 1 on failure (not enough arguments)
 * author: Frank Onyema Orji
 **/
int main(int argc, char *argv[], char *envp[])
{
	int print, status;
	struct user_regs_struct regs;
	pid_t pid;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <full_path> [path_args]\n", argv[0]);
		return (1);
	}

	setbuf(stdout, NULL);
	pid = fork();

	if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, pid, NULL, NULL);
		execve(argv[1], argv + 1, envp);
	}
	else
	{
		wait(&status);
		for (print = 1; !WIFEXITED(status); print ^= 1)
		{
			ptrace(PT_GETREGS, pid, NULL, &regs);
			if (print)
				printf("%lu\n", (size_t)regs.orig_rax);
			ptrace(PT_SYSCALL, pid, NULL, NULL);
			wait(&status);
		}
	}

	return (0);
}
