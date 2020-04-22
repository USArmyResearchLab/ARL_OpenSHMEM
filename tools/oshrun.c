/* oshrun.c
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the ARL_LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/prctl.h>

char* version = "oshrun (" VENDOR_STRING ") " BUILD_STRING;
char* help =
"oshrun [-h|--help] [--version] -np <#> <program> <program arguments>     \n"
"The arguments for oshrun are:                                            \n"
"  -h | --help          Display this help message.                        \n"
"  --version            Display build information.                        \n"
"  -np <#>              The number of PEs to be used in the execution.    \n"
"  <program>            The program executable to be launched. A relative \n"
"                       or full path to the program is required.          \n"
"  <program arguments>  Flags and other parameters to pass to the program.";

int cleanup(void) {
	int rc = 0;
	char shm[64];
	int i = 0;
	while (1) {
		snprintf(shm, sizeof(shm), "/dev/shm/shmem_init.%d.%d", getpid(), i++);
		struct stat sb;
		if (!stat(shm, &sb)) {
			if (remove(shm)) rc = 1;
		}
		else return rc;
	}
}

void trap(int sig) {
	killpg(getpid(), SIGTERM);
	if (cleanup()) perror("oshrun: cleanup");
	exit(1);
}

int main(int argc, char *argv[], char *envp[])
{
	int npes = 1;
	int rc = 0;
	char runid[32], n_pes[32], my_pe[32];

	signal(SIGHUP, trap);
	signal(SIGINT, trap);
	signal(SIGTERM, trap);
	signal(SIGQUIT, trap);

	while(*argv++) {
		if (!strcmp(*argv,"-np")) {
			npes = atoi(*++argv);
		} else if (!strcmp(*argv,"-h") || !strcmp(*argv, "--help")) {
			puts(help);
			return 0;
		} else if (!strcmp(*argv,"--version")) {
			puts(version);
			return 0;
		} else {
			struct stat sb;
			if (stat(*argv, &sb)) {
				fprintf(stderr, "Unrecognized option: %s\n%s\n", *argv, help);
				return 1;
			} else if (!(sb.st_mode & S_IXUSR)) {
				fprintf(stderr, "%s is not executable\n", *argv);
				return 1;
			} else break;
		}
	}

	if (npes < 0 || npes > sysconf(_SC_CHILD_MAX)) {
		fprintf(stderr, "Illegal number of PEs: %d\n", npes);
		return 1;
	}

	int ne = 0;
	while (envp[ne++]);
	char **e = calloc(ne + 3, sizeof(char*));
	ne = 0;
	while (*envp) e[ne++] = *envp++;
	e[ne++] = runid;
	e[ne++] = n_pes;
	e[ne++] = my_pe;
	snprintf(runid, sizeof(runid), "SHMEM_RUN_ID=%d", getpid());
	snprintf(n_pes, sizeof(n_pes), "SHMEM_N_PES=%d", npes);

	pid_t *pids = calloc(npes, sizeof(pid_t));
	for (int pe = 0; pe < npes; pe++) {
		if (!(pids[pe] = fork())) {
			snprintf(my_pe, sizeof(my_pe), "SHMEM_MY_PE=%d", pe);
			prctl(PR_SET_PDEATHSIG, SIGTERM);
			return execvpe(*argv, argv, e);
		}
	}

	for (int pe = 0; pe < npes; pe++) {
		int status;
		int ret = 0;
		do {
			if (waitpid(pids[pe], &status, 0) == -1) {
				perror("waitpid");
				return 1;
			}
			if (WIFEXITED(status)) {
				ret = WEXITSTATUS(status);
			} else if (WIFSIGNALED(status)) {
				fprintf(stderr, "oshrun: child %d killed by signal %d\n",
					pids[pe], WTERMSIG(status));
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		if (ret) rc = ret;
		pids[pe] = 0;
	}
	if (cleanup()) {
		perror("oshrun: cleanup");
		return 1;
	}
	if (rc) fprintf(stderr,"oshrun: child exited return code %d\n", rc);
	return rc;
}
