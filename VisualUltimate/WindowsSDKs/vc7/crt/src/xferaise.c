/* _Feraise function */
#include <errno.h>
#include <ymath.h>
_STD_BEGIN

void (_Feraise)(int except)
	{	/* report floating-point exception */
	if ((except & (_FE_DIVBYZERO | _FE_INVALID)) != 0)
		errno = EDOM;
	else if ((except & (_FE_UNDERFLOW | _FE_OVERFLOW)) != 0)
		errno = ERANGE;
	}
_STD_END

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
