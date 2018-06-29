; This is a part of the Active Template Library.
; Copyright (C) 1996-2000 Microsoft Corporation
; All rights reserved.
;
; This source code is only intended as a supplement to the
; Active Template Library Reference and related
; electronic documentation provided with the library.
; See these sources for detailed information regarding the	
; Active Template Library product.


PUBLIC	CComStdCallThunkHelper

_TEXT	SEGMENT

CComStdCallThunkHelper PROC NEAR

	mov rax, rcx;		; save the 1st argument (pThunk pointer)
	mov rcx, 8[rcx];	; replace the 1st argument (pThunk) with the pThunk->pThis
	jmp qword ptr 16[rax];	; get pThunk->pfn

CComStdCallThunkHelper ENDP

_TEXT	ENDS

END