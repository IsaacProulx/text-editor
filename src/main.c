#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "line.c"

struct Tab {
    char *path;
    FILE *file; 
    struct Line *head;
    struct Line *tail;
    struct Line *curr;
};

struct Tab *createTab(char *path) {
    char *line;
    size_t len = 0;
    ssize_t read;
    struct Tab *tab = malloc(sizeof(struct Tab));
    tab->path = path;

    tab->file = fopen(path, "r");
    if(tab->file == NULL) {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }
    read = getline(&line, &len, tab->file);
    tab->head = tab->tail = createLineList(line, read);
    while((read = getline(&line, &len, tab->file)) != -1) {
        tab->tail = appendLine(tab->tail, line, read);
    }
    fclose(tab->file);

    return tab;
}

void clearScreen() {
    printf("\x1b[1;1H\x1b[2J");
    fflush(stdout);
}

void displayLine(int x, int y, struct Line *line) {
    printf("\x1B[%d;%dH%s",y,x, line->text);
    
}

void handleInput(struct Tab *tab) {
    char run = 1;
    char ch[4];
    ssize_t readLen;
    char line = 1;
    char column = 1;
    //struct Line *curr;
    //struct Line *head;
    //struct Line *tail;

    tab->curr = tab->head;
    int i = 1;
    for(;;) {
        displayLine(1, i++, tab->curr);
        if(tab->curr->next == NULL) break;
        tab->curr = tab->curr->next;
    }
    printf("\x1B[1;1H");
    fflush(stdout);
    tab->curr = tab->head;
    
    while(run) {
        readLen = read(STDIN_FILENO, ch, 4);
        switch(ch[0]) {
            case 0x7F:
                //backspace
                if(column == 1) break;
                if(removeCharFromLine(tab->curr, column-1) == 1) {
                    column--; 
                    printf("\x1b[2K");
                }
                break;
            case 0x1B:
                if(readLen == 1) {
                    run = 0;
                    break;
                }
                //read(STDIN_FILENO, &ch, 1);
                //printf("%d\n",ch[1]);
                switch(ch[2]) {
                    case 'A':
                        //up
                        if(line == 1) break;
                        tab->curr = getLine(tab->head, --line-1);
                        printf("\x1b[A");
                        break;
                    case 'B':
                        //down
                        tab->curr = getLine(tab->head, line++);
                        if(tab->curr == NULL) tab->curr = tab->tail = appendLine(tab->tail, "", 0);
                        printf("\x1b[B");
                        break;
                    case 'C':
                        //right
                        if(column > tab->curr->len) break;
                        column++;
                        printf("\x1b[C");
                        break;
                    case 'D':
                        //left
                        if(column == 1) break;
                        column--;
                        printf("\x1b[D");
                        break;
                    default:
                        printf("%d", ch[2]);
                        break;
                }
                break;
            default:
                if(ch[0] <= 'z' && ch[0] >= 'a') {
                    //printf("%c", ch[0]);
                    addCharToLine(tab->curr, column-1, ch[0]);
                    column++;
                } else {
                    printf("%c:%d", ch[0],ch[0]);
                }
        }
        //int i = 1;
        //for(;;) {
        displayLine(1, line, tab->curr);
        printf("\x1B[%d;%dH", line, column);
        fflush(stdout);
            //if(curr->next == NULL) break;
            //curr = curr->next;
        //}
        //fflush(stdout);
    }

    freeLines(tab->head);
    free(tab);
}



int main(int argc, char **argv) {
    struct termios oldTerm, newTerm;
    
    tcgetattr(STDIN_FILENO, &oldTerm);
    
    newTerm = oldTerm;
    //newTerm.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    //newTerm.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
    //newTerm.c_oflag &= ~(OPOST);
    newTerm.c_lflag &= ~(ICANON | ECHO);
    newTerm.c_cflag |=  (CS8);
    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTerm);
    clearScreen();

    struct Tab *tab;
    if(argc == 2) {
        tab = createTab(argv[1]);
    }
    handleInput(tab);
        
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    clearScreen();
        
    return 0;
}
