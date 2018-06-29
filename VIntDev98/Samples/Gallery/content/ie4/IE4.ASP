<%@ LANGUAGE="VBSCRIPT" %>

<%If InStr(Request.ServerVariables("HTTP_User_Agent"),"MSIE 4") = 0 then%>
This sample requires Microsoft Internet Explorer 4.0 or higher.
<%Else%>
<FRAMESET ROWS="90,*" FRAMESPACING=0>
	<FRAME NAME=Header SRC=IE4.htm SCROLLING=None NORESIZE FRAMEBORDER=0>
	<FRAME NAME=IE4Main SRC=Main.htm SCROLLING=Auto NORESIZE FRAMEBORDER=0>
</FRAMESET>
<%End If%>


