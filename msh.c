#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 80

void s_handler(int signal)
{
    printf("\n");
//    printf(" Type \"exit\" to terminate\n");
}


typedef struct variable_declaration
{
    char str[SIZE], *token;
    char *str_cpy;
    char *argv[SIZE];
    char *prompt;
} shell_str;


/* Global Variables */
int  status = 0;
shell_str st_var;


/* Receiving  command on shell */
int usr_input()
{
    int argc = 0, i=0;
    fgets(st_var.str, SIZE, stdin);

    strcpy((&st_var.str[strlen(st_var.str) - 1]),"\0");
    st_var.token=(char *)malloc(sizeof(strlen(st_var.str)));
    st_var.str_cpy=(char *)malloc(sizeof(strlen(st_var.str)));
    strcpy(st_var.str_cpy, st_var.str);
    
    st_var.token = strtok(st_var.str, " ");
    while(st_var.token != NULL)
    {
        st_var.argv[i]=(char *)malloc(sizeof(strlen(st_var.token)));
        strcpy(st_var.argv[i], st_var.token);
        st_var.token = strtok(NULL, " ");
        i++;
    }
    st_var.argv[i] = NULL;
    argc = i;
    return 0;
}



int main(int argc_m, char *argv_m[])
{
    pid_t pid;
    int ret;
    st_var.prompt=(char *)malloc(sizeof(char)*20);
    strcpy(st_var.prompt, "msh");
    
    /* Handling Intrupt Signal */
    struct sigaction s_act;
    memset(&s_act, 0, sizeof(s_act));
    s_act.sa_handler = s_handler;
    sigaction(SIGINT, &s_act, NULL);
    
    system("clear");
    printf("\n\t\t\t\t============= Welcome To Mini Shell===============\n");
    printf("Type \"exit\" to terminate\n\n");
    while(1)
    {
        
        printf("[%s ~]# ", st_var.prompt);

	/* Reading String fron Shell */
        usr_input();

        if(st_var.argv[0] != NULL )
        {

    /* Make a Directory */
            if (strncmp(st_var.argv[0], "mkdir", 5) == 0)
            {
                if(st_var.argv[1] != NULL )
                {
                    status = mkdir(st_var.argv[1], 0666);
                    perror("mkdir ");
                    continue;
                }
            }


    /* Change to Directory */ 
            if (strncmp(st_var.argv[0], "cd", 2) == 0)
            {
                if(st_var.argv[1] != NULL )
                {
                    status = chdir(st_var.argv[1]);
                    perror("cd ");
                    continue;
                }
            }


    /* Remove Directory */
            if (strncmp(st_var.argv[0], "rmdir", 5) == 0)
            {
                if(st_var.argv[1] != NULL )
                {
                    status = rmdir(st_var.argv[1]);
                    perror("rmdir ");
                    continue;
                }
            }
            
    /* Remove File */
            if (strncmp(st_var.argv[0], "rm", 2) == 0)
            {
                if(st_var.argv[1] != NULL )
                {
                    status = unlink(st_var.argv[1]);
                    perror("rm ");
                    continue;
                }
            }

	
	/* Changing Shell Prompt */
            if (strncmp(st_var.argv[0], "PS1=", 4) == 0)
            {
                char *p_str = (char *)malloc(sizeof(strlen(st_var.argv[0]) + 1));
                strcpy(p_str, st_var.argv[0]);
                char *t_str = (char *)malloc(sizeof(strlen(st_var.argv[0]) + 1));
                t_str = strtok(p_str, "=");
                t_str = strtok(NULL, "=");
                strcpy(st_var.prompt,t_str);
                free(p_str);
                free(t_str);
                continue;
             }
            

	/* Working with echo $ */
            if (strncmp(st_var.argv[0], "echo", 4) == 0)
            {
                if(st_var.argv[1] != NULL )
                {
                    char *echo_str = (char *)malloc(sizeof(strlen(st_var.argv[1])));
                    strcpy(echo_str, st_var.argv[1]);
                    
                    if (strcmp(echo_str,"$$") == 0)
                    {
                        printf("PID: %d\n", getpid());
                        free(echo_str);
                        continue;
                    }

                    if (strcmp(echo_str,"$SHELL") == 0)
                    {
                        printf("%s\n", argv_m[0] );
//                        system("pwd");
                        free(echo_str);
                        continue;
                    }

                    if (strcmp(echo_str,"$?") == 0)
                    {
                        printf("%d\n", status);
                        free(echo_str);
                        continue;
                    }
                    
                }
             }


	/* Exit from Shell */            
            if (strncmp(st_var.argv[0], "exit", 4) == 0)
            {
                system("clear");
                printf("\n\t\t\t\t============= Mini Shell Terminated ===============\n\n");
                exit (9);
            }

	/* Creating Child Process */
            pid = fork();
            if (pid == -1)
            {
               perror("Fork Failed");
               exit(-10);
             }
            if(pid)
            {
                wait(&status);
            }
            else
            {
 
            /* Executing command on shell */
                ret = execvp(st_var.argv[0], st_var.argv);
                if (ret == -1)
                {
                    perror("ERROR ");
                    return -11;
                }
            }
        }
    }
    free(st_var.str);
    free(st_var.str_cpy);
    free(st_var.token);
    free(st_var.prompt);
    free(st_var.argv);

    return 0;
}

