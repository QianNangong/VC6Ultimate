////////////////////////////////////////////////////////////////////////////
// Implementatation of TextJump class
//
// Class to store text and URL data in a single class
//
// This is a part of the Internet Information Server SDK Samples
// Copyright (C) 1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the Software 
// Development Kit Reference and related electronic documentation provided.
//
////////////////////////////////////////////////////////////////////////////

class TextJump 
{
	private String	m_strURL;
	private String	m_strText;

	TextJump(String strURL,String strText)
	{
		m_strURL = strURL;
		m_strText = strText;
	}

	public String getString()	{ return m_strText; }
	public String getURL()		{ return m_strURL; }
}

