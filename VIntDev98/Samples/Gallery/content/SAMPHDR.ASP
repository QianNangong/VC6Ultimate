<%@ LANGUAGE=VBScript %>
<%
Select Case Request("SampID")
	Case 1
		Title="Data Entry"
		Run="DEntry/Entry.asp"
		BTS="DEntryB.htm"
	Case 2
		Title="Quote of the Day"
		Run="Quote/Quote.asp"
		BTS="QuoteB.htm"
	Case 3
		Title="Databound Grid"
		Run="Grid/Grid.asp"
		BTS="GridB.htm"
	Case 4
		Title="Custom Query"
		Run="Query/Query.asp"
		BTS="QueryB.htm"
	Case 5
		Title="User Preferences"
		Run="Prefer/Prefer.asp"
		BTS="PrefB.htm"
	Case 6
		Title="Hit Counter"
		Run="Counter/Counter.asp"
		BTS="CounterB.htm"
	Case 7
		Title="Survey"
		Run="Survey/Survey.asp"
		BTS="SurveyB.htm"
	Case 8
		Title="Using Controls"
		Run="Controls/DTCTest.asp"
		BTS="ControlB.htm"
	Case 9
		Title="Shopping Cart"
		Run="ShopCart/ShopCart.htm"
		BTS="ShopCrtB.htm"
	Case 10
		Title="User Notification"
		Run="UserNote/UserNote.asp"
		BTS="UserNtB.htm"
	Case 11
		Title="Email Notification"
		Run="Email/Email.htm"
		BTS="EmailB.htm"
	Case 12
		Title="Guest Book"
		Run="GuestBk/GuestBk.asp"
		BTS="GuestBkB.htm"
	Case 13
		Title="Transitions & Filters"
		Run="IE4/IE4.asp"
		BTS="IE4B.htm"
	Case 14
		Title="VJ++ Applet"
		Run="Applet/Applet.asp"
		BTS="AppletB.htm"
	Case 15
		Title="Random Advertisement"
		Run="RandomAd/RandomAd.asp"
		BTS="RandAdB.htm"
	Case 16
		Title="Using Themes"
		Run="Themes/Themes.htm"
		BTS="ThemeB.htm"
	Case 17
		Title="Multimedia"
		Run="MMedia/MMedia.htm"
		BTS="MMediaB.htm"
	Case 18
		Title="Time Line"
		Run="TimeLine/TLFish.htm"
		BTS="TimeLnB.htm"
End Select
%>
<HTML>
<HEAD>
<META NAME="GENERATOR" Content="Microsoft Visual Studio 6.0">
</HEAD>
<body bgcolor="#FFFFFF" TOPMARGIN="56" LEFTMARGIN="40" LINK="FFFFFF" VLINK="#FFFFFF">
<font FACE="Arial, Helvetica, Verdana" SIZE="2">
<font SIZE="5"><%=Title%> Sample</font>
<br><br>
<table CELLSPACING="0" CELLPADDING="0" WIDTH="100%">
	<tr>
		<td BGCOLOR="#510000"><font FACE="Arial, Helvetica, Verdana" SIZE="1"><nobr>&nbsp;&nbsp;<a HREF="<%=Run%>" TARGET="SampMain"><b>Run Sample</b></a>&nbsp;&nbsp;</nobr></td>
		<td BGCOLOR="#FFFFFF" WIDTH="2">&nbsp;</td>
		<td BGCOLOR="#FFFFFF" WIDTH="60%">&nbsp;</td>
		<td BGCOLOR="#FFFFFF" WIDTH="2">&nbsp;</td>
		<td BGCOLOR="#510000"><font FACE="Arial, Helvetica, Verdana" SIZE="1"><nobr>&nbsp;&nbsp;<a HREF="<%=BTS%>" TARGET="SampMain"><b>Behind the Scenes</b></a>&nbsp;&nbsp;</nobr></td>
	</tr>
</table>
</BODY>
</HTML>
