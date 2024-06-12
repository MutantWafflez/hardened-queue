#include "threading.h"

#ifdef __WIN32__
#include "windows.h"

CRITICAL_SECTION QueueCriticalSection;
#else
#include <pthread.h>

pthread_mutex_t QueueMutex;
#endif

void InitThreadingMechanisms() {
#ifdef __WIN32
    InitializeCriticalSection(&QueueCriticalSection);
#else
    pthread_mutex_init(&QueueMutex, NULL);
#endif
}

void EnterQueueCriticalSection() {
#ifdef __WIN32__
    EnterCriticalSection(&QueueCriticalSection);
#else
    pthread_mutex_lock(&QueueMutex);
#endif
}

void ExitQueueCriticalSection() {
#ifdef __WIN32__
    LeaveCriticalSection(&QueueCriticalSection);
#else
    pthread_mutex_unlock(&QueueMutex);
#endif
}