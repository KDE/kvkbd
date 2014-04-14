/* $XFree86: xc/programs/xterm/keysym2ucs.h,v 1.1 1999/06/12 15:37:18 dawes Exp $ */
/*
 * This module converts keysym values into the corresponding ISO 10646-1
 * (UCS, Unicode) values.
 */

#ifndef KEYSYM2UCS_H
#define KEYSYM2UCS_H

#include <X11/X.h>

class KeySymConvert
{
public:
    long convert(KeySym keysym);
};

#endif // KEYSYM2UCS_H
