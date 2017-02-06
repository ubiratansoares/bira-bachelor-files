#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
// Busca o arquivo chamado s no diretorio dir
int busca(char* s, char* dir) {
    DIR *dp;
    struct dirent *ep;
    
    dp = opendir(dir);
    if (dp != NULL) {
        while (ep = readdir(dp)) { // para cada arquivo do diretorio...
            if (strcmp(ep->d_name, s) == 0) { // se achou
                closedir(dp);
                return 1;
            }
        }
        
        closedir(dp);
    }
    return 0; // nao achou
}

// Passa por todos os diretorios da path, procurando por s
// Retorna o caminho absoluto encontrado
char *traduzPath(char* s) {
    FILE *f = fopen("/tmp/shell.conf", "r"); // arquivo contem uma linha indicando a variavel path
    char buf[255];
    char *final;

    if (*s == '/') return s; // se o caminho ja é absoluto, ou a path não existe
    if (!f) return NULL;

    while (fscanf(f, "%[^:]: ", buf) == 1) { // caminhos na path sao separados por :
        if (busca(s, buf) == 1) { // se achou o arquivo na pasta
            final = (char*) malloc(sizeof(char)*(strlen(s) + strlen(buf) + 3));
            *final = 0;
            sprintf(final, "%s/%s", buf, s); // concatena os nomes
            fclose(f);
            return final;
        }
    }
    fclose(f);
    return NULL;
}

#define MAX_JOBS 100

int fg;
pid_t jobs[MAX_JOBS];
int qjobs;

void controlC(int signum) {
    signal(SIGINT, controlC);
    if (fg != -1) // mata o grupo inteiro
        kill(-jobs[fg], SIGINT);
}

void controlZ(int signum) {
    signal(SIGTSTP, controlZ);
    if (fg != -1)
        kill(-jobs[fg], SIGTSTP);
    fg = -1;
}

int main(int argc, const char* argv[], const char* envp[]) {
    int i, k, it;
    char buf[300], *p;
    char **args, *cmd;
    int bg, espera;
    args = (char**) malloc(sizeof(char)*100);

    printf(">");

    signal(SIGINT, controlC);
    signal(SIGTSTP, controlZ);
    qjobs = 0; 

    while (fgets(buf, 299, stdin) != NULL) { // para cada comando
        buf[strlen(buf)-1] = '\0';           
        i = 0;
        for (k = 0, p = buf; *p; p += k, i++) { // para cada argumento
            args[i] = (char*) malloc(sizeof(char)*100);
            sscanf(p, " %s%n ", args[i], &k);
        }
		args[i] = 0;

        bg = 1;
        // testa comandos builtin
        if (strcmp(args[0], "pwd") == 0) { getcwd(buf, 300); printf("%s\n", buf); }
        else if (strcmp(args[0], "cd") == 0) { chdir(args[1]); }
        else if (strcmp(args[0], "quit") == 0) { return 0; }
        else if (strcmp(args[0], "bg") == 0) { 
            int jid, pid;
            if (sscanf(args[1], "%d", &jid) == 1 && jid < qjobs) {
                kill(jobs[jid], SIGCONT);
            } else if (sscanf(args[1], "%%%d", &pid) == 1) {
				kill(pid, SIGCONT);
			} else 
                printf("Informe um job id ou pid valido!\n");
        } else if (strcmp(args[0], "fg") == 0) { 
            int jid, i, pid;
            if (sscanf(args[1], "%d", &jid) == 1) {
                fg = jid;
                kill(jobs[jid], SIGCONT);
            } else if (sscanf(args[1], "%%%d", &pid) == 1) {
				jid = -1;
				for (i = 0; i < qjobs; i++) {
					if (jobs[i] == pid) jid = i;
				}
				if (jid >= 0) {
					fg = jid;
					kill(pid, SIGCONT);
				} else printf("processo nao pertencente ao shell!\n");
			} else {
                printf("Informe um job id ou pid valido!\n");
            }
            bg = 0;
        }
        else if (strcmp(args[0], "jobs") == 0) {
            int i;
            for (i = 0; i < qjobs; i++) printf("[%d] %d\n", i, jobs[i]);
        } else {
            {
				int ini, fim, p[2], pg, pid;
				int in, out, lout=STDOUT_FILENO, lin=STDIN_FILENO, lerr=STDERR_FILENO, tem;
				sigset_t sigs;

				sigemptyset(&sigs);
				sigaddset(&sigs, SIGPIPE);
				sigprocmask(SIG_UNBLOCK, &sigs, NULL);

				bg = (strcmp(args[i-1], "&") == 0);
				if (bg) i--;

                //setbuf(stdout, NULL);
                //setbuf(stdin, NULL);

				tem = 1;
				while (tem && i > 2) {
					tem = 0;
					if (strcmp(args[i-2], ">") == 0) {
						lout = open(args[i-1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
						i -= 2; tem = 1;
					} else if (strcmp(args[i-2], "<") == 0) {
						lin = open(args[i-1], O_RDONLY);
						i -= 2; tem = 1;
					} else if (strcmp(args[i-2], ">>") == 0) {
						lout = open(args[i-1], O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
						i -= 2; tem = 1;
					} else if (strcmp(args[i-2], "2>") == 0) {
						lerr = open(args[i-1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
						i -= 2; tem = 1;
					}
				}

				pg = 0;
				for (ini = 0, fim = 1; fim <= i; fim++) {
					if (args[fim] && strcmp(args[fim], "|") == 0 || fim == i) {
                        if (fim != i) {
                            pipe(p);
                            in = p[0]; out = p[1];
                        }

						if ((pid = fork()) == 0) { // cria um filho para executar       
							setpgid(0, pg);
							
							if (fim != i) dup2(out, STDOUT_FILENO);
							else { 		  
								dup2(lout, STDOUT_FILENO); 
								dup2(lerr, STDERR_FILENO); 
							}

							if (ini != 0) dup2(in, STDIN_FILENO);
							else          dup2(lin, STDIN_FILENO);

							cmd = traduzPath(args[ini]);

							if (!cmd)  {
								printf("%s: programa nao encontrado\n", args[ini]);
								return;
							}

							args[fim] = 0;

							execv(cmd, &args[ini]);
						} else if (ini == 0)
							pg = jobs[qjobs] = pid;
						ini = fim+1;
					}
				}

				fg = qjobs;
				qjobs++;
            }
        }
                
        if (!bg) {
			int status;

            waitpid(jobs[fg], &status, WUNTRACED); // espera o filho terminar
            waitpid(-jobs[fg], &status, WUNTRACED); // espera o grupo terminar

            if ( WIFEXITED(status) ) {  // remove o processo da lista
                jobs[fg] = jobs[qjobs-1];
                qjobs--;
            } else
				fg = -1;
        } else fg = -1; 
                

        while (i--) free(args[i]); // libera os argumentos alocados

        for (it = 0; it < qjobs; it++) { // atualiza a lista de jobs
			waitpid(-jobs[it], NULL, WNOHANG);
            if (kill(-jobs[it], 0) == -1) { // se morreu
                printf("morreu [%d] %d\n", it, jobs[it]);
                jobs[it] = jobs[qjobs-1];
                qjobs--;
            }
        }

        printf(">");
    }
    free(args);

    printf("exit\n");
    return 0;
}
