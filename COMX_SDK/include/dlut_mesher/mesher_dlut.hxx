/*HEAD mesher_dlut HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_MESHER_DLUT_H_200611755216
#define _KMAS_COMX_MESHER_DLUT_H_200611755216

#include "base\root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {32CD4FD1-CC1F-4e43-A9CD-ACB2EB466CEB}
const MUID IID_IMesherDlut = 
{ 0x32cd4fd1, 0xcc1f, 0x4e43, { 0xa9, 0xcd, 0xac, 0xb2, 0xeb, 0x46, 0x6c, 0xeb } };

interface IMesherDlut : public IRoot
{
     virtual TStatus STDCALL MeshStepFile(const char* fname, bool& ret) = 0;
     virtual TStatus STDCALL MeshIgesFile(const char* fname, bool& ret) = 0;
     virtual TStatus STDCALL SetMeshSize(const double size) = 0;
     virtual TStatus STDCALL GetLastError(char* err_msg) = 0;
     virtual TStatus STDCALL GetMeshSize(double& size) = 0;
};

// {56f04f51-87a5-4806-aa75-659b2fb1d179}
const MUID IID_IMesherDlut2 =
{0x56f04f51, 0x87a5, 0x4806, {0xaa, 0x75, 0x65, 0x9b, 0x2f, 0xb1, 0xd1, 0x79 } };

interface IMesherDlut2 : public IMesherDlut
{
    // virtual TStatus STDCALL MeshStepFiles(const char* fname, int len, bool& ret) = 0;
    virtual TStatus STDCALL StartIncrementMode() = 0;
    virtual TStatus STDCALL EndIncrementMode() = 0;
};

// {09B20B42-F7B1-4cf3-9467-485950E8DC32}
const MTYPEID CLSID_IMesherDlut = 
{ 0x9b20b42, 0xf7b1, 0x4cf3, { 0x94, 0x67, 0x48, 0x59, 0x50, 0xe8, 0xdc, 0x32 } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

