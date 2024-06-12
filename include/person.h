#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* ID;
    char* firstName;
    char* lastName;
} Person;

void PersonListDeleter(void* data);

void FreePersonArray(Person** array, size_t size);

void FirstLastPersonPrint(const void* data, FILE* fp);

int PersonListComparator(const void* dataOne, const void* dataTwo);

Person* CopyPerson(const Person* person);

#endif //PERSON_H
