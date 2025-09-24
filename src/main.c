#include <editor.h>

int main(int argc, char *argv[])
{
    setup_editor();

    char c;
    while (1)
    {
        process_key(read_key());
    }

    return 0;
}