#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(){
    int flag = 1, pid, i, tube[2];
    while(flag){
        int size = 0;
        char c, *d = "abd";
        char * const ar[20];
        char * const param[] ={"", "", ""};
        printf("prompt>");
        fflush(stdout);
        scanf("%c", &c);
        while(c - '\n'){
            //printf("yes1\n");
            i = 0;
            while(c - ' ' && c - '\n'){
                param[size][i++] = c;
                scanf("%c", &c);
            }
            //printf("yes1\n");
            if(i){
                param[size++][i] = '\0';
                //printf("%d", size);
                //ar = d;
                //ar = param[size];
                /*printf("1\n");
                printf("%s", ar);
                printf("1\n");*/
            }
            if(c - '\n'){
                scanf("%c", &c);
            }
        }
        //*(ar + size + 1) = NULL;
        /*for(i = 1; i <= size; i++){
            printf("%s ", param[i]);
        }*/
        if(size){
            if(strcmp("exit", param[0])){
                //pipe(tube);
                pid = fork();
                //printf("yes ..%d\n", pid);
                if(pid < 0){
                    printf("Error al crear el hijo");
                    return 0;
                }else if(pid > 0){
                    printf("papa\n");
                    //close(tube[0]);
                    //write(tube[1], ar, sizeof(ar));
                    /*if(strcmp(param[size - 1], "&")){
                        printf("espero\n");
                    }*/
                    wait(NULL);
                    printf("fin papa\n");
                }else{
                    printf("hijo\n");
                    //close(tube[1]);
                    //read(tube[0], ar, sizeof(char**));
                    //printf("%s", *(ar+1));
                    char *a = "si", *b, *c;
                    //char *const ar1[] = {"echo", "hola", param[0], NULL};
                    //ar1[2][2] = 'c';
                    //ar1[2][3] = '\0';
                    /**(ar1 + 2) = b;
                    *(ar1 + 3) = c;
                    *(ar1 + 4) = NULL;*/
                    //printf("%s\n", *(ar1 + 4) );
                    /*for(i = 0; i < size; i++){
                        printf("%s ", param[i]);
                    }*/
                    //execvp(param[0], param);
                    printf("fin hijo\n");
                    return 0;
                }
            }else{
                flag = 0;
            }
        }
    }
    printf("prompt terminated\n");
    return 0;
}