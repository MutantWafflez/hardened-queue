#ifndef SECURITY_H
#define SECURITY_H

#include "linkedlist.h"

// "Security"
extern const char* AdminPassword;

extern const char* InvalidPasswordErrorMsg;
extern const char* CannotReadPasswordErrorMsg;

typedef char* (*GetPassword)(void);
typedef void (*PasswordCallback)(void);

void AttemptAgainstPassword(GetPassword, PasswordCallback);

char* ReadPassword();

void DisableSignals();

#endif //SECURITY_H
