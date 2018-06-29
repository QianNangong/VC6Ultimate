@echo off
midl /nologo /no_warn AEIntrfc.IDL /tlb AEIntrfc.TLB
midl /nologo /no_warn AEExpdtr.IDL /tlb AEExpdtr.TLB
pause
