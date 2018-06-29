////////////////////////////////////////////////////////////////////////////
// Implementatation of DoubleBuffer class
//
// Simply encapsulates the data required to store off-screen images.
// 
// This is a part of the Internet Information Server SDK Samples
// Copyright (C) 1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the Software 
// Development Kit Reference and related electronic documentation provided.
//
////////////////////////////////////////////////////////////////////////////

import java.applet.*;
import java.awt.*;

class DoubleBuffer 
{
	private Image		m_imgBuff;				
    private Graphics	m_gBuff;			
	private Rectangle	m_rect;
	private Applet		m_app;

	//////////////////////////////////////////////////////////////////////////////////
	DoubleBuffer(Applet app, Rectangle rect)
	{
		m_rect = rect;
		m_app = app;

		// build a buffer for the off-screen image
		m_imgBuff = m_app.createImage(rect.width,rect.height);
		m_gBuff = m_imgBuff.getGraphics();
	}

	//////////////////////////////////////////////////////////////////////////////////
	public Rectangle getSize()		{ return m_rect; }
	public Image getImage()			{ return m_imgBuff; }
	public Graphics getGraphics()	{ return m_gBuff; }
}

