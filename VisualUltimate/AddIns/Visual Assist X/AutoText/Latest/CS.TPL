a::#d:
#define 
a::#if:
#if $end$
$selected$
#endif

a::b:
bool
a::r:
return
a::n:
null
a::fl:
float
a::n0:
!= 0
a::Gui:
GuidAttribute("$GUID_STRING$"), 
a:namespace { ... }:namespace:
namespace $end$
{
	$selected$
}

a::struct:
struct $end$ 
{
}
a::switch:
switch ($end$)
{
	$selected$
}

a::switch:
switch ($end$)
{
case :
	break;
}

a:://-:
// $end$ [$MONTH$/$DAY$/$YEAR$ %USERNAME%]
a::///:
//////////////////////////////////////////////////////////////////////////

a:<summary> ... </summary>:sum:
/// <summary>
///   $end$
/// </summary>

a::/*-:
/*
 *	$end$
 */
a::/**:
/************************************************************************/
/* $end$                                                                     */
/************************************************************************/
a:if () { ... }:if:
if ($end$)
{
	$selected$
}

a:if () { ... } else { }:if:
if ($end$)
{
	$selected$
} 
else
{
}

a:if () { } else { ... }::
if ($end$)
{
} 
else
{
	$selected$
}

a:while () { ... }:while:
while ($end$)
{
	$selected$
}

a:for () { ... }:for:
for ($end$)
{
	$selected$
}

a:for loop forward:forr:
for (int $Index$ = 0; $Index$ < $Length$ ; $Index$++)
{
	$end$
}

a:for loop reverse:forr:
for (int $Index$ = $Length$ - 1; $Index$ >= 0 ; $Index$--)
{
	$end$
}

a::fore:
foreach ($end$)
{
}

a:do { ... } while ():do:
do 
{
	$selected$
} while ($end$);

a:try { ... } catch {}:try:
try
{
	$selected$
}
catch (System.Exception ex)
{
	$end$
}

a:try { ... } catch {} finally {}:try:
try
{
	$selected$
}
catch (System.Exception ex)
{
	$end$
}
finally
{
}

a::bas:
base.$MethodName$($MethodArgs$);


a:File header detailed::
/********************************************************************
	created:	$DATE$
	created:	$DAY$:$MONTH$:$YEAR$   $HOUR$:$MINUTE$
	filename: 	$FILE$
	file path:	$FILE_PATH$
	file base:	$FILE_BASE$
	file ext:	$FILE_EXT$
	author:		$Author$
	
	purpose:	$end$
*********************************************************************/

