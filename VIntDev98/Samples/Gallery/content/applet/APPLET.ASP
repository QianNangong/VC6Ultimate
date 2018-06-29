<% @LANGUAGE="VBSCRIPT" %>
<HTML>
<HEAD> 
</HEAD>
<%
	' GetPrefixValue is a helper function which removes the 
	' document name from a fully-qualified URL
	Function GetPrefixValue
		Dim strFullPath, strLastChar
		strFullPath = "http://"
		strFullPath = strFullPath & Request.ServerVariables("SERVER_NAME")
		strFullPath = strFullPath & Request.ServerVariables("PATH_INFO")
		' now str has a value like: "http://servname/exair/freqflyer/default.asp"
		' we need to cut off the "/default.asp" part
		strLastChar = ""
		Do Until strLastChar = "/"
			strLastChar = right(strFullPath, 1)
			strFullPath = left(strFullPath, len(strFullPath) - 1)	
		Loop
		GetPrefixValue = strFullPath
	End Function
%>
<BODY bgColor=#ffffff leftMargin=40 link=#500000 vLink=#505050>
<BASEFONT FACE="VERDANA, ARIAL, HELVETICA" SIZE=2>
		<APPLET
			CODE=CoolHeadLines.class
			NAME=CoolHeadLines
			CODEBASE=Applets
			WIDTH=170
			HEIGHT=76>

			<PARAM NAME=BackColor VALUE="255 255 255">
			<PARAM NAME=TextColor VALUE="0 0 0">
			<PARAM NAME=HiliteTextColor VALUE="60 179 113">
			<PARAM NAME=ScrollDelay VALUE=10>
			<PARAM NAME=MessageDelay VALUE=4>	
	  		<PARAM NAME=URLPrefix VALUE="<%=GetPrefixValue()%>">

			'Build up the list of headlines
			<PARAM NAME=Text0 VALUE="Data Entry">
			<PARAM NAME=URL0 VALUE="../Sample.asp?SampID=1">
			<PARAM NAME=Text1 VALUE="Quote of the Day">
			<PARAM NAME=URL1 VALUE="../Sample.asp?SampID=2">
			<PARAM NAME=Text2 VALUE="Databound Grid">
			<PARAM NAME=URL2 VALUE="../Sample.asp?SampID=3">
			<PARAM NAME=Text3 VALUE="Custom Query">
			<PARAM NAME=URL3 VALUE="../Sample.asp?SampID=4">
			<PARAM NAME=Text4 VALUE="User Preferences">
			<PARAM NAME=URL4 VALUE="../Sample.asp?SampID=5">
			<PARAM NAME=Text5 VALUE="Hit Counter">
			<PARAM NAME=URL5 VALUE="../Sample.asp?SampID=6">
			<PARAM NAME=Text6 VALUE="Survey">
			<PARAM NAME=URL6 VALUE="../Sample.asp?SampID=7">
			<PARAM NAME=Text7 VALUE="Using Controls">
			<PARAM NAME=URL7 VALUE="../Sample.asp?SampID=8">
			<PARAM NAME=Text8 VALUE="Shopping Cart">
			<PARAM NAME=URL8 VALUE="../Sample.asp?SampID=9">
			<PARAM NAME=Text9 VALUE="User Notification">
			<PARAM NAME=URL9 VALUE="../Sample.asp?SampID=10">
			<PARAM NAME=Text10 VALUE="Guest Book">
			<PARAM NAME=URL10 VALUE="../Sample.asp?SampID=12">
			<PARAM NAME=Text11 VALUE="Transitions & Filters">
			<PARAM NAME=URL11 VALUE="../Sample.asp?SampID=13">
			<PARAM NAME=Text12 VALUE="Timeline">
			<PARAM NAME=URL12 VALUE="../Sample.asp?SampID=18">
			<PARAM NAME=Text13 VALUE="Random Ad">
			<PARAM NAME=URL13 VALUE="../Sample.asp?SampID=15">
			<PARAM NAME=Text14 VALUE="Using Themes">
			<PARAM NAME=URL14 VALUE="../Sample.asp?SampID=16">
			<PARAM NAME=Text15 VALUE="Multimedia">
			<PARAM NAME=URL15 VALUE="../Sample.asp?SampID=17">
			<PARAM NAME=NumItems VALUE=16>
		</APPLET>
<BR>
This applet shows the names of the other samples available in the Gallery. To see more 
information on a particular sample just click on the title.
</BODY>
</HTML>