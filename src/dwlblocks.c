#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

#define SIGPLUS			SIGRTMIN
#define SIGMINUS		SIGRTMIN
#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))
#define CMDLENGTH		50
#define MIN( a, b ) ( ( a < b) ? a : b )
#define STATUSLENGTH (LENGTH(blocks) * CMDLENGTH + 1)

typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;

void getcmds(int time);
void getsigcmds(unsigned int signal);
void setupsignals();
void sighandler(int signum, siginfo_t *si, void *ucontext);
int getstatus(char *str, char *last);
void statusloop();
void termhandler();
void chldhandler();
void pstdout();

static void (*writestatus) () = pstdout;

#include "blocks.h"

static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][STATUSLENGTH];
static int statusContinue = 1;

char environment[2] = { '0', '\0'};

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output)
{
        setenv("BLOCK_BUTTON", environment, 1);

	strcpy(output, block->icon);
	FILE *cmdf = popen(block->command, "r");
	if (!cmdf) {
                unsetenv("BLOCK_BUTTON");
		return;
        }
	int i = strlen(block->icon);
	if(fgets(output+i, CMDLENGTH-i-delimLen, cmdf) == NULL) {
                unsetenv("BLOCK_BUTTON");
                return;
        }
	i = strlen(output);
	if (i == 0) {
		//return if block and command output are both empty
		pclose(cmdf);
                unsetenv("BLOCK_BUTTON");
		return;
	}
	//only chop off newline if one is present at the end
	i = output[i-1] == '\n' ? i-1 : i;
	if (delim[0] != '\0') {
		strncpy(output+i, delim, delimLen); 
	}
	else
		output[i++] = '\0';
	pclose(cmdf);

        unsetenv("BLOCK_BUTTON");
}

void getcmds(int time)
{
	const Block* current;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if ((current->interval != 0 && time % current->interval == 0) || time == -1)
			getcmd(current,statusbar[i]);
	}
}

void getsigcmds(unsigned int signal)
{
	const Block *current;
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if (current->signal == signal)
			getcmd(current,statusbar[i]);
	}
}

void setupsignals()
{
	struct sigaction sa = { .sa_sigaction = sighandler, .sa_flags = SA_SIGINFO };
	for (unsigned int i = 0; i < LENGTH(blocks); i++) {
		if (blocks[i].signal > 0)
			sigaction(SIGMINUS+blocks[i].signal, &sa, NULL);
	}

}

int getstatus(char *str, char *last)
{
	strcpy(last, str);
	str[0] = '\0';
	for (unsigned int i = 0; i < LENGTH(blocks); i++)
		strcat(str, statusbar[i]);
	str[strlen(str)-strlen(delim)] = '\0';
	return strcmp(str, last);//0 if they are the same
}

void pstdout()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;
	printf("%s\n",statusstr[0]);
	fflush(stdout);
}

void statusloop()
{
	setupsignals();
	int i = 0;
	getcmds(-1);
	while (1) {
		getcmds(i++);
		writestatus();
		if (!statusContinue)
			break;
		sleep(1.0);
	}
}

void sighandler(int signum, siginfo_t *si, void *ucontext)
{
	if (si->si_value.sival_int) {
                environment[0] = '0' + si->si_value.sival_int;
                getsigcmds(signum-SIGPLUS);
                writestatus();
                environment[0] = '0';
        } else {
		getsigcmds(signum-SIGPLUS);
		writestatus();
	}
}

void termhandler()
{
	statusContinue = 0;
}

void chldhandler()
{
	while (0 < waitpid(-1, NULL, WNOHANG));
}

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++) {//Handle command line arguments
		if (!strcmp("-d",argv[i]))
			strncpy(delim, argv[++i], delimLen);
		else if (!strcmp("-p",argv[i]))
			writestatus = pstdout;
	}

	delimLen = MIN(delimLen, strlen(delim));
	delim[delimLen++] = '\0';
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	signal(SIGCHLD, chldhandler);
	statusloop();
	
        return 0;
}
