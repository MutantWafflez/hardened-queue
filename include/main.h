#ifndef MAIN_H
#define MAIN_H

#include "linkedlist.h"
#include "person.h"

extern const char* HelpString;
extern const Person* AllPersons;
extern LinkedList* Queue;
extern int PeopleCount;
extern bool QueueIsLocked;
extern FILE* LogFile;

#endif //MAIN_H
