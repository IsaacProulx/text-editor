#include <stdint.h>
#include <stdlib.h>
#define MAX_LINE_LEN 512

struct Line;

struct Line {
    struct Line *prev;
    struct Line *next;
    char *text;
    uint16_t len;
};

struct Line* getLine(struct Line *head, ssize_t index);
struct Line* createLineList(char *text, uint16_t len);
struct Line* appendLine(struct Line *tail, char *text, uint16_t len);
int addCharToLine(struct Line *line, uint16_t index, char ch);
int removeCharFromLine(struct Line *line, uint16_t index);
void freeLines(struct Line *head);
