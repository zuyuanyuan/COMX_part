/*HEAD comx_comment HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_COMX_COMMENT_H_20091116113042
#define _KMAS_COMX_COMX_COMMENT_H_20091116113042

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {800E12B4-6016-47ea-80FC-2B521ED0D657}
 const MUID IID_IComxComment = 
{ 0x800e12b4, 0x6016, 0x47ea, { 0x80, 0xfc, 0x2b, 0x52, 0x1e, 0xd0, 0xd6, 0x57 } };

const unsigned int COMX_COMMENT_LIST_END = (unsigned int)(-1);

interface IComxComment : public IRoot
{
     virtual TStatus STDCALL GetFirstCompCategory(char *category, unsigned int &id) = 0;
	 virtual TStatus STDCALL GetNextCompCategory(char *category, unsigned int &id) = 0;
	 virtual TStatus STDCALL GetFirstCompOfCategory(char *comp_name, unsigned int &id) = 0;
	 virtual TStatus STDCALL GetNextCompOfCategory(char *comp_name, unsigned int &id) = 0;
     virtual TStatus STDCALL GetCompCLSID(const char* category, const char* comp_name, MUID& clsid, char* name) = 0;
     virtual TStatus STDCALL GetCompFirstIID(const char* category, const char* comp_name, MUID& iid, char* name, unsigned int &id) = 0;
	 virtual TStatus STDCALL GetCompNextIID(const char *category, const char* comp_name, MUID& iid, char *name, unsigned int &id) = 0;
     virtual TStatus STDCALL GetCompFirstEID(const char* category, const char* comp_name, MUID& eid, char* name, unsigned int &id) = 0;
	 virtual TStatus STDCALL GetCompNextEID(const char* category, const char* comp_name, MUID& eid, char* name, unsigned int &id) = 0;
     virtual TStatus STDCALL GetCompComment(const char* category, const char* comp_name, char* comment) = 0;
     virtual TStatus STDCALL SetCompComment(const char* category, const char* comp_name, const char* comment) = 0;
     virtual TStatus STDCALL SetInterfaceComment(const char* iid_name, const char* comment) = 0;
     virtual TStatus STDCALL GetInterfaceComment(const char* iid_name, char* comment) = 0;
};

// {D84A57A0-97E1-4c03-AD01-05630F5ADDBB}
 const MTYPEID CLSID_IComxComment = 
{ 0xd84a57a0, 0x97e1, 0x4c03, { 0xad, 0x1, 0x5, 0x63, 0xf, 0x5a, 0xdd, 0xbb } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

