<SCRIPT RUNAT=SERVER Language="JavaScript">
//*****************************************************************
// Microsoft Scripting Library 
// ADO conversion utilities for extracting data on the server.
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
//*****************************************************************

//*****************************************************************
// function RecordSetAsArray(rsADO,includeHeaderRow)
//	A helper function to convert an ADO recordset into a
//	jscript array.  Optionally may include the field names
//	as the first row of the array by setting includeHeaderRow
//	parameter to true.
//
//*****************************************************************
function RecordSetAsArray(rsADO,includeHeaderRow)
{
	var rsArray = new Array;
	var nFields = rsADO.Fields.Count;
	if (nFields)
	{
		var n = 0;
		if (includeHeaderRow)
		{	// include first row as header of field names
			var rsRow = new Array(nFields);
			for (var i=0; i < nFields; i++)
				rsRow[i] = rsADO(i).name;
			rsArray[n++] = rsRow;
		}
			
		while (!rsADO.EOF)
		{	// add each record to array
			rsRow = new Array(nFields);
			for (var i=0; i < nFields; i++)
				rsRow[i] = rsADO(i).value;
			rsArray[n++] = rsRow;
			rsADO.MoveNext();
		}
	}
	return rsArray;
}

//*****************************************************************
// function RecordSetAsDSO(rsADO)
//	A helper function to convert an ADO recordset into an
//	HTML Data Source Object (DSO).  This function creates
//	an HTML table where the <TD> cells are identified (id) 
//	as the corresponsing ADO field names, and the <TD> cell
//	contents contain the ADO field values.
//
//	The return value is a string containing the HTML DSO.
//*****************************************************************
function RecordSetAsDSO(rsADO)
{
	var htmlDSO = '';
	var nFields = rsADO.Fields.Count;
	if (nFields)
	{
		var n = 0;
		htmlDSO += '<' + 'table>\n';			
		while (!rsADO.EOF)
		{	// add each record to table
			htmlDSO += '<' + 'tr>';			
			for (var i=0; i < nFields; i++)
			{
				htmlDSO += '<' + 'td id=' + rsADO(i).name + '>' + rsADO(i).value + '<' + '/td>';
			}
			htmlDSO += '<' + '/tr>\n';			
			rsADO.MoveNext();
		}
		htmlDSO += '<' + '/table>';			
	}
	return htmlDSO;
}

//*****************************************************************
// function RecordSetAsList(rsADO,field)
//	A helper function to convert an ADO recordset into a
//	jscript array.  The field parameter identifies which
//	field to put into the list. The field paramter can be 
//	either a field index or a field name.
//
//*****************************************************************
function RecordSetAsList(rsADO,field)
{
	var rsArray = new Array;
	var nFields = rsADO.Fields.Count;
	if (nFields)
	{
		var n = 0;
		while (!rsADO.EOF)
		{	// add each value of field into array
			rsArray[n++] = rsADO(field).value;
			rsADO.MoveNext();
		}
	}
	return rsArray;
}

//*****************************************************************
// function RecordAsObject(rsADO,objectName)
//	A helper function to convert an ADO record into a jscript 
//	object. The field names of the record will be used as the 
//	object property names, and the data as the object values. 
//	Optionally may indicate the name of the object constructor 
//	to be returned, defaults is 'RSObject'.
//
//*****************************************************************
function RecordAsObject(rsADO,objectName)
{
	if (typeof(objectName) == 'undefined')
		objectName = 'RSObject';
	var strObject = 'function ' + objectName + '() { ';
	var nFields = rsADO.Fields.Count;
	if (!rsADO.EOF)
	{
		var value = new String;
		var propName = new String;
		for (var i=0; i < nFields; i++)
		{
			propName = rsADO(i).name;
			var c0 = propName.charAt(0);
			if ((propName.indexOf(' ') == -1) && ((c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z')))
			{	// exclude fields that are invalid variable names 
				value = rsADO(i).value;
				if (isNaN(parseFloat(value)))
					strObject += 'this.' + propName + ' = "' + value + '"; ';
				else
					strObject += 'this.' + propName + ' = ' + value + '; ';
			}
		}
	}
	strObject += '}';
	eval(strObject);
	return eval('new ' + objectName);
}

</SCRIPT>
