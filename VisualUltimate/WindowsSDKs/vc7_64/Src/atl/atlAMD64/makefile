D=retail

LIB32=lib
OBJS=$(D)\stdcallthunk_amd64.obj $(D)\QIThunk.obj
TARGET=$(D)\atlamd64.lib

GOAL : createdir $(TARGET)
	copy $(TARGET) ..\..\..\lib\amd64

createdir :
	if not exist $(D)\. md $(D)
	if not exist ..\..\..\lib\amd64 md ..\..\..\lib\amd64
	
$(TARGET) : $(OBJS)
	@-if exist $@ erase $@
	@$(LIB32) /out:$@ @<<
$(OBJS)
<<

clean :
	erase $(D)\*.* /q
	
.SUFFIXES: .s

.s{$D}.obj:
	ml64 $(ml64FLAGS) -c -Fo$@ $<
	