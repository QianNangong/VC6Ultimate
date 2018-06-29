<SCRIPT RUNAT=SERVER Language="JavaScript">
//*****************************************************************
// MSL : Microsoft Scripting Libary 
// Remote Scripting utilities for server.
//
//	The Remote Scripting utilities for the server primarily 
//	consists of the RSDispatch routine.  This is the function
//	placed at the top of the ASP page which handles dispatching
//	to a valid public_description function and marshalls the
//	return value back to the client.
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
//*****************************************************************

//*****************************************************************
// function RSDispatch()
//	This function is used to dispatch to the proper server
//	function in the public_description object based on the
//	Request.QueryString("_method") parameter passed in.
//
//*****************************************************************
function RSDispatch(vtable,methodname)
{
	if (typeof(methodname) != 'string')
	{
		var methodtype = Request.QueryString('_mtype') + '';
		if (methodtype != 'execute')
			return false;
		methodname = Request.QueryString('_method') + '';
		if (methodname == '' || methodname == 'undefined')
			return false;
	}

	var dispatchVTbl = null;
	if (typeof(vtable) == 'object')
		dispatchVTbl = vtable;
	else if (typeof(public_description) == 'object')
		dispatchVTbl = public_description;
	if (dispatchVTbl == null)
		return false;

	var strResponse = '<METHOD VERSION="1.0.8044">'
	if (methodname == 'GetServerProxy') 
	{ 
		strResponse += Return(GetServerProxy(dispatchVTbl));
	}
	else
	{
		if (typeof(dispatchVTbl[methodname]) == 'function')
		{	// validate function is in dispatchVTbl
			var params = '';
			var pcount = Request.QueryString('pcount');
			for (var i=0; i < pcount; i++)
			{	// extract parameter values
				params += 'Request.QueryString("p' + i + '").item';
				if (i < pcount-1)
					params += ',';
			}

			var dispatch = 'dispatchVTbl.' + methodname + '(' + params + ')';
			//Response.write(dispatch);	// USED FOR DEBUG
			// validated against dispatchVTbl, safe to eval on the server
			var r = eval(dispatch); 
			strResponse += Return(r);
		}
		else
		{
			var strError = '<RETURN_VALUE TYPE=ERROR>';
			strError += escape(methodname + ' : not a public function');
			strError += '</RETURN_VALUE>';
			strResponse += strError;
		}
	}
	strResponse += '</METHOD>';
	
	Response.Write(strResponse);
	return true;
}

//*****************************************************************
// function Return(x)
//	This function is used to translate the data in given variable x,
//	into a string for transport to the client.  If the variable
//	is an object, then the type is set to EVAL_OBJECT to indicate 
//	to the client that this is an object being marshalled and it 
//	should be "evaluated".  
//*****************************************************************
function Return(x)
{
	var returnType = typeof(x);
	if (returnType == 'object' || returnType == 'number' || returnType == 'boolean')
		return '<RETURN_VALUE TYPE=EVAL_OBJECT>' + escape(uneval(x)) + '</RETURN_VALUE>';
	else
		return '<RETURN_VALUE TYPE=SIMPLE>' + escape(x) + '</RETURN_VALUE>';
}

//*****************************************************************
// function GetServerProxy(pd)
//	This function returns a server proxy given a public_description.
//
//*****************************************************************
function GetServerProxy(pd)
{
	if (!unevalInitialized)
	{
		initUneval();
		unevalInitialized = true;
	}

	var o = new Object;
	for (var name in pd)
	{
		if (typeof(pd[name]) == 'function' && pd.__isPublicMember(name))
		{
			o[name] = Function('return 0;');
		}
	}
	return o;
}


//*****************************************************************
// function uneval(obj) 
//	This function takes a given jscript object and creates a 
//	string representing the object instance in its current state, 
//	such that when the string is "evaluated" with the "eval" 
//	function, the object will be recreated. This function is used 
//	to to "marshall" jscript objects across the client/server
//	boundary.
//
//*****************************************************************
var unevalInitialized;
var unevalNextIdentifier;
function unevalGetNextID()
{
	return '_o' + unevalNextID++;
}

function uneval(obj)
{
	if (!unevalInitialized)
	{
		initUneval();
		unevalInitialized = true;
	}
	unevalNextID = 0;
	var s = 'var undefined;' + unevalDecl(obj) + unevalInst(obj);
	unevalClear(obj);
	return s;
}

function unevalDecl(obj)
{
	var s = '';
	switch (typeof(obj))
	{
		case 'undefined':
		case 'boolean':
		case 'number':
		case 'string':
			break;

		default:
			if (null != obj && !obj.__mark && 'string' != typeof(obj.__decl))
				{
				obj.__mark = true;
				obj.__decl = unevalGetNextID();
				s = obj.__unevalDecl();
				delete obj.__mark;
				}
			break;
	}
	return s;
}

function unevalInst(obj)
{
	var s = '';
	switch (typeof(obj))
	{
		case 'undefined':
			s = 'undefined';
			break;
		case 'boolean':
		case 'number':
			s = String(obj);
			break;
		case 'string':
			s = unevalString(obj);
			break;

		default:
			if (null == obj)
				s = 'null';
			else if (obj.__mark)
				s = '"ERROR: Cycle in uneval graph."';
			else
				{
				obj.__mark = true;
				s = obj.__unevalInst();
				delete obj.__mark;
				}
			break;
	}
	return s;
}

function unevalClear(obj)
{
	switch (typeof(obj))
	{
		case 'undefined':
		case 'boolean':
		case 'number':
		case 'string':
			break;

		default:
			if (null != obj && !obj.__mark && 'string' == typeof(obj.__decl))
			{
				obj.__mark = true;
				obj.__unevalClear();
				delete obj.__mark;
			}
			break;
	}
	return ;
}

function unevalDoNothing() 
{	}

function unevalConvertToString(obj)
{	return String(obj);		}

function unevalString(str) 
{	return '"' + str.replace(/([^\\])'/g,'$1\\"') + '"'; 	}


//*****************************************************************
// function initUneval()
//
//	This function sets up the prototype __uneval functions that
//  are used to support uneval function for all data types.
//
//*****************************************************************
function initUneval()
{
	// instrinsic objects
	Object.__unevalDecl = unevalDoNothing;
	Object.__unevalInst = function () { return 'Object'; }
	Object.__unevalClear = unevalDoNothing;

	Boolean.__unevalDecl = unevalDoNothing;
	Boolean.__unevalInst = function () { return 'Boolean'; }
	Boolean.__unevalClear = unevalDoNothing;

	Number.__unevalDecl = unevalDoNothing;
	Number.__unevalInst = function () { return 'Number'; }
	Number.__unevalClear = unevalDoNothing;

	String.__unevalDecl = unevalDoNothing;
	String.__unevalInst = function () { return 'String'; }
	String.__unevalClear = unevalDoNothing;

	Date.__unevalDecl = unevalDoNothing;
	Date.__unevalInst = function () { return 'Date'; }
	Date.__unevalClear = unevalDoNothing;

	Function.__unevalDecl = unevalDoNothing;
	Function.__unevalInst = function () { return 'Function'; }
	Function.__unevalClear = unevalDoNothing;

	Array.__unevalDecl = unevalDoNothing;
	Array.__unevalInst = function () { return 'Array'; }
	Array.__unevalClear = unevalDoNothing;

	// object members
	Object.prototype.__enumMembers = function(retval,func)
	{
		var isPublicMember = this.__isPublicMember;
		if ('object' == typeof(this.__unevalProperties))
		{
			var unevalProperties = this.__unevalProperties;
			var length = unevalProperties.length;
			for (var i = 0; i < length; i++)
			{
				if (isPublicMember(unevalProperties[i]))
					func(retval, this, unevalProperties[i]);
			}
		}
		else
		{
			for (var i in this)
			{
				if (isPublicMember(i))
					func(retval, this, i);
			}
		}
	}

	Object.prototype.__unevalDeclMember = function (retval, obj, member)
	{
		retval.otherDecl += unevalDecl(obj[member]);
		retval.myDecl += obj.__decl + '[' + unevalString(member) + ']=' + unevalInst(obj[member]) + ';';
	}

	Object.prototype.__unevalDecl = function()
	{
		var retval = { otherDecl:'', myDecl:'' };
		this.__enumMembers(retval, this.__unevalDeclMember);
		return retval.otherDecl + 'var ' + this.__decl + '=' + this.__unevalConstructor() + ';' + retval.myDecl;
	}

	Object.prototype.__unevalInst = function ()
	{	return this.__decl;		}

	Object.prototype.__unevalClearMember = function(retval, obj, member)
	{	unevalClear(obj[member]);	}

	Object.prototype.__unevalClear = function()
	{
		delete this.__decl;
		this.__enumMembers(null, this.__unevalClearMember);
	}

	Object.prototype.__isPublicMember = function(member)
	{	return '__' != member.substr(0,2);		}

	Object.prototype.__unevalConstructor = function ()
	{	return 'new Object';		}

	// overrides for simple types
	Boolean.prototype.__unevalConstructor = function()
	{	return 'new Boolean(' + String(this) + ')';	}
	Number.prototype.__unevalConstructor = function()
	{	return 'new Number(' + String(this) + ')';	}
	String.prototype.__unevalConstructor = function()
	{	return 'new String(' + unevalString(this) + ')';	}
	Date.prototype.__unevalConstructor = function()
	{	return 'new Date(Date.parse("' + String(this) + '"))';	}

	// overrides for function
	Function.prototype.__unevalDecl = function()
	{	return String(this).replace(/function ([^\(]*)/,'function ' + this.__decl);	}

	// overrides for array
	Array.prototype.__unevalDecl = function()
	{
		var decl = this.__decl;
		var r = '';    
		var s = 'var ' +  decl + '= new Array(' + this.length + ');';
		var length = this.length;
		for (var i = 0; i < length; i++)
		{
			r += unevalDecl(this[i]);
			s += decl + '[' + i + ']=' + unevalInst(this[i]) + ';';
		}
		return r + s;
	}

}	// end of initUneval

</SCRIPT>
