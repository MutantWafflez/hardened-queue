#include <string.h>
#include "security.h"
#include "file_helpers.h"
#include "colors.h"

// Yes, the password is in plain-text (top-notch security)
// TODO: NOT plain-text password
const char* AdminPassword = "53rocks";

const char* InvalidPasswordErrorMsg = "Invalid password. Please re-type the command, and try again.\n";
const char* CannotReadPasswordErrorMsg = "Could not read password. Try again.\n";

void AttemptAgainstPassword(GetPassword getPassword, PasswordCallback callback) {
    char* inputtedPassword = getPassword();
    if (!inputtedPassword) {
        PrintWithColor(stderr, CannotReadPasswordErrorMsg, FOREGROUND_RED);
        return;
    }

    if (strcmp(inputtedPassword, AdminPassword) == 0) {
        free(inputtedPassword);
        callback();
        return;
    }

    PrintWithColor(stderr, InvalidPasswordErrorMsg, FOREGROUND_RED);
}


#ifdef __WIN32__
#include <conio.h>
#include <windows.h>

char* ReadPassword() {
    printf("Input password:");
    char* line = malloc(sizeof(char) * MaxLineLength);
    char* lineSeeker = line;
    int readChar;
    while ((readChar = getch()) != '\r') {
        if (readChar == '\b') {
            if (lineSeeker > line) {
                lineSeeker--;
                printf("\b \b");
            }
            continue;
        }

        if (lineSeeker - line >= MaxLineLength) {
            free(line);
            printf("\n");
            return NULL;
        }
        printf("*");

        *lineSeeker = (char)readChar;
        lineSeeker++;
    }
    *lineSeeker = '\0';

    printf("\n");
    return line;
}

void DisableSignals() {
    SetConsoleCtrlHandler(NULL, TRUE);
}
#else
#include <termios.h>
#include <signal.h>
#include <unistd.h>

char* ReadPassword() {
    printf("Input password:");
    static struct termios oldTerminal;
    static struct termios newTerminal;

    tcgetattr(STDIN_FILENO, &oldTerminal);
    newTerminal = oldTerminal;
    newTerminal.c_lflag &= ~(ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);

    char* line = malloc(sizeof(char) * MaxLineLength);
    if (fgets(line, BUFSIZ, stdin) == NULL) {
        line[0] = '\0';
    }
    else {
        line[strlen(line)-1] = '\0';
    }
    printf("\n");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal);
    return line;
}

void DisableSignals() {
    sigset_t signals;

    sigemptyset(&signals);
    sigaddset(&signals, SIGINT);
    sigaddset(&signals, SIGTSTP);
    sigprocmask(SIG_BLOCK, &signals, 0);
}
#endif