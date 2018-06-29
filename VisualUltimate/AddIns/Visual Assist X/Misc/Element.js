
namespace htmlElement
{
Public property  	 src;//  	Gets an HtmlElementCollection of all elements underneath the current element.
Public property  	 all;//  	Gets an HtmlElementCollection of all elements underneath the current element.
Public property 	canHaveChildren;// 	Gets a value indicating whether this element can have child elements.
Public property 	children;// 	Gets an HtmlElementCollection of all children of the current element.
Public property 	clientRectangle;// 	Gets the bounds of the client area of the element in the HTML document.
Public property 	document;// 	Gets the HtmlDocument to which this element belongs.
Public property 	domElement;// 	Gets an unmanaged interface pointer for this element.
Public property 	enabled;// 	Gets or sets whether the user can input data into this element.
Public property 	firstChild;// 	Gets the next element below this element in the document tree.
Public property 	id;// 	Gets or sets a label by which to identify the element.
Public property 	innerHtml;// 	Gets or sets the HTML markup underneath this element.
Public property 	innerText;// 	Gets or sets the text assigned to the element.
Public property 	name;// 	Gets or sets the name of the element.
Public property 	nextSibling;// 	Gets the next element at the same level as this element in the document tree.
Public property 	offsetParent;// 	Gets the element from which OffsetRectangle is calculated.
Public property 	offsetRectangle;// 	Gets the location of an element relative to its parent.
Public property 	outerHtml;// 	Gets or sets the current element's HTML code.
Public property 	outerText;// 	Gets or sets the current element's text.
Public property 	parent;// 	Gets the current element's parent element.
Public property 	scrollLeft;// 	Gets or sets the distance between the edge of the element and the left edge of its content.
Public property 	scrollRectangle;// 	Gets the dimensions of an element's scrollable region.
Public property 	scrollTop;// 	Gets or sets the distance between the edge of the element and the top edge of its content.
Public property 	style;// 	Gets or sets a comma-delimited list of styles for the current element.
Public property 	tabIndex;// 	Gets or sets the location of this element in the tab order.
Public property 	tagName;// 	Gets the name of the HTML tag.

Public method  	 appendChild();//  	 Adds an element to another element's subtree.
Public method 	attachEventHandler();// 	Adds an event handler for a named event on the HTML Document Object Model (DOM).
Public method 	detachEventHandler();// 	Removes an event handler from a named event on the HTML Document Object Model (DOM).
Public method 	equals();// 	Overloaded. Overridden.  
Public method 	focus();// 	Puts user input focus on the current element.
Public method 	getAttribute();// 	Retrieves the value of the named attribute on the element.
Public method 	getElementsByTagName();// 	Retrieves a collection of elements represented in HTML by the specified HTML tag.
Public method 	getHashCode();// 	Overridden.  
Public method 	getType();//  	Gets the Type of the current instance. (Inherited from Object.)
Public method 	insertAdjacentElement();// 	Insert a new element into the Document Object Model (DOM).
Public method 	invokeMember();// 	Overloaded. Executes a method unique to the current element.
Public method Static 	op_Equality();// 	Compares two elements for equality.
Public method Static 	op_Inequality();// 	Compares two HtmlElement objects for inequality.
Public method 	raiseEvent();// 	Causes the named event to call all registered event handlers.
Public method Static 	referenceEquals();//  	Determines whether the specified Object instances are the same instance. (Inherited from Object.)
Public method 	removeFocus();// 	Removes focus from the current element, if that element has focus.
Public method 	scrollIntoView();// 	Scrolls through the document containing this element until the top or bottom edge of this element is aligned with the document's window.
Public method 	setAttribute();// 	Sets the value of the named attribute on the element.
Public method 	toString();//  	Returns a String that represents the current Object. (Inherited from Object.)

Protected method  	 finalize();//   	 Allows an Object to attempt to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection. (Inherited from Object.)
Protected method 	memberwiseClone();//  	Creates a shallow copy of the current Object. (Inherited from Object.)

Public event  	 click;//  	Occurs when the user clicks on the element with the left mouse button.
Public event 	doubleClick;// 	Occurs when the user clicks the left mouse button over an element twice, in rapid succession.
Public event 	drag;// 	Occurs when the user drags text to various locations.
Public event 	dragEnd;// 	Occurs when a user finishes a drag operation.
Public event 	dragLeave;// 	Occurs when the user is no longer dragging an item over this element.
Public event 	dragOver;// 	Occurs when the user drags text over the element.
Public event 	focusing;// 	Occurs when the element first receives user input focus.
Public event 	gotFocus;// 	Occurs when the element has received user input focus.
Public event 	keyDown;// 	Occurs when the user presses a key on the keyboard.
Public event 	keyPress;// 	Occurs when the user presses and releases a key on the keyboard.
Public event 	keyUp;// 	Occurs when the user releases a key on the keyboard.
Public event 	losingFocus;// 	Occurs when the element is losing user input focus.
Public event 	lostFocus;// 	Occurs when the element has lost user input focus.
Public event 	mouseDown;// 	Occurs when the user presses a mouse button.
Public event 	mouseEnter;// 	Occurs when the user first moves the mouse cursor over the current element.
Public event 	mouseLeave;// 	Occurs when the user moves the mouse cursor off of the current element.
Public event 	mouseMove;// 	Occurs when the user moves the mouse cursor across the element.
Public event 	mouseOver;// 	Occurs when the mouse cursor enters the bounds of the element.
Public event 	mouseUp;// 	Occurs when the user releases a mouse button.

}




