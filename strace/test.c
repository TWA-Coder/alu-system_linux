#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid = fork();
    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execve(argv[1], argv+1, envp);
    } else {
        int status;
        int i = 0;
        struct user_regs_struct regs;
        while (1) {
            wait(&status);
            if (WIFEXITED(status)) break;
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);
            printf("Trap %d: orig_rax=%llu\n", i++, regs.orig_rax);
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
        }
    }
}
