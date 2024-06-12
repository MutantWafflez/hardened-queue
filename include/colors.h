#ifndef COLORS_H
#define COLORS_H

#ifdef __WIN32__
// Windows header file simply includes all the colors
#include <windows.h>
#else
#define FOREGROUND_RED 31
#endif

#endif //COLORS_H
