// uncaught -- uncaught_exception for Microsoft

  #include <eh.h>
  #include <exception>
_STD_BEGIN

_CRTIMP2 bool __cdecl uncaught_exception()
	{	// report if handling a throw
	return (__uncaught_exception());
	}
_STD_END


/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
