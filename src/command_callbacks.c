#include "command_callbacks.h"
#include "file_helpers.h"
#include "threading.h"

void QuitCommandCallback() {
    EnterQueueCriticalSection();
    BackupQueue();
    ExitQueueCriticalSection();

    DestroyLinkedList(&Queue);
    FreePersonArray((Person**)&AllPersons, PeopleCount);
    printf("Quitting hardened-queue...\n");

    PrintCurrentDateTimeToLog();
    fprintf(LogFile, " Quitting shell.\n");

    fclose(LogFile);
    exit(EXIT_SUCCESS);
}

void PopCommandCallback() {
    EnterQueueCriticalSection();
    Person* nextPerson = RemoveAtHead(Queue);
    ExitQueueCriticalSection();

    FirstLastPersonPrint(nextPerson, stdout);
    printf(" is now up!\n");

    PrintCurrentDateTimeToLog();
    fprintf(LogFile, " Popped %s %s (%s) from the queue.\n", nextPerson->firstName, nextPerson->lastName, nextPerson->ID);

    PersonListDeleter(nextPerson);
}

void ClearCommandCallback() {
#ifdef __WIN32__
    system("cls");
#else
    system("clear");
#endif
}

void LockCommandCallback() {
    QueueIsLocked = !QueueIsLocked;
    char* queueStatusStr = QueueIsLocked ? "locked" : "unlocked";

    PrintCurrentDateTimeToLog();
    fprintf(LogFile, " Queue %s\n", queueStatusStr);

    printf("Queue is now %s.\n", queueStatusStr);
}