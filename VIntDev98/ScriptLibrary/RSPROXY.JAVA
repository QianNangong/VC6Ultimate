// ************************************************************************
// Microsoft Script Library
// Visual InterDev 6.0 Remote Scripting proxy
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************
import java.applet.*;
import java.awt.*;

import java.net.URL;
import java.io.InputStream;
import java.io.IOException;



//==============================================================================
// Main Class for applet RSProxy
//
//==============================================================================
public class RSProxy extends Applet implements Runnable
{

	// APPLET INFO SUPPORT:
	// The getAppletInfo() method returns a string describing the applet's
	// author, copyright date, or miscellaneous information.
    //--------------------------------------------------------------------------
    public String getAppletInfo()
    {
		return	"Name: RSProxy\r\n" +
				"Author: Microsoft\r\n" +
				"Copyright (c) 1998 Microsoft Corp.";
    }

    public void init()				{ resize(0,0); }
    public void destroy()			{}
    public void paint(Graphics g)	{}
    public void start()				{}
    public void stop()				{}

    
	// Event Status Codes
	//--------------------------------------------------------------------------
	static final int E_FAIL = -1;
    static final int S_COMPLETED = 0;
    static final int S_PENDING = 1;
    static final int S_PARTIAL = 2;
    

    // Request Object Definition
	//--------------------------------------------------------------------------
    static final int REQUEST_NEXT = 0;
    static final int REQUEST_PREV = 1;
    static final int REQUEST_ID = 2;
    static final int REQUEST_URL = 3;
    static final int REQUEST_URLCONTEXT = 4;
	static final int REQUEST_MODE = 5;
	static final int REQUEST_THREAD = 6;
    static final int C_REQUEST = 7;
    
    // Request Object Modes
	//--------------------------------------------------------------------------
    static final int REQUEST_MODE_COMPLETE = 0;
    static final int REQUEST_MODE_PARTIAL = 1;

    // Response Object Definition
	//--------------------------------------------------------------------------
    static final int RESPONSE_NEXT = 0;
    static final int RESPONSE_REQUESTID = 1;
    static final int RESPONSE_STATUS = 2;
    static final int RESPONSE_MESSAGE = 3;
    static final int RESPONSE_DATA = 4;
    static final int C_RESPONSE = 5;

    // Other Constants
	//--------------------------------------------------------------------------
    static final int BUFFER_SIZE = 512;
    static final int EXPECTED_MAX_CONCURRENT_CALLS = 5;


    
    // Private Members
	//--------------------------------------------------------------------------
	// Request list. Requests are inserted at head and removed when completed.
    private Object m_requestLock;
    private Object[] m_requestHead;
    // Response queue. 
    private Object m_responseLock;
    private Object[] m_responseHead;
    private Object[] m_responseTail;

        
    // Public Methods
	//--------------------------------------------------------------------------

 	// RSProxy class constructor
	public RSProxy()
    {
        m_requestLock = new Object();
        m_responseLock = new Object();
    }

 	// initiates a request given an id and url
    public void startRequest(String requestID,String url_context,String url,int mode)
    {
        Object[] request = newRequest(requestID,url_context,url,mode);
        insertRequest(request);
        Thread thread = new Thread(this,requestID);
		request[REQUEST_THREAD] = thread;
        thread.start();
    }

	// used to poll applet for response
    public synchronized boolean waitForResponse()
    {
        // a response is already queued up
        if (null != m_responseHead)
            return true;
        
        // a response cannot be generated without a request
        if (null == m_requestHead)
            return false;
            
        try
        {	// try waiting for a response
            while (null == m_responseHead)
                wait();
            return true;
        }
        catch (InterruptedException ie)
        {
            return false;
        }
    }

	public synchronized void cancelRequest(String requestId)
	{
		Object [] request = findRequest(requestId);
		
		if (request == null)
			return;
		if (request[REQUEST_THREAD] != null)
			((Thread)request[REQUEST_THREAD]).stop();
	}
	// The following methods get the response information
	// Only the UI thread should call these methods.
    public boolean hasResponse()
    {	return null != m_responseHead;		}

    public void endResponse()
    {	removeResponse();				    }

    public String getRequestID()
    {
        if (null == m_responseHead)
			return "";
        return (String)m_responseHead[RESPONSE_REQUESTID];
    }

    public int getStatus()
    {                                                              
        if (null == m_responseHead)
            return 0;
        return getInteger(m_responseHead[RESPONSE_STATUS]);
    }

    public String getMessage()
    {
        if (null == m_responseHead)
            return "";
        return (String)m_responseHead[RESPONSE_MESSAGE];
    }

    public String getData()
    {
        if (null == m_responseHead)
            return "";
        return (String)m_responseHead[RESPONSE_DATA];
    }


    // Runnable Protocol    
	//--------------------------------------------------------------------------
    public void run()
    {
        String requestID = Thread.currentThread().getName();
        runUrlDownloadThread(requestID);
    }

    // Protected Methods
	//--------------------------------------------------------------------------

	// spawns worker thread to asynchronously retrieve data for given request
    protected void runUrlDownloadThread(String requestID)
    {   
        Object[] request = findRequest(requestID);
        Object[] response;
                
        if (null == request)
            response = newErrorResponse(requestID,"Unknown request id.");
        else
        {
            try
            {            
                int bytesRead;
                byte[] buffer = new byte[BUFFER_SIZE];
				URL contextURL = new URL((String)request[REQUEST_URLCONTEXT]);
                InputStream stream = new URL(contextURL,(String)request[REQUEST_URL]).openStream();
				StringBuffer responseBuffer = new StringBuffer();
                
                while (-1 != (bytesRead = stream.read(buffer)))
                {
					if (request[REQUEST_MODE] == getObject(REQUEST_MODE_PARTIAL))
					{	// generate response as partial data buffers are received
						response = newDataResponse(requestID,new String(buffer,0,0,bytesRead),false);
						handleResponse(request,response);
					}
					else //if (request[REQUEST_MODE] == getObject(REQUEST_MODE_COMPLETE))	
					{	// generate a single response when request is complete
						responseBuffer.append(new String(buffer,0,0,bytesRead));
					}
                }
				stream.close();

				if (request[REQUEST_MODE] == getObject(REQUEST_MODE_PARTIAL))
					response = newDataResponse(requestID, "", true);
				else //if (request[REQUEST_MODE] == getObject(REQUEST_MODE_COMPLETE))	
					response = newDataResponse(requestID,responseBuffer.toString(),true);
            }
            catch (Exception e)
            {
                response = newErrorResponse(requestID,e.getMessage());
            }
		}
        
		handleResponse(request,response);
    }

	// handle response, inserts response into queue, removes request when complete
    protected synchronized void handleResponse(Object[] request,Object[] response)
    {
        insertResponse(response);
		int status = getInteger(response[RESPONSE_STATUS]);
        if (S_COMPLETED == status || E_FAIL == status)
            removeRequest(request);
        notifyAll();
    }
 
    // Object constructors
	//--------------------------------------------------------------------------

    // constructor for new Request object    
    protected Object[] newRequest(String requestID,String url_context,String url,int mode)
    {
        Object[] request = new Object[C_REQUEST];
        request[REQUEST_ID] = requestID;
        request[REQUEST_URL] = url;
        request[REQUEST_URLCONTEXT] = url_context;
        request[REQUEST_MODE] = getObject(mode);
        return request;
    }
    
    // constructor for new Response object    
    protected Object[] newDataResponse(String requestID,String data,boolean finished)
    {
        Object[] response = new Object[C_RESPONSE];
        response[RESPONSE_REQUESTID] = requestID;
        response[RESPONSE_STATUS] = new Integer(finished ? S_COMPLETED : S_PARTIAL);
        response[RESPONSE_MESSAGE] = "Completed";
        response[RESPONSE_DATA] = data;
        return response;
    }

	// constructor for new ErrorResponse object
    protected Object[] newErrorResponse(String requestID,String message)
    {
        Object[] response = new Object[C_RESPONSE];
        response[RESPONSE_REQUESTID] = requestID;
        response[RESPONSE_STATUS] = new Integer(E_FAIL);
        response[RESPONSE_MESSAGE] = message;
        response[RESPONSE_DATA] = "";
		return response;
    }
    
    // list management routines
	//--------------------------------------------------------------------------

	// find a given request object in the request list
    protected Object[] findRequest(String requestID)
    {
        synchronized (m_requestLock)
		{
            Object[] i;
            for (i = m_requestHead; null != i; i = (Object[])i[REQUEST_NEXT])
			{
                if (0 == requestID.compareTo((String)i[REQUEST_ID]))
                    return i;
			}
		}
        return null;
    }
    
	// insert a request object at the head of the request list
    protected void insertRequest(Object[] request)
    {
        synchronized (m_requestLock)
		{
            request[REQUEST_PREV] = null;
            request[REQUEST_NEXT] = m_requestHead;
            if (null != m_requestHead)
                m_requestHead[REQUEST_PREV] = request;
            m_requestHead = request;
		}
	}
    
	// remove a request object from the request list
    protected void removeRequest(Object[] request)
    {
        synchronized (m_requestLock)
		{
            if (null == request[REQUEST_PREV])
                m_requestHead = (Object[])request[REQUEST_NEXT];
            else
                ((Object[])request[REQUEST_PREV])[REQUEST_NEXT] = request[REQUEST_NEXT];
                
            if (null != request[REQUEST_NEXT])
                ((Object[])request[REQUEST_NEXT])[REQUEST_PREV] = request[REQUEST_PREV];
		}
    }

	// insert a response object onto the tail of the queue
    protected void insertResponse(Object[] response)
    {
        synchronized (m_responseLock)
		{
            if (null == m_responseHead)
			{
                m_responseHead = response;
                m_responseTail = response;
			}
            else
			{
                m_responseTail[RESPONSE_NEXT] = response;
                m_responseTail = response;
			}
		}
    }
	
    // remove response at the head of the queue
    protected void removeResponse()
    {
        synchronized (m_responseLock)
		{
            if (null != m_responseHead)
			{
                m_responseHead = (Object[])m_responseHead[RESPONSE_NEXT];
                if (null == m_responseHead)
                    m_responseTail = null;
			}
		}
    }

    // private helper functions
	//--------------------------------------------------------------------------
	
	// casts int as Object
	private Object getObject(int integer)
	{	return new Integer(integer);	}

	// casts Object as int
	private int getInteger(Object obj)
	{
		if (null == obj)
			return 0;
		return ((Integer)obj).intValue();
	}

}
