/*HEAD ui_progress_bar HXX ONESTEP */


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
#ifndef _KMAS_COMX_UI_PROGRESS_BAR_H
#define _KMAS_COMX_UI_PROGRESS_BAR_H

#include "base/root.hxx"

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {A4F842A6-459E-4b73-A235-B6AB1F5E52B7}
const MUID IID_IUiProgressBar = 
{ 0xa4f842a6, 0x459e, 0x4b73, { 0xa2, 0x35, 0xb6, 0xab, 0x1f, 0x5e, 0x52, 0xb7 } };

interface IUiProgressBar : public IRoot
{
    virtual TStatus STDCALL OnProgress(const int &step , const char *msg) = 0;
    virtual TStatus STDCALL OnInitialize() = 0;
    virtual TStatus STDCALL OnExitialize() = 0;
};

// {A23CB457-CEFF-42d5-BE9D-A63688D1AE8C}
const MUID IID_IUiProgressBar2 = 
{ 0xa23cb457, 0xceff, 0x42d5, { 0xbe, 0x9d, 0xa6, 0x36, 0x88, 0xd1, 0xae, 0x8c } };

interface IUiProgressBar2 : public IUiProgressBar
{
	virtual TStatus STDCALL OnInitializeEx(const bool is_ind = false) = 0;
};

// {8ED2E68B-B4DA-4681-BB15-DF7905101CEF}
const MUID IID_IUiProgressBarDriver = 
{ 0x8ed2e68b, 0xb4da, 0x4681, { 0xbb, 0x15, 0xdf, 0x79, 0x5, 0x10, 0x1c, 0xef } };

interface IUiProgressBarDriver : public IRoot
{
     virtual TStatus STDCALL UpdateUI(const int &step, const char *msg) = 0;
     virtual TStatus STDCALL Startup(bool is_ind = false) = 0;
     virtual TStatus STDCALL Closure() = 0;
};

// {24F21EBA-5CD0-43db-BACF-F2E2D934EA8F}
const MUID CLSID_IUiProgressBar = 
{ 0x24f21eba, 0x5cd0, 0x43db, { 0xba, 0xcf, 0xf2, 0xe2, 0xd9, 0x34, 0xea, 0x8f } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif