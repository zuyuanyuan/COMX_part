/*HEAD xml_stream.hxx COMX_COMPONENT_MODULE */
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

#ifndef _KMAS_COMX_XML_STREAM_HXX
#define _KMAS_COMX_XML_STREAM_HXX

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
// {50A63E95-B74D-47a2-9752-0907002A42C2}
const MUID IID_IXmlStream = 
{ 0x50a63e95, 0xb74d, 0x47a2, { 0x97, 0x52, 0x9, 0x7, 0x0, 0x2a, 0x42, 0xc2 } };

interface IXmlStream : public IRoot
{
	virtual TStatus STDCALL SetDoc(const char* szDoc, bool &isbool) = 0;
	virtual TStatus STDCALL Load( const char* szFileName, bool &isbool ) = 0;	
	virtual TStatus STDCALL IsWellFormed() = 0;
	virtual TStatus STDCALL Save( const char* szFileName, bool &isbool ) = 0;
	virtual TStatus STDCALL GetDoc(char* ch) = 0;
	virtual TStatus STDCALL GetDocFlags(int& temp) = 0;
	virtual TStatus STDCALL SetDocFlags(int nFlags) = 0; 
	virtual TStatus STDCALL GetError(char* ch) = 0;
	virtual TStatus STDCALL RestorePos( const char* szPosName, bool &isbool ) = 0;
	virtual TStatus STDCALL WriteTextFile(const char* szFileName, std::string& strDoc, std::string* pstrError=NULL, int* pnFlags=NULL ) = 0;
	virtual TStatus STDCALL Clear() = 0;
};

// {C93585A6-3015-402c-9F7D-3F6AA2835A28}
const MUID IID_IXmlStreamNavigate = 
{ 0xc93585a6, 0x3015, 0x402c, { 0x9f, 0x7d, 0x3f, 0x6a, 0xa2, 0x83, 0x5a, 0x28 } };

interface IXmlStreamNavigate : public IRoot
{
	virtual TStatus STDCALL FindElem( const char* szName, bool &isbool ) = 0;
	virtual TStatus STDCALL FindChildElem( const char* szName, bool &isbool ) = 0;
	virtual TStatus STDCALL IntoElem(bool &isbool) = 0;
	virtual TStatus STDCALL OutOfElem(bool &isbool) = 0;
	virtual TStatus STDCALL GetTagName(char* ch)  = 0;
	virtual TStatus STDCALL GetAttribName( int n, char* ch )  = 0;
	virtual TStatus STDCALL FindNode( int nType, int& temp ) = 0;
	virtual TStatus STDCALL SavePos( const char* szPosName, bool &isbool ) = 0;
	virtual TStatus STDCALL ResetChildPos() = 0;
	virtual TStatus STDCALL ResetMainPos() = 0;
	virtual TStatus STDCALL ResetPos() = 0;
	virtual TStatus STDCALL GetChildTagName(char* ch)  = 0;
	virtual TStatus STDCALL GetData(char* ch)  = 0;
	virtual TStatus STDCALL GetChildData(char* ch) = 0;
	virtual TStatus STDCALL GetElemContent(char* ch)  = 0;
	virtual TStatus STDCALL GetAttrib(const char* szAttrib, char* ch)  = 0;
	virtual TStatus STDCALL GetChildAttrib( const char* szAttrib, char* ch )  = 0;
	virtual TStatus STDCALL GetNodeType(int& temp) = 0;
};

// {9E4C524F-B0C4-4c78-8222-1BC7A50EB544}
const MUID IID_IXmlStreamCreate = 
{ 0x9e4c524f, 0xb0c4, 0x4c78, { 0x82, 0x22, 0x1b, 0xc7, 0xa5, 0xe, 0xb5, 0x44 } };

interface IXmlStreamCreate : public IRoot
{
	virtual TStatus STDCALL AddElem( const char* szName, bool &isbool, const char* szData=NULL, int nFlags=0 ) = 0 ;
	virtual TStatus STDCALL AddElem( const char* szName, bool &isbool, int nValue, int nFlags=0 ) = 0;
	virtual TStatus STDCALL InsertElem( const char* szName, bool &isbool, const char* szData=NULL, int nFlags=0 ) = 0;
	virtual TStatus STDCALL InsertElem( const char* szName, bool &isbool, int nValue, int nFlags=0 ) = 0;
	virtual TStatus STDCALL AddChildElem( const char* szName, bool &isbool, const char* szData=NULL, int nFlags=0 ) = 0;
	virtual TStatus STDCALL AddChildElem( const char* szName, bool &isbool, int nValue, int nFlags=0 ) = 0;
	virtual TStatus STDCALL InsertChildElem( const char* szName, bool &isbool, const char* szData=NULL, int nFlags=0 ) = 0;
	virtual TStatus STDCALL InsertChildElem( const char* szName, bool &isbool, int nValue, int nFlags=0 ) = 0;
	virtual TStatus STDCALL AddAttrib( const char* szAttrib, const char* szValue, bool &isbool ) = 0;
	virtual TStatus STDCALL AddAttrib( const char* szAttrib, int nValue, bool &isbool ) = 0;
	virtual TStatus STDCALL AddChildAttrib( const char* szAttrib, const char* szValue, bool &isbool ) = 0;
	virtual TStatus STDCALL AddChildAttrib( const char* szAttrib, int nValue, bool &isbool ) = 0;
	virtual TStatus STDCALL AddSubDoc( const char* szSubDoc, bool &isbool ) = 0;
	virtual TStatus STDCALL InsertSubDoc( const char* szSubDoc, bool &isbool ) = 0;
	virtual TStatus STDCALL GetSubDoc(char* ch) = 0;
	virtual TStatus STDCALL AddChildSubDoc( const char* szSubDoc, bool &isbool ) = 0;
	virtual TStatus STDCALL InsertChildSubDoc( const char* szSubDoc, bool &isbool ) = 0;
	virtual TStatus STDCALL GetChildSubDoc(char* ch) = 0;
	virtual TStatus STDCALL AddNode( int nType, const char* szText, bool &isbool ) = 0;
	virtual TStatus STDCALL InsertNode( int nType, const char* szText, bool &isbool ) = 0;
};

// {DF045339-D3EC-407e-B5F1-424B98B72585}
const MUID IID_IXmlStreamModify = 
{ 0xdf045339, 0xd3ec, 0x407e, { 0xb5, 0xf1, 0x42, 0x4b, 0x98, 0xb7, 0x25, 0x85 } };

interface IXmlStreamModify : public IRoot
{
	virtual TStatus STDCALL RemoveElem(bool &isbool) = 0;
	virtual TStatus STDCALL RemoveChildElem(bool &isbool) = 0;
	virtual TStatus STDCALL RemoveNode(bool &isbool) = 0;
	virtual TStatus STDCALL SetAttrib( const char* szAttrib, const char* szValue, bool &isbool ) = 0;
	virtual TStatus STDCALL SetAttrib( const char* szAttrib, int nValue, bool &isbool ) = 0;
	virtual TStatus STDCALL SetChildAttrib( const char* szAttrib, const char* szValue, bool &isbool ) = 0;
	virtual TStatus STDCALL SetChildAttrib( const char* szAttrib, int nValue, bool &isbool ) = 0;
	virtual TStatus STDCALL SetData( const char* szData, bool &isbool, int nFlags=0 ) = 0;
	virtual TStatus STDCALL SetChildData( int nValue, bool &isbool ) = 0;
	virtual TStatus STDCALL SetChildData( const char* szData,bool &isbool, int nFlags=0 )=0;
	virtual TStatus STDCALL SetElemContent( const char* szContent, bool &isbool ) = 0;
};

// {FF538A1E-8776-48fc-ABA2-118AEACFE793}
const MUID CLSID_IXmlStream = 
{ 0xff538a1e, 0x8776, 0x48fc, { 0xab, 0xa2, 0x11, 0x8a, 0xea, 0xcf, 0xe7, 0x93 } };
} // namespace comx
} // namespace Die_maker
} // namespace KMAS




#endif