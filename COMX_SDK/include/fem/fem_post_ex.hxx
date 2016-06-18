/*HEAD fem_post_ex HXX COMX_COMPONENT_MODULE */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

================================================================================
Date			          Name					 Description of Change
2007/06/05			YanKK					create post_ex
2007/06/25			YanKK					modify import pst file func()

$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_FEM_POST_EX_H_20076512234
#define _KMAS_COMX_FEM_POST_EX_H_20076512234

#include <fem/fem_core.hxx>
#include <fem/fem_render.hxx>
#include <base/root.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {57F45606-61D6-4eb9-A9E8-1AD780198F8D}
 const MUID IID_IFemPostEx = 
{ 0x57f45606, 0x61d6, 0x4eb9, { 0xa9, 0xe8, 0x1a, 0xd7, 0x80, 0x19, 0x8f, 0x8d } };

interface IFemPostEx : public IRoot
{
     virtual TStatus STDCALL ActivateFrame(const int findex) = 0;
     virtual TStatus STDCALL GetActivateFrame(int& findex) = 0;
     virtual TStatus STDCALL GetFrameNum(int& fnum) = 0;
     virtual TStatus STDCALL SetDisplayMode(const int mode) = 0;
	 virtual TStatus STDCALL SetVisualMode(const int mode) = 0;
     virtual TStatus STDCALL Clear() = 0;
	 virtual TStatus STDCALL Activate(const char * pspVal , const TFemRenderColorType &type) = 0;
	 virtual TStatus STDCALL GetActivateTitle( char * pspVal ) = 0;

	 virtual TStatus STDCALL IsDisplayCurFrame( const bool is_display) = 0;
	 virtual TStatus STDCALL IsConnectFemAppreance( const bool is_ceonnect ) = 0;
 	 virtual TStatus STDCALL Is_Springback_Post(bool &is_springback) = 0;

	 virtual TStatus STDCALL ImportFrameMesh(double *nodes/*I*/, const int node_num/**/,
		int *elements/*I*/, const int elem_num/*I*/) = 0;
	 virtual TStatus STDCALL ImportFrameMesh(IFemCore *& p_femcore) = 0;
	 virtual TStatus STDCALL ImportAfterSBMesh(double *nodes/*I*/, const int node_num/**/,
		int *elements/*I*/, const int elem_num/*I*/) = 0;
//	 virtual TStatus STDCALL ClearSBMesh() = 0;

	 virtual TStatus STDCALL SetAfterSBVisualMode(const int mode) = 0;
	 virtual TStatus STDCALL SetAfterSBDisplayMode(const int mode) = 0;

	 virtual TStatus STDCALL ImportPsyVal(const char *title, const char *unit, 
          const TFemRenderColorType &type, double *vals, const int &clen, const int findex) = 0;

	  virtual TStatus STDCALL IsHavePostContent(bool &is_have) = 0;

	 virtual TStatus STDCALL ActivateSelectSinglEleByPoint( ) = 0;
	 virtual TStatus STDCALL GetEdgePickupEleId(  int * &eleIds , int &len ) = 0;
	 virtual TStatus STDCALL GetMaxEdgeFormEle(const double r_value , int &eleId , double &e1 ) = 0;//使用前需先调用GetEdgePickupEleId()
	 virtual TStatus STDCALL SetIsDisplayEdgeSelEle(bool is_display_egdele) = 0;

	 virtual TStatus STDCALL ActivateSelectSingleNodeByPoint( ) = 0;
	 virtual TStatus STDCALL GetPickupNode(  int & node_id , fem::TPoint & pt  ) = 0;
	 virtual TStatus STDCALL AddBaseQcl( const int group , fem::TPoint *pt , const int pt_num , bool is_actived_qcl ) = 0;//添加一条qcl线，显示
	 virtual TStatus STDCALL AddMutiQcls( const int  dir , const int num , const double distence ) = 0;
	 virtual TStatus STDCALL ClearAllQcl() = 0;
	 virtual TStatus STDCALL SetIsDisplayQcl(bool is_display_QCL , bool is_dis_actived_qcl , const int ctrl_who) = 0;//ctrl_who,0,1;
	 
	 virtual TStatus STDCALL ActivateSelectMutiEleByRect( ) = 0;
     virtual TStatus STDCALL ClosePickupDlg() = 0;
	 virtual TStatus STDCALL GetPickupEleId(  int * &eleIds , int &len ) = 0; 	 
	 virtual TStatus STDCALL SetIsDisplayRangeSelEle(bool is_display_egdele) = 0;
	 virtual TStatus STDCALL GetMaxFormEle(const double r_value , int &eleId , double &e1 , double &e2 ) = 0;//使用前需先调用GetPickupEleId()
	 virtual TStatus STDCALL GetMaxFormEle( int &eleId , double &equ_strain ) = 0;
	 virtual TStatus STDCALL GetMinFormEle(const double r_value , int &eleId , double &e1 , double &e2 ) = 0;
	 virtual TStatus STDCALL GetMinFormEle(int &eleId , double &equ_strain  ) = 0;

	 virtual TStatus STDCALL GetCurFemcore(IFemCore *& p_cur_femcore) = 0;
	 virtual TStatus STDCALL GetPerFrameFemcore(IFemCore *& p_cur_femcore , const int frame_sn) = 0;
	 //得到frame_sn帧的femcore数据, 0 <= frame_sn <=  (总帧数 - 1)
	 virtual TStatus STDCALL GetPerFrameSinglePsyVal( const int frame_sn , const char* title , const TFemRenderColorType &type, const int ele_or_node_id , double  &para_val  ) = 0;
	 /*title == "Thickness 厚度" , "Strain 等效应变" , "Stress 等效应力" , "Major Strain 主应变" ,"Minor Strain 次应变" */

};

// {66836110-1CB1-46f9-967C-8F51D4F78EE9}
 const MUID IID_IFemPostExSerialize = 
{ 0x66836110, 0x1cb1, 0x46f9, { 0x96, 0x7c, 0x8f, 0x51, 0xd4, 0xf7, 0x8e, 0xe9 } };

 interface IFemPostExSerialize : public IRoot
 {
	 virtual TStatus STDCALL ImportResult(const char *fnam) = 0;
 };

// {40EE5F76-7957-405b-BDAC-7FF9FBB8E1EA}
 const MTYPEID CLSID_IFemPostEx = 
{ 0x40ee5f76, 0x7957, 0x405b, { 0xbd, 0xac, 0x7f, 0xf9, 0xfb, 0xb8, 0xe1, 0xea } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

