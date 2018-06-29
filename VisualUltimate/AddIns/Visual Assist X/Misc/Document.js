// http://msdn.microsoft.com/en-us/library/system.windows.forms.htmldocument_members(VS.80).aspx
namespace document 
{
Public property  	activeElement;//  	Provides the HtmlElement which currently has user input focus.
Public property 	activeLinkColor;// 	Gets or sets the Color of a hyperlink when clicked by a user.
Public property 	all;// 	Gets an instance of HtmlElementCollection, which stores all HtmlElement objects for the document.
Public property 	backColor;// 	Gets or sets the background color of the HTML document.
Public property 	body;// 	Gets the HtmlElement for the BODY tag.
Public property 	cookie;// 	Gets or sets the HTTP cookies associated with this document.
Public property 	defaultEncoding;// 	Gets the encoding used by default for the current document.
Public property 	domain;// 	Gets or sets the string describing the domain of this document for security purposes.
Public property 	domDocument;// 	Gets the unmanaged interface pointer for this HtmlDocument.
Public property 	encoding;// 	Gets or sets the character encoding for this document.
Public property 	focused;// 	Gets a value indicating whether the document has user input focus.
Public property 	foreColor;// 	Gets or sets the text color for the document.
Public property 	forms;// 	Gets a collection of all of the <FORM> elements in the document.
Public property 	images;// 	Gets a collection of all image tags in the document.
Public property 	linkColor;// 	Gets or sets the color of hyperlinks.
Public property 	links;// 	Gets a list of all the hyperlinks within this HTML document.
Public property 	rightToLeft;// 	Gets or sets the direction of text in the current document.
Public property 	title;// 	Gets or sets the text value of the <TITLE> tag in the current HTML document.
Public property 	url;// 	Gets the URL describing the location of this document.
Public property 	visitedLinkColor;// 	Gets or sets the Color of links to HTML pages that the user has already visited.
Public property 	window;// 	Gets the HtmlWindow associated with this document.

Public method  	 attachEventHandler();//  	 Adds an event handler for the named HTML DOM event.
Public method  	 attachEvent();//  	 Adds an event handler for the named HTML DOM event.
Public method 	createElement();// 	Creates a new HtmlElement of the specified HTML tag type.
Public method 	detachEventHandler();// 	Removes an event handler from a named event on the HTML DOM.
Public method 	detachEvent();// 	Removes an event handler from a named event on the HTML DOM.
Public method 	equals();// 	Overloaded. Overridden.  
Public method 	execCommand();// 	Executes the specified command against the document.
Public method 	focus();// 	Sets user input focus on the current document.
Public method 	htmlElement[] GetElementById();// 	Retrieves a single HtmlElement using the element's ID attribute as a search key.
Public method 	getElementFromPoint();// 	Retrieves the HTML element located at the specified client coordinates.
Public method 	getElementsByTagName();// 	Retrieve a collection of elements with the specified HTML tag.
Public method 	getHashCode();// 	Overridden.  
Public method 	getType();//  	Gets the Type of the current instance. (Inherited from Object.)
Public method 	invokeScript();// 	Overloaded. Executes an Active Scripting function defined in an HTML page.
Public method Static 	op_Equality();// 	 
Public method Static 	op_Inequality();// 	 
Public method 	openNew();// 	Gets a new HtmlDocument to use with the Write method.
Public method Static 	referenceEquals();//  	Determines whether the specified Object instances are the same instance. (Inherited from Object.)
Public method 	toString();//  	Returns a String that represents the current Object. (Inherited from Object.)
Public method 	write();// 	Writes a new HTML page.

Protected method  	 finalize();//   	 Allows an Object to attempt to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection. (Inherited from Object.)
Protected method 	memberwiseClone();//  	Creates a shallow copy of the current Object. (Inherited from Object.)

Public event  	onClick;//  	Occurs when the user clicks anywhere on the document.
Public event 	onContextMenuShowing;// 	Occurs when the user requests to display the document's context menu.
Public event 	onFocusing;// 	Occurs before focus is given to the document.
Public event 	onLosingFocus;// 	Occurs while focus is leaving a control.
Public event 	onMouseDown;// 	Occurs when the user clicks the left mouse button.
Public event 	onMouseLeave;// 	Occurs when the mouse is no longer hovering over the document.
Public event 	onMouseMove;// 	Occurs when the mouse is moved over the document.
Public event 	onMouseOver;// 	Occurs when the mouse is moved over the document.
Public event 	onMouseUp;// 	Occurs when the user releases the left mouse button.
Public event 	onStop;// 	Occurs when navigation to another Web page is halted.

    object location = "";	
    function open() { [native code] }
    function addEventListener() { [native code] }	
    function write() { [native code] }
    string cookie = "";	
    function getElementById() { [native code] }
    object body = [object HTMLBodyElement];	
    function createElement() { [native code] }
    function createTextNode() { [native code] }	
    string title = "";
    string referrer = "";	
    object styleSheets = [object StyleSheetList];
    string baseURI = "";	
    function compareDocumentPosition() { [native code] }
    object textContent = null;	
    function isSameNode() { [native code] }
    function lookupPrefix() { [native code] }	
    function isDefaultNamespace() { [native code] }
    function lookupNamespaceURI() { [native code] }	
    function isEqualNode() { [native code] }
    function getFeature() { [native code] }	
    function setUserData() { [native code] }
    function getUserData() { [native code] }	
    string nodeName = "#document";
    object nodeValue = null;	
    number nodeType = "9";
    object parentNode = null;	
    object childNodes = [object NodeList];
    object firstChild = [object DocumentType];	
    object lastChild = [object HTMLHtmlElement];
    object previousSibling = null;	
    object nextSibling = null;
    object attributes = null;	
    object ownerDocument = null;
    function insertBefore() { [native code] }	
    function replaceChild() { [native code] }
    function removeChild() { [native code] }	
    function appendChild() { [native code] }
    function hasChildNodes() { [native code] }	
    function cloneNode() { [native code] }
    function normalize() { [native code] }	
    function isSupported() { [native code] }
    object namespaceURI = null;	
    object prefix = null;
    object localName = null;	
    function hasAttributes() { [native code] }
    object doctype = [object DocumentType];	
    object implementation = [object DOMImplementation];
    object documentElement = [object HTMLHtmlElement];	
    function createDocumentFragment() { [native code] }
    function createComment() { [native code] }	
    function createCDATASection() { [native code] }
    function createProcessingInstruction() { [native code] }	
    function createAttribute() { [native code] }
    function createEntityReference() { [native code] }	
    function getElementsByTagName() { [native code] }
    function importNode() { [native code] }	
    function createElementNS() { [native code] }
    function createAttributeNS() { [native code] }	
    function getElementsByTagNameNS() { [native code] }
    number ELEMENT_NODE = "1";	
    number ATTRIBUTE_NODE = "2";
    number TEXT_NODE = "3";	
    number CDATA_SECTION_NODE = "4";
    number ENTITY_REFERENCE_NODE = "5";	
    number ENTITY_NODE = "6";
    number PROCESSING_INSTRUCTION_NODE = "7";	
    number COMMENT_NODE = "8";
    number DOCUMENT_NODE = "9";	
    number DOCUMENT_TYPE_NODE = "10";
    number DOCUMENT_FRAGMENT_NODE = "11";	
    number NOTATION_NODE = "12";
    string URL = "";	
    object images = [object HTMLCollection];
    object applets = [object HTMLCollection];	
    object links = [object HTMLCollection];
    object forms = [object HTMLCollection];	
    object anchors = [object HTMLCollection];
    function close() { [native code] }	
    function getElementsByName() { [native code] }
    number width = "1903";	
    number height = "1953";
    string alinkColor = "#ee0000";	
    string linkColor = "#0000ee";
    string vlinkColor = "#551a8b";	
    string bgColor = "#ffffff";
    string fgColor = "#000000";	
    string domain = "";
    object embeds = [object HTMLCollection];	
    function getSelection() { [native code] }
    function writeln() { [native code] }	
    function clear() { [native code] }
    function captureEvents() { [native code] }	
    function releaseEvents() { [native code] }
    function routeEvent() { [native code] }	
    string compatMode = "BackCompat";
    object plugins = [object HTMLCollection];	
    string designMode = "off";
    function execCommand() { [native code] }	
    function execCommandShowHelp() { [native code] }
    function queryCommandEnabled() { [native code] }	
    function queryCommandIndeterm() { [native code] }
    function queryCommandState() { [native code] }	
    function queryCommandSupported() { [native code] }
    function queryCommandText() { [native code] }	
    function queryCommandValue() { [native code] }
    string characterSet = "UTF-8";	
    string dir = "ltr";
    string contentType = "text/html";	
    string lastModified = "05/13/2008 14:09:08";
    function getBoxObjectFor() { [native code] }	
    function setBoxObjectFor() { [native code] }
    function createEvent() { [native code] }	
    string preferredStylesheetSet = "";
    object defaultView = [object Window];	
    function createRange() { [native code] }
    function createNodeIterator() { [native code] }	
    function createTreeWalker() { [native code] }
    function getAnonymousNodes() { [native code] }	
    function getAnonymousElementByAttribute() { [native code] }
    function addBinding() { [native code] }	
    function removeBinding() { [native code] }
    function getBindingParent() { [native code] }	
    function loadBindingDocument() { [native code] }
    function removeEventListener() { [native code] }	
    function dispatchEvent() { [native code] }
    string inputEncoding = "UTF-8";	
    object xmlEncoding = null;
    boolean xmlStandalone = "false";	
    object xmlVersion = null;
    boolean strictErrorChecking = "true";	
    string documentURI = "";
    function adoptNode() { [native code] }	
    domConfig: unavailable	
    domConfig: unavailable
    function normalizeDocument() { [native code] }	
    function renameNode() { [native code] }
    function createExpression() { [native code] }	
    function createNSResolver() { [native code] }
    function evaluate() { [native code] }

};
