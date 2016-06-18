/*==============================================================================

  Copyright 2006 Jilin Kingmesh Corp.
  All rights reserved
  
================================================================================ 
  File description:
	
  -- Please append file description informations here --
  A dialog about check and modify overlap condition .
================================================================================
  Date            Name                    Description of Change
                  Xinagkui Zhang          Create
  03/08/2009      Xiangkui Zhang          Abolish the IAcisRender implement, this 
                                          interface still can execute connect 
                                          function, but nothing will be done when
                                          its member functions were called.
$HISTORY$
================================================================================*/

#ifndef _KMAS_ACIS_RENDER_H
#define _KMAS_ACIS_RENDER_H

#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TAcisDisplayMode{
	ACIS_WIRE		=	0x0001,
	ACIS_LIGHT		=	0x0002,
	ACIS_FILL		=	0x0004,
	ACIS_HIDE_LINE	=	0x0008
};

// {B0068DD3-2F70-41ac-B9F4-835BD0F464F7}
const MUID IID_IAcisRender = 
{ 0xb0068dd3, 0x2f70, 0x41ac, { 0xb9, 0xf4, 0x83, 0x5b, 0xd0, 0xf4, 0x64, 0xf7 } };

interface IAcisRender : public IRoot{
     virtual TStatus STDCALL GetDisplayMode(int &mode) = 0;
     virtual TStatus STDCALL SetDisplayMode(int mode) = 0;
	 virtual TStatus STDCALL SetVisualMode(bool isVisual) = 0;
	 virtual TStatus STDCALL GetVisualMode(bool &isVisual) = 0;

     virtual TStatus STDCALL SetFaceColor(int red, int green, int blue, int alpha) = 0;
	 virtual TStatus STDCALL SetWireColor(int red, int green, int blue, int alpha) = 0;
	 virtual TStatus STDCALL SetNodeColor(int red, int green, int blue, int alpha) = 0;
	 virtual TStatus STDCALL SetBorderColor(int red, int green, int blue, int alpha) = 0;
	 virtual TStatus STDCALL SetHighColor(int red, int green, int blue, int alpha) = 0; 
};

// {BE3E48EB-39B4-4871-B652-E65D400A99EE}
const MUID IID_IAcisRenderOption = 
{ 0xbe3e48eb, 0x39b4, 0x4871, { 0xb6, 0x52, 0xe6, 0x5d, 0x40, 0xa, 0x99, 0xee } };

interface IAcisRenderOption : public IRoot
{
	virtual TStatus STDCALL EnableModifyNotifyProgressbar(bool flag) = 0;
};

// {B1D9F00F-51AF-4a7a-8C80-E8CB77089905}
const MUID CLSID_IAcisRender = 
{ 0xc54e250b, 0xd29b, 0x4dd0, { 0xbd, 0x9e, 0x2e, 0x7a, 0x52, 0xa3, 0x82, 0x87 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif