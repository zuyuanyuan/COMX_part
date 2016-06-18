/*HEAD plugin HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_COMX_UI_PLUGIN_H_2006111014629
#define _KMAS_COMX_COMX_UI_PLUGIN_H_2006111014629

#include "base\root.hxx"
#include "wtypes.h"

#include <ui\plugin_callback.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TPluginMaskType
{
    PLUGIN_TOOLBAR   = 0x0001,
    PLUGIN_MENU      = 0x0002,
    PLUGIN_SEPERATOR = 0x0004,
};

enum TPluginFlagType
{
    PLUGIN_ENABLE = 0x0001,
    PLUGIN_CHECK  = 0x0002
};

#define PLUGIN_RADIO PLUGIN_CHECK

enum TPluginToolbarDockPosType
{
    PLUGIN_LEFT   = 0x0001,
    PLUGIN_RIGHT  = 0x0002,
    PLUGIN_TOP    = 0x0004,
    PLUGIN_BOTTOM = 0x0008
};

struct TCOMXPluginToolbarItem
{
     unsigned int            nID;
     char                    name[256];
     int                     nFlags;
     int                     mask;
     char                    tip[256];
     char                    help[256];
     char                    menu_path[1024];
     int                     menu_index;
};

struct TCOMXPluginToolbar
{
     TCOMXPluginToolbarItem     *items;
     int                        size;
     char                       name[256];
     char                       menu_path[1024];
     unsigned int               bmp_id;
     unsigned int               bmp_id_float;
     unsigned int               bmp_id_gray;
     TPluginToolbarDockPosType  dock_pos;
     bool                       is_newline;
     bool                       is_show;
};

// {65A6F695-55BB-4daa-B4BD-8B04B338E164}
 const MUID IID_IComxUiPlugin = 
{ 0x65a6f695, 0x55bb, 0x4daa, { 0xb4, 0xbd, 0x8b, 0x4, 0xb3, 0x38, 0xe1, 0x64 } };

interface IComxUiPlugin : public IRoot
{
     virtual TStatus STDCALL GetInstance(HINSTANCE& hInstance) = 0;
     virtual TStatus STDCALL GetToolbarCount(int& cnt) = 0;
     virtual TStatus STDCALL GetToolbar(const int& index, TCOMXPluginToolbar*& tb) = 0;
     virtual TStatus STDCALL DispatchCmd(const unsigned int& nID) = 0;
     virtual TStatus STDCALL GetMask(const unsigned int& nID, unsigned int& mask) = 0;
};

// {340624E8-B3E2-4063-AA9B-26354504A684}
const MUID IID_IComxUiPlugin2 = 
{ 0x340624e8, 0xb3e2, 0x4063, { 0xaa, 0x9b, 0x26, 0x35, 0x45, 0x4, 0xa6, 0x84 } };

interface IComxUiPlugin2 : public IComxUiPlugin
{
    virtual TStatus STDCALL ResetFlag(const int index) = 0;
};

// {DEB27811-E054-4982-ACF6-E3FE96B909E0}
const MUID IID_IComxUiPlugin3 = 
{ 0xdeb27811, 0xe054, 0x4982, { 0xac, 0xf6, 0xe3, 0xfe, 0x96, 0xb9, 0x9, 0xe0 } };

interface IComxUiPlugin3 : public IComxUiPlugin2
{
	virtual TStatus STDCALL GetToolbarItemBitmap(const int tb_index, const int item_index, char *bitmap) = 0;
};

// {07F84F4E-3EFB-4493-93FB-FC6735C71DF3}
 const MUID IID_IComxUiPluginServer = 
{ 0x7f84f4e, 0x3efb, 0x4493, { 0x93, 0xfb, 0xfc, 0x67, 0x35, 0xc7, 0x1d, 0xf3 } };

interface IComxUiPluginServer : public IRoot
{
     virtual TStatus STDCALL GenerateCmdID(unsigned int& nID) = 0;
     virtual TStatus STDCALL GetPluginCount(int& cnt) = 0;
     virtual TStatus STDCALL GetPlugin(const int& index, IComxUiPlugin*& plugin) = 0;
};

// {F449D3F7-CBEB-43ed-ABCB-A7EF9DC39DFF}
const MUID IID_IComxUiPluginServer2 = 
{ 0xf449d3f7, 0xcbeb, 0x43ed, { 0xab, 0xcb, 0xa7, 0xef, 0x9d, 0xc3, 0x9d, 0xff } };

interface IComxUiPluginServer2 : public IComxUiPluginServer
{
    virtual TStatus STDCALL ResetToolbar(const char* tb_name) = 0;
};

// {6DFD4BC8-50B7-4605-8D8C-26354A9407F0}
const MUID IID_IComxUiPluginButton = 
{ 0x6dfd4bc8, 0x50b7, 0x4605, { 0x8d, 0x8c, 0x26, 0x35, 0x4a, 0x94, 0x7, 0xf0 } };

interface IComxUiPluginButton : public IRoot
{
    virtual TStatus STDCALL ActiveButton(int index, int button) = 0;
    virtual TStatus STDCALL CloseButton(int index, int button) = 0;
};

// {54F5F099-995F-46a5-B26B-8149E989EA3B}
const MUID IID_IComxUiPluginServerCueline = 
{ 0x54f5f099, 0x995f, 0x46a5, { 0xb2, 0x6b, 0x81, 0x49, 0xe9, 0x89, 0xea, 0x3b } };

interface IComxUiPluginServerCueline : public IRoot
{
    virtual TStatus STDCALL GetCueline(char *&cueline) = 0;
    virtual TStatus STDCALL SetCueline(const char *cueline) = 0;
    virtual TStatus STDCALL ResetCueline() = 0;
};

// {124C491C-0166-4521-A316-9AABEF416D82}
const MUID IID_IComxUiPluginServerFrm = 
{ 0x124c491c, 0x166, 0x4521, { 0xa3, 0x16, 0x9a, 0xab, 0xef, 0x41, 0x6d, 0x82 } };

interface IComxUiPluginServerFrm : public IRoot
{
    virtual TStatus STDCALL Relayout() = 0;
    virtual TStatus STDCALL GetMainWnd(HWND &hWnd) = 0;
    virtual TStatus STDCALL GetPackageCLSID(char *clsid) = 0;
    virtual TStatus STDCALL GetClientWnd(HWND &hWnd) = 0;
};

// {56C24B2C-2C6C-45b1-8F0E-74F03997BAA3}
const MUID IID_IComxUiPluginServerFrm2 = 
{ 0x56c24b2c, 0x2c6c, 0x45b1, { 0x8f, 0xe, 0x74, 0xf0, 0x39, 0x97, 0xba, 0xa3 } };

interface IComxUiPluginServerFrm2 : public IComxUiPluginServerFrm
{
    virtual TStatus STDCALL GetDockWnd(HWND &hWnd) = 0;
};

// {72F6CA4B-DD59-4277-86A8-4CAFA4BEE204}
const MUID IID_IComxUiPluginServerEvent = 
{ 0x72f6ca4b, 0xdd59, 0x4277, { 0x86, 0xa8, 0x4c, 0xaf, 0xa4, 0xbe, 0xe2, 0x4 } };

interface IComxUiPluginServerEvent : public IRoot
{
    virtual TStatus STDCALL CloseAllPane() = 0;
    virtual TStatus STDCALL Initialize() = 0;
};

// {41857A48-D696-4baf-9741-232365F92F11}
const MUID IID_IComxUiPluginEvent = 
{ 0x41857a48, 0xd696, 0x4baf, { 0x97, 0x41, 0x23, 0x23, 0x65, 0xf9, 0x2f, 0x11 } };

interface IComxUiPluginEvent : public IRoot
{
    virtual TStatus STDCALL OnCloseAllPane() = 0;
    virtual TStatus STDCALL OnInitialize() = 0;
};

// {2E682135-A3E3-429e-ABAD-9D03996D3432}
const MUID IID_IComxUiPluginServerEvent2 = 
{ 0x2e682135, 0xa3e3, 0x429e, { 0xab, 0xad, 0x9d, 0x3, 0x99, 0x6d, 0x34, 0x32 } };

interface IComxUiPluginServerEvent2 : public IComxUiPluginServerEvent
{
    virtual TStatus STDCALL GetContentMenu(int *&ids, char titles[256][256], int &num) = 0;
};

// {C770FD71-55EB-4c47-ABE4-A829FF9CEA74}
const MUID IID_IComxUiPluginEvent2 = 
{ 0xc770fd71, 0x55eb, 0x4c47, { 0xab, 0xe4, 0xa8, 0x29, 0xff, 0x9c, 0xea, 0x74 } };

interface IComxUiPluginEvent2 : public IComxUiPluginEvent
{
    virtual TStatus STDCALL OnInitContentMenu(int *&ids, char titles[256][256], int &num) = 0;
};

// {9B289DE7-3B55-4165-86B0-A20B7E800524}
const MUID IID_IComxUiPluginServerViewer = 
{ 0x9b289de7, 0x3b55, 0x4165, { 0x86, 0xb0, 0xa2, 0xb, 0x7e, 0x80, 0x5, 0x24 } };

interface IComxUiPluginServerViewer : public IRoot
{
    virtual TStatus STDCALL SwitchViewer(int view_index) = 0;
};

// {C684BE41-5B91-41f5-9291-7731C740ED42}
const MUID IID_IComxUiPluginViewer = 
{ 0xc684be41, 0x5b91, 0x41f5, { 0x92, 0x91, 0x77, 0x31, 0xc7, 0x40, 0xed, 0x42 } };

interface IComxUiPluginViewer : public IRoot
{
    virtual TStatus STDCALL OnSwitchViewer(int view_index) = 0;
};

// {41618450-8C51-4b3e-B9D0-ACCD26D4A633}
const MUID IID_IComxUiPluginServer2DViewerModule = 
{ 0x41618450, 0x8c51, 0x4b3e, { 0xb9, 0xd0, 0xac, 0xcd, 0x26, 0xd4, 0xa6, 0x33 } };

interface IComxUiPluginServer2DViewerModule : public IRoot
{
    virtual TStatus STDCALL SetScale(const double scale) = 0;
    virtual TStatus STDCALL Clear() = 0;
    virtual TStatus STDCALL AddLine(const double x1, const double y1, const double x2, const double y2, unsigned long &id) = 0;
    virtual TStatus STDCALL AddLines(double *vertexes, const int num, unsigned long &id) = 0;
    virtual TStatus STDCALL Center() = 0;
    virtual TStatus STDCALL AddHelpInfo(const char *msg) = 0;
    virtual TStatus STDCALL ClearHelpInfo() = 0;
};

enum T2DViewerAutPointType
{
	VIEW2D_AUX_CENTER = 0,
	VIEW2D_AUX_LEFT_CONTACT = 1,
	VIEW2D_AUX_RIGHT_CONTACT = 2,
	VIEW2D_AUX_TOP_CONTACT = 3,
	VIEW2D_AUX_BOTTOM_CONTACT = 4,
	VIEW2D_AUX_LEFT_TOP = 5,
	VIEW2D_AUX_LEFT_BOTTOM = 6,
	VIEW2D_AUX_RIGHT_TOP = 7,
	VIEW2D_AUX_RIGHT_BOTTOM = 8
};

// {FE2D6A4B-ED6A-497a-9F46-70C3A60960E6}
const MUID IID_IComxUiPluginServer2DViewerModule2 = 
{ 0xfe2d6a4b, 0xed6a, 0x497a, { 0x9f, 0x46, 0x70, 0xc3, 0xa6, 0x9, 0x60, 0xe6 } };

interface IComxUiPluginServer2DViewerModule2 : IComxUiPluginServer2DViewerModule
{
	virtual TStatus STDCALL GetAuxPoint(unsigned long id_shape, int type, unsigned long &pid) = 0;
	virtual TStatus STDCALL MapAuxPoint(unsigned long pid, const double x0, const double y0, const double angle) = 0;
	virtual TStatus STDCALL GetBox(unsigned long pid, double &l, double &t, double &r, double &b) = 0;
	virtual TStatus STDCALL GetAuxPointPos(unsigned long pid, double &x, double &y) = 0;
	virtual TStatus STDCALL OffsetAuxPoint(unsigned long pid, const double delta_x, const double delta_y) = 0;
};

// {9DC82852-ABA8-4f8b-9373-DC27AD77F46B}
const MUID IID_IComxUiPluginServer2DViewerModule3 = 
{ 0x9dc82852, 0xaba8, 0x4f8b, { 0x93, 0x73, 0xdc, 0x27, 0xad, 0x77, 0xf4, 0x6b } };

interface IComxUiPluginServer2DViewerModule3 : IComxUiPluginServer2DViewerModule2
{
	virtual TStatus STDCALL AddRectangle(unsigned long sptid, unsigned long eptid, unsigned long &id) = 0;
};


// {1849BD61-AEE0-49d1-A903-144543D81BD3}
const MUID IID_IComxUiPluginServer2DViewerAuxModule = 
{ 0x1849bd61, 0xaee0, 0x49d1, { 0xa9, 0x3, 0x14, 0x45, 0x43, 0xd8, 0x1b, 0xd3 } };

interface IComxUiPluginServer2DViewerAuxModule : public IRoot
{
	virtual TStatus STDCALL AddDim(unsigned long sptid, unsigned long eptid, unsigned long extptid, int mode/*0-Horizontal, 1-Vertical*/, char *msg, unsigned long &id) = 0;
	virtual TStatus STDCALL AddMark(unsigned long ptid, unsigned long &id) = 0;
};

// {96A8C9F6-6295-4e3f-B235-ACA8B1AA2B4A}
const MUID IID_IComxUiPluginServer2DViewerShapeProp = 
{ 0x96a8c9f6, 0x6295, 0x4e3f, { 0xb2, 0x35, 0xac, 0xa8, 0xb1, 0xaa, 0x2b, 0x4a } };

interface IComxUiPluginServer2DViewerShapeProp : public IRoot
{
    virtual TStatus STDCALL AddMappingItem(unsigned long id, const double x0, const double y0, const double angle) = 0;
    virtual TStatus STDCALL EnableMapping(unsigned long id, bool flag) = 0;
    virtual TStatus STDCALL EnableOnlyDisplayMappedItems(unsigned long id, bool flag) = 0;
    virtual TStatus STDCALL EnableInteract(unsigned long id, bool flag) = 0;
    virtual TStatus STDCALL SetColor(unsigned long id, const double r, const double g, const double b) = 0;
    virtual TStatus STDCALL SetMappingColor(unsigned long id, const double r, const double g, const double b) = 0;
};

// {7310E861-5D3E-4537-A582-0D52DB683466}
const MUID IID_IComxUiPluginServer2DViewerShapeProp2 = 
{ 0x7310e861, 0x5d3e, 0x4537, { 0xa5, 0x82, 0xd, 0x52, 0xdb, 0x68, 0x34, 0x66 } };

interface IComxUiPluginServer2DViewerShapeProp2 : public IComxUiPluginServer2DViewerShapeProp
{
	virtual TStatus STDCALL SetMode(unsigned long id, int mode) = 0; // 0 - wire; 1 - fill.
	virtual TStatus STDCALL EnableSchematicMode(unsigned long id, bool flag) = 0; // just is used for rectangle shape current.
	virtual TStatus STDCALL GetDrawAreaAspectRatio(double &ratio) = 0;
};

// {BAF6333B-959A-432c-8409-31514D723E66}
const MUID IID_IComxUiPluginServerListviewSidebar = 
{ 0xbaf6333b, 0x959a, 0x432c, { 0x84, 0x9, 0x31, 0x51, 0x4d, 0x72, 0x3e, 0x66 } };

interface IComxUiPluginServerListviewSidebar : public IRoot
{
	virtual TStatus STDCALL ClearListView(const char *id) = 0;
	virtual TStatus STDCALL SetListViewItemCount(const char *id, const int num) = 0;
	virtual TStatus STDCALL SetListViewItem(const char *id, const int i, const int j, const char *val) = 0;
	virtual TStatus STDCALL ShowListView(const char *id, bool flag) = 0;
	virtual TStatus STDCALL RegisterListViewCB(const char *id, fn_comx_plugin_callback_t cb) = 0;
	virtual TStatus STDCALL UnregisterListViewCB(const char *id, fn_comx_plugin_callback_t cb) = 0;
    virtual TStatus STDCALL IsShownListView(const char *id, bool &flag) = 0;
};

// {4BC6C9BD-797C-4c4b-A9A5-4B2464B41687}
const MUID IID_IComxUiPluginPickupToolbar = 
{ 0x4bc6c9bd, 0x797c, 0x4c4b, { 0xa9, 0xa5, 0x4b, 0x24, 0x64, 0xb4, 0x16, 0x87 } };

interface IComxUiPluginPickupToolbar : public IRoot
{
	virtual TStatus STDCALL ActivatePickupToolbar() = 0;
	virtual TStatus STDCALL ClosurePickupToolbar() = 0;
};

// {8EC4AF0E-9C72-41f9-8D39-AA20BD145028}
const MUID IID_IComxUiPluginPickupToolbarEvent = 
{ 0x8ec4af0e, 0x9c72, 0x41f9, { 0x8d, 0x39, 0xaa, 0x20, 0xbd, 0x14, 0x50, 0x28 } };

interface IComxUiPluginPickupToolbarEvent : public IRoot
{
	virtual TStatus STDCALL OnCurrentRecordColorModified(double r, double g, double b) = 0;
	virtual TStatus STDCALL OnActivatePickupToolbar() = 0;
	virtual TStatus STDCALL OnClosurePickupToolbar() = 0;
};

// {DE060123-14FA-4407-B0BD-F473A9B7B333}
const MUID IID_IComxUiPluginPickupToolbarConnector = 
{ 0xde060123, 0x14fa, 0x4407, { 0xb0, 0xbd, 0xf4, 0x73, 0xa9, 0xb7, 0xb3, 0x33 } };

interface IComxUiPluginPickupToolbarConnector : public IRoot
{
	virtual TStatus STDCALL SetPickupType(const int type) = 0;
	virtual TStatus STDCALL GetPickupType(int &type) = 0;
	virtual TStatus STDCALL GetPickupTypeList(int *&types, int &len) = 0;
	virtual TStatus STDCALL SetPickupMethod(const int method) = 0;
	virtual TStatus STDCALL GetPickupMethod(int &method) = 0;
	virtual TStatus STDCALL SetPickupRange(const int range) = 0;
	virtual TStatus STDCALL GetPickupRangeList(int *&ranges, int &len) = 0;
	virtual TStatus STDCALL GetPickupRange(int &range) = 0;
	virtual TStatus STDCALL SetAutoPickupFlag(bool flag) = 0;
	virtual TStatus STDCALL GetAutoPickupFlag(bool &flag) = 0;
	virtual TStatus STDCALL ResetFilter() = 0;
	virtual TStatus STDCALL SelectAll() = 0;
	virtual TStatus STDCALL Deselect() = 0;
	virtual TStatus STDCALL InverseSelect() = 0;
	virtual TStatus STDCALL SetFilterColor(double r, double g, double b) = 0;
	virtual TStatus STDCALL GetFilterColor(double &r, double &g, double &b) = 0;
	virtual TStatus STDCALL GetCurrentRecordColor(double &r, double &g, double &b) = 0;
	virtual TStatus STDCALL EnableColorFilter(bool flag) = 0;
	virtual TStatus STDCALL IsEnableColorFilter(bool &flag) = 0;
};

// {BDD075BF-A6A6-424d-B8F5-0ECB73581623}
 const MTYPEID CLSID_IPlugServer = 
{ 0xbdd075bf, 0xa6a6, 0x424d, { 0xb8, 0xf5, 0xe, 0xcb, 0x73, 0x58, 0x16, 0x23 } }; 

#define BEGIN_TOOLBAR_GROUP() static TCOMXPluginToolbar S_tbs[] = \
{

#define END_TOOLBAR_GROUP() };\
    static int S_tbs_num = sizeof(S_tbs) / sizeof(TCOMXPluginToolbar);

#define BEGIN_TOOLBAR() {\
    NULL,\
    0,

#define END_TOOLBAR() },

#define TOOLBAR_BMP_ID(id) id,
#define TOOLBAR_BMP_FLOAT_ID(id) id,
#define TOOLBAR_BMP_GRAY_ID(id) id,
#define TOOLBAR_DOCK(type) type,
#define TOOLBAR_NEWLINE(flag) flag,
#define TOOLBAR_NAME(name) name,
#define TOOLBAR_SHOW(flag) flag
#define TOOLBAR_MENUPATH(path) path,

#define BEGIN_TOOLBAR_IMPL(gid) static TCOMXPluginToolbarItem items_tbs##gid[] = \
    {

#define END_TOOLBAR_IMPL(gid) };

#define BEGIN_GENERATE_INIT_TOOLBAR_FUN() void init_toolbar()\
{

#define INIT_TOOLBAR(gid) S_tbs[gid].items = items_tbs##gid;\
    S_tbs[gid].size = sizeof(items_tbs##gid) / sizeof(TCOMXPluginToolbarItem);

#define END_GENERATE_INIT_TOOLBAR_FUN() }

#define BEGIN_TOOLBAR_ITEM() {
#define END_TOOLBAR_ITEM() },

#define TOOLBAR_ITEM_ID(nid) nid,
#define TOOLBAR_ITEM_NAME(name) name,
#define TOOLBAR_ITEM_FLAG(flag) flag,
#define TOOLBAR_ITEM_MASK(mask) mask,
#define TOOLBAR_ITEM_TIP(tip) tip,
#define TOOLBAR_ITEM_HELP(help) help,
#define TOOLBAR_ITEM_MENU_PATH(path) path,
#define TOOLBAR_ITEM_MENU_INDEX(index) index,

//////////////////////////////////////////////////////////////////////////
// Define some tool macros

#define IMPL_GET_INSTANCE(hInst, ret) AFX_MANAGE_STATE(AfxGetStaticModuleState());\
    hInst = AfxGetInstanceHandle();\
    ret = true

#define IMPL_GET_TOOLBAR_COUNT(cnt, ret) AFX_MANAGE_STATE(AfxGetStaticModuleState());\
    cnt = S_tbs_num;\
    ret = true

#define IMPL_GET_TOOLBAR(index, tb, ret) AFX_MANAGE_STATE(AfxGetStaticModuleState());\
    assert(index >= 0 && index < S_tbs_num);\
    tb = &S_tbs[index];\
    ret = true

#define IMPL_DISPATCH_CMD(nID, dispatch_cmd, ret) int tb_index = -1;\
    int item_index = -1;\
    AFX_MANAGE_STATE(AfxGetStaticModuleState());\
    for (int loop = 0; loop < S_tbs_num; ++loop)\
    {\
        for (int loop_sub = 0; loop_sub < S_tbs[loop].size; ++loop_sub)\
        {\
            if(S_tbs[loop].items[loop_sub].nID == nID)\
            {\
                tb_index = loop;\
                item_index = loop_sub;\
\
                ret = true;\
                break;\
            }\
        }\
        if (ret == true)\
        {\
            break;\
        }\
    }\
\
    if (ret == true)\
    {\
        dispatch_cmd(tb_index, item_index);\
    }

#define IMPL_GET_MASK(nID, mask, ret) AFX_MANAGE_STATE(AfxGetStaticModuleState());\
    for (int loop = 0; loop < S_tbs_num; ++loop)\
    {\
        for (int loop_sub = 0; loop_sub < S_tbs[loop].size; ++loop_sub)\
        {\
            if(S_tbs[loop].items[loop_sub].nID == nID)\
            {\
                mask = S_tbs[loop].items[loop_sub].mask;\
                return true;\
            }\
        }\
    }

#define GET_INDEX_FROM_ID(nID, index) \
	{\
    for (int loop = 0; loop < S_tbs_num; ++loop)\
    {\
        for (int loop_sub = 0; loop_sub < S_tbs[loop].size; ++loop_sub)\
        {\
            if(S_tbs[loop].items[loop_sub].nID == nID)\
            {\
                index = loop;\
				break;\
            }\
        }\
    }\
	}

#define IMPL_RESET_FLAG(index, ret) AFX_MANAGE_STATE(AfxGetStaticModuleState());\
    for (int loop_sub = 0; loop_sub < S_tbs[index].size; ++loop_sub)\
    {\
        S_tbs[index].items[loop_sub].nFlags &= ~PLUGIN_CHECK;\
    }\
    ret = true;

#define GET_TOOLBAR_VARIATION() S_tbs
#define GET_TOOLBAR_NUMBER() S_tbs_num

inline void COMX_ActivateToolbarItem(const char *tb_name, const int index)
{
    TFactory factory;
    QUERY_SERVICE(factory, CLSID_IPlugServer, IComxUiPluginServer, p_ui_plugin_server);

    int plugin_cnt = 0;
    p_ui_plugin_server->GetPluginCount(plugin_cnt);

    for (int plugin_index = 0; plugin_index < plugin_cnt; ++plugin_index)
    {
        IComxUiPlugin *p_plugin = NULL;
        p_ui_plugin_server->GetPlugin(plugin_index, p_plugin);

        int tb_cnt = 0;
        p_plugin->GetToolbarCount(tb_cnt);

        bool is_finish = false;
        for (int tb_index = 0; tb_index < tb_cnt; ++tb_index)
        {
            TCOMXPluginToolbar *p_tb = NULL;
            p_plugin->GetToolbar(tb_index, p_tb);

            if (strcmp(p_tb->name , tb_name) == 0)
            {
                assert(index >= 0 && index < p_tb->size);

                unsigned int nID = p_tb->items[index].nID;
                p_plugin->DispatchCmd(nID);

                is_finish = true;
                break;
            }
        }

        p_plugin->Release();

        if (is_finish)
        {
            break;
        }
    }

    p_ui_plugin_server->Release();
}

inline void COMX_GetToolbarItem(const char *tb_name, const int index, TCOMXPluginToolbarItem *&item)
{
    TFactory factory;
    QUERY_SERVICE(factory, CLSID_IPlugServer, IComxUiPluginServer, p_ui_plugin_server);

    int plugin_cnt = 0;
    p_ui_plugin_server->GetPluginCount(plugin_cnt);

    for (int plugin_index = 0; plugin_index < plugin_cnt; ++plugin_index)
    {
        IComxUiPlugin *p_plugin = NULL;
        p_ui_plugin_server->GetPlugin(plugin_index, p_plugin);

        int tb_cnt = 0;
        p_plugin->GetToolbarCount(tb_cnt);

        bool is_finish = false;
        for (int tb_index = 0; tb_index < tb_cnt; ++tb_index)
        {
            TCOMXPluginToolbar *p_tb = NULL;
            p_plugin->GetToolbar(tb_index, p_tb);

            if (strcmp(p_tb->name , tb_name) == 0)
            {
                assert(index >= 0 && index < p_tb->size);

                item = &p_tb->items[index];                

                is_finish = true;
                break;
            }
        }

        p_plugin->Release();

        if (is_finish)
        {
            break;
        }
    }

    p_ui_plugin_server->Release();
}


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

