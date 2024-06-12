#include <string.h>
#include "person.h"

void PersonListDeleter(void* data) {
    Person* person = (Person*)data;

    free(person->ID);
    free(person->firstName);
    free(person->lastName);
    free(person);
}

void FreePersonArray(Person** array, size_t size) {
    for (int i = 0; i < size; i++) {
        Person* person = *array + i;
        free(person->ID);
        free(person->firstName);
        free(person->lastName);
    }

    free(*array);
    *array = NULL;
}

void FirstLastPersonPrint(const void* data, FILE* fp) {
    const Person* person = data;
    fprintf(fp, "%s %s", person->firstName, person->lastName);
}

int PersonListComparator(const void* dataOne, const void* dataTwo) {
    return strcasecmp(((Person*)dataOne)->ID, ((Person*)dataTwo)->ID);
}

Person* CopyPerson(const Person* person) {
    Person* newPerson = malloc(sizeof(Person));
    Person copiedPerson = {
            .ID = malloc(sizeof(char) * (strlen(person->ID) + 1)),
            .firstName = malloc(sizeof(char) * (strlen(person->firstName) + 1)),
            .lastName = malloc(sizeof(char) * (strlen(person->lastName) + 1)),
    };

    strcpy(copiedPerson.ID, person->ID);
    strcpy(copiedPerson.firstName, person->firstName);
    strcpy(copiedPerson.lastName, person->lastName);

    *newPerson = copiedPerson;
    return newPerson;
}