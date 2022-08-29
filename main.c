#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(){
    int flag = 1, pid, i, tube[2];
    while(flag){
        int size = 0;
        char c, param[10][20];
        char *const ar[] = {param[0], param[1], param[2], param[3], param[4] , param[5],
                            param[6], param[7], param[8], param[9], NULL};
        printf("prompt>");
        fflush(stdout);
        scanf("%c", &c);
        while(c - '\n'){
            i = 0;
            while(c - ' ' && c - '\n'){
                param[size][i++] = c;
                scanf("%c", &c);
            }
            if(i){
                param[size++][i] = '\0';
            }
            if(c - '\n'){
                scanf("%c", &c);
            }
        }
        if(size){
            if(strcmp("exit", param[0])){
                for(i = 0; i < 10 - size; i++){
                    param[i + size][0] = '\0';
                }
                pid = fork();
                if(pid < 0){
                    printf("Error al crear el hijo");
                    return 0;
                }else if(pid > 0){
                    printf("papa\n");
                    wait(NULL);
                    printf("fin papa\n");
                }else{
                    printf("hijo\n");
                    execvp(ar[0], ar);
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