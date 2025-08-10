#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "line.c"

void clearScreen() {
    printf("\x1b[1;1H\x1b[2J");
    fflush(stdout);
}

void displayLine(int x, int y, struct Line *line) {
    printf("\x1B[%d;%dH%s",y,x, line->text);
    
}

void handleInput() {
    char run = 1;
    char ch[4];
    ssize_t readLen;
    char line = 1;
    char column = 1;
    struct Line *curr;
    struct Line *head;
    struct Line *tail;

    head = tail = createLineList("First Text", 10);
    tail = appendLine(tail, " More Text", 10);

    curr = head; 
    int i = 1;
    for(;;) {
        displayLine(1, i++, curr);
        if(curr->next == NULL) break;
        curr = curr->next;
    }
    printf("\x1B[1;1H");
    fflush(stdout);
    curr = head;
    
    while(run) {
        readLen = read(STDIN_FILENO, ch, 4);
        switch(ch[0]) {
            case 0x7F:
                //backspace
                if(column == 1) break;
                if(removeCharFromLine(curr, column-1) == 1) {
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
                        curr = getLine(head, --line-1);
                        printf("\x1b[A");
                        break;
                    case 'B':
                        //down
                        curr = getLine(head, line++);
                        if(curr == NULL) curr = tail = appendLine(tail, "", 0);
                        printf("\x1b[B");
                        break;
                    case 'C':
                        //right
                        if(column > curr->len) break;
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
                    addCharToLine(curr, column-1, ch[0]);
                    column++;
                } else {
                    printf("%c:%d", ch[0],ch[0]);
                }
        }
        //int i = 1;
        //for(;;) {
        displayLine(1, line, curr);
        printf("\x1B[%d;%dH", line, column);
        fflush(stdout);
            //if(curr->next == NULL) break;
            //curr = curr->next;
        //}
        //fflush(stdout);
    }

    freeLines(head);
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

    handleInput();
        
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
    clearScreen();
        
    return 0;
}
