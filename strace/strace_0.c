#include "strace.h"

/**
 * tracer - traces a process
 * @pid: process ID to trace
 *
 * Return: 0 on success
 */
static int tracer(pid_t pid)
{
	int status;
	struct user_regs_struct regs;

	waitpid(pid, &status, 0);

	/* Set trace option for sys calls */
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

	while (1)
	{
		/* Wait for syscall enter */
		if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
			break;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			break;

		/* Get registers to read the syscall number (orig_rax) */
		ptrace(PTRACE_GETREGS, pid, 0, &regs);
		printf("%lu\n", (unsigned long)regs.orig_rax);

		/* Wait for syscall exit */
		if (ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1)
			break;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
			break;
	}

	return (0);
}

/**
 * main - entry point for strace_0
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int argc, char **argv, __attribute__((unused)) char **envp)
{
	pid_t child_pid;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
		return (EXIT_FAILURE);
	}
	
	setbuf(stdout, NULL);

	child_pid = fork();
	if (child_pid == -1)
		return (EXIT_FAILURE);

	if (child_pid == 0)
	{
		/* Allow parent to trace */
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		/* Stop self to wait for parent to attach */
		kill(getpid(), SIGSTOP);
		execve(argv[1], argv + 1, envp);
		return (EXIT_FAILURE);
	}
	else
	{
		tracer(child_pid);
	}

	return (EXIT_SUCCESS);
}
