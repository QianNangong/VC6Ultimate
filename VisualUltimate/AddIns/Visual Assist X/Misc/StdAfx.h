/*
//stdafx.h and stdafxva.h
//
//Stdafx.h is the first file parsed after [re]install of Visual Assist, and at
//restart of MSDev after you press Rebuild on the Performance tab of the
//Visual Assist options dialog.
//
//stdafxva.h is parsed AFTER stdafx.h. Headers specified on the Directories
//tab are parsed AFTER parsing of stdafxva.h.
//
//Stdafx.h and stdafxva.h are overwritten during [re]installation of Visual
//Assist. If you wish to change these files, make a backup of your changes.
//Restore your changes after each installation. Make your changes prior to
//start of MSDev.
//
//If you wish to change stdafx.h or stdafxva.h after start of MSDev, ie after
//Visual Assist has done some parsing, press Rebuild on the Performance tab
//and restart MSDev.
//
//Stdafx.h contains preprocessor macros that MUST be known before parsing of
//any other header file, ie before MS headers, 3rd party headers and those in
//projects.
//
//Stdafxva.h directs Visual Assist to parse a core set of MS headers prior to
//parsing of headers on the Directories tab.
//
//The contents of stdafx.h help Visual Assist resolve other definitions,
//replace definitions that Visual Assist cannot resolve, and replace
//definitions that inadvertently alter scope.
//
//Additions and changes to stdafx.h are generally done upon advice from
//support@wholetomato.com.
//
//The most common, customer-made addition to stdafx.h is the one that coerces
//Visual Assist to ignore a preprocessor macro completely. The macro is
//#defined here to be nothing. Visual Assist finds this definition before
//finding any others, and uses it when resolving definitions that use the
//macro.
 */

///////////////////////////////////////////////////////////////////////////////
// macro overrides

#define END_INTERFACE_PART_STATIC(i) END_INTERFACE_PART(i)
#define _COM_SMARTPTR_TYPEDEF(Interface, IID) \
	typedef _com_ptr_t<Interface>\
	Interface ## Ptr;
#define MIDL_INTERFACE(x) struct
#define STATIC_DOWNCAST(cls, obj) ; cls
#define DYNAMIC_DOWNCAST(cls, obj) ; cls
#define _STD_BEGIN namespace std {

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    const AFX_MSGMAP* theClass::GetMessageMap() const \
        { return GetThisMessageMap(); } \
    const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
    { \
        typedef theClass ThisClass;                          \
        typedef baseClass TheBaseClass;                      \
        static const AFX_MSGMAP_ENTRY _messageEntries[] =  \
        { bogus_entries =

#define END_MESSAGE_MAP() \
		{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
	}; \
		static const AFX_MSGMAP messageMap = \
		{ &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
		return &messageMap; \
	}

#define __declspec(x)
#define _declspec(x)
#define __DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_ARGLIST_EX(x)
#define __DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_2_ARGLIST_EX(x)

// Parsing of IDL files
#define cpp_quote(x) x;


#define __attribute__(x)

// Added for speed
#define ASSERT(x)
#define TRACE(x)
#define _T(x)
#define __T(x)

#define AfxEnableMemoryTracking(bTrack) AfxEnableMemoryTracking(bTrack)



// fix for stlport defines
#define __msl_basic_string std::basic_string
#define __msl_b_str_ref std::b_str_ref
#define __msl_basic_istream std::basic_istream
#define __msl_basic_ostream std::basic_ostream
#define __msl_string std::string
#define __msl_wstring std::wstring
#define __msl_iterator_traits std::iterator_traits



#define GetCurrentTime() GetCurrentTime;  // to fix CTime::GetCurrentTime();

// param annotations
#define OPTIONAL()
#define IN()
#define __in()
#define __in_opt()
#define __in_deref()
#define __in_deref_opt()
#define __in_bcount(s)
#define __in_ecount(s)
#define __in_xcount(s)
#define OUT()
#define __out()
#define __out_opt()
#define __out_deref()
#define __out_deref_opt()
#define __out_bcount(s)
#define __out_ecount(s)
#define __out_xcount(s)
#define __out_full()
#define __out_part()
#define INOUT()
#define __inout()
#define __inout_opt()
#define __inout_deref()
#define __inout_deref_opt()
#define __inout_bcount(s)
#define __inout_ecount(s)
#define __inout_xcount(s)
#define __inout_full()
#define __inout_part()

#define __opt()
#define __attribute__()
#define __gc()
#define __if_exists(x) __if_exists

#define _ecount(s)
#define _bcount(s)
#define _full(s)
#define _part(s, l)
#define __nullterminated()
#define __nullnullterminated()
#define __possibly_notnullterminated()
#define __checkReturn()
#define __reserved()


// driver annotations
#define __drv_arg(arg, anno_list)
#define __drv_arg(__param(n), anno_list)
#define __drv_deref(anno_list)
#define __drv_fun(anno_list)
#define __drv_in(anno_list)
#define __drv_in_deref(anno_list)
#define __drv_out(anno_list)
#define __drv_out_deref(anno_list)
#define __drv_ret(anno_list)
#define __drv_when(cond, anno_list)
#define __drv_valueIs(list)
#define __drv_strictTypeMatch(mode)
#define __drv_strictType(typename, mode)
#define __drv_notPointer()
#define __drv_isObjectPointer()
#define __drv_constant()
#define __drv_nonConstant()
#define __drv_formatString(kind)
#define __drv_preferredFunction(name, reason)
#define __drv_reportError(string)
#define __drv_inTry()
#define __drv_notInTry()
#define __drv_allocatesMem(type)
#define __drv_freesMem(type)
#define __drv_aliasesMem()
#define __drv_acquiresResource(kind)
#define __drv_releasesResource(kind)
#define __drv_acquiresResourceGlobal(kind, param)
#define __drv_releasesResourceGlobal(kind, param)
#define __drv_acquiresCriticalRegion()
#define __drv_releasesCriticalRegion()
#define __drv_acquiresCancelSpinLock()
#define __drv_releasesCancelSpinLock()
#define __drv_mustHold(kind)
#define __drv_neverHold(kind)
#define __drv_mustHoldGlobal(kind, param)
#define __drv_neverHoldGlobal(kind, param)
#define __drv_mustHoldCriticalRegion()
#define __drv_neverHoldCriticalRegion()
#define __drv_mustHoldCancelSpinLock()
#define __drv_neverHoldCancelSpinLock()
#define __drv_acquiresExclusiveResource(kind)
#define __drv_releasesExclusiveResource(kind)
#define __drv_acquiresExclusiveResourceGlobal(kind, param)
#define __drv_releasesExclusiveResourceGlobal(kind, param)
#define __drv_functionClass()
#define __drv_floatSaved()
#define __drv_floatRestored()
#define __drv_floatUsed()
#define __drv_maxIRQL(value)
#define __drv_minIRQL(value)
#define __drv_setsIRQL(value)
#define __drv_requiresIRQL(value)
#define __drv_raisesIRQL(value)
#define __drv_savesIRQL()
#define __drv_restoresIRQL()
#define __drv_savesIRQLGlobal(kind, param)
#define __drv_restoresIRQLGlobal(kind, param)
#define __drv_minFunctionIRQL(value)
#define __drv_maxFunctionIRQL(value)
#define __drv_sameIRQL()
#define __drv_isCancelIRQL()
#define __drv_clearDoInit()
#define __drv_interlocked()


// SAL annotations
#define __success(x)
#define _Success_(x)
#define _SAL_nop_impl_


// prevent STLPort from redefining
#define std std


// VC7 AFX defines
#define AFX_NOVTABLE
#define AFX_DATA
#define AFX_DATADEF
#define AFX_API
#define AFX_COMDAT
#define AFX_CORE_DATA
#define AFX_CORE_DATADEF
#define AFX_OLE_DATA
#define AFX_OLE_DATADEF

#define AFX_EXT_CLASS
#define AFX_EXT_API
#define AFX_EXT_DATA


struct tagVARIANT
{
//    union 
//        {
//        struct __tagVARIANT
//            {
            VARTYPE vt;
            WORD wReserved1;
            WORD wReserved2;
            WORD wReserved3;
//            union 
//                {
                LONGLONG llVal;
                LONG lVal;
                BYTE bVal;
                SHORT iVal;
                FLOAT fltVal;
                DOUBLE dblVal;
                VARIANT_BOOL boolVal;
                _VARIANT_BOOL bool;
                SCODE scode;
                CY cyVal;
                DATE date;
                BSTR bstrVal;
                IUnknown *punkVal;
                IDispatch *pdispVal;
                SAFEARRAY *parray;
                BYTE *pbVal;
                SHORT *piVal;
                LONG *plVal;
                LONGLONG *pllVal;
                FLOAT *pfltVal;
                DOUBLE *pdblVal;
                VARIANT_BOOL *pboolVal;
                _VARIANT_BOOL *pbool;
                SCODE *pscode;
                CY *pcyVal;
                DATE *pdate;
                BSTR *pbstrVal;
                IUnknown **ppunkVal;
                IDispatch **ppdispVal;
                SAFEARRAY **pparray;
                VARIANT *pvarVal;
                PVOID byref;
                CHAR cVal;
                USHORT uiVal;
                ULONG ulVal;
                ULONGLONG ullVal;
                INT intVal;
                UINT uintVal;
                DECIMAL *pdecVal;
                CHAR *pcVal;
                USHORT *puiVal;
                ULONG *pulVal;
                ULONGLONG *pullVal;
                INT *pintVal;
                UINT *puintVal;
//                struct __tagBRECORD
//                    {
                    PVOID pvRecord;
                    IRecordInfo *pRecInfo;
//                    } 	__VARIANT_NAME_4;
//                } 	__VARIANT_NAME_3;
//            } 	__VARIANT_NAME_2;
        DECIMAL decVal;
//        } 	__VARIANT_NAME_1;
} ;

// VA needs a bit of help parsing some STD libs
namespace std {
	template<class _T1, class _T2>
		class pair {
		typedef _T1 first_type;
		typedef _T2 second_type;
		_T1 first;
		_T2 second;
	};
	
	template<class _Ty>
		class VAiterator : public std::vector::iterator{
		typedef VAiterator<_Ty> iterator;
		_Ty m_tst2;
		_Ty operator->();
		_Ty operator[]();
		_Ty* operator*();
		//		VAiterator _Inc();
		//		VAiterator _Dec();
	};
	
	template <class T>
		class VAListIterator :public T {
		T m_tst;
		T operator->();
		T operator[]();	
	};
	
	template<class _Ty, class _A>
		class vector {
public:
	typedef allocator<_Ty> _A;
	typedef *_Ty _Tptr;
	typedef *_Ty _Ctptr;
	typedef _Ty reference;
	typedef _Ty const_reference;
	typedef _Ty value_type;
	class VAListIterator :public _Ty {
		_Ty m_tst2;
		_Ty operator->();
		_Ty operator[]();	
	};
	typedef std::VAiterator<_Ty> iterator;
	typedef std::VAiterator<_Ty> reverse_iterator;
	typedef std::VAiterator<_Ty> const_iterator;
	};
	
	template<class _K, class _Ty, class _Kfn, class _Pr, class _A>
		class _Tree {
		typedef _Ty iterator;
	};
	
	template<class _K, class _Ty>
		class map {
		typedef _Tree<_K, pair<_K,_Ty>> _Imp;
		
		typedef std::VAiterator<_Ty> iterator;
		typedef std::VAiterator<_Ty> reverse_iterator;
		typedef std::VAiterator<_Ty> const_iterator;
		
		class iterator : public std::VAiterator<std::pair<_K, _Ty>>{};
		class reverse_iterator : public std::VAiterator<std::pair<_K, _Ty>>{};
		class const_iterator : public std::VAiterator<std::pair<_K, _Ty>>{};
		class const_reverse_iterator : public std::VAiterator<std::pair<_K, _Ty>>{};
		_Ty operator[]();
		typedef _K  reference;
	};
	template<class _Ty, class _A = allocator<_Ty> >
		class list {
		typedef _Ty _A;
		typedef _Ty allocator_type;
		typedef _Ty difference_type;
		typedef *_Ty _Tptr;
		typedef *_Ty _Ctptr;
		typedef _Ty reference;
		typedef _Ty const_reference;
		typedef _Ty value_type;
		
		typedef std::VAiterator<_Ty> iterator;
		typedef std::VAiterator<_Ty> reverse_iterator;
		typedef std::VAiterator<_Ty> const_iterator;
	};
	template<class _Kty, class _Ty>
		class hash_map
		: public _Hash<_Hmap_traits<_Kty, _Ty, _Tr, _Alloc, false> >
	{	// hash table of {key, mapped} values, unique keys
		typedef hash_compare<_Kty, less<_Kty>> _Tr;
		typedef allocator<pair<const _Kty, _Ty>> _Alloc; 
		typedef pair<_Kty, _Ty>  iterator;
		typedef pair<_Kty, _Ty>  const_iterator;
		typedef pair<_Kty, _Ty>  reverse_iterator;
		typedef pair<_Kty, _Ty>  const_reverse_iterator;
		_Ty operator[]();
		typedef _K  reference;
	};
};
namespace std
{
	typedef std::basic_string<char, char_traits<char>, allocator<char> > string; // for vc6 std::string
};

class CString :public ATL::CAtlString, ATL::CStringT, ATL::CSimpleStringT{}; // for VC7

namespace ATL
{
	class CString :public ATL::CAtlString, ATL::CStringT, ATL::CSimpleStringT{}; // for VC7

	template <class TBase = CWindow>
		class ATL_NO_VTABLE CWindowImplRoot : public CWindow, public CMessageMap
	{
	};
}

namespace WTL 
{
	template <class T, class TBase = CWindow, class TWinTraits = CFrameWinTraits>
		class ATL_NO_VTABLE CFrameWindowImpl : public CFrameWindowImplBase<CWindow, CFrameWinTraits>, CWindowImplBaseT, CWindowImplRoot, CWindow
	{
	};

	template <class TBase = CWindow, class TWinTraits = CFrameWinTraits>
		class ATL_NO_VTABLE CFrameWindowImplBase : public CWindowImplBaseT, CWindowImplRoot, CWindow 
	{
	};
}

namespace cli
{
	template<typename T>
	class array : public System::Array
	{
		T operator[]();
	};

	template<typename T>
	class interior_ptr
	{
		T* operator*();
	};

	template<typename T>
	class pin_ptr
	{
		T* operator*();
	};

	template<typename T>
	class safe_cast
	{
	};
}


// ImgSource and ISEffects libraries
#define _ISFnDeco(functionName) functionName
#define _ISEFnDeco(fn) fn


// __MACHINE intrinsics (case=7146)
#define __MACHINEI(x) x;
#define __MACHINEX64(x) x;
#define __MACHINEIA32(x) x;
#define __MACHINEX86_X64(x) x;
#define __MACHINEX86X_X64(x) x;
#define __MACHINEX86X_IA64(x) x;
#define __MACHINEIA64(x) x;
#define __MACHINEW64(x) x;
#define __MACHINEIW64(x) x;
#define __MACHINESA(x) x;
#define __MACHINEARMX(x) x;
#define __MACHINECC(x) x;
#define __MACHINECE(x) x;
#define __MACHINE(x) x;

#define _NOEXCEPT
