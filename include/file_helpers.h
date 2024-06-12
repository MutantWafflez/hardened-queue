#ifndef FILE_HELPERS_H
#define FILE_HELPERS_H

#include <stdio.h>
#include "main.h"
#include "linkedlist.h"

extern const char* DefaultPersonFileName;
extern const char* BackupFileName;

extern const size_t MaxLineLength;
extern const size_t NumOfColumns;
extern const size_t PersonArrayGrowthSize;

#define READ_ERROR_NO_FILE (-1)
#define READ_ERROR_REALLOC (-2)

void PrintWithColor(FILE* fp, const char* format, int colorCode, ...);

char* FixedReadLine(FILE* fp);

Person* ReadPersonFile(const char* filePath, int* foundPersons);

void InterpretPersonFileError(const char* fileName, int personCount);

void BackupQueue();

void PrintCurrentDateTimeToLog();

#endif //FILE_HELPERS_H
