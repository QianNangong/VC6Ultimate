        page    ,132
        title   chkstk - C stack checking routine
;***
;chkstk.asm - C stack checking routine
;
;       Copyright (c) Microsoft Corporation. All rights reserved.
;
;Purpose:
;       Provides support for automatic stack checking in C procedures
;       when stack checking is enabled.
;
;*******************************************************************************

.xlist
        include cruntime.inc
.list

; size of a page of memory

_PAGESIZE_      equ     1000h


        CODESEG

page
;***
;_chkstk - check stack upon procedure entry
;
;Purpose:
;       Provide stack checking on procedure entry. Method is to simply probe
;       each page of memory required for the stack in descending order. This
;       causes the necessary pages of memory to be allocated via the guard
;       page scheme, if possible. In the event of failure, the OS raises the
;       _XCPT_UNABLE_TO_GROW_STACK exception.
;
;       NOTE:  Currently, the (EAX < _PAGESIZE_) code path falls through
;       to the "lastpage" label of the (EAX >= _PAGESIZE_) code path.  This
;       is small; a minor speed optimization would be to special case
;       this up top.  This would avoid the painful save/restore of
;       ecx and would shorten the code path by 4-6 instructions.
;
;Entry:
;       EAX = size of local frame
;
;Exit:
;       ESP = new stackframe, if successful
;
;Uses:
;       EAX
;
;Exceptions:
;       _XCPT_GUARD_PAGE_VIOLATION - May be raised on a page probe. NEVER TRAP
;                                    THIS!!!! It is used by the OS to grow the
;                                    stack on demand.
;       _XCPT_UNABLE_TO_GROW_STACK - The stack cannot be grown. More precisely,
;                                    the attempt by the OS memory manager to
;                                    allocate another guard page in response
;                                    to a _XCPT_GUARD_PAGE_VIOLATION has
;                                    failed.
;
;*******************************************************************************

public _alloca_probe

_chkstk proc

_alloca_probe    =  _chkstk

        cmp     eax, _PAGESIZE_         ; more than one page?
        jae     short probesetup        ;   yes, go setup probe loop
                                        ;   no
        neg     eax                     ; compute new stack pointer in eax
        add     eax,esp
        add     eax,4
        test    dword ptr [eax],eax     ; probe it
        xchg    eax,esp
        mov     eax,dword ptr [eax]
        push    eax
        ret

probesetup:
        push    ecx                     ; save ecx
        lea     ecx,[esp] + 8           ; compute new stack pointer in ecx
                                        ; correct for return address and
                                        ; saved ecx

probepages:
        sub     ecx,_PAGESIZE_          ; yes, move down a page
        sub     eax,_PAGESIZE_          ; adjust request and...

        test    dword ptr [ecx],eax     ; ...probe it

        cmp     eax,_PAGESIZE_          ; more than one page requested?
        jae     short probepages        ; no

lastpage:
        sub     ecx,eax                 ; move stack down by eax
        mov     eax,esp                 ; save current tos and do a...

        test    dword ptr [ecx],eax     ; ...probe in case a page was crossed

        mov     esp,ecx                 ; set the new stack pointer

        mov     ecx,dword ptr [eax]     ; recover ecx
        mov     eax,dword ptr [eax + 4] ; recover return address

        push    eax                     ; prepare return address
                                        ; ...probe in case a page was crossed
        ret
_chkstk endp

        end
