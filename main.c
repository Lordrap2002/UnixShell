#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

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
    int flag = 1, pid, i, record, tube, in, out, size, back, tubo;
    FILE *history, *log;
    record = abrirHistorial();
    while(flag){
        size = tube = in = out = back = 0;
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
                param[size][i] = '\0';
                if(!strcmp("&", param[size])){
                    size--;
                    back++;
                }else if(!strcmp("<", param[size])){
                    in = size;
                }else if(!strcmp(">", param[size])){
                    out = size;
                }else if(!strcmp("|", param[size])){
                    tube = size;
                }
                size++;
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
                            if(!strcmp("&", param[i])){
                                back++;
                            }else if(!strcmp("<", param[i])){
                                in = i;
                            }else if(!strcmp(">", param[i])){
                                out = i;
                            }else if(!strcmp("|", param[i])){
                                tube = i;
                            }
                        }
                        if(back){
                            size--;
                        }
                        printf("\n");
                    }else{
                        printf("No commands in history.\n");
                        continue;
                    }
                }
                //crear tubo
                char *mitubo = "/tmp/mitubo";
                if(tube){
                    mkfifo(mitubo, 0666);
                }
                //crear hijo
                pid = fork();
                if(pid < 0){
                    printf("Error al crear el hijo\n");
                    return 0;
                }else if(pid > 0){//papá
                    record++;
                    //guardar historial
                    history = fopen("history.dat", "ab");
                    fwrite(&size, sizeof(int), 1, history);
                    fwrite(param, sizeof(param), 1, history);
                    fclose(history);
                    //esperar si hay &
                    if(!back){
                        wait(NULL);
                    }
                    if(tube){
                        sleep(1);
                        unlink(mitubo);
                    }
                }else{//hijo
                    int pid2 = 1;
                    if(out){
                        size -= 2;
                        char file[22] = "./";
                        strcat(file, param[out + 1]);
                        int fd = open(file, O_WRONLY | O_CREAT);
                        if(fd < 0){
                            printf("Unable to open file.");
                            return 0;
                        }
                        if(dup2(fd, STDOUT_FILENO) < 0) {
                            printf("Unable to duplicate file descriptor.");
                            return 0;
                        }
                    }else if(in){
                        size -= 2;
                        char file[22] = "./";
                        strcat(file, param[in + 1]);
                        int fd = open(file, O_RDONLY);
                        if(fd < 0){
                            printf("Unable to open file.\n");
                            return 0;
                        }
                        if(dup2(fd, STDIN_FILENO) < 0) {
                            printf("Unable to duplicate file descriptor.\n");
                            return 0;
                        }
                    }else if(tube){
                        pid2 = fork();
                        if(pid2 < 0){
                            printf("Error al crear el hijo del hijo\n");
                            return 0;
                        }
                    }
                    if(pid2 > 0){ //hijo
                        if(tube){
                            size = tube;
                            tubo = open(mitubo, O_WRONLY);
                            write(tubo, "prueba", sizeof("prueba"));
                            if(dup2(tubo, STDOUT_FILENO) < 0) {
                                printf("Unable to duplicate file descriptor of pipe hijo1.");
                                return 0;
                            }
                            close(tubo);
                        }
                        char *const arg[] = {(size ? param[0] : NULL), (size > 1 ? param[1] : NULL),
                                            (size > 2 ? param[2] : NULL), (size > 3 ? param[3] : NULL),
                                            (size > 4 ? param[4] : NULL) , (size > 5 ? param[5] : NULL),
                                            (size > 6 ? param[6] : NULL), (size > 7 ? param[7] : NULL), 
                                            (size > 8 ? param[8] : NULL), (size > 9 ? param[9] : NULL),
                                            NULL};
                        execvp(arg[0], arg);
                    }else{ //hijo del hijo
                        size -= (tube + 1);
                        tubo = -1;
                        char buf[7];
                        while (tubo == -1) {
                            tubo = open(mitubo, O_RDONLY);
                        }
                        read(tubo, buf, 7);
                        if(dup2(tubo, STDIN_FILENO) < 0) {
                            printf("Unable to duplicate file descriptor of pipe hijo2.");
                            return 0;
                        }
                        close(tubo);    
                        char *const arg[] = {(size ? param[1 + tube] : NULL), (size > 1 ? param[2 + tube] : NULL),
                                            (size > 2 ? param[3 + tube] : NULL), (size > 3 ? param[4 + tube] : NULL),
                                            (size > 4 ? param[5 + tube] : NULL) , (size > 5 ? param[6 + tube] : NULL),
                                            (size > 6 ? param[7 + tube] : NULL), (size > 7 ? param[8 + tube] : NULL), 
                                            (size > 8 ? param[9 + tube] : NULL), NULL};
                        execvp(arg[0], arg);
                    }
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