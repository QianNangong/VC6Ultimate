/***
*setlocal.c - Contains the setlocale function
*
*       Copyright (c) 1988-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Contains the setlocale() function.
*
*******************************************************************************/

#include <locale.h>
#include <internal.h>

#if !defined (_WIN32)

static char _clocalestr[] = "C";

#else  /* !defined (_WIN32) */

#include <cruntime.h>
#include <setlocal.h>
#include <mtdll.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> /* for strtol */
#include <dbgint.h>
#include <ctype.h>
#include <awint.h>

/* C locale */
extern char __clocalestr[];


#ifdef _DEBUG
// free as much of memory as possible in debug mode to minimise leaks.
#include <sect_attribs.h>
#include <internal.h>
void __cdecl _locterm(void);

#pragma data_seg(".CRT$XPX")
_CRTALLOC(".CRT$XPX") static _PVFV pterm = _locterm;
#pragma data_seg()

#endif  /* _DEBUG */


__declspec(selectany) struct {
        const char * catname;
        char * locale;
        int (* init)(threadlocinfo *);
} __lc_category[LC_MAX-LC_MIN+1] = {
        /* code assumes locale initialization is "__clocalestr" */
        { "LC_ALL",     NULL,           __init_dummy /* never called */ },
        { "LC_COLLATE", __clocalestr,    __init_collate  },
        { "LC_CTYPE",   __clocalestr,    __init_ctype    },
        { "LC_MONETARY",__clocalestr,    __init_monetary },
        { "LC_NUMERIC", __clocalestr,    __init_numeric  },
        { "LC_TIME",    __clocalestr,    __init_time }
};

static const char _first_127char[] = {
        1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
        69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
        86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,101,102,
        103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
        120,121,122,123,124,125,126,127
};

extern struct __lc_time_data __lc_time_c;
extern threadlocinfo __initiallocinfo;
extern const unsigned short _wctype[];
static const short *_ctype_loc_style = _wctype+2;



/*
 * Flag indicating whether or not setlocale() is active. Its value is the
 * number of setlocale() calls currently active.
 */
_CRTIMP int __setlc_active;
/* These functions are for enabling STATIC_CPPLIB functionality */
_CRTIMP int __cdecl ___setlc_active_func(void)
{
    return __setlc_active;
}

/*
 * Flag indicating whether or not a function which references the locale
 * without having locked it is active. Its value is the number of such
 * functions.
 */
_CRTIMP int __unguarded_readlc_active;
/* These functions are for enabling STATIC_CPPLIB functionality */
_CRTIMP int * __cdecl ___unguarded_readlc_active_add_func(void)
{
    return &__unguarded_readlc_active;
}

/* helper function prototypes */
char * _expandlocale(char *, char *, LC_ID *, UINT *, int);
void _strcats(char *, int, ...);
void __lc_lctostr(char *, const LC_STRINGS *);
int __lc_strtolc(LC_STRINGS *, const char *);
static char * __cdecl _setlocale_set_cat(pthreadlocinfo, int, const char *);
static char * __cdecl _setlocale_get_all(pthreadlocinfo);
static pthreadlocinfo __cdecl __updatetlocinfo_lk(void);
static char * __cdecl _setlocale_lk(pthreadlocinfo, int, const char *);
void __cdecl __free_lconv_mon(struct lconv *);
void __cdecl __free_lconv_num(struct lconv *);
void __cdecl __free_lc_time(struct __lc_time_data *);
#endif  /* !defined (_WIN32) */



/***
*__freetlocinfo() - free threadlocinfo
*
*Purpose:
*       Free up the per-thread locale info structure specified by the passed
*       pointer.
*
*Entry:
*       pthreadlocinfo ptloci
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl __freetlocinfo (
        pthreadlocinfo ptloci
        )
{
    int category;
    /*
     * Free up lconv struct
     */
    if ( (ptloci->lconv != NULL) &&
         (ptloci->lconv != &__lconv_c) &&
         (*(ptloci->lconv_intl_refcount) == 0))
    {
        if ( (ptloci->lconv_mon_refcount != NULL) &&
             (*(ptloci->lconv_mon_refcount) == 0))
        {
            _free_crt(ptloci->lconv_mon_refcount);
            __free_lconv_mon(ptloci->lconv);
        }

        if ( (ptloci->lconv_num_refcount != NULL) &&
             (*(ptloci->lconv_num_refcount) == 0))
        {
            _free_crt(ptloci->lconv_num_refcount);
            __free_lconv_num(ptloci->lconv);
        }

        _free_crt(ptloci->lconv_intl_refcount);
        _free_crt(ptloci->lconv);
    }

    /*
     * Free up ctype tables
     */
    if ( (ptloci->ctype1_refcount != NULL) &&
         (*(ptloci->ctype1_refcount) == 0) )
    {
        _free_crt(ptloci->ctype1_refcount);
        _free_crt(ptloci->ctype1);
    }

    /*
     * Free up the __lc_time_data struct
     */
    if ( ptloci->lc_time_curr != &__lc_time_c &&
         ((ptloci->lc_time_curr->refcount) == 0) )
    {
        __free_lc_time(ptloci->lc_time_curr);
        _free_crt(ptloci->lc_time_curr);
    }

    for (category = LC_MIN; category <= LC_MAX; ++category) {
        if ((ptloci->lc_category[category].locale != __clocalestr) &&
              (ptloci->lc_category[category].refcount != NULL) &&
              (*ptloci->lc_category[category].refcount == 0) )
        {
            _free_crt(ptloci->lc_category[category].refcount);
        }
        if ((ptloci->lc_category[category].wlocale != NULL) &&
              (ptloci->lc_category[category].wrefcount != NULL) &&
              (*ptloci->lc_category[category].wrefcount == 0) )
        {
            _free_crt(ptloci->lc_category[category].wrefcount);
        }
    }

    /*
     * Free up the threadlocinfo struct
     */
    _free_crt(ptloci);
}

/***
* __addlocaleref(pthreadlocinfo ptloci)
*
* Purpose:
*       Increment the refrence count for each element in the localeinfo struct.
*
*******************************************************************************/
void __cdecl __addlocaleref( pthreadlocinfo ptloci)
{
    int category;
    InterlockedIncrement(&(ptloci->refcount));
    if ( ptloci->lconv_intl_refcount != NULL )
        InterlockedIncrement(ptloci->lconv_intl_refcount);

    if ( ptloci->lconv_mon_refcount != NULL )
        InterlockedIncrement(ptloci->lconv_mon_refcount);

    if ( ptloci->lconv_num_refcount != NULL )
        InterlockedIncrement(ptloci->lconv_num_refcount);

    if ( ptloci->ctype1_refcount != NULL )
        InterlockedIncrement(ptloci->ctype1_refcount);

    for (category = LC_MIN; category <= LC_MAX; ++category) {
        if (ptloci->lc_category[category].locale != __clocalestr &&
            ptloci->lc_category[category].refcount != NULL)
            InterlockedIncrement(ptloci->lc_category[category].refcount);
        if (ptloci->lc_category[category].wlocale != NULL &&
            ptloci->lc_category[category].wrefcount != NULL)
            InterlockedIncrement(ptloci->lc_category[category].wrefcount);
    }
    InterlockedIncrement(&(ptloci->lc_time_curr->refcount));
}

/***
* __removelocaleref(pthreadlocinfo ptloci)
*
* Purpose:
*       Decrement the refrence count for each elemnt in the localeinfo struct.
*
******************************************************************************/
void * __cdecl __removelocaleref( pthreadlocinfo ptloci)
{
    int category;
    if ( ptloci != NULL )
    {
        InterlockedDecrement(&(ptloci->refcount));

        if ( ptloci->lconv_intl_refcount != NULL )
            InterlockedDecrement(ptloci->lconv_intl_refcount);

        if ( ptloci->lconv_mon_refcount != NULL )
            InterlockedDecrement(ptloci->lconv_mon_refcount);

        if ( ptloci->lconv_num_refcount != NULL )
            InterlockedDecrement(ptloci->lconv_num_refcount);

        if ( ptloci->ctype1_refcount != NULL )
            InterlockedDecrement(ptloci->ctype1_refcount);

        for (category = LC_MIN; category <= LC_MAX; ++category) {
            if (ptloci->lc_category[category].locale != __clocalestr &&
                ptloci->lc_category[category].refcount != NULL)
                InterlockedDecrement(ptloci->lc_category[category].refcount);
            if (ptloci->lc_category[category].wlocale != NULL &&
                ptloci->lc_category[category].wrefcount != NULL)
                InterlockedDecrement(ptloci->lc_category[category].wrefcount);
        }
        InterlockedDecrement(&(ptloci->lc_time_curr->refcount));
    }
    return ptloci;
}

/***
*
* __copytlocinfo_lk(pthreadlocinfo ptlocid, pthreadlocinfo ptlocis)
*
* Purpose:
*       Copy the contenets of ptlocis to ptlocid and increase the refcount of all the
*       elements in ptlocid after copy.
*
******************************************************************************/
static void __cdecl __copytlocinfo_lk(
    pthreadlocinfo ptlocid,
    pthreadlocinfo ptlocis)
{
    if (ptlocis != NULL && ptlocid != NULL && ptlocid != ptlocis) {
        *ptlocid = *ptlocis;
        ptlocid->refcount = 0;
        __addlocaleref(ptlocid);
    }
}

/***
*
* __updatelocinfoEx_lk(pthreadlocinfo *pptlocid, pthreadlocinfo ptlocis)
*
* Purpose:
*       Update *pptlocid to ptlocis. This might include freeing contenes of *pptlocid.
*
******************************************************************************/
static pthreadlocinfo __cdecl __updatetlocinfoEx_lk(
    pthreadlocinfo *pptlocid,
    pthreadlocinfo ptlocis)
{
    pthreadlocinfo ptloci;

    if (ptlocis == NULL || pptlocid == NULL)
        return NULL;
    ptloci = *pptlocid;
    if ( ptloci != ptlocis)
    {
        /*
         * Update to the current locale info structure and increment the
         * reference counts.
         */
        *pptlocid = ptlocis;
        __addlocaleref(ptlocis);
        /*
         * Decrement the reference counts in the old locale info
         * structure.
         */
        if ( ptloci != NULL )
        {
            __removelocaleref(ptloci);
        }
        /*
         * Free the old locale info structure, if necessary.  Must be done
         * after incrementing reference counts in current locale in case
         * any refcounts are shared with the old locale.
         */
        if ( (ptloci != NULL) &&
             (ptloci->refcount == 0) &&
             (ptloci != &__initiallocinfo) )
            __freetlocinfo(ptloci);

    }

    return ptlocis;
}

#ifdef _MT
/***
*__updatetlocinfo() - refresh the thread's locale info
*
*Purpose:
*       If this thread does not have it's ownlocale which means that either
*       ownlocale flag in ptd is not set or ptd->ptloci == NULL, then Update
*       the current thread's reference to the locale information to match the
*       current global locale info. Decrement the reference on the old locale
*       information struct and if this count is now zero (so that no threads
*       are using it), free it.
*
*Entry:
*
*Exit:
*
*       if (!_getptd()->ownlocale || _getptd()->ptlocinfo == NULL)
*           _getptd()->ptlocinfo == __ptlocinfo
*       else
*           _getptd()->ptlocinfo
*
*Exceptions:
*
*******************************************************************************/
pthreadlocinfo __cdecl __updatetlocinfo(void)
{
    pthreadlocinfo ptloci;
    _ptiddata ptd = _getptd();

    if (!(ptd->_ownlocale && __globallocalestatus)|| !ptd->ptlocinfo) {
        _mlock(_SETLOCALE_LOCK);
        __try
        {
            ptloci = __updatetlocinfoEx_lk(&ptd->ptlocinfo, __ptlocinfo);
        }
        __finally
        {
            _munlock(_SETLOCALE_LOCK);
        }
    } else {
        ptloci = _getptd()->ptlocinfo;
    }

    return ptloci;
}

/***
* _setlocaletype(int i)
*
* Purpose:
*       To set _ownlocale flag on threadinfo sturct. If this flag is set, this thread
*       is going own it's threadlocinfo struct. Setlocale call on other thread will have
*       no effect on this thread's locale.
*
*******************************************************************************/
int __cdecl _configthreadlocale(int i)
{
    /*
     * ownlocale flag struct:
     * bits: 000000000000000000000000000000PG
     * P is set when _ENABLE_PER_THREAD_LOCALE is called for this thread
     * or _ENABLE_PER_THREAD_LOCALE_NEW was set when this thread was created.
     * G is set if _ENABLE_PER_THREAD_LOCALE_GLOBAL is currently set.
     *
     * __globallocalestatus structure:
     * bits: 11111111111111111111111111111N1G
     * G is set if _ENABLE_PER_THREAD_LOCALE_GLOBAL is set.
     * G is 0 if _ENABLE_PER_THREAD_LOCALE_GLOBAL is not set.
     * N is set if _ENABLE_PER_THREAD_LOCALE_NEW is set.
     * N is 0 if _ENABLE_PER_THREAD_LOCALE_NEW is not set.
     */
    _ptiddata ptd = _getptd();
    if (i == _ENABLE_PER_THREAD_LOCALE ||
        i == _DISABLE_PER_THREAD_LOCALE ) { // PER_THREAD_LOCALE
        if (i == _ENABLE_PER_THREAD_LOCALE) {
            ptd->_ownlocale = ptd->_ownlocale | 0x2;
        } else {
            ptd->_ownlocale = ptd->_ownlocale & 0x1;
        }
    } else if (i  == _ENABLE_PER_THREAD_LOCALE_GLOBAL ||
               i == _DISABLE_PER_THREAD_LOCALE_GLOBAL) { // PER_THREAD_LOCALE_GLOBAL
        if (i == _ENABLE_PER_THREAD_LOCALE_GLOBAL) {
            ptd->_ownlocale = ptd->_ownlocale | 0x1;
            InterlockedExchange(&__globallocalestatus, __globallocalestatus | 0x1); // Set G flag;
        } else {
            ptd->_ownlocale = ptd->_ownlocale & 0x2;
            InterlockedExchange(&__globallocalestatus, __globallocalestatus & 0xfffffffe); // Unset G flag;
        }
    } else if (i == _ENABLE_PER_THREAD_LOCALE_NEW ||
               i == _DISABLE_PER_THREAD_LOCALE_NEW) { // PER_THREAD_LOCALE_NEW
        if (i == _ENABLE_PER_THREAD_LOCALE_GLOBAL) {
            InterlockedExchange(&__globallocalestatus, __globallocalestatus | 0x4); // Set N flag;
        } else {
            InterlockedExchange(&__globallocalestatus, __globallocalestatus & 0xfffffffb); // Unset G flag;
        }
    } else {
        return 0;
    }
    __updatetlocinfo();
    return 1;
}
#endif  /* _MT */

#ifdef _DEBUG
void __cdecl _locterm(void)
{
    if (__ptlocinfo != &__initiallocinfo) {
#ifdef _MT
        _mlock(_SETLOCALE_LOCK);
        __try
        {
#endif  /* _MT */
            __ptlocinfo = __updatetlocinfoEx_lk(&__ptlocinfo, &__initiallocinfo);
#ifdef _MT
        }
        __finally
        {
            _munlock(_SETLOCALE_LOCK);
        }
#endif  /* _MT */
    }
}
#endif  /* _DEBUG */

/***
*char * setlocale(int category, char *locale) - Set one or all locale categories
*
*Purpose:
*       The setlocale() routine allows the user to set one or more of
*       the locale categories to the specific locale selected by the
*       user.  [ANSI]
*
*       NOTE: Under !_INTL, the C libraries only support the "C" locale.
*       Attempts to change the locale will fail.
*
*Entry:
*       int category = One of the locale categories defined in locale.h
*       char *locale = String identifying a specific locale or NULL to
*                  query the current locale.
*
*Exit:
*       If supplied locale pointer == NULL:
*
*           Return pointer to current locale string and do NOT change
*           the current locale.
*
*       If supplied locale pointer != NULL:
*
*           If locale string is '\0', set locale to default.
*
*           If desired setting can be honored, return a pointer to the
*           locale string for the appropriate category.
*
*           If desired setting can NOT be honored, return NULL.
*
*Exceptions:
*       Compound locale strings of the form "LC_COLLATE=xxx;LC_CTYPE=xxx;..."
*       are allowed for the LC_ALL category.  This is to support the ability
*       to restore locales with the returned string, as specified by ANSI.
*       Setting the locale with a compound locale string will succeed unless
*       *all* categories failed.  The returned string will reflect the current
*       locale.  For example, if LC_CTYPE fails in the above string, setlocale
*       will return "LC_COLLATE=xxx;LC_CTYPE=yyy;..." where yyy is the
*       previous locale (or the C locale if restoring the previous locale
*       also failed).  Unrecognized LC_* categories are ignored.
*
*******************************************************************************/

#if !defined (_WIN32)

char * __cdecl setlocale (
        int _category,
        const char *_locale
        )
{
    if ( (_locale == NULL) ||
         (_locale[0] == '\0') ||
         ( (_locale[0]=='C') && (_locale[1]=='\0'))  )
        return(__clocalestr);
    else
        return(NULL);
}

#else  /* !defined (_WIN32) */

char * __cdecl setlocale (
        int _category,
        const char *_locale
        )
{
    char * retval;
    pthreadlocinfo ptloci = NULL;
#ifdef _MT
    _ptiddata ptd = _getptd();

    /* Validate category */
    if ( (_category < LC_MIN) || (_category > LC_MAX) )
        return NULL;

    __updatetlocinfo();
    // Note here that we increment the _ownlocale for this thread. We need this
    // to make sure that the locale is not updated to some other locale by call to
    // stricmp().
    // Don't set any flag that aligns with N, P or G
    ptd->_ownlocale |= 0x10;
    __try {
        if ((ptloci = _malloc_crt(sizeof(threadlocinfo))) == NULL)
            return NULL;
        __copytlocinfo_lk(ptloci, ptd->ptlocinfo);

        if (( ptloci != NULL) && (retval = _setlocale_lk(ptloci, _category, _locale))) {
            _mlock(_SETLOCALE_LOCK);
            __try {
                (void)__updatetlocinfoEx_lk(&ptd->ptlocinfo, ptloci);
                __removelocaleref(ptloci);
                // Note that after incrementing _ownlocale, if this thread doesn't
                // have it's own locale, _ownlocale variable should be 1.
                if (ptd->_ownlocale == 1) {
                    (void)__updatetlocinfoEx_lk(&__ptlocinfo, ptd->ptlocinfo);
                    // All these variables are for backward compatiblity.
                    memcpy(__lc_handle, __ptlocinfo->lc_handle, sizeof(LCID)*(LC_MAX-LC_MIN+1));
                    __lc_codepage = __ptlocinfo->lc_codepage;
                    __lc_collate_cp = __ptlocinfo->lc_collate_cp;
                    __lc_clike = __ptlocinfo->lc_clike;
                    __lc_time_curr = __ptlocinfo->lc_time_curr;
                    __lconv = __ptlocinfo->lconv;
                    _pctype = __ptlocinfo->pctype;
                    __mb_cur_max = __ptlocinfo->mb_cur_max;
                }
            } __finally {
                _munlock(_SETLOCALE_LOCK);
            }
        } else {
            __removelocaleref(ptloci);
            __freetlocinfo(ptloci);
        }
    } __finally {
        ptd->_ownlocale &= 0xffffffef;
    }


#else  /* _MT */
    if ( (_category < LC_MIN) || (_category > LC_MAX) )
        return NULL;

    if ( _locale == NULL  )
        return _setlocale_lk(__ptlocinfo, _category, NULL);

    if ((ptloci = _malloc_crt(sizeof(threadlocinfo))) == NULL)
        return NULL;
    __copytlocinfo_lk(ptloci, __ptlocinfo);

    if (( ptloci != NULL) && (retval = _setlocale_lk(ptloci, _category, _locale))) {
        __updatetlocinfoEx_lk(&__ptlocinfo,ptloci);
        __removelocaleref(ptloci);
        memcpy(__lc_handle, __ptlocinfo->lc_handle, sizeof(LCID)*(LC_MAX-LC_MIN+1));
        __lc_codepage = __ptlocinfo->lc_codepage;
        __lc_collate_cp = __ptlocinfo->lc_collate_cp;
        __lc_clike = __ptlocinfo->lc_clike;
        __lc_time_curr = __ptlocinfo->lc_time_curr;
        __lconv = __ptlocinfo->lconv;
        _pctype = __ptlocinfo->pctype;
        __mb_cur_max = __ptlocinfo->mb_cur_max;
    } else {
        __removelocaleref(ptloci);
        __freetlocinfo(ptloci);
    }
#endif  /* _MT */
    return retval;
}

static char * __cdecl _setlocale_lk(
        pthreadlocinfo ploci,
        int _category,
        const char *_locale
        )
{
    char * retval;
    /* Interpret locale */

    if (_category != LC_ALL)
    {
        retval = (_locale) ? _setlocale_set_cat(ploci, _category,_locale) :
            ploci->lc_category[_category].locale;

    } else { /* LC_ALL */
        char lctemp[MAX_LC_LEN];
        int i;
        int same = 1;
        int fLocaleSet = 0; /* flag to indicate if anything successfully set */

        if (_locale != NULL)
        {
            if ( (_locale[0]=='L') && (_locale[1]=='C') && (_locale[2]=='_') )
            {
                /* parse compound locale string */
                size_t len;
                const char * p = _locale;  /* start of string to parse */
                const char * s;

                do {
                    s = strpbrk(p,"=;");

                    if ((s==(char *)NULL) || (!(len=(size_t)(s-p))) || (*s==';'))
                        return NULL;  /* syntax error */

                    /* match with known LC_ strings, if possible, else ignore */
                    for (i=LC_ALL+1; i<=LC_MAX; i++)
                    {
                        if ((!strncmp(__lc_category[i].catname,p,len))
                            && (len==strlen(__lc_category[i].catname)))
                        {
                            break;  /* matched i */
                        }
                    } /* no match if (i>LC_MAX) -- just ignore */

                    if ((!(len = strcspn(++s,";"))) && (*s!=';'))
                        return NULL;  /* syntax error */

                    if (i<=LC_MAX)
                    {
                        strncpy(lctemp, s, len);
                        lctemp[len]='\0';   /* null terminate string */

                        /* don't fail unless all categories fail */
                        if (_setlocale_set_cat(ploci, i,lctemp))
                            fLocaleSet++;       /* record a success */
                    }
                    if (*(p = s+len)!='\0')
                        p++;  /* skip ';', if present */

                } while (*p);

                retval = (fLocaleSet) ? _setlocale_get_all(ploci) : NULL;

            } else { /* simple LC_ALL locale string */

                /* confirm locale is supported, get expanded locale */
                if (retval = _expandlocale((char *)_locale, lctemp, NULL, NULL, _category))
                {
                    for (i=LC_MIN; i<=LC_MAX; i++)
                    {
                        if (i!=LC_ALL)
                        {
                            if (strcmp(lctemp, ploci->lc_category[i].locale))
                            {
                                if (_setlocale_set_cat(ploci, i, lctemp))
                                {
                                    fLocaleSet++;   /* record a success */
                                }
                                else
                                {
                                    same = 0;       /* record a failure */
                                }
                            }
                            else
                                fLocaleSet++;   /* trivial succcess */
                        }
                    }
                    if (same) /* needn't call setlocale_get_all() if all the same */
                    {
                        retval = _setlocale_get_all(ploci);
                        /* retval set above */
                    }
                    else
                        retval = (fLocaleSet) ? _setlocale_get_all(ploci) : NULL;
                }
            }
        } else { /* LC_ALL & NULL */
            retval = _setlocale_get_all (ploci);
        }
    }

    /* common exit point */
    return retval;
} /* setlocale */


static char * __cdecl _setlocale_set_cat (
        pthreadlocinfo ploci,
        int category,
        const char * locale
        )
{
    char * oldlocale;
    LCID oldhandle;
    UINT oldcodepage;
    LC_ID oldid;

    LC_ID idtemp;
    UINT cptemp;
    char lctemp[MAX_LC_LEN];
    char * pch = NULL;
    short out[sizeof(_first_127char)];
    int i;
#ifdef _MT
    _ptiddata _ptd = _getptd();
    struct _is_ctype_compatible *_Lcid_c = _ptd->_setloc_data._Lcid_c; // __setloc_data._Lcid_c is array
    int _LOC_CCACHE = sizeof(_ptd->_setloc_data._Lcid_c)/sizeof(struct _is_ctype_compatible);
#else  /* _MT */
    struct _is_ctype_compatible *_Lcid_c = __setloc_data._Lcid_c; // __setloc_data._Lcid_c is array
    int _LOC_CCACHE = sizeof(__setloc_data._Lcid_c)/sizeof(struct _is_ctype_compatible);
#endif  /* _MT */
    struct _is_ctype_compatible buf1, buf2;

    if (!_expandlocale((char *)locale, lctemp, &idtemp, &cptemp, category))
    {
        return NULL;            /* unrecognized locale */
    }
    if (!strcmp(lctemp, ploci->lc_category[category].locale))
    {
        return ploci->lc_category[category].locale;
    }

    if (!(pch = (char *)_malloc_crt(sizeof(int)+strlen(lctemp)+1)))
    {
        return NULL;  /* error if malloc fails */
    }

    oldlocale = ploci->lc_category[category].locale; /* save for possible restore*/
    oldhandle = ploci->lc_handle[category];
    memcpy((void *)&oldid, (void *)&ploci->lc_id[category], sizeof(oldid));
    oldcodepage = ploci->lc_codepage;

    /* update locale string */
    ploci->lc_category[category].locale = strcpy(pch+sizeof(int),lctemp);
    ploci->lc_handle[category] = MAKELCID(idtemp.wLanguage, SORT_DEFAULT);
    memcpy((void *)&ploci->lc_id[category], (void *)&idtemp, sizeof(idtemp));

    /* To speedup locale based comparisions, we identify if the current
     * local has first 127 character set same as CLOCALE. If yes then
     * ploci->lc_clike = TRUE.
     */

    if (category==LC_CTYPE)
    {
        ploci->lc_codepage = cptemp;
        buf1 = _Lcid_c[_LOC_CCACHE -1];
        /* brings the recently used codepage to the top. or else shifts
         * every thing down by one so that new _Lcid_c can be placed at
         * the top.
         */
        for ( i = 0; i < _LOC_CCACHE; i++)
        {
            if (ploci->lc_codepage == _Lcid_c[i].id)
            {
                if (i) {
                    _Lcid_c[0] = _Lcid_c[i];
                    _Lcid_c[i] = buf1;
                }
                break;
            }
            else
            {
                buf2 = _Lcid_c[i];
                _Lcid_c[i] = buf1;
                buf1 = buf2;
            }
        }
        if ( i == _LOC_CCACHE)
        {
            if ( __crtGetStringTypeA(CT_CTYPE1,
                                      _first_127char,
                                      sizeof(_first_127char),
                                      out,
                                      ploci->lc_codepage,
                                      ploci->lc_handle[LC_CTYPE],
                                      TRUE ))
            {
                int j;
                for ( j = 0; j < sizeof(_first_127char); j++)
                    out[j] = out[j]&0x1ff;
                if ( !memcmp(out, _ctype_loc_style, sizeof(_first_127char)*sizeof(short)))
                    _Lcid_c[0].is_clike = TRUE;
                else
                    _Lcid_c[0].is_clike = FALSE;
            }
            else
                _Lcid_c[0].is_clike = FALSE;
            _Lcid_c[0].id = ploci->lc_codepage;
        }
        ploci->lc_clike = _Lcid_c[0].is_clike;
    }

    if ( category == LC_COLLATE )
        ploci->lc_collate_cp = cptemp;

    if (__lc_category[category].init(ploci))
    {
        /* restore previous state! */
        ploci->lc_category[category].locale = oldlocale;
        _free_crt(pch);
        ploci->lc_handle[category] = oldhandle;
        ploci->lc_codepage = oldcodepage;

        return NULL; /* error if non-zero return */
    }

    /* locale set up successfully */
    /* Cleanup */
    if ((oldlocale != __clocalestr) &&
        (InterlockedDecrement(ploci->lc_category[category].refcount) == 0)
        )
    {
        _ASSERT(0);
        _free_crt(ploci->lc_category[category].refcount);
        _free_crt(ploci->lc_category[category].wlocale);
        ploci->lc_category[category].wlocale = NULL;
    }
    if (pch) {
        *(int *)pch  = 1;
    }
    ploci->lc_category[category].refcount = (int *)pch;

    return ploci->lc_category[category].locale;
} /* _setlocale_set_cat */



static char * __cdecl _setlocale_get_all ( pthreadlocinfo ploci)
{
    int i;
    int same = 1;
    char *pch = NULL;
    int *refcount = NULL;
    /* allocate memory if necessary */
    if ( (refcount = _malloc_crt(sizeof(int)+(MAX_LC_LEN+1) * (LC_MAX-LC_MIN+1) + CATNAMES_LEN))
           == NULL)
        return NULL;

    pch = (char *)(refcount +1);
    *pch = '\0';
    *refcount = 1;
    for (i=LC_MIN+1; ; i++)
    {
        _strcats(pch, 3, __lc_category[i].catname,"=",ploci->lc_category[i].locale);
        if (i<LC_MAX)
        {
            strcat(pch,";");
            if (strcmp(ploci->lc_category[i].locale, ploci->lc_category[i+1].locale))
                same=0;
        }
        else
        {
            if (!same) {
                if (ploci->lc_category[LC_ALL].refcount != NULL &&
                    InterlockedDecrement(ploci->lc_category[LC_ALL].refcount) == 0) {
                    _ASSERT(0);
                    _free_crt(ploci->lc_category[LC_ALL].refcount);
                    _free_crt(ploci->lc_category[LC_ALL].wlocale);
                    ploci->lc_category[LC_ALL].wlocale = NULL;
                }
                ploci->lc_category[LC_ALL].refcount = refcount;
                return ploci->lc_category[LC_ALL].locale = pch;
            } else {
                _free_crt(refcount);
                if (ploci->lc_category[LC_ALL].refcount != NULL &&
                    InterlockedDecrement(ploci->lc_category[LC_ALL].refcount) == 0) {
                    _ASSERT(0);
                    _free_crt(ploci->lc_category[LC_ALL].refcount);
                    _free_crt(ploci->lc_category[LC_ALL].wlocale);
                }
                ploci->lc_category[LC_ALL].wlocale = NULL;
                ploci->lc_category[LC_ALL].refcount = NULL;
                ploci->lc_category[LC_ALL].locale = (char *)NULL;
                return ploci->lc_category[LC_CTYPE].locale;
            }
        }
    }
} /* _setlocale_get_all */


char * _expandlocale (
        char *expr,
        char * output,
        LC_ID * id,
        UINT * cp,
        int category
        )
{
#ifdef _MT
    _psetloc_struct _psetloc_data = &_getptd()->_setloc_data;
#else  /* _MT */
    _psetloc_struct _psetloc_data = &__setloc_data;
#endif  /* _MT */
    UINT *pcachecp = &_psetloc_data->_cachecp;
    LC_ID *pcacheid = &_psetloc_data->_cacheid;
    char *cachein = _psetloc_data->_cachein;
    char *cacheout = _psetloc_data->_cacheout;

    if (!expr)
        return NULL; /* error if no input */


    if (((*expr=='C') && (!expr[1]))
        )  /* for "C" locale, just return */
    {
        *output = 'C';
        output[1] = '\0';
        if (id)
        {
            id->wLanguage = 0;
            id->wCountry  = 0;
            id->wCodePage = 0;
        }
        if (cp)
        {
            *cp = CP_ACP; /* return to ANSI code page */
        }
        return output; /* "C" */
    }

    /* first, make sure we didn't just do this one */
    if (strcmp(cacheout,expr) && strcmp(cachein,expr))
    {
        /* do some real work */
        LC_STRINGS names;

        if (__lc_strtolc((LC_STRINGS *)&names, (const char *)expr))
            return NULL;  /* syntax error */

        if (!__get_qualified_locale((LPLC_STRINGS)&names,
            (LPLC_ID)pcacheid, (LPLC_STRINGS)&names))
            return NULL;    /* locale not recognized/supported */

        /* begin: cache atomic section */

        *pcachecp = pcacheid->wCodePage;

        __lc_lctostr((char *)cacheout, &names);

        /* Don't cache "" empty string */
        if (*expr)
            strcpy(cachein, expr);
        else
            strcpy(cachein, cacheout);

        /* end: cache atomic section */
    }
    if (id)
        memcpy((void *)id, (void *)pcacheid, sizeof(*pcacheid));   /* possibly return LC_ID */
    if (cp)
        memcpy((void *)cp, (void *)pcachecp, sizeof(*pcachecp));   /* possibly return cp */

    strcpy(output,cacheout);
    return cacheout; /* return fully expanded locale string */
}

/* helpers */

int __cdecl __init_dummy(pthreadlocinfo ploci)  /* default routine for locale initializer */
{
        return 0;
}

void _strcats ( char *outstr, int n, ...)
{
    int i;
    va_list substr;

    va_start (substr, n);

    for (i =0; i<n; i++)
    {
        strcat(outstr, va_arg(substr, char *));
    }
    va_end(substr);
}

int __lc_strtolc ( LC_STRINGS *names, const char *locale)
{
    int i;
    size_t len;
    char ch;

    memset((void *)names, '\0', sizeof(LC_STRINGS));  /* clear out result */

    if (*locale=='\0')
        return 0; /* trivial case */

    /* only code page is given */
    if (locale[0] == '.' && locale[1] != '\0')
    {
        strcpy((char *)names->szCodePage, &locale[1]);
        return 0;
    }

    for (i=0; ; i++)
    {
        if (!(len=strcspn(locale,"_.,")))
            return -1;  /* syntax error */

        ch = locale[len];

        if ((i==0) && (len<MAX_LANG_LEN) && (ch!='.'))
            strncpy((char *)names->szLanguage, locale, len);

        else if ((i==1) && (len<MAX_CTRY_LEN) && (ch!='_'))
            strncpy((char *)names->szCountry, locale, len);

        else if ((i==2) && (len<MAX_CP_LEN) && (ch=='\0' || ch==','))
            strncpy((char *)names->szCodePage, locale, len);

        else
            return -1;  /* error parsing locale string */

        if (ch==',')
        {
            /* modifier not used in current implementation, but it
               must be parsed to for POSIX/XOpen conformance */
        /*  strncpy(names->szModifier, locale, MAX_MODIFIER_LEN-1); */
            break;
        }

        if (!ch)
            break;
        locale+=(len+1);
    }
    return 0;
}

void __lc_lctostr ( char *locale, const LC_STRINGS *names)
{
    strcpy(locale, (char *)names->szLanguage);
    if (*(names->szCountry))
        _strcats(locale, 2, "_", names->szCountry);
    if (*(names->szCodePage))
        _strcats(locale, 2, ".", names->szCodePage);
    /* if (names->szModifier)
     * _strcats(locale, 2, ",", names->szModifier);
     * */
}


#endif  /* !defined (_WIN32) */
