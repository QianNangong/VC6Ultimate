/***
*new.cxx - defines C++ new routine
*
*       Copyright (c) 1990-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ new routine.
*
*******************************************************************************/


#include <cruntime.h>
#include <malloc.h>
#include <new.h>
#include <stdlib.h>
#ifdef WINHEAP
#include <winheap.h>
#include <rtcsup.h>
#else  /* WINHEAP */
#include <heap.h>
#endif  /* WINHEAP */

void * operator new( size_t cb )
{
        void *res = _nh_malloc( cb, 1 );

        RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));

        return res;
}

