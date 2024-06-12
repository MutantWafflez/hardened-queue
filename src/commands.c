#include "commands.h"
#include "file_helpers.h"
#include "colors.h"
#include "threading.h"

void ViewCommand() {
    if (Queue->length == 0) {
        printf("Queue is empty!\n");
    }
    else {
        printf("THE QUEUE%s\n""---------\n", QueueIsLocked ? " (LOCKED)" : "");

        EnterQueueCriticalSection();
        PrintLinkedList(Queue, stdout);
        ExitQueueCriticalSection();
    }
}

void AddCommand(const char* ID) {
    const Person dummyPerson = {
            .ID = (char*)ID
    };

    EnterQueueCriticalSection();
    if (InList(Queue, &dummyPerson)) {
        PrintWithColor(stderr, "Error: Person already in queue! Please wait until this person is popped off the queue.\n",FOREGROUND_RED);
        return;
    }

    Person* foundPerson = NULL;
    for (int i = 0; i < PeopleCount; i++) {
        if (PersonListComparator(AllPersons + i, &dummyPerson) == 0) {
            foundPerson = CopyPerson(AllPersons + i);
            InsertAtTail(Queue, foundPerson);
            break;
        }
    }
    ExitQueueCriticalSection();

    if (!foundPerson) {
        PrintWithColor(stderr, "Error: No person on the roster with ID \'%s\'. Please re-type the command and try again.\n", FOREGROUND_RED, ID);
        return;
    }

    FirstLastPersonPrint(foundPerson, stdout);
    printf(" has been added to the queue successfully!\n");

    PrintCurrentDateTimeToLog();
    fprintf(LogFile, " Added %s %s (%s) to queue.\n", foundPerson->firstName, foundPerson->lastName, foundPerson->ID);
}