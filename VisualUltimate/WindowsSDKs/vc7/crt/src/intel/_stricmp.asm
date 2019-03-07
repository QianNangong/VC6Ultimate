        page    ,132
        title   stricmp
;***
;stricmp.asm - contains ASCII only, case-insensitive, string comparision routine
;       __ascii_stricmp
;
;       Copyright (c) Microsoft Corporation. All rights reserved.
;
;Purpose:
;       contains __ascii_stricmp()
;
;*******************************************************************************

        .xlist
        include cruntime.inc
        .list

page
;***
;int __ascii_stricmp(dst, src) - compare strings, ignore case
;
;Purpose:
;       _stricmp/_strcmpi perform a case-insensitive string comparision.
;       For differences, upper case letters are mapped to lower case.
;       Thus, "abc_" < "ABCD" since "_" < "d".
;
;       Algorithm:
;
;       int _strcmpi (char * dst, char * src)
;       {
;               int f,l;
;
;               do {
;                       f = tolower(*dst);
;                       l = tolower(*src);
;                       dst++;
;                       src++;
;               } while (f && f == l);
;
;               return(f - l);
;       }
;
;Entry:
;       char *dst, *src - strings to compare
;
;Exit:
;       AX = -1 if dst < src
;       AX =  0 if dst = src
;       AX = +1 if dst > src
;
;Uses:
;       CX, DX
;
;Exceptions:
;
;*******************************************************************************

        CODESEG

        public  __ascii_stricmp
__ascii_stricmp proc \
        uses edi esi ebx, \
        dst:ptr, \
        src:ptr

        ; load up args

        mov     esi,[src]       ; esi = src
        mov     edi,[dst]       ; edi = dst

        mov     al,-1           ; fall into loop

        align   4

chk_null:
        or      al,al
        jz      short done

        mov     al,[esi]        ; al = next source byte
        add     esi,1
        mov     ah,[edi]        ; ah = next dest byte
        add     edi,1

        cmp     ah,al           ; first try case-sensitive comparision
        je      short chk_null  ; match

        sub     al,'A'
        cmp     al,'Z'-'A'+1
        sbb     cl,cl
        and     cl,'a'-'A'
        add     al,cl
        add     al,'A'          ; tolower(*dst)

        xchg    ah,al           ; operations on AL are shorter than AH

        sub     al,'A'
        cmp     al,'Z'-'A'+1
        sbb     cl,cl
        and     cl,'a'-'A'
        add     al,cl
        add     al,'A'          ; tolower(*src)

        cmp     al,ah           ; inverse of above comparison -- AL & AH are swapped
        je      short chk_null

                                ; dst < src     dst > src
        sbb     al,al           ; AL=-1, CY=1   AL=0, CY=0
        sbb     al,-1           ; AL=-1         AL=1
done:
        movsx   eax,al          ; extend al to eax

        ret

__ascii_stricmp endp
        end
