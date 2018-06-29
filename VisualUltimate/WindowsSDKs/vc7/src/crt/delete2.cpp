/***
*delete2.cpp - defines C++ vector delete routine
*
*       Copyright (c) 1990-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ delete routine.
*
*******************************************************************************/

#include <cruntime.h>
#include <malloc.h>
#include <new.h>
#include <windows.h>
#include <rtcsup.h>

void operator delete[]( void * p )
{
    RTCCALLBACK(_RTC_Free_hook, (p, 0))

    operator delete(p);
}
