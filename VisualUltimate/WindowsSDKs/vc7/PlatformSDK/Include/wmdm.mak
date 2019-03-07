#
# Microsoft Windows Media Technologies.
# Copyright (c) Microsoft Corporation. All rights reserved.
#

#----- Include the PSDK's WIN32.MAK to pick up defines------------------------------------
!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory-------
all: $(OUTDIR)
	$(midl) -Zp8 /out $(OUTDIR) .\mswmdm.idl
	$(midl) -Zp8 /out $(OUTDIR) .\wmdmlog.idl

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)


#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
