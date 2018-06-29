// This is a part of the Active Template Library.
// Copyright (C) 1996-2000 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the	
// Active Template Library product.

	.global	_WndProcThunkProc
	.proc	_WndProcThunkProc
	.align	32

_WndProcThunkProc:
	// On entry, gp is actually a pointer to the pRealWndProcDesc member of 
	// the _WndProcThunk struct
	mov	r14=gp
	ld8	r15=[r14],8	// r15 = thunk.pRealWndProcDesc, r14 = &thunk.pThis
	ld8	r16=[r15],8	// r16 = thunk.pRealWndProcDesc->pfn, r15 = &thunk.pRealWndProcDesc->gp
	ld8	gp=[r15]	// gp = thunk.pRealWndProcDesc->gp
	ld8	r32=[r14]	// r32 = thunk.pThis
	mov	b6=r16
	br.cond.sptk.many	b6
	.endp	_WndProcThunkProc
