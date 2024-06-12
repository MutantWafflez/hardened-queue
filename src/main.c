#include <string.h>
#include "person.h"
#include "linkedlist.h"
#include "file_helpers.h"
#include "commands.h"
#include "security.h"
#include "command_callbacks.h"
#include "colors.h"
#include "threading.h"

const char* HelpString =
        "Commands\n"
        "--------\n"
        "add ID            Adds the person with the specified ID to the internal queue.\n"
        "pop               Removes the person from the front of the queue. Requires password.\n"
        "view              Prints out the current state of the internal queue.\n"
        "clear             Clears the current terminal shell. Requires password.\n"
        "backup            Backs up the current state of the internal queue to file.\n"
        "lock              Locks/unlocks the internal queue. Locking prevents any new persons from being added to the queue.\n"
        "help              Prints out all commands.\n"
        "quit/exit         Backs up the internal queue, then terminates the shell. Requires password.\n";

const Person* AllPersons;
LinkedList* Queue;
int PeopleCount;
bool QueueIsLocked = false;
FILE* LogFile;

const size_t MaxShellTokens = 2;
const unsigned int BackupTimeSeconds = 60;
const char* DefaultPrompt = "hardened-queue";

// Very simple thread - backup the queue, then sleep for x seconds, and repeat.
#ifdef __WIN32__
#include <windows.h>

_Noreturn DWORD WINAPI QueueBackupThread() {
    while(1) {
        Sleep(BackupTimeSeconds * 1000);
        EnterQueueCriticalSection();
        BackupQueue();
        ExitQueueCriticalSection();
    }
}
#else
#include <pthread.h>
#include <unistd.h>

_Noreturn void* QueueBackupThread() {
    while(1) {
        sleep(BackupTimeSeconds);
        EnterQueueCriticalSection();
        BackupQueue();
        ExitQueueCriticalSection();
    }
}
#endif

int main(int argc, char* argv[]) {
    DisableSignals();

    const char* backupFileName = NULL;
    const char* userPrompt = DefaultPrompt;
    // Windows has no getopt()
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            if (i + 1 >= argc) {
                PrintWithColor(stderr, "Invalid argument: '-b' requires a coupled argument.\n", FOREGROUND_RED);
                return EXIT_FAILURE;
            }

            backupFileName = argv[i++ + 1];
        }
        else if (strcmp(argv[i], "-p") == 0) {
            if (i + 1 >= argc) {
                PrintWithColor(stderr, "Invalid argument: '-p' requires a coupled argument.\n", FOREGROUND_RED);
                return EXIT_FAILURE;
            }

            userPrompt = argv[i++ + 1];
        }
        else {
            PrintWithColor(stderr, "Invalid argument: '%s' is not a valid argument.\n", FOREGROUND_RED, argv[i]);
            return EXIT_FAILURE;
        }
    }

    AllPersons = ReadPersonFile(DefaultPersonFileName, &PeopleCount);
    if (!AllPersons) {
        InterpretPersonFileError(DefaultPersonFileName, PeopleCount);
        return EXIT_FAILURE;
    }
    printf("Successfully read & parsed %d people from %s file!\n", PeopleCount, DefaultPersonFileName);

    Queue = CreateList(PersonListComparator, FirstLastPersonPrint, PersonListDeleter);
    if (backupFileName) {
        int backupCount;
        Person* backupQueuePersons = ReadPersonFile(backupFileName, &backupCount);
        if (!backupQueuePersons) {
            InterpretPersonFileError(backupFileName, backupCount);
            return EXIT_FAILURE;
        }

        for (int i = 0; i < backupCount; i++) {
            Person* newPerson = CopyPerson(backupQueuePersons + i);
            InsertAtTail(Queue, newPerson);
        }
        FreePersonArray(&backupQueuePersons, backupCount);
        printf("Successfully loaded queue from backup!\n");
    }

    LogFile = fopen("queue_log.log", "w");
    InitThreadingMechanisms();

    // Create and instantly detach worker thread
#ifdef __WIN32__
    CloseHandle(CreateThread(NULL, 0, QueueBackupThread, NULL, 0, NULL));
#else
    pthread_t tid;
    pthread_create(&tid, NULL, QueueBackupThread, NULL);
    pthread_detach(tid);
#endif

    PrintCurrentDateTimeToLog();
    fprintf(LogFile, " Started shell.\n");
    // And it begins!
    while(1) {
        printf("%s> ", userPrompt);
        char* line = FixedReadLine(stdin);
        if (!line) {
            PrintWithColor(stderr, "Error: Could not read line. Either the line was too long, or some other error occurred. Try again.\n", FOREGROUND_RED);
            fflush(stdin);
            continue;
        }

        char* tokens[MaxShellTokens];
        if (!(*tokens = strtok(line, " "))) {
            free(line);
            PrintWithColor(stderr, "Error: Too few arguments in command. Try again.\n", FOREGROUND_RED);
            continue;
        }

        for (int i = 1; i < MaxShellTokens; i++) {
            char* token;
            if ((token = strtok(NULL, " ")) != NULL) {
                tokens[i] = token;
            }
            else {
                tokens[i] = NULL;
                break;
            }
        }
        // Check for if there's another token after the max
        if (tokens[MaxShellTokens - 1] && strtok(NULL, " ")) {
            free(line);
            PrintWithColor(stderr, "Error: Too many arguments in command. Try again.\n", FOREGROUND_RED);
            continue;
        }

        char* command = *tokens;
        char* arg = tokens[1];
        if (strcmp(command, "add") == 0) {
            if (!arg) {
                PrintWithColor(stderr, "Error: No ID provided. Try again.\n", FOREGROUND_RED);
                free(line);
                continue;
            }

            if (QueueIsLocked) {
                PrintWithColor(stderr, "Error: Cannot add to queue while it is locked. Please defer to a staff member.\n", FOREGROUND_RED);
                free(line);
                continue;
            }

            AddCommand(arg);
        }
        else if (strcmp(command, "pop") == 0) {
            if (Queue->length == 0) {
                PrintWithColor(stderr, "Error: Cannot pop off an empty queue.\n", FOREGROUND_RED);
                free(line);
                continue;
            }

            AttemptAgainstPassword(ReadPassword, PopCommandCallback);
        }
        else if (strcmp(command, "view") == 0) {
            ViewCommand();
        }
        else if (strcmp(command, "clear") == 0) {
            AttemptAgainstPassword(ReadPassword, ClearCommandCallback);
        }
        else if (strcmp(command, "backup") == 0) {
            EnterQueueCriticalSection();
            BackupQueue();
            ExitQueueCriticalSection();
        }
        else if (strcmp(command, "lock") == 0) {
            AttemptAgainstPassword(ReadPassword, LockCommandCallback);
        }
        else if (strcmp(command, "help") == 0) {
            printf("%s", HelpString);
        }
        else if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            // Manually free line here, since the callback exits if it succeeds.
            free(line);

            AttemptAgainstPassword(ReadPassword, QuitCommandCallback);
            continue;
        }
        else {
            PrintWithColor(stderr, "Error: Invalid command: \'%s\'. Use command 'help' for list of commands.\n", FOREGROUND_RED, command);
        }

        free(line);
    }

    fclose(LogFile);
    DestroyLinkedList(&Queue);
    FreePersonArray((Person**)&AllPersons, PeopleCount);
    return EXIT_SUCCESS;
}