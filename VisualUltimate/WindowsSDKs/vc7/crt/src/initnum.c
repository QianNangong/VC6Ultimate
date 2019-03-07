/***
*initnum.c - contains __init_numeric
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the locale-category initialization function: __init_numeric().
*
*       Each initialization function sets up locale-specific information
*       for their category, for use by functions which are affected by
*       their locale category.
*
*       *** For internal use by setlocale() only ***
*
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#include <setlocal.h>
#include <malloc.h>
#include <nlsint.h>
#include <dbgint.h>

void __cdecl __free_lconv_num(struct lconv *);

extern struct lconv *__lconv_intl;

#ifdef _MT
/*
 * Reference counter for numeric locale info. The value is non-NULL iff the
 * numeric info is not from the C locale.
 */
int *__lconv_num_refcount;

extern int *__lconv_intl_refcount;
#endif  /* _MT */

static void fix_grouping(
        char *grouping
        )
{
        /*
         * ANSI specifies that the fields should contain "\3" [\3\0] to indicate
         * thousands groupings (100,000,000.00 for example).
         * NT uses "3;0"; ASCII 3 instead of value 3 and the ';' is extra.
         * So here we convert the NT version to the ANSI version.
         */

        while (*grouping)
        {
            /* convert '3' to '\3' */
            if (*grouping >= '0' && *grouping <= '9')
            {
                *grouping = *grouping - '0';
                grouping++;
            }

            /* remove ';' */
            else if (*grouping == ';')
            {
                char *tmp = grouping;

                do
                    *tmp = *(tmp+1);
                while (*++tmp);
            }

            /* unknown (illegal) character, ignore */
            else
                grouping++;
        }
}

/***
*int __init_numeric() - initialization for LC_NUMERIC locale category.
*
*Purpose:
*
*Entry:
*       None.
*
*Exit:
*       0 success
*       1 fail
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __init_numeric (
        void
        )
{
        struct lconv *lc;
        int ret = 0;
        LCID ctryid;
#ifdef _MT
        int *lc_refcount;
#endif  /* _MT */

        if ( (__lc_handle[LC_NUMERIC] != _CLOCALEHANDLE) ||
             (__lc_handle[LC_MONETARY] != _CLOCALEHANDLE) )
        {
            /*
             * Allocate structure filled with NULL pointers
             */
            if ( (lc = (struct lconv *)_calloc_crt(1, sizeof(struct lconv)))
                 == NULL )
                return 1;

            /*
             * Copy over all fields (esp., the monetary category)
             */
            *lc = *__lconv;

#ifdef _MT
            /*
             * Allocate a new reference counter for the lconv structure
             */
            if ( (lc_refcount = _malloc_crt(sizeof(int))) == NULL )
            {
                _free_crt(lc);
                return 1;
            }
            *lc_refcount = 0;
#endif  /* _MT */

            if ( __lc_handle[LC_NUMERIC] != _CLOCALEHANDLE )
            {
#ifdef _MT
                /*
                 * Allocate a new reference counter for the numeric info
                 */
                if ( (__lconv_num_refcount = _malloc_crt(sizeof(int))) == NULL )
                {
                    _free_crt(lc);
                    _free_crt(lc_refcount);
                    return 1;
                }
                *__lconv_num_refcount = 0;
#endif  /* _MT */

                /*
                 * Numeric data is country--not language--dependent. NT
                 * work-around.
                 */
                ctryid = MAKELCID(__lc_id[LC_NUMERIC].wCountry, SORT_DEFAULT);

                ret |= __getlocaleinfo(LC_STR_TYPE, ctryid, LOCALE_SDECIMAL,
                        (void *)&lc->decimal_point);
                ret |= __getlocaleinfo(LC_STR_TYPE, ctryid, LOCALE_STHOUSAND,
                        (void *)&lc->thousands_sep);
                ret |= __getlocaleinfo(LC_STR_TYPE, ctryid, LOCALE_SGROUPING,
                        (void *)&lc->grouping);

                if (ret) {
                        /* Clean up before returning failure */
                        __free_lconv_num(lc);
                        _free_crt(lc);
#ifdef _MT
                        _free_crt(lc_refcount);
#endif  /* _MT */
                        return -1;
                }

                fix_grouping(lc->grouping);
            }
            else {
                /*
                 * C locale for just the numeric category.
                 */
#ifdef _MT
                /*
                 * NULL out the reference count pointer
                 */
                __lconv_num_refcount = NULL;
#endif  /* _MT */
                lc->decimal_point = __lconv_c.decimal_point;
                lc->thousands_sep = __lconv_c.thousands_sep;
                lc->grouping = __lconv_c.grouping;
            }

            /*
             * Clean up old __lconv and reset it to lc
             */
#ifdef _MT
            /*
             * If this is part of LC_ALL, then we need to free the old __lconv
             * set up in init_monetary() before this.
             */
            if ( (__lconv_intl_refcount != NULL) &&
                 (*__lconv_intl_refcount == 0) &&
                 (__lconv_intl_refcount != __ptlocinfo->lconv_intl_refcount) )
            {
                _free_crt(__lconv_intl_refcount);
                _free_crt(__lconv_intl);
            }
            __lconv_intl_refcount = lc_refcount;
#else  /* _MT */
            __free_lconv_num(__lconv);

            /*
             * Recall that __lconv is dynamically allocated (hence must be
             * freed) iff __lconv and __lconv_intl are equal iff __lconv_intl
             * is non-NULL.
             */
            _free_crt(__lconv_intl);
#endif  /* _MT */

            __lconv = __lconv_intl = lc;

        }
        else {
            /*
             * C locale for BOTH numeric and monetary categories.
             */
#ifdef _MT
            /*
             * If this is part of LC_ALL, then we need to free the old __lconv
             * set up in init_monetary() before this.
             */
            if ( (__lconv_intl_refcount != NULL) &&
                 (*__lconv_intl_refcount == 0) &&
                 (__lconv_intl_refcount != __ptlocinfo->lconv_intl_refcount) )
            {
                _free_crt(__lconv_intl_refcount);
                _free_crt(__lconv_intl);
            }
            /*
             * NULL out the reference count pointer
             */
            __lconv_num_refcount = NULL;
            __lconv_intl_refcount = NULL;
#else  /* _MT */
            __free_lconv_num(__lconv);

            /*
             * Recall that __lconv is dynamically allocated (hence must be
             * freed) iff __lconv and __lconv_intl are equal iff __lconv_intl
             * is non-NULL.
             */
            _free_crt(__lconv_intl);
#endif  /* _MT */
            __lconv = &__lconv_c;           /* point to new one */
            __lconv_intl = NULL;

        }

        /*
         * set global decimal point character
         */
        *__decimal_point = *__lconv->decimal_point;
        __decimal_point_length = 1;

        return 0;

}

/*
 *  Free the lconv numeric strings.
 *  Numeric values do not need to be freed.
 */
void __cdecl __free_lconv_num(
        struct lconv *l
        )
{
        if (l == NULL)
            return;

#ifdef _MT
        if ( (l->decimal_point != __lconv->decimal_point) &&
             (l->decimal_point != __lconv_c.decimal_point) )
#else  /* _MT */
        if ( l->decimal_point != __lconv_c.decimal_point )
#endif  /* _MT */
            _free_crt(l->decimal_point);

#ifdef _MT
        if ( (l->thousands_sep != __lconv->thousands_sep) &&
             (l->thousands_sep != __lconv_c.thousands_sep) )
#else  /* _MT */
        if ( l->thousands_sep != __lconv_c.thousands_sep )
#endif  /* _MT */
            _free_crt(l->thousands_sep);

#ifdef _MT
        if ( (l->grouping != __lconv->grouping) &&
             (l->grouping != __lconv_c.grouping) )
#else  /* _MT */
        if ( l->grouping != __lconv_c.grouping )
#endif  /* _MT */
            _free_crt(l->grouping);
}
