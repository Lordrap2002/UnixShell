#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(){
    int flag = 1, pid, i;
    while(flag){
        int size = 0;
        char c, *tok, param[20][100];
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
                pid = fork();
                if(pid < 0){
                    printf("Error al crear el hijo");
                    return 0;
                }else if(pid){
                    printf("papa\n");
                    if(strcmp(param[size - 1], "&")){
                        wait(NULL);
                    }
                    printf("fin papa\n");
                }else{
                    printf("hijo\n");
                    return 0;
                }
            }else{
                flag--;
            }
        }
    }
    printf("prompt terminated\n");
    return 0;
}