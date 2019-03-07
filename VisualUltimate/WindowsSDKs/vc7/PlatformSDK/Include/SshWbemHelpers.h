////////////////////////////////////////////////////////////////////////////////////////////
//  SshWbemHelpers.h                                                                      //
//                                                                                        //
//  The follwing classes are defined:                                                     //
//                                                                                        //
//       CWbemServices - threadsafe wrapper class for IWbemServices                       //
//       CWbemClassObject - wrapper class for IWbemClassObject                            //
//       CWbemException - wrapper for IErrorInfo, assists with WBEM error reporting       //
//                                                                                        //
//  These classes provide a convenient way to use the three most common WBEM interfaces:  //
//  IWbemLocator, IWbemServices, and IWbemClassObject. The also make it easy to           //
//  the WBEM __ExtendedStatus error object and interpret the HRESULT. The use of these    //
//  classes along with the smart pointers and the COM helper classes _bstr_t and          //
//  _variant_t can greatly reduce the chances of memory leaks in your code.               //
//                                                                                        //
//  See T_SafeVector.h for help with safe arrays and arrays of embedded objects.          //
//                                                                                        //
//  Copyright (c)1997 - 1999 Microsoft Corporation, All Rights Reserved                       //
////////////////////////////////////////////////////////////////////////////////////////////


#if !defined(__SdkWbemHelpers_H)
#define      __SdkWbemHelpers_H
#pragma once

#pragma warning( disable : 4290) // C++ Exception Specification Ignored

#include <wbemidl.h>


class CWbemClassObject;
class CWbemServices;

//---------------------------------------------------------------------------------------
//
// Smart pointers for the WBEM interfaces. The _COM_SMARTPTR_TYPEDEF creates smart pointer types
// named XXXPtr where XXX is the interface pointer, these types are actually classes derived from
// the COM helper class _com_ptr_t. Use these types just as you would the interface pointer, only
// AddRef and Release are called for you at the appropriate times.
// 

_COM_SMARTPTR_TYPEDEF(IUnsecuredApartment,          IID_IUnsecuredApartment);  // IUnsecuredApartmentPtr
_COM_SMARTPTR_TYPEDEF(IWbemObjectSink,              IID_IWbemObjectSink);      // IWbemObjectSinkPtr
_COM_SMARTPTR_TYPEDEF(IWbemClassObject,             IID_IWbemClassObject);     // IWbemClassObjectPtr
_COM_SMARTPTR_TYPEDEF(IWbemServices,                IID_IWbemServices);        // IWbemServicesPtr
_COM_SMARTPTR_TYPEDEF(IWbemContext,                 IID_IWbemContext );        // IWbemContextPtr
_COM_SMARTPTR_TYPEDEF(IWbemCallResult,              IID_IWbemCallResult);      // IWbemCallResultPtr
_COM_SMARTPTR_TYPEDEF(IWbemQualifierSet,            IID_IWbemQualifierSet);    // IWbemQualifierSetPtr
_COM_SMARTPTR_TYPEDEF(IWbemLocator,                 IID_IWbemLocator);         // IWbemLocatorPtr
_COM_SMARTPTR_TYPEDEF(IWbemObjectAccess,            IID_IWbemObjectAccess);    // IWbemObjectAccessPtr
_COM_SMARTPTR_TYPEDEF(IEnumWbemClassObject,         IID_IEnumWbemClassObject); // IEnumWbemClassObjectPtr


//---------------------------------------------------------------------------------------
// CWbemException
//
//  This class assists with WBEM error handling and the WBEM __ExtendedStatus error object
// 
//  Create an instance of this class immediately after a WBEM call failure, and pass
//  in to the constructor the HRESULT from the failed COM call and a descriptive message.
//
//  Call the GetWbemError() method to retrieve the __ExtendedStatus object.
// 
//  This object inherits from _com_error, see _com_error documentation for information on
//  how to access the encapsulated IErrorInfo interface.
//

class CWbemException : public _com_error
{
private:
	CWbemClassObject *  m_pWbemError;
	HRESULT             m_hr;
	_bstr_t             m_sDescription;
	
	static IErrorInfo * GetErrorObject();
	static IErrorInfo * MakeErrorObject(_bstr_t);
	void GetWbemStatusObject();
	
public:
	// use this constructor to capture the WBEM error object
	// pass in the HRESULT from the failed COM call and a descriptive message
	CWbemException(HRESULT hr,_bstr_t sMessage);
	
	// use this constructor to create a generic error, does not return WBEM error
	CWbemException(_bstr_t sMessage);
	
	// returns the WBEM __ExtendedStatus object if one exists
	CWbemClassObject GetWbemError();
	
	// returns the description passed to the constructor
	_bstr_t GetDescription() { return m_sDescription;  }
	
	// returns the HRESULT passed to the constructor, WBEM_E_FAILED if none passed
	HRESULT GetErrorCode()   { return m_hr;            }
	
	// returns the string representation of the error code for the supplied HRESULT
	static _bstr_t GetWbemErrorText(HRESULT hr);
};





//-----------------------------------------------------------------------------------
// CWbemClassObject
//
//  This class encapsulates a WBEM object, it transparently handles AddRef and Release.
//  It also provides access to the objects methods.
//
//  Assigning an instance of this object or passing it by value does not copy the 
//  object, only the pointer. Use Clone to make a new object. Use one of the casting
//  operators to extract the underlying pointer.
//
//  See IWbemClassObject for more information on the WBEM methods.
//

class CWbemClassObject
{
private:
	IWbemClassObjectPtr     m_pWbemObject;

	// global object count for WBEM objects encapsulated by this class
	// helpful for debugging
	static DWORD s_dwObjectCount;
	
public:

	// these constructors will addref the pointer
	CWbemClassObject(const CWbemClassObject&  _in);
	CWbemClassObject(IWbemClassObject * const _in);
	CWbemClassObject(IWbemClassObjectPtr& _in);
	CWbemClassObject(IUnknown * _in);
	CWbemClassObject(IUnknownPtr& _in);

	CWbemClassObject();
	~CWbemClassObject();
	
	void Attach(IWbemClassObject * pWbemObject)
	{
		m_pWbemObject.Attach(pWbemObject);
	}
	
	void Attach(IWbemClassObject * pWbemObject,bool bAddRef)
	{
		m_pWbemObject.Attach(pWbemObject,bAddRef);
	}
	
	// this operator allows you to use the object as a pointer to 
	// call other IWbemClassObject methods without casting
	IWbemClassObject * operator->()
	{
		return m_pWbemObject;
	}
	
	unsigned long GetObjectSize();
	
	// Retrives the MOF description
	_bstr_t GetObjectText()
	{
		_bstr_t bRet;
		
		BSTR bstr;
		
		if( !FAILED(m_pWbemObject->GetObjectText(0,&bstr)) )
		{
			bRet = _bstr_t(bstr,false);
		}
		
		return bRet;
	}
	
	IWbemClassObject * Detach()
	{
		return m_pWbemObject.Detach();
	}
	
	// various casting operators

	operator IWbemClassObject*()
	{
		return m_pWbemObject;
	}
	
	operator IWbemClassObject**()
	{
		return &m_pWbemObject;
	}
	
	operator IWbemClassObjectPtr()
	{
		return m_pWbemObject;
	}
	
	
	operator IUnknown *()
	{
		return (IUnknown *)(IWbemClassObject *)m_pWbemObject;
	}
	
	
	// address of operator
	IWbemClassObject ** operator &()
	{
		return &m_pWbemObject;
	}
	
	// these operators allow you to check if the underlying pointer is NULL in an if statement
	// just like a regular pointer
	bool operator !() 
	{
		return m_pWbemObject == NULL;
	}
	
	operator bool() 
	{
		return m_pWbemObject != NULL;
	}

	bool IsNull() const 
	{ 
		return m_pWbemObject == NULL;
	}

	
	// AddRef and Release
	ULONG AddRef()
	{
		return m_pWbemObject->AddRef();
	}
	
	ULONG Release()
	{
		return m_pWbemObject->Release();
	}
	
	// assignment operators
	IWbemClassObject* operator=(IWbemClassObject* _p)
	{
		m_pWbemObject = _p;
		return m_pWbemObject;
	}
	
	IWbemClassObjectPtr operator=(IWbemClassObjectPtr& _p)
	{
		m_pWbemObject = _p;
		return m_pWbemObject;
	}
	
	
	IWbemClassObject* operator=(IUnknown * _p)
	{
		m_pWbemObject = _p;
		return m_pWbemObject;
	}
	
	IWbemClassObjectPtr operator=(IUnknownPtr& _p)
	{
		m_pWbemObject = _p;
		return m_pWbemObject;
	}
	
	
	IWbemClassObject* operator=(const CWbemClassObject& _p)
	{
		m_pWbemObject = _p.m_pWbemObject;
		return m_pWbemObject;
	}
	
	// make a copy of the object
	HRESULT Clone(CWbemClassObject& _newObject)
	{
		return m_pWbemObject->Clone(_newObject);
	}
	
	// spawn a new blank instance of the object
	CWbemClassObject SpawnInstance()
	{
		CWbemClassObject coRet;
		
		m_pWbemObject->SpawnInstance(0,coRet);
		
		return coRet; 
	}
	
	// get a method signatures for a class 
	HRESULT GetMethod(const IN _bstr_t& _name, CWbemClassObject& coInSignature,
		CWbemClassObject& coOutSignature, long _lFlags = 0)
	{
		return m_pWbemObject->GetMethod(_name, _lFlags, coInSignature, coOutSignature);
	}
	
	// this operator is defined so that the object can be stored in ordered lists
	// or other structures
	bool operator<(const CWbemClassObject& _comp)
	{
		return m_pWbemObject < _comp.m_pWbemObject;
	}
	
	/***** Get and Put property access methods */

	//*** put overloads ***
	HRESULT Put(const _bstr_t& _Name,_variant_t _value,CIMTYPE vType = 0)
	{
		return m_pWbemObject->Put(_Name,0,&_value,vType);
	}
	
	HRESULT Put(const _bstr_t& _Name,const _bstr_t& _value,CIMTYPE vType = 0)
	{
		return m_pWbemObject->Put(_Name,0,&_variant_t(_value),vType);
	}
	
	HRESULT Put(const _bstr_t& _Name, const long _value, CIMTYPE vType = 0)
	{
		return m_pWbemObject->Put(_Name,0,&_variant_t(_value), vType);
	}
	
	HRESULT Put(const _bstr_t& _Name, const bool _value,CIMTYPE vType = 0)
	{
		return m_pWbemObject->Put(_Name,0,&_variant_t(_value),vType);
	}
	
	//*** get overloads ***
	HRESULT Get(const _bstr_t& _Name, _bstr_t& _value)
	{
		_variant_t v1;
		HRESULT hr = m_pWbemObject->Get (_Name, 0, &v1, NULL, NULL);
		_value = v1;
		return hr;
	}
	
	HRESULT Get(const _bstr_t& _Name, long& _value)
	{
		_variant_t v1;
		HRESULT hr = m_pWbemObject->Get (_Name, 0, &v1, NULL, NULL);
		_value = v1;
		return hr;
		
	}
	
	HRESULT Get(const _bstr_t& _Name, bool& _value)
	{
		_variant_t v1;
		HRESULT hr = m_pWbemObject->Get (_Name, 0, &v1, NULL, NULL);
		_value = v1;
		return hr;
	}
	
	HRESULT Get(const _bstr_t& _Name,_variant_t& _value)
	{
		_value.Clear();
		return m_pWbemObject->Get (_Name, 0, &_value, NULL, NULL);
	}
	
	
	_variant_t Get(const _bstr_t& _Name,CIMTYPE& vType,long& lFlavor)
	{
		_variant_t vRet;
		
		m_pWbemObject->Get (_Name, 0, &vRet, &vType, &lFlavor);
		
		return vRet;
	}
	
	
	_bstr_t GetString   (const _bstr_t& _Name);
	_int64  GetI64      (const _bstr_t& _Name);
	long    GetLong     (const _bstr_t& _Name);
	bool    GetBool     (const _bstr_t& _Name);
	_bstr_t GetCIMTYPE  (const _bstr_t& _Name);
	
	// retrieves a WBEM object embedded in a property
	CWbemClassObject GetEmbeddedObject(const _bstr_t& _Name);
	
	
	// use these for property enumeration
	HRESULT BeginEnumeration(long _lFlags = WBEM_FLAG_LOCAL_ONLY)
	{
		return m_pWbemObject->BeginEnumeration(_lFlags);
	}
	
	
	HRESULT Next(_bstr_t& _sName,_variant_t& _value)
	{
		HRESULT     hr;
		BSTR        bstr = NULL;
		
		hr = m_pWbemObject->Next(0,&bstr,&_value,NULL,NULL);
		
		if(hr == S_OK)
		{
			_sName = _bstr_t(bstr,false);
		}
		
		return hr;
	}
	
	HRESULT Next(_bstr_t& _sName,_variant_t& _value,CIMTYPE& _vartype)
	{
		HRESULT     hr;
		BSTR        bstr = NULL;
		
		_value.Clear();
		hr = m_pWbemObject->Next(0,&bstr,&_value,&_vartype,NULL);
		
		if(hr == S_OK)
		{
			_sName = _bstr_t(bstr,false);
		}
		
		return hr;
	}
	
	HRESULT Next(_bstr_t& _sName,_variant_t& _value,CIMTYPE& _vartype,long& _flavor)
	{
		HRESULT     hr;
		BSTR        bstr = NULL;
		
		_value.Clear();
		hr = m_pWbemObject->Next(0,&bstr,&_value,&_vartype,&_flavor);
		
		if(hr == S_OK)
		{
			_sName = _bstr_t(bstr,false);
		}
		
		return hr;
	}
	
	HRESULT EndEnumeration()
	{
		return m_pWbemObject->EndEnumeration();
	}
	
};


// these operators are needed for storage in some data structures
__inline bool operator<(const CWbemClassObject& _X, const CWbemClassObject& _Y) 
{
	return _X < _Y;
}

__inline bool operator==(const CWbemClassObject& _X, const CWbemClassObject& _Y)
{
	return _X == _Y;
}



//-----------------------------------------------------------------------------
// CWbemServices
//
//  Provides thread safe encapsulation of the IWbemServices interface and provides
//  access to it's methods. See the IWbemServices documentation for information 
//  about the methods.
//
//  This clas also encapsulates IWbemLocator::ConnectServer(). When using this call, 
//	 provide the address of a NULL IWbemServices pointer to act as a cache. This class
//  actually stores the interface pointer in an IStream. A real interface needs
//  to be kept somewhere or DCOM will garbage collect the object. When the instance
//  of this class is deleted, the cached pointer still needs to be released. Use a
//  smart pointer (IWbemServicesPtr) and keep it in the same scope as the 
//  CWbemServices instance.
//

class CWbemServices 
{
private:
	IWbemContextPtr     m_pCtx;
	IStream *           m_pServicesStream; // encapsulated IWbemServices pointer
	IUnknownPtr         m_pUnkCache;
	CRITICAL_SECTION    m_cs;
	
	bool GetInterfacePtr(IWbemServicesPtr & pServices,bool bThrowException = true);
	void CommonInit(IWbemServicesPtr& pServ);
	
public:
	// constructors will AddRef the pointer
	CWbemServices(IWbemContext * _pContext = NULL);
	CWbemServices(const CWbemServices& _p);
	CWbemServices(const IWbemServicesPtr& _in);
	CWbemServices(const IUnknownPtr& _in);
	CWbemServices(IUnknown * _in);
	CWbemServices(IWbemServices *_in,IWbemContext * _pContext = NULL);
	~CWbemServices();
	
	CWbemServices& operator=(IUnknown * _p);
	CWbemServices& operator=(IUnknownPtr& _p);
	CWbemServices& operator=(IWbemServices *_p);
	CWbemServices& operator=(const CWbemServices& _p);
	
	// when logging in, capture the IWbemServices pointer and keep it somewhere
	// this pointer is stored internaly in an IStream and DCOM will garbage collect the object if
	// there isn't at least one reference to it

	// Login with integrated security
	void ConnectServer(_bstr_t sNetworkResource, IWbemServices **_WbemServicesCache) throw(CWbemException);
	
	// Login as a specific user
	void ConnectServer
		(
		_bstr_t sNetworkResource,
		_bstr_t sUser,
		_bstr_t sPassword,
		IWbemServices **_WbemServicesCache,
		long    lSecurityFlags = 0
		) throw(CWbemException);
	
	
	// create an instance a of a class given the class name
	// encapsulates GetObject and SpawnInstance
	CWbemClassObject CreateInstance
		(
		_bstr_t             _sClassName,
		IWbemCallResultPtr& _cr
		)
	{
		CWbemClassObject coClassDef = GetObject(_sClassName,_cr);
		CWbemClassObject coRet;
		
		if(!coClassDef.IsNull())
		{
			coRet = coClassDef.SpawnInstance();
		}
		
		return coRet;
	}
	
	CWbemClassObject CreateInstance(_bstr_t _sClassName)
	{
		IWbemCallResultPtr crUnused;
		return CreateInstance(_sClassName,crUnused);
	}
	
	HRESULT DeleteInstance
		(
		_bstr_t             _sClass, 
		IWbemCallResultPtr& _cr
		)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->DeleteInstance(_sClass, 0, m_pCtx, &_cr);
		return hr;
	}
	
	HRESULT DeleteInstance(_bstr_t _sClass)
	{
		IWbemCallResultPtr crUnused;
		return DeleteInstance(_sClass, crUnused);
	}
	
	CWbemClassObject GetObject
		(
		_bstr_t                 _sName,
		IWbemCallResultPtr&     _cr,
		bool                    _bThrowOnError = false
		);
	
	CWbemClassObject GetObject(_bstr_t _sName,bool _bThrowOnError = false)
	{
		IWbemCallResultPtr crUnused;
		return GetObject(_sName,crUnused,_bThrowOnError);
	}
	
	
	HRESULT GetMethodSignatures(const _bstr_t& _sObjectName,const _bstr_t& _sMethodName,CWbemClassObject& _in,CWbemClassObject& _out)
	{
		HRESULT hr = E_FAIL;
		
		CWbemClassObject methodClass = GetObject(_sObjectName);
		
		if(methodClass)
		{
			hr = methodClass.GetMethod(_sMethodName,_in,_out);
		}
		
		return hr;
	}
	
	
	HRESULT PutInstance
		(
		CWbemClassObject&   _object,
		IWbemCallResultPtr& _cr,
		long                _lFlags = WBEM_FLAG_CREATE_OR_UPDATE
		)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->PutInstance(_object,_lFlags,m_pCtx,&_cr);
		return hr;
	}
	
	HRESULT PutInstance
		(
		CWbemClassObject&   _object,
		long                _lFlags = WBEM_FLAG_CREATE_OR_UPDATE
		)
	{
		IWbemCallResultPtr crUnused;
		return PutInstance(_object,crUnused,_lFlags);
	}
	
	bool IsNull() 
	{  
		IWbemServicesPtr pServices;
		bool bRet = GetInterfacePtr(pServices,false) == NULL;
		
		return bRet;
	}
	
	operator bool()
	{
		IWbemServicesPtr pServices;
		bool bRet = GetInterfacePtr(pServices,false) != NULL;
		return bRet;
	}

	operator !()
	{
		IWbemServicesPtr pServices;
		bool bRet = GetInterfacePtr(pServices,false) == NULL;
		return bRet;
	}

	
	HRESULT CreateInstanceEnum
		(
		_bstr_t Class, 
		long lFlags, 
		IEnumWbemClassObject **ppEnum
		)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->CreateInstanceEnum(Class, lFlags, m_pCtx, ppEnum);
		return hr;
	}
	
	
	HRESULT CreateInstanceEnumAsync
		(
		_bstr_t Class, 
		IWbemObjectSink * ppSink,
		long lFlags = 0
		)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->CreateInstanceEnumAsync(Class, lFlags, m_pCtx, ppSink);
		return hr;
	}
	
	
	HRESULT CreateClassEnum
		(
		_bstr_t Class, 
		long lFlags, 
		IEnumWbemClassObject **ppEnum
		)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->CreateClassEnum(Class, lFlags, m_pCtx, ppEnum);
		return hr;
	}
	
	HRESULT ExecQuery
		(
		_bstr_t QueryLanguage,
		_bstr_t Query,
		long lFlags,
		IEnumWbemClassObject **ppEnum
		) 
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->ExecQuery(QueryLanguage, Query,lFlags, m_pCtx, ppEnum);
		return hr;
	}
	
	HRESULT ExecQuery
		(
		_bstr_t Query,
		long lFlags,
		IEnumWbemClassObject **ppEnum
		) 
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->ExecQuery(_bstr_t("WQL"), Query,lFlags, m_pCtx, ppEnum);
		return hr;
	}
	
	HRESULT ExecQuery
		(
		_bstr_t Query,
		IEnumWbemClassObject **ppEnum
		) 
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->ExecQuery(_bstr_t("WQL"), Query,0, m_pCtx, ppEnum);
		return hr;
	}
	
	
	
	HRESULT ExecQueryAsync
		(
		_bstr_t Query,
		IWbemObjectSink * pSink,
		long lFlags = 0
		)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->ExecQueryAsync(_bstr_t("WQL"), Query,0, m_pCtx, pSink);
		return hr;
	}
	
	
	HRESULT ExecMethod
		(
		_bstr_t sPath,
		_bstr_t sMethod,
		CWbemClassObject& inParams,
		CWbemClassObject& outParams
		)
	{
		IWbemCallResultPtr crUnused;
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->ExecMethod(sPath, sMethod,0, m_pCtx, inParams,&outParams,&crUnused);
		return hr;
	}
	
	HRESULT CancelAsyncCall(IWbemObjectSink * pSink)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		HRESULT hr = pServices->CancelAsyncCall(pSink);
		return hr;
	}
	
	// context values are stored and used for every call until cleared
	HRESULT SetContextValue(_bstr_t sName,_variant_t value);
	HRESULT GetContextValue(_bstr_t sName,_variant_t& value);
	HRESULT DeleteContextValue(_bstr_t sName);
	HRESULT DeleteAllContextValues();
	HRESULT SetContext(IWbemContext * pWbemContext);
	HRESULT GetContext(IWbemContext ** ppWbemContext);
	
	HRESULT GetServices(IWbemServices ** ppServices)
	{
		IWbemServicesPtr pServices;
		GetInterfacePtr(pServices);
		
		*ppServices = pServices.Detach();
		
		return S_OK;
	}
	
};


#endif //__SdkWbemHelpers_H

