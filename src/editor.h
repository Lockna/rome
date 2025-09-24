#ifndef EDITOR_H
#define EDITOR_H
#include <stddef.h>

struct editor_data
{
    size_t columns;
    size_t rows;
};

static struct editor_data editor;

#define CTRL_KEY(k) ((k) & 0x1f)

void setup_editor();
void process_key(char key);
char read_key();

#endif