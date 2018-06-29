/***
* fp8.c - Set default FP precision
*
*       Copyright (c) 1993-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/
#include <float.h>

void  _setdefaultprecision(void);

/*
 * Routine to set default FP precision to 64 bits.
 */

void _setdefaultprecision()
{
        _controlfp(_PC_64, _MCW_PC);
}

