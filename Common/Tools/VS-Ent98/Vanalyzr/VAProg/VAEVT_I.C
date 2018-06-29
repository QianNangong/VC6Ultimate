/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.03.0110 */
/* at Mon May 25 22:01:08 1998
 */
/* Compiler settings for vaevt.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ISystemDebugEventFire = {0x6c736dC1,0xAB0D,0x11d0,{0xA2,0xAD,0x00,0xA0,0xC9,0x0F,0x27,0xE8}};


const IID IID_ISystemDebugEventFireAuto = {0x6c736dee,0xAB0e,0x11d0,{0xA2,0xAD,0x00,0xA0,0xC9,0x0F,0x27,0xE8}};


const IID IID_ISystemDebugEventInstall = {0x6c736dC0,0xAB0D,0x11d0,{0xA2,0xAD,0x00,0xA0,0xC9,0x0F,0x27,0xE8}};


const IID IID_ISystemDebugEventInstallAuto = {0x6c736ded,0xAB0D,0x11d0,{0xA2,0xAD,0x00,0xA0,0xC9,0x0F,0x27,0xE8}};


#ifdef __cplusplus
}
#endif

