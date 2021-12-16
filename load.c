#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//macro
#define MAX_ONE_LINE_SIZE 4096
#define MAX_CMD_SIZE 255
#define MAX_PARAM_SIZE 255
#define COMMAND_PRINTSTRING "printString"
#define COMMAND_PRINTSTRINGLOOP "printStringLoop"
#define COMMAND_NEWLINE "Newline"
#define COMMAND_WAITFORKEYPRESS "waitForKeyPress"
//Global variables
int script_line_size = 0;
char **pstrScript;
int run_current_line;
int run_current_char;

//function declaration
void load(char *fileName);

void free_malloc_variables();

void run_script();

void GetCmd(char *cmd);

void GetStringParam(char *strParam);

int GetIntParam();

int main(int argc, char *argv[]) {
    load("C:\\Users\\13047\\Desktop\\script\\script.txt");
    run_script();
    free_malloc_variables();
}

void free_malloc_variables() {
    if (!script_line_size) {
        return;
    }
    for (int i = 0; i < script_line_size; i++) {
        free(pstrScript[i]);
    }
    free(pstrScript);
}

void load(char *fileName) {
    FILE *filePoint;
    if (!(filePoint = fopen(fileName, "rb"))) {
        printf("file open error:\n");
        exit(0);
    }
    while (!feof(filePoint)) {
        if (fgetc(filePoint) == '\n') {
            script_line_size++;
        }
    }
    script_line_size++;
    fclose(filePoint);
    if (!(filePoint = fopen(fileName, "rb"))) {
        printf("file open error:\n");
        exit(0);
    }
    pstrScript = (char **) malloc(sizeof(char *) * script_line_size);
    for (int i = 0; i < script_line_size; i++) {
        pstrScript[i] = (char *) malloc(MAX_ONE_LINE_SIZE + 1);
        fgets(pstrScript[i], MAX_ONE_LINE_SIZE, filePoint);
    }
    fclose(filePoint);
}

void run_script() {
    char strCmd[MAX_CMD_SIZE];
    char strParam[MAX_PARAM_SIZE];
    for (int i = 0; i < script_line_size; i++) {
        run_current_line = i;
        run_current_char = 0;
        //get Command
        GetCmd(strCmd);
        //run
        if (stricmp(strCmd, COMMAND_PRINTSTRING) == 0) {
            GetStringParam(strParam);
            printf("%s\n", strParam);
        } else if (stricmp(strCmd, COMMAND_PRINTSTRINGLOOP) == 0) {
            GetStringParam(strParam);
            int count = GetIntParam();
            for (int i = 1; i <= count; i++) {
                printf("%d:%s\n", i, strParam);
            }
        } else if (stricmp(strCmd, COMMAND_NEWLINE) == 0) {
            printf("\n");
        } else if (stricmp(strCmd, COMMAND_WAITFORKEYPRESS) == 0) {
            printf("\n");
        } else {
            printf("Error:Invalid Command.\n");
            break;
        }
    }
}

void GetCmd(char *cmd) {
    char *line = pstrScript[run_current_line];
    int sizes = 0;
    char current_char;
    while (run_current_char < (int) strlen(line)) {
        current_char = line[run_current_char++];
        if (current_char == ' ' || current_char == '\n' || current_char == '\r') {
            break;
        }
        cmd[sizes++] = current_char;
    }
    cmd[sizes] = '\0';
    strupr(cmd);
}

void GetStringParam(char *strParam) {
    int sizes = 0;
    run_current_char++;
    char current_char;
    char *line = pstrScript[run_current_line];
    while (run_current_char < (int) strlen(line)) {
        current_char = line[run_current_char++];
        if (current_char == '"' || current_char == '\n') {
            break;
        }
        strParam[sizes++] = current_char;
    }
    run_current_char++;
    strParam[sizes] = '\0';
}

int GetIntParam() {
    int sizes = 0;
    char result[MAX_PARAM_SIZE];
    char current_char;
    char *line = pstrScript[run_current_line];
    while (run_current_char < (int) strlen(line)) {
        current_char = line[run_current_char++];
        if (current_char == ' ' || current_char == '\n') {
            break;
        }
        result[sizes++] = current_char;
    }
    result[sizes] = '\0';
    return atoi(result);
}

