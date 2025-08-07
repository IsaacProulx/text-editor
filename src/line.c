#include <string.h>
#include "line.h"

struct Line* getLine(struct Line *head, ssize_t index) {
    struct Line *found = head;
    for(int i=0; i<index; i++) {
        found = found->next;
        if(found == NULL) return NULL;
    }
    return found;
}

struct Line* createLineList(char *text, uint16_t len) {
    struct Line *head;
    struct Line temp;
    temp.next = NULL;
    temp.prev = NULL;
    temp.text = malloc(MAX_LINE_LEN);
    temp.len = len;

    memcpy(temp.text, text, len);

    head = malloc(sizeof(struct Line));
    memcpy(head, &temp, sizeof(struct Line));
    return head;
}

struct Line* appendLine(struct Line *tail, char *text, uint16_t len) {
    struct Line temp;
    temp.next = NULL;
    temp.prev = tail;
    temp.text = malloc(MAX_LINE_LEN);
    temp.len = len;
    
    memcpy(temp.text, text, len);
    
    tail->next = malloc(sizeof(struct Line));
    memcpy(tail->next, &temp, sizeof(struct Line));
    return tail->next;
}

int addCharToLine(struct Line *line, uint16_t index, char ch) {
    if(line->len >= MAX_LINE_LEN) return -1;
    if(line->len != 0 && index != line->len)
        memcpy(line->text+index+1, line->text+index, line->len-index);
    line->len++;
    line->text[index] = ch;

    return 1;
}

int removeCharFromLine(struct Line *line, uint16_t index) {
    if(index > line->len || line->len == 0) return -1;
    //if(index != line->len-1)
        memcpy(line->text+index-1, line->text+index, line->len-index);
    line->text[line->len--] = 0;

    return 1;
}


void freeLines(struct Line *head) {
    while(head->next != NULL) {
        head = head->next;
        free(head->prev);
    }
    free(head);
    return;
}
