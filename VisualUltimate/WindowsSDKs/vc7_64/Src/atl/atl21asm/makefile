D=retail

LIB32=lib
OBJS=$(D)\stdcallthunk.obj $(D)\QIThunk.obj
TARGET=$(D)\atl21asm.lib

GOAL : createdir $(TARGET)
	copy $(TARGET) ..\..\..\lib\IA64

createdir :
	if not exist $(D)\. md $(D)
	
$(TARGET) : $(OBJS)
	@-if exist $@ erase $@
	@$(LIB32) /out:$@ @<<
$(OBJS)
<<

clean :
	erase $(D)\*.* /q
	
.SUFFIXES: .s

.s{$D}.obj:
	ias $(IASFLAGS) -o $@ $<
	