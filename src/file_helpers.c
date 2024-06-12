#include <malloc.h>
#include <string.h>
#include "file_helpers.h"
#include <stdarg.h>
#include <time.h>

const char* DefaultPersonFileName = "roster.csv";
const char* BackupFileName = "queue_backup.csv";

const size_t MaxLineLength = 200;
const size_t NumOfColumns = 3;
const size_t PersonArrayGrowthSize = 100;

char* FixedReadLine(FILE* fp) {
    char* line = malloc(sizeof(char) * MaxLineLength);
    char* lineSeeker = line;
    int readChar;
    while ((readChar = fgetc(fp)) != '\n' && readChar != EOF) {
        if (lineSeeker - line >= MaxLineLength) {
            free(line);
            return NULL;
        }

        if (readChar == '\r') {
            continue;
        }

        *lineSeeker = (char)readChar;
        lineSeeker++;
    }
    *lineSeeker = '\0';

    return line;
}

Person* ReadPersonFile(const char* filePath, int* foundPersons) {
    FILE* personFile = fopen(filePath, "r");
    *foundPersons = 0;
    if (!personFile) {
        *foundPersons = READ_ERROR_NO_FILE;
        return NULL;
    }

    size_t personArraySize = PersonArrayGrowthSize;
    Person* personArray = malloc(sizeof(Person) * personArraySize);
    int personCount = 0;
    while (!feof(personFile)) {
        char* line = FixedReadLine(personFile);
        if (!line) {
            fclose(personFile);
            FreePersonArray(&personArray, personCount);
            *foundPersons = personCount;
            return NULL;
        }

        if (personCount >= personArraySize) {
            Person* newArray = realloc(personArray, sizeof(Person) * (personArraySize += PersonArrayGrowthSize));
            if (!newArray) {
                fclose(personFile);
                FreePersonArray(&personArray, personCount);
                free(line);
                *foundPersons = READ_ERROR_REALLOC;
                return NULL;
            }

            personArray = newArray;
        }

        char* tokens[NumOfColumns];
        *tokens = strtok(line, ",");
        for (int i = 1; i < NumOfColumns; i++) {
            char* token;
            if ((token = strtok(NULL, ",")) != NULL) {
                tokens[i] = token;
            }
            else {
                fclose(personFile);
                FreePersonArray(&personArray, personCount);
                free(line);
                *foundPersons = personCount;
                return NULL;
            }
        }

        Person person = {
                .ID = malloc(sizeof(char) * (strlen(*tokens) + 1)),
                .firstName = malloc(sizeof(char) * (strlen(tokens[1]) + 1)),
                .lastName = malloc(sizeof(char) * (strlen(tokens[2]) + 1)),
        };

        strcpy(person.ID, *tokens);
        strcpy(person.firstName, tokens[1]);
        strcpy(person.lastName, tokens[2]);

        personArray[personCount++] = person;
        free(line);
    }

    fclose(personFile);
    *foundPersons = personCount;
    return personArray;
}

void InterpretPersonFileError(const char* fileName, int personCount) {
    fprintf(stderr, "Error reading person file!\nReason: ");
    switch (personCount) {
        case READ_ERROR_NO_FILE:
            fprintf(stderr, "No %s file found in current working directory.\n", fileName);
            break;
        case READ_ERROR_REALLOC:
            fprintf(stderr, "Person array re-allocation error. Is your %s file excessively large?\n", fileName);
            break;
            // Error reading a specific line
        default:
            fprintf(stderr, "Couldn't parse %s line %d. Either the line is too long, or has too many/few commas.\n", fileName, personCount + 1);
            break;
    }
}

void BackupQueue() {
    FILE* backupFile = fopen(BackupFileName, "w");
    if (!backupFile) {
        fprintf(stderr, "WARNING: Failed to create/open %s file.\n", BackupFileName);
        return;
    }

    for (ListNode* curNode = Queue->head; curNode; curNode = curNode->next) {
        Person* person = curNode->data;
        fprintf(backupFile, "%s,%s,%s", person->ID, person->firstName, person->lastName);
        if (curNode->next) {
            fprintf(backupFile, "\n");
        }
    }

    PrintCurrentDateTimeToLog();
    fprintf(LogFile, " Backed up queue.\n");
    fclose(backupFile);
}

void PrintCurrentDateTimeToLog() {
    char logString[100];

    time_t curTime = time(NULL);
    struct tm* localTime = localtime(&curTime);
    if (!localTime || !strftime(logString, sizeof(logString), "[%c]", localTime)) {
        fprintf(LogFile, "[Could not retrieve datetime]");
        return;
    }

    fprintf(LogFile, "%s", logString);
}

#ifdef __WIN32__
#include <windows.h>

void PrintWithColor(FILE* fp, const char* format, int colorCode, ...) {
    va_list args;
    va_start(args, colorCode);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    WORD saved_attributes = consoleInfo.wAttributes;
    SetConsoleTextAttribute(hConsole, colorCode);

    vfprintf(fp, format, args);

    SetConsoleTextAttribute(hConsole, saved_attributes);
    va_end(args);
}
#else
void PrintWithColor(FILE* fp, const char* format, int colorCode, ...) {
    va_list args;

    va_start(args, colorCode);
    char* formattedString = malloc(sizeof(char) * (vsnprintf(NULL, 0, format, args) + 1));
    va_end(args);

    va_start(args, colorCode);
    vsprintf(formattedString, format, args);
    va_end(args);

    fprintf(fp, "\033[%dm%s", colorCode, formattedString);
    fprintf(fp, "\033[0m");

    free(formattedString);
}
#endif