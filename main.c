#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int abrirHistorial(){
    int record;
    FILE *log;
    log = fopen("log.txt", "r");
    if(log != NULL){
        fscanf(log, "%d", &record);
        fclose(log);
        return record;
    }
    return 0;
}

int main(){
    int flag = 1, pid, i, tube[2], record;
    FILE *history, *log;
    record = abrirHistorial();
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
                    strcpy(param[i + size],"");
                }
                pid = fork();
                if(pid < 0){
                    printf("Error al crear el hijo");
                    return 0;
                }else if(pid > 0){
                    printf("papa\n");
                    if(strcmp("!!", param[0])){
                        record++;
                    }
                    if(strcmp(param[size - 1], "&")){
                        wait(NULL);
                        printf("espero\n");
                    }
                    printf("fin papa\n");
                }else{
                    printf("hijo\n");
                    if(!strcmp("!!", param[0])){
                        if(record){
                            history = fopen("history.dat", "rb");
                            fseek(history, -sizeof(param), SEEK_END);
                            fread(param, sizeof(param), 1, history);
                            fclose(history);
                            printf("prompt>");
                            for(i = 0; i < 10; i++){
                                printf("%s ", param[i]);
                            }
                            printf("\n");
                        }else{
                            printf("No commands in history.\n");
                            return 0;
                        }
                    }
                    history = fopen("history.dat", "ab");
                    fwrite(param, sizeof(param), 1, history);
                    fclose(history);
                    execvp(ar[0], ar);
                    printf("fin hijo\n");
                    return 0;
                }
            }else{
                flag = 0;
            }
        }
    }
    log = fopen("log.txt", "w");
    fprintf(log, "%d", record);
    fclose(log);
    printf("prompt terminated\n");
    return 0;
}

/*
for(i = 0; i < 10; i++){
    printf("%s ", param[i]);
}
printf("\n");
*/