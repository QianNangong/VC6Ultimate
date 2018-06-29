#pragma once
//-------------------------------------------------------------------------------------------------
#include <windows.h>
#include <wchar.h>
#include <stdio.h>
//---------------------------------------------------------------------------------------------
#undef FindText
//---------------------------------------------------------------------------------------------
#define _DbgBreak() __asm { int 3 }
#ifdef _Break
	#undef _Break
#endif
#ifdef _ToDo
	#undef _ToDo
#endif
#ifdef SpAssert
	#undef SpAssert
#endif
//---------------------------------------------------------------------------------------------
#if (!defined(NDEBUG) || defined(_RDEBUG)) || defined(_BREAK)
	#define SpAssert(BooleanExpression) do{ if (!(BooleanExpression)) _DbgBreak(); } while(0)
	#define _Break() { _DbgBreak(); } 
	#define _ToDo() { _DbgBreak(); } 
#else
	#define SpAssert(BooleanExpression) do {} while(0)
	#define _Break() { } 
	#define _ToDo() { } 
#endif
//---------------------------------------------------------------------------------------------
void* (__fastcall *GetApp)() = 0;
//---------------------------------------------------------------------------------------------
class _variant_t;
//---------------------------------------------------------------------------------------------
struct _bstr_t {
	_bstr_t():m_Data(NULL)																					{ }
	_bstr_t(const _bstr_t& s): m_Data(s.m_Data)											{ _AddRef(); }
//	_bstr_t(const char* s): m_Data(new Data_t(s))										{ }
	_bstr_t(const wchar_t* s): m_Data(new Data_t(s))								{ }
	_bstr_t(const _variant_t& var);
	//_bstr_t(BSTR bstr, bool fCopy): m_Data(new Data_t(bstr, fCopy))	{ }

	_bstr_t(size_t length): m_Data(new Data_t(length))							{ }

	~_bstr_t()																											{ _Free(); }

	// Assignment operators
	_bstr_t& operator=(const _bstr_t& s)														{	const_cast<_bstr_t*>(&s)->_AddRef(); _Free(); m_Data = s.m_Data; return *this; }

//	_bstr_t& operator=(const char* s)																{ _Free(); m_Data = new Data_t(s); return *this; }
	_bstr_t& operator=(const wchar_t* s)														{ _Free(); m_Data = new Data_t(s); return *this; }
	_bstr_t& operator=(const _variant_t& var);

	// Operators
	_bstr_t& operator+=(const _bstr_t& s)														{ Data_t* newData = new Data_t(*this, s); _Free(); m_Data = newData; return *this; }
	_bstr_t operator+(const _bstr_t& s) const												{ _bstr_t b = *this; b += s; return b; }

	// Friend operators
//	friend _bstr_t operator+(const char* s1, const _bstr_t& s2)			{ _bstr_t b = s1; b += s2; return b; }
	friend _bstr_t operator+(const wchar_t* s1, const _bstr_t& s2)	{ _bstr_t b = s1; b += s2; return b; }

	// Extractors
	operator const wchar_t*() const																	{	return (m_Data) ? m_Data->m_wstr : NULL; }
	operator wchar_t*() const																				{	return (wchar_t*)(m_Data ? m_Data->m_wstr : NULL); }
//	operator const char*() const																		{	return (m_Data != NULL) ? m_Data->GetString() : NULL; }
//	operator char*() const																					{ return const_cast<char*>((m_Data != NULL) ? m_Data->GetString() : NULL); }

	// Comparison operators
	bool operator!() const																					{	return (m_Data) ? !m_Data->m_wstr : true; }
	bool operator==(wchar_t const* str) const												{	return _Compare(str) == 0; }
	bool operator!=(wchar_t const* str) const												{ return _Compare(str) != 0; }
	bool operator<(wchar_t const* str) const												{ return _Compare(str) < 0; }
	bool operator>(wchar_t const* str) const												{ return _Compare(str) > 0; }
	bool operator<=(wchar_t const* str) const												{ return _Compare(str) <= 0; }
	bool operator>=(wchar_t const* str) const												{ return _Compare(str) >= 0; }

	// Low-level helper functions
	BSTR copy() const																								{	return m_Data ? m_Data->Copy() : NULL; }
	unsigned int length() const																			{ return m_Data ? m_Data->Length() : 0; }
	bool isEmpty() const																						{ return (!m_Data || !m_Data->m_wstr || !*m_Data->m_wstr); }

	// Binary string assign
	void Assign(BSTR s)																							{ if (m_Data) m_Data->Assign(s); else m_Data = new Data_t(s, TRUE);	}

private:
	// Referenced counted wrapper
	struct Data_t {
//		Data_t(const char* s): m_str(NULL), m_RefCount(1)							{ m_wstr = _com_util::ConvertStringToBSTR(s); }
		Data_t(const wchar_t* s): /*m_str(NULL), */m_RefCount(1)					{ m_wstr = ::SysAllocString(s); }
		Data_t(BSTR bstr, bool bCopy): /*m_str(NULL), */m_RefCount(1)			{ 
			if (bCopy && bstr){
				int len = ::SysStringByteLen(bstr);
				m_wstr = ::SysAllocStringByteLen((char* const)(bstr), len);
			}else
				m_wstr = bstr;
			//m_wstr = (bCopy && bstr) ? ::SysAllocStringByteLen((char* const)(bstr), ::SysStringByteLen(bstr)) : bstr; 
		}
		Data_t(const _bstr_t& s1, const _bstr_t& s2): /*m_str(NULL), */m_RefCount(1){
			const unsigned int l1 = s1.length();
			const unsigned int l2 = s2.length();
			if (l1 + l2 == 0){
				m_wstr=0;
				return;
			}
			m_wstr = ::SysAllocStringByteLen(NULL, (l1 + l2) * sizeof(wchar_t));
			const wchar_t* wstr1 = static_cast<const wchar_t*>(s1); if (wstr1) memcpy(m_wstr, wstr1, (l1 + 1) * sizeof(wchar_t));
			const wchar_t* wstr2 = static_cast<const wchar_t*>(s2);	if (wstr2) memcpy(m_wstr + l1, wstr2, (l2 + 1) * sizeof(wchar_t));
		}

		Data_t(size_t length): /*m_str(NULL), */m_RefCount(1)							{ m_wstr = ::SysAllocStringLen(NULL, length); }

		// Reference counting routines
		unsigned long AddRef()																						{	InterlockedIncrement(reinterpret_cast<long*>(&m_RefCount)); return m_RefCount; }
		unsigned long Release()																						{	if (!InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount))) { delete this; return 0; }	return m_RefCount; }

		// Extractors
		operator const wchar_t*() const																		{ return m_wstr; }
//		operator const char*() const																			{	return GetString(); }

		// Low-level helper functions
//		const char* GetString() const																			{ if (m_str == NULL) m_str = _com_util::ConvertBSTRToString(m_wstr); return m_str; }

		BSTR Copy() const																									{ if (m_wstr) return ::SysAllocStringByteLen(reinterpret_cast<char*>(m_wstr), ::SysStringByteLen(m_wstr)); else return NULL; }
		void Assign(BSTR s)																								{	_Free(); if (s) m_wstr = ::SysAllocStringByteLen(reinterpret_cast<char*>(s), ::SysStringByteLen(s)); }
		unsigned int Length() const																				{	return m_wstr ? ::SysStringLen(m_wstr) : 0; }
		//-----------------------------
		static int Compare(wchar_t const* A, wchar_t const* B, unsigned int l1, unsigned int l2){
			unsigned int len = l1;
			if (len > l2) 
				len = l2;

			while (len-- > 0) {
				if (*A++ != *B++)
					return A[-1] - B[-1];
			}
			return (l1 < l2) ? -1 : (l1 == l2) ? 0 : 1;
		}
		//-----------------------------
		int Compare(const _bstr_t& str) const{
			const wchar_t* _str = str;
			if (!m_wstr) { return _str ? -1 : 0; }
			if (!_str) return 1;

			const unsigned int l1 = ::SysStringLen(m_wstr);
			const unsigned int l2 = ::SysStringLen((wchar_t*)str);

			return Compare(m_wstr, _str, l1, l2);
		}
		//-----------------------------
		int CompareW(wchar_t const* str) const{
			if (!m_wstr) { return str ? -1 : 0; }
			if (!str) return 1;

			const unsigned int l1 = ::SysStringLen(m_wstr);
			const unsigned int l2 = ::wcslen(str);

			return Compare(m_wstr, str, l1, l2);
		}
		//-----------------------------
		wchar_t*			m_wstr;
	private:
//		mutable char*	m_str;
		unsigned long	m_RefCount;

		// Never allow default construction
		Data_t();

		// Never allow copy
		Data_t(const Data_t& s);

		// Prevent deletes from outside. Release() must be used.
		~Data_t()																											{ _Free(); }

		void _Free()																									{ if (m_wstr) ::SysFreeString(m_wstr); m_wstr=0;/*if (m_str)  delete [] m_str;*/ }
	};

private:
	// Reference counted representation
	Data_t* m_Data;

private:
	// Low-level utilities
	void _AddRef()																									{ if (m_Data) m_Data->AddRef(); }
	void _Free()																										{ if (m_Data) { m_Data->Release(); m_Data = NULL; } }
	int _Compare(const _bstr_t& str) const													{ if (m_Data == str.m_Data) return 0; if (!m_Data) return -1; if (!str.m_Data) return 1; return m_Data->Compare(str); }
	int _Compare(wchar_t const* str) const													{ if (m_Data->m_wstr == str) return 0; if (!m_Data) return -1; return m_Data->CompareW(str); }
};
//---------------------------------------------------------------------------------------------
#include "Interfaces.h"
//---------------------------------------------------------------------------------------------
#undef EXPORT
#define EXPORT __declspec(dllexport) void
//---------------------------------------------------------------------------------------------
//#define  BSTR _BSTR
//---------------------------------------------------------------------------------------------
int InStr(int Start, const BSTR string1, const BSTR string2){
	WCHAR* p=wcsstr(string1+Start-1, string2);
	if (!p)
		return 0;
	return p-string1+1;
}
//---------------------------------------------------------------------------------------------
int InStr(const BSTR string1, const BSTR string2){
	WCHAR* p=wcsstr(string1, string2);
	if (!p)
		return 0;
	return p-string1+1;
}
//---------------------------------------------------------------------------------------------
_bstr_t LCase(BSTR str){
	_bstr_t s(str);
	WCHAR* p = s;
	CharLowerW(p);
	return s;
}
//---------------------------------------------------------------------------------------------
inline BOOL IsWhiteChar(WCHAR c){
	return (c==L' ' || c==L'\t');
}
//---------------------------------------------------------------------------------------------
_bstr_t RTrim(BSTR string1){
	if (!string1 || !*string1)
		return L"";
	int Len=wcslen(string1);
	BSTR end = string1+Len;
	while(end>string1 && IsWhiteChar(end[-1]))
		--end;
// 	while(string1<end && IsWhiteChar(*string1))
// 		++string1;
	int len=end-string1;
	_bstr_t s(len);
	WCHAR* p = s;
	memcpy(p, string1, len*2);
	p[len]=0;
	return s;
}
//---------------------------------------------------------------------------------------------
_bstr_t LTrim(BSTR string1){
	if (!string1 || !*string1)
		return L"";
	int Len=wcslen(string1);
	BSTR end = string1+Len;
// 	while(end>string1 && IsWhiteChar(end[-1]))
// 		--end;
	while(string1<end && IsWhiteChar(*string1))
		++string1;
	int len=end-string1;
	_bstr_t s(len);
	WCHAR* p = s;
	memcpy(p, string1, len*2);
	p[len]=0;
	return s;
}
//---------------------------------------------------------------------------------------------
_bstr_t Trim(BSTR string1){
	if (!string1 || !*string1)
		return L"";
	int Len=wcslen(string1);
	BSTR end = string1+Len;
	while(end>string1 && IsWhiteChar(end[-1]))
		--end;
	while(string1<end && IsWhiteChar(*string1))
		++string1;
	int len=end-string1;
	_bstr_t s(len);
	WCHAR* p = s;
	memcpy(p, string1, len*2);
	p[len]=0;
	return s;
}
//---------------------------------------------------------------------------------------------
_bstr_t Left(const BSTR string1, int Length){
	_bstr_t s(Length);
	WCHAR* p = s;
	wcsncpy(p, string1, Length);
	p[Length]=0;
	return s;
}
//---------------------------------------------------------------------------------------------
_bstr_t Right(const BSTR string1, int Length){
	int len=wcslen(string1);
	if (Length>=len)
		Length=len;
	_bstr_t s(Length);
	WCHAR* p = s;
	wcsncpy(p, string1+len-Length, Length);
	p[Length]=0;
	return s;
}
//---------------------------------------------------------------------------------------------
_bstr_t Mid(const BSTR string1, int Start, int Length){
	_bstr_t s(Length);
	WCHAR* p = s;
	wcsncpy(p, string1+Start-1, Length);
	p[Length]=0;
	return s;
}
//---------------------------------------------------------------------------------------------
_bstr_t Mid(const BSTR string1, int Start){
	int Length = wcslen(string1) - (Start-1);
	return Mid(string1, Start, Length);
}
//---------------------------------------------------------------------------------------------
inline int Len(const BSTR string1){
	return wcslen(string1);
}
//---------------------------------------------------------------------------------------------
inline SYSTEMTIME Now(){
	SYSTEMTIME t;
	GetLocalTime(&t);
	return t;
}
//---------------------------------------------------------------------------------------------
_bstr_t CStr(const SYSTEMTIME& Date){
	_bstr_t s(19);
	WCHAR* p = s;
	swprintf(p, L"%02d/%02d/%04d %02d:%02d:%02d", Date.wDay, Date.wMonth, Date.wYear, Date.wHour, Date.wMinute, Date.wSecond);
	return s;
}
//---------------------------------------------------------------------------------------------
inline BOOL FileExistsW(WCHAR const* szFileName, BOOL& IsDirectory){
	DWORD dwFileAttributes=GetFileAttributesW(szFileName);
	BOOL bExist=(dwFileAttributes!=-1);
	if (bExist)
		IsDirectory=(dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY);
	return bExist;
}
//---------------------------------------------------------------------------------------------
inline IApplication& gApp(){
	return *(IApplication*)GetApp();
}
//---------------------------------------------------------------------------------------------
