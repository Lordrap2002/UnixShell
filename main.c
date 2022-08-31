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
        printf("prompt>");
        fflush(stdout);
        //leer entrada
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
                //verificar historial
                if(!strcmp("!!", param[0])){
                    if(record){
                        history = fopen("history.dat", "rb");
                        fseek(history, -sizeof(param) - sizeof(int), SEEK_END);
                        fread(&size, sizeof(int), 1, history);
                        fread(param, sizeof(param), 1, history);
                        fclose(history);
                        printf("prompt>");
                        for(i = 0; i < size; i++){
                            printf("%s ", param[i]);
                        }
                        printf("\n");
                    }else{
                        printf("No commands in history.\n");
                        continue;
                    }
                }
                //crear hijo
                pid = fork();
                if(pid < 0){
                    printf("Error al crear el hijo");
                    return 0;
                }else if(pid > 0){
                    //papá
                    printf("papa\n");
                    record++;
                    //guardar historial
                    history = fopen("history.dat", "ab");
                    fwrite(&size, sizeof(int), 1, history);
                    fwrite(param, sizeof(param), 1, history);
                    fclose(history);
                    //esperar si hay &
                    if(strcmp(param[size - 1], "&")){
                        printf("espero\n");
                        wait(NULL);
                    }
                    printf("fin papa\n");
                }else{
                    //hijo
                    printf("hijo\n");
                    char *const arg[] = {(size ? param[0] : NULL), (size > 1 ? param[1] : NULL),
                                        (size > 2 ? param[2] : NULL), (size > 3 ? param[3] : NULL),
                                        (size > 4 ? param[4] : NULL) , (size > 5 ? param[5] : NULL),
                                        (size > 6 ? param[6] : NULL), (size > 7 ? param[7] : NULL), 
                                        (size > 8 ? param[8] : NULL), (size > 9 ? param[9] : NULL), NULL};
                    execvp(arg[0], arg);
                    printf("fin hijo\n");
                    return 0;
                }
            }else{
                flag = 0;
            }
        }
    }
    //guardar log (número de comandos en el historial)
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