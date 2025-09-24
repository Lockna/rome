#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <editor.h>
#include <panic.h>

static struct termios orig_tty;

void clean_up_editor()
{
    // Reset to original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_tty);

    // ANSI Escape code for clearing the screen
    write(STDOUT_FILENO, "\x1b[2J", 4);

    // Setting the cursor position to 0,0
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void enter_raw_mode()
{
    struct termios editor;

    // Get a copy of current terminal settings saved
    if (tcgetattr(STDIN_FILENO, &orig_tty) != 0)
        panic("Couldn't copy terminal settings");

    // Register callback to be called when application is closed
    if (atexit(clean_up_editor) != 0)
        panic("Couldn't register on termination callback");

    editor = orig_tty;

    editor.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    editor.c_oflag &= ~(OPOST);
    editor.c_cflag &= ~(CS8);
    editor.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    editor.c_cc[VMIN] = 0;
    editor.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor) != 0)
        panic("Couldn't set terminal settings for editor use");
}

void setup_editor()
{
    if (!isatty(STDIN_FILENO))
    {
        panic("not a tty");
    }

    enter_raw_mode();

    // ANSI Escape code for clearing the screen
    write(STDOUT_FILENO, "\x1b[2J", 4);

    // Setting the cursor position to 0,0
    write(STDOUT_FILENO, "\x1b[H", 3);

    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0)
    {
        panic("Couldn't receive window size from os");
    }

    editor.columns = ws.ws_col;
    editor.rows = ws.ws_row;
}

char read_key()
{
    char key;
    int bytes_read;

    while ((bytes_read = read(STDIN_FILENO, &key, 1) != 1))
    {
        if (bytes_read == -1 && errno != EAGAIN)
            panic("Failed to read byte from stdin");
    }

    return key;
}

void process_key(char key)
{
    switch (key)
    {
    case CTRL_KEY('q'):
        exit(0);
    default:
        printf("%d\r\n", key);
        break;
    }
}