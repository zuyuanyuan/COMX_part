/*HEAD comx_storage HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date            Name            Description of Change

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_COMX_STORAGE_H_200771034239
#define _KMAS_COMX_COMX_STORAGE_H_200771034239

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TComxStorageItemType
{
     COMX_STORAGE_FILE		=		0,
     COMX_STORAGE_DIRECTORY	=		1,
};

struct TComxStorageItem
{
     TComxStorageItemType type;
     bool is_empty;
     char name[256];
     char vpath[1024];
};

const unsigned long INVALID_COMX_STORAGE_ID = (unsigned long)(-1);

// {67C2294E-1ECD-4d6b-98E2-43E4F57B376B}
 const MUID IID_IComxStorage = 
{ 0x67c2294e, 0x1ecd, 0x4d6b, { 0x98, 0xe2, 0x43, 0xe4, 0xf5, 0x7b, 0x37, 0x6b } };

interface IComxStorage : public IRoot
{
     virtual TStatus STDCALL SetRoot(const char* path) = 0;
     virtual TStatus STDCALL SetHome(const char* path) = 0;
     virtual TStatus STDCALL SetTmp(const char* path) = 0;
     virtual TStatus STDCALL GetPhysicsPath(const char* vpath, char* path) = 0;
     virtual TStatus STDCALL CreateDirectory(const char* vpath) = 0;
     virtual TStatus STDCALL Copy(const char* target_vpath) = 0;
     virtual TStatus STDCALL Move(const char* target_vpath) = 0;
     virtual TStatus STDCALL Delete() = 0;
};

// {C6D6746B-1839-4c22-A76D-E62416093B6C}
 const MUID IID_IComxStorageNameBuffer = 
{ 0xc6d6746b, 0x1839, 0x4c22, { 0xa7, 0x6d, 0xe6, 0x24, 0x16, 0x9, 0x3b, 0x6c } };

interface IComxStorageNameBuffer : public IRoot
{
     virtual TStatus STDCALL PickUp(const char* vpath) = 0;
     virtual TStatus STDCALL GetFirstItem(TComxStorageItem& item, unsigned long& id) = 0;
     virtual TStatus STDCALL GetNextItem(const unsigned long cid, TComxStorageItem& item, unsigned long &nid) = 0;
};

// {9370B9E1-53E6-4fb6-AD52-204C0BC7404C}
const MUID IID_IComxStorageNameBuffer2 = 
{ 0x9370b9e1, 0x53e6, 0x4fb6, { 0xad, 0x52, 0x20, 0x4c, 0xb, 0xc7, 0x40, 0x4c } };

interface IComxStorageNameBuffer2 : IComxStorageNameBuffer
{
	 virtual TStatus STDCALL AppendExcludeKey(const char *key) = 0;
	 virtual TStatus STDCALL ClearExcludeKey() = 0;
	 virtual TStatus STDCALL EnablePickupSubDirectory(bool flag) = 0;
};

// {18091018-C8C5-44f4-B593-242C8FFAB313}
 const MTYPEID CLSID_IComxStorage = 
{ 0x18091018, 0xc8c5, 0x44f4, { 0xb5, 0x93, 0x24, 0x2c, 0x8f, 0xfa, 0xb3, 0x13 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

