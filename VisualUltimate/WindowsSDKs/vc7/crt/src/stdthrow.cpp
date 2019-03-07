// throw -- terminate on thrown exception REPLACEABLE
#include <cstdio>
#include <cstdlib>
#include <exception>
_STD_BEGIN

_CRTIMP2 void __cdecl _Debug_message(const char *s1, const char *s2)
	{	// report error and die
	::fflush(0);
	::fputs(s1, stderr);
	::fputs(s2 != 0 ? s2 : "unknown", stderr);
	::fputs("\n", stderr);
	::abort();
	}
_STD_END

_X_STD_BEGIN
_CRTIMP2 void __cdecl _Throw(const exception& ex)
	{	// report throw exception and die
	std::_Debug_message("exception: ", ex.what());
	}
_X_STD_END

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
