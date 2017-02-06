/* 
 * tsh - A tiny shell program with job control
 * 
 * Storti Francesca 26  Marzo 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    } 

    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
{   
	//variabili ausiliarie
	//variabile per la maschera dei segnali
	sigset_t maskered;
	//vettore in cui verrà copiato il comando immesso da shell e modificato dalla parseline
	char **argv = malloc(MAXLINE);
	
   	int p,state;
	//variabile in cui verrà memorizzato il pid dell'eventuale processo figlio
	pid_t pid;
    	
	//errore il comando è un LF
	if (cmdline[0] == 10){
		printf("LF");
		  return;}
		
	
	//lettura del comando
	p = parseline(cmdline, argv);

	//se non è un comando builtin
	if (builtin_cmd(argv)==0) {
		//procedura per bloccare il segnale SIGCHLD
		//svuota la schera dei segnali
		if(sigemptyset(&maskered)<0){
			app_error("eval: sigemptyset error");}
		//aggiunge SIGCHLD alla maschera dei segnali
        	if(sigaddset(&maskered,SIGCHLD)<0){
			app_error("eval: sigaddset error");}
		//blocca il segnale aggiunto precedentemente alla maschera
        	if(sigprocmask(SIG_BLOCK,&maskered,NULL)<0){
			unix_error("eval: sigprocmask error");}

		//creazione processo figlio
		if((pid=fork())<0)
			//richiama funzione di gestione errore 
			unix_error("eval: fork error");

		if (pid ==0) {
			//codice eseguito dal figlio

			//modifica il grouppid del figlio per invio segnali di stop e di terminazione
			//sostituendolo con pid del figlio	
			if(setpgid(0,0)<0){
				app_error("eval: setpgid error");}

			//procedura per sbloccare il segnale SIGCHLD 
			if(sigprocmask(SIG_UNBLOCK,&maskered,NULL)<0){
				unix_error("eval: sigprocmask error");}

			//errore nel nuovo processo, comando non riconosciuto
			if(execve(argv[0],argv,environ)< 0){
				printf(" %s : Command not found\n",argv[0]);
				exit(0);
			}
		} 


		//codice eseguito dal padre
		if (pid > 0) {
			
			//controlla se il valore tornato è true o false e modifica il valore di state
			//state sarà BG se il valore restituito da parseline è true, FG altrimenti 
			state = (p)?BG:FG;

			//aggiungo il nuovo processo alla lista dei jobs della shell con il relativo pid, stato e comando 
			 if(addjob(jobs, pid, state, cmdline)==0){
				//controllo se lista piena, stampa messaggio di errore, uccide il nuovo processo ed esce
				printf("Errore limite jobs");
				if(kill(pid,SIGINT)!=0)
					unix_error("eval: kill function");
				}

			//procedura per sboccare il segnale SIGCHLD dalla parte del padre
			if(sigprocmask(SIG_UNBLOCK,&maskered,NULL)<0){
				unix_error("eval: sigprocmask error");}

			//se il job è eseguito in BG stampa informazioni per lavoro in BG
			if (state == BG){ 
				printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);}
			
			//se il job è eseguito in FG 
			if (state == FG) {
				//avvia la procedura di attesa per il lavoro in FG
				waitfg(pid);}
			}
		}
    return;

}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }
    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) 
{    
	//comando quit per la shell  
    	if (strcmp(argv[0],"quit")== 0)	exit(0);
    	

	//comando jobs: stampa la lista dei jobs
	if (strcmp(argv[0],"jobs")==0) {
	listjobs(jobs); 
	return 1;
    	}
	
	//comando bg
   	 if (strcmp(argv[0],"bg")==0) {
		do_bgfg(argv);
		return 1;}
    	

	//comando fg
	if (strcmp(argv[0],"fg")==0){
	do_bgfg(argv);
	return 1;}

	//la funzione ritorna 0 se il comando non appartiene al gruppo precedente
	return 0;     /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
	//errore se l'utente non ha immesso l'identificativo del processo da modificare
	if(argv[1]==NULL){
		printf(" %s Command requires a PID or a %%jobid argument\n",argv[0]);
		return;}

	struct job_t *temp=NULL;
	
	
	//se l'utente identifica il processo tramite il jobid ricaviamo il puntatore al job_t dalla lista dei jobs
	if (argv[1][0]=='%'){
		
		//modifico temporaneamente il primo carattere dell'arg[1] 
		//per renderlo invisibile alla funzione atoi
      		argv[1][0]=' ';

            	//ricavo la struttura job_t per il processo
		temp=getjobjid(jobs,atoi(argv[1]));

		//ricoreggo il primo valore dell'argv[1]
		argv[1][0]='%';
		
		//controllo se l'identificativo è costituito da un numero
		//il controllo deve saltare il primo carattere
		int i=1;
		for(i=1;i<strlen(argv[1]);i++){
			//stampa messaggio di errore se l'identificativo non è un numero ed esce
		 	if (argv[1][i]< '0' || argv[1][i]> '9' ){
				printf("%s Argument must be a PID or a %%jobid\n",argv[0]);
				return;}
			}

		//errore se il processo identificato da temp non è presente nella lista
			if (temp==NULL){
               		printf("(%s) No such job\n",argv[1]);
				return;}
	}





	//se l'utente identifica il processo tramite il jobpid ricaviamo il puntatore al job_t dalla losta dei jobs
          else{

			//controllo se l'identificativo è costituito da un numero
			int i=0;
			for(i=0;i<strlen(argv[1]);i++){
				//errore se l'identificativo non è un numero
		 		if (argv[1][i]< '0' || argv[1][i]> '9' ){
				printf("%s Argument must be a PID or a %%jobid\n",argv[0]);
				return;}
			}

			//lettura del pid 
          		pid_t pid=atoi(argv[1]);
            		temp=getjobpid(jobs,pid);

			//errore se il processo identificato da temp non è presente nella lista
			if (temp==NULL){
                              printf("(%s) No such process\n",argv[1]);
					return;}
		}


	
	
	//esecuzione del comando bg
	if (strcmp(argv[0],"bg")==0){
      		
		//controllo se il  processo si trova in stato di stop altrimenti non spedisco il SIGCONT
		if(temp->state==ST){
		//spedisce il segnale che riattiva tutti i processi del gruppo temp->pid


		//mascheriamo il sigchld per evitare che venga spedito il SIGCONT
			//su un processo su cui è stato effettuato il reaping 

			//procedura per masherare il segnale SIGCHLD
			sigset_t maskered;
			//svuota la maschera
			if(sigemptyset(&maskered)<0){
				app_error("BG : sigemptyset error");}
			if(sigaddset(&maskered,SIGCHLD)<0){
				app_error("BG: sigaddset error");}

			if(sigprocmask(SIG_BLOCK,&maskered,NULL)<0)
				unix_error("BG: sigprocmask error");

			//spedisce il segnale che riattiva tutti i processi del gruppo temp->pid

           		if(kill(-temp->pid,SIGCONT)!=0){
				unix_error("BG : kill error");}	
			
			//riattiva i segnale SIGCHLD
			if(sigprocmask(SIG_UNBLOCK,&maskered,NULL)<0)
				unix_error("BG: sigprocmask error");}
		}

		printf("[%d] (%d) %s", temp->jid, temp->pid, temp->cmdline);
		//modifica dello stato del processo 
		temp->state=BG;

	return;	
				
	}
	
	
	
	//esecuzione del comando fg
	if (strcmp(argv[0],"fg")==0){

		//se il processo si trova nello stato di stop schermiamo il segnale SIGCHLD e mandiamo il 
		// segnale che riattiva il processo
		if(temp->state==ST){

			//mascheriamo il sigchld per evitare che venga spedito il SIGCONT
			//su un processo su cui è stato effettuato il reaping 
			

			//procedura per masherare il segnale SIGCHLD
			sigset_t maskered;
			//svuota la maschera
			if(sigemptyset(&maskered)<0){
				app_error("FG : sigemptyset error");}
			if(sigaddset(&maskered,SIGCHLD)<0){
				app_error("FG: sigaddset error");}

			if(sigprocmask(SIG_BLOCK,&maskered,NULL)<0)
				unix_error("dobgfg: sigprocmask error");

			//spedisce il segnale che riattiva tutti i processi del gruppo temp->pid

           		if(kill(-temp->pid,SIGCONT)!=0){
				unix_error("FG : kill error");}
			//riattiva i segnale SIGCHLD
			if(sigprocmask(SIG_UNBLOCK,&maskered,NULL)<0)
				unix_error("dobgfg: sigprocmask error");
		}
		
		//modifica lo stato del processo in FG
		temp->state=FG;
		//richiama la funzione di attesa per il processo in FG
		waitfg(temp->pid);
  		}
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{	
	int status=FG;
      struct job_t *pidtemp;

	// ciclo di attesa fine lavoro figlio, si esce se il job ha ricevuto un segnale di stop 
	// o se è terminato ed è stato eseguito il reaping
	while(waitpid(pid,&status,WNOHANG)==0){
		pidtemp=getjobpid(jobs,pid);

		//controllo se il processo è stato stoppato 
		//o se è già  stato eseguito il reaping per quel processo
		if ((pidtemp==NULL) || ((pidtemp->state)==ST))
			break;
    	}
	return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
	int status;
    	pid_t retpid;
	
	//reaping                                                                                                                
        if( (retpid=waitpid(-1, &status, WUNTRACED | WNOHANG) ) < 0){
		//stampa messaggio di errore a video
                unix_error("sigchdl:reaping error");}
	
	//se il processo termina da solo cancelliamo il processo dalla lista dei jobs
	if(WIFEXITED(status)){
    		deletejob(jobs,retpid);
			//printf("sigchld: Errore non è possibile cancellare il job dalla lista\n");
	}
	
	//se il processo termina perchè ha ricevuto un segnale SIGINT
	else if(WIFSIGNALED(status)){
		//stampa messaggio a video con i dati del processo
		printf("Job [%d] (%d) terminated by signal %d \n", pid2jid(retpid), retpid,WTERMSIG(status));
		//aggiorna la lista dei jobs cancellando il processo 
		deletejob(jobs,retpid);
			//printf("sigchld: Errore non è possibile cancellare il job dalla lista\n");
		}
	 
	//se il processo viene stoppato perchè ha ricevuto un segnale SIGSTP
        else if( WIFSTOPPED(status)){
		//stampa messaggio a video con i dati del processo
		printf("Job [%d] (%d) stopped by signal %d \n", pid2jid(retpid), retpid,WSTOPSIG(status));
       	struct job_t *temp=getjobpid(jobs,retpid);
		//aggiorna lo stato del processo  
		temp->state= ST;
		} 
      
	//se il processo termina per cause ignote
	  else {
		//messaggio di errore 
        	printf("child %d terminated abnormally\n", retpid);}
                                                                   
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
	pid_t fpid=fgpid(jobs);
	
	//se il comando viene intercettato dalla shell stampa messaggio di errore
	if(fpid==0){
 		//printf(" ATTENZIONE da %d comando non disponibile per shell\n ",getpid());
		return;}
	else {
		//invio segnale di terminazione per tutti i processi apparteneneti al gruppo fpid


		//mascheriamo il sigchld per evitare che venga spedito il SIGSTP
			//su un processo su cui è stato effettuato il reaping 

			//procedura per masherare il segnale SIGCHLD
			sigset_t maskered;
			//svuota la maschera
			if(sigemptyset(&maskered)<0)
				app_error("sigint: sigemptyset error");
			if(sigaddset(&maskered,SIGCHLD)<0)
				app_error("sigint: sigaddset error");

			if(sigprocmask(SIG_BLOCK,&maskered,NULL)<0)
				unix_error("sigint: sigprocmask error");

			//spedisce il segnale che riattiva tutti i processi del gruppo temp->pid

           		iif(kill(-fpid,SIGINT)!=0)
			unix_error("sigint: kill error");
			
			//riattiva i segnale SIGCHLD
			if(sigprocmask(SIG_UNBLOCK,&maskered,NULL)<0)
				unix_error("sigint: sigprocmask error");
		
		
			//aggiornamento lista dei jobs
			deletejob(jobs,fpid);
			return;
	}
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */

void sigtstp_handler(int sig) 
{
	pid_t spid=fgpid(jobs);

      //se il comando viene intercettato dalla shell stampa messaggio di errore
	if(spid==0){
                //printf(" ATTENZIONE da %d comando non disponibile per shell\n ",getpid());
                return;}

      if(spid!=0){
		
			
		//invio segnale di pausa per tutti i processi apparteneneti al gruppo spid
		
		//mascheriamo il sigchld per evitare che venga spedito il SIGSTP
			//su un processo su cui è stato effettuato il reaping 

			//procedura per masherare il segnale SIGCHLD
			sigset_t maskered;
			//svuota la maschera
			if(sigemptyset(&maskered)<0)
				app_error("sigtstp : sigemptyset error");
			if(sigaddset(&maskered,SIGCHLD)<0)
				app_error("sigtstp: sigaddset error");

			if(sigprocmask(SIG_BLOCK,&maskered,NULL)<0)
				unix_error("sigtstp: sigprocmask error");

			//spedisce il segnale che riattiva tutti i processi del gruppo temp->pid

           		if(kill(-spid,SIGTSTP)!=0)
			unix_error("sigstp: kill error");
			
			//riattiva i segnale SIGCHLD
			if(sigprocmask(SIG_UNBLOCK,&maskered,NULL)<0)
				unix_error("sigtstp: sigprocmask error");

			//aggiornamento dello stato del processo
            		struct job_t *temp=getjobpid(jobs,spid);
			temp->state= ST;
			return;
	}
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

// listbkjobs --- prints the list of jobs in background

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}



