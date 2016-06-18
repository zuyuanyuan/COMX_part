/*HEAD fem_core HXX ONESTEP */
/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --
A dialog about check and modify overlap condition .
================================================================================
Date            Name                    Description of Change
                Zhang Xiang-kui 	      Create

$HISTORY$
================================================================================
*/
#ifndef _KMAS_FEM_CORE_H
#define _KMAS_FEM_CORE_H

#include "fem/fem_base.hxx"
#include "base/root.hxx"

#include "fem/fem_core_compatible.hxx"

//using namespace fem;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {97221907-5B52-4cc2-ACED-73CA8AEDAFD5}
const MUID IID_IFemCore2 = 
{ 0x97221907, 0x5b52, 0x4cc2, { 0xac, 0xed, 0x73, 0xca, 0x8a, 0xed, 0xaf, 0xd5 } };

interface IFemCore2: public IRoot
{
     virtual TStatus STDCALL InsertNode(const int &pos, const fem::TPoint &pt) = 0;
     virtual TStatus STDCALL InsertNode(
		 const int &pos, 
		 const fem::TPoint *&pts, 
		 const int &clen) = 0;
     virtual TStatus STDCALL InsertElement(
		 const int &pos, 
		 const int &n1, 
		 const int &n2, 
		 const int &n3, 
		 const int &n4 = -1) = 0;
     virtual TStatus STDCALL InsertElement(
		 const int &pos, 
		 const int *elements, 
		 const int &clen) = 0;
     virtual TStatus STDCALL GetNodeCount(int &count) = 0;
     virtual TStatus STDCALL GetNode(const int &pos, fem::TPoint &pt) = 0;
     virtual TStatus STDCALL GetNode(
		 const int &spos, 
		 const int &epos, 
		 fem::TPoint *points) = 0;
     virtual TStatus STDCALL GetElementCount(int &count) = 0;
     virtual TStatus STDCALL GetElement(
		 const int &pos, 
		 int &n1, 
		 int &n2, 
		 int &n3, 
		 int &n4) = 0;
     virtual TStatus STDCALL GetElement(
		 const int &spos, 
		 const int &epos, 
		 int *elements) = 0;
     virtual TStatus STDCALL DeleteNode(const int &pos) = 0;
     virtual TStatus STDCALL DeleteNode(const int &spos, const int &epos) = 0;
     virtual TStatus STDCALL DeleteNode(int *node_ids, const int &clen) = 0;
     virtual TStatus STDCALL DeleteElement(const int &pos) = 0;
     virtual TStatus STDCALL DeleteElement(const int &spos, const int &epos) = 0;
     virtual TStatus STDCALL DeleteElement(int *element_ids, const int &clen) = 0;
     virtual TStatus STDCALL UpdateNode(const int &pos, const fem::TPoint &pt) = 0;
     virtual TStatus STDCALL InverseElement(const int &pos) = 0;
     virtual TStatus STDCALL GetSideCount(int &count) = 0;
     virtual TStatus STDCALL GetSide(
		 const int &pos, 
		 int &start_node_id, 
		 int &end_node_id) = 0;
     virtual TStatus STDCALL GetSide(
		 const int &spos, 
		 const int &epos, 
		 int *side_node_ids) = 0;
     virtual TStatus STDCALL Clean(bool is_force = false) = 0;
     virtual TStatus STDCALL Clear() = 0;
     virtual TStatus STDCALL IsInitialize(bool &is_init) = 0;
};


// {64E9A4CA-C094-444a-86C6-15AF09D69B36}
const MUID IID_IFemCore3 = 
{ 0x64e9a4ca, 0xc094, 0x444a, { 0x86, 0xc6, 0x15, 0xaf, 0x9, 0xd6, 0x9b, 0x36 } };

interface IFemCore3 : public IFemCore2
{
	virtual TStatus STDCALL SetValidFlag(bool flag) = 0;
};

// {6166C581-328B-4b86-9B6D-17E04855E275}
const MUID IID_IFemCoreCache = 
{ 0x6166c581, 0x328b, 0x4b86, { 0x9b, 0x6d, 0x17, 0xe0, 0x48, 0x55, 0xe2, 0x75 } };

interface IFemCoreCache : public IRoot
{
     virtual TStatus STDCALL GetCacheInfo(fem::TFemCoreCacheBase *&cache_info) = 0;
     virtual TStatus STDCALL RefreshCache() = 0;
};

// {C47FC880-471E-476e-BA82-D3B3E44F10BA}
const MUID IID_IFemCoreModifiedNotify = 
{ 0xc47fc880, 0x471e, 0x476e, { 0xba, 0x82, 0xd3, 0xb3, 0xe4, 0x4f, 0x10, 0xba } };

interface IFemCoreModifiedNotify : public IRoot
{
     virtual TStatus STDCALL Notify() = 0;
};

// {5E834976-C505-4e35-AFC0-13A7BC324BBC}
const MUID IID_IFemCoreGapUtil = 
{ 0x5e834976, 0xc505, 0x4e35, { 0xaf, 0xc0, 0x13, 0xa7, 0xbc, 0x32, 0x4b, 0xbc } };

interface IFemCoreGapUtil : public IRoot
{
     virtual TStatus STDCALL MergeNode(const int &master, const int &aux) = 0;
     virtual TStatus STDCALL AutoMergeNodes(const double &precision = 0.1) = 0;   
};

// {A0A941FC-E503-4440-9783-9E5929935ED8}
const MUID IID_IFemCoreBaseUtil = 
{ 0xa0a941fc, 0xe503, 0x4440, { 0x97, 0x83, 0x9e, 0x59, 0x29, 0x93, 0x5e, 0xd8 } };

interface IFemCoreBaseUtil : public IRoot
{
     virtual TStatus STDCALL GetMinSideLength(double &min_side_length) = 0;
     virtual TStatus STDCALL ClearFreeNodes() = 0;
};

// {508DFB17-1AC4-4bbf-B9BB-1C9468A30251}
const MUID IID_IFemCoreBaseUtil2 = 
{ 0x508dfb17, 0x1ac4, 0x4bbf, { 0xb9, 0xbb, 0x1c, 0x94, 0x68, 0xa3, 0x2, 0x51 } };

interface IFemCoreBaseUtil2 : public IFemCoreBaseUtil
{
	virtual TStatus STDCALL MinDistanceNode2mesh(
		const double pt[], 
		int & number,
		double & dmin) =0;
};

// {2DDB32E0-2EF2-449a-80A7-B0770AFF7305}
const MUID IID_IFemCoreBaseUtil3 = 
{ 0x2ddb32e0, 0x2ef2, 0x449a, { 0x80, 0xa7, 0xb0, 0x77, 0xa, 0xff, 0x73, 0x5 } };

interface IFemCoreBaseUtil3 : public IFemCoreBaseUtil2
{
	virtual TStatus STDCALL GetArea(double &area) = 0;
	virtual TStatus STDCALL GetAverageNormal(double &x, double &y, double &z) = 0;
};

// {AC8F07D0-8474-4535-B39E-DAE4EC65517F}
 const MUID IID_IFemCoreBaseUtil4 = 
{ 0xac8f07d0, 0x8474, 0x4535, { 0xb3, 0x9e, 0xda, 0xe4, 0xec, 0x65, 0x51, 0x7f } };

interface IFemCoreBaseUtil4 : public IFemCoreBaseUtil3
{
	virtual TStatus STDCALL GetMaxSideLength(double &max_side_length) = 0;	
};

// {973895CC-6DAE-4ab1-874B-B8D7E31BE4E0}
const MUID IID_IFemCoreSeedTravel = 
{ 0x973895cc, 0x6dae, 0x4ab1, { 0x87, 0x4b, 0xb8, 0xd7, 0xe3, 0x1b, 0xe4, 0xe0 } };

interface IFemCoreSeedTravel : public IRoot
{
     virtual TStatus STDCALL SetRange(int range[], const int &clen) = 0;
     virtual TStatus STDCALL SetSeeds(int seeds[], const int &clen) = 0;
     virtual TStatus STDCALL AppendExceptElement(const int &eid) = 0;
     virtual TStatus STDCALL ResetSeedsTravel() = 0;
     virtual TStatus STDCALL SeedsTravel() = 0;
     virtual TStatus STDCALL SeedsTravelExt() = 0;
};


// {88B8E8C5-A003-460d-BEED-3D1D3CC851EE}
const MUID IID_IFemCoreSeedTravel2 = 
{ 0x88b8e8c5, 0xa003, 0x460d, { 0xbe, 0xed, 0x3d, 0x1d, 0x3c, 0xc8, 0x51, 0xee } };

interface IFemCoreSeedTravel2 : public IFemCoreSeedTravel
{
	virtual TStatus STDCALL SeedsTravelExt2() = 0;
};

// {8670AB4A-D7D8-4820-BDB6-F57B94FE98CD}
const MUID IID_IFemCoreSeedTravelClient = 
{ 0x8670ab4a, 0xd7d8, 0x4820, { 0xbd, 0xb6, 0xf5, 0x7b, 0x94, 0xfe, 0x98, 0xcd } };

interface IFemCoreSeedTravelClient : public IRoot
{
     virtual TStatus STDCALL SeedTravelTransform(
		 const int &src_eid, 
		 const int &tar_eid, 
		 const int &first_nid, 
		 const int &second_nid) = 0;
     virtual TStatus STDCALL SeedTravelElemPostProcess(
		 const int &eid, 
		 bool &break_flag) = 0;
};

typedef TStatus (*pfn_stt_t)(const int &src_eid, 
							 const int &tar_eid, 
							 const int &first_nid, 
							 const int &second_nid);
typedef TStatus (*pfn_stepp_t)(const int &eid, bool &break_flag);

// {9A2ACB16-4A19-4da5-9374-8A67A5482D9B}
const MUID IID_IFemCoreSeedTravelClient2 = 
{ 0x9a2acb16, 0x4a19, 0x4da5, { 0x93, 0x74, 0x8a, 0x67, 0xa5, 0x48, 0x2d, 0x9b } };

interface IFemCoreSeedTravelClient2 : public IFemCoreSeedTravelClient
{
	virtual TStatus STDCALL GetClientCallabck(
		pfn_stt_t &pfn_stt, 
		pfn_stepp_t &pfn_stepp) = 0;
};

// {62CE99B8-EB96-4bb0-9191-597A081C5FA7}
const MUID IID_IFemCoreLCSUtil = 
{ 0x62ce99b8, 0xeb96, 0x4bb0, { 0x91, 0x91, 0x59, 0x7a, 0x8, 0x1c, 0x5f, 0xa7 } };

interface IFemCoreLCSUtil : public IRoot
{
     virtual TStatus STDCALL CreateLCS(
		 const fem::TPoint &opt, 
		 const fem::TNormal &z_axis) = 0;
     virtual TStatus STDCALL CreateLCS(
		 const fem::TPoint &opt, 
		 const fem::TNormal &x_axis, 
		 const fem::TNormal &y_axis, 
		 const fem::TNormal &z_axis) = 0;
     virtual TStatus STDCALL ClearLCS() = 0;
     virtual TStatus STDCALL GetWCSPoint(const int &nid, fem::TPoint &pt) = 0;
     virtual TStatus STDCALL SetWCSPoint(const int &nid, const fem::TPoint &pt) = 0;
     virtual TStatus STDCALL GetLCSPoint(const int &nid, fem::TPoint &pt) = 0;
     virtual TStatus STDCALL SetLCSPoint(const int &nid, const fem::TPoint &pt) = 0;
     virtual TStatus STDCALL TransformWCS2LCS(
		 const fem::TPoint &input_pt, 
		 fem::TPoint &output_pt) = 0;
     virtual TStatus STDCALL TransformLCS2WCS(
		 const fem::TPoint &input_pt, 
		 fem::TPoint &output_pt) = 0;
     virtual TStatus STDCALL TransformWCS2LCS4Normal(
		 const fem::TNormal &input, 
		 fem::TNormal &output) = 0;
     virtual TStatus STDCALL TransformLCS2WCS4Normal(
		 const fem::TNormal &input, 
		 fem::TNormal &output) = 0;
};

// {5C9017B3-3268-4907-BEEA-0EA88F8D761F}
const MUID IID_IFemCoreLCSUtil2 = 
{ 0x5c9017b3, 0x3268, 0x4907, { 0xbe, 0xea, 0xe, 0xa8, 0x8f, 0x8d, 0x76, 0x1f } };

interface IFemCoreLCSUtil2 : public IFemCoreLCSUtil
{
	virtual TStatus STDCALL GetLCS(
		fem::TPoint &opt, 
		fem::TNormal &x_axis, 
		fem::TNormal &y_axis, 
		fem::TNormal &z_axis) = 0;
};

enum TFemCoreVSLType
{
	TO_PARENT_SNAP,
	FROM_PARENT_SNAP
};

// {A530B941-CCFC-492c-9647-733CBA04296F}
const MUID IID_IFemCoreSnap = 
{ 0xa530b941, 0xccfc, 0x492c, { 0x96, 0x47, 0x73, 0x3c, 0xba, 0x4, 0x29, 0x6f } };

interface IFemCoreSnap : public IRoot
{
	virtual TStatus STDCALL CreatePartSnap(
		int *eids, 
		const int elen, 
		const char *name, 
		IFemCoreSnap *&sub_snap) = 0;
	virtual TStatus STDCALL CreateTransSnap(
		int *nmaps, 
		const char *name, 
		IFemCoreSnap *&sub_snap) = 0;
	virtual TStatus STDCALL CreateVirtualParentSnap(int *nids, int *eids) = 0;
	virtual TStatus STDCALL GetParent(IFemCoreSnap *&parent) = 0;
	virtual TStatus STDCALL GetSubSnap(const char *name, IFemCoreSnap *&snap) = 0;
	virtual TStatus STDCALL GetSubSnapCount(int &cnt) = 0;
	virtual TStatus STDCALL GetName(char *name) = 0;
	virtual TStatus STDCALL LocateNodeTo(IFemCoreSnap *snap, int &nid) = 0;
	virtual TStatus STDCALL LocateElemTo(IFemCoreSnap *snap, int &eid) = 0;
	virtual TStatus STDCALL LocateNodesTo(
		IFemCoreSnap *snap, 
		int *nids, 
		const int nlen) = 0;
	virtual TStatus STDCALL LocateElemsTo(
		IFemCoreSnap *snap, 
		int *eids, 
		const int elen) = 0;
	virtual TStatus STDCALL LocateVirtualNode(TFemCoreVSLType type, int &nid) = 0;
	virtual TStatus STDCALL LocateVirtualElem(TFemCoreVSLType type, int &eid) = 0;
	virtual TStatus STDCALL LocateVirtualNodes(
		TFemCoreVSLType type, 
		int *nids, 
		const int nlen) = 0;
	virtual TStatus STDCALL LocateVirtualElems(
		TFemCoreVSLType type, 
		int *eids, 
		const int elen) = 0;
	virtual TStatus STDCALL UpdateParentSnap() = 0;
	virtual TStatus STDCALL UpdateSubSnap(IFemCoreSnap *snap = NULL) = 0;
	virtual TStatus STDCALL InterSection(
		IFemCoreSnap *right, 
		IFemCoreSnap *&res, 
		char *name) = 0;
	virtual TStatus STDCALL Union(
		IFemCoreSnap *right, 
		IFemCoreSnap *&res, 
		char *name) = 0;
	virtual TStatus STDCALL Difference(
		IFemCoreSnap *right, 
		IFemCoreSnap *&res, 
		char *name) = 0;
};

// {51BB5FF9-7A73-496c-9AFE-F6422785FFD6}
const MUID IID_IFemCoreSnap2 = 
{ 0x51bb5ff9, 0x7a73, 0x496c, { 0x9a, 0xfe, 0xf6, 0x42, 0x27, 0x85, 0xff, 0xd6 } };

interface IFemCoreSnap2 : public IFemCoreSnap
{
	/**
	 * This function is designed to create a split snap.
	 *  The Limitation:
	 *   1)Any new snap can't be created from a split snap.
	 *   2)A split snap can't act as target and source of update functions.
	 *   3)A split snap can't act any role in set functions. 
	 *   4)A split snap can't act as target of node location functions.
	 * 
	 * @param name     The name of split snap that you are creating.
	 * @param sub_snap Return Value.
	 * 
	 * @return true: successful.
	 *         false: fail.
	 */
	virtual TStatus STDCALL CreateSplitSnap(
		const char *name, 
		IFemCoreSnap2 *&sub_snap) = 0;
	virtual TStatus STDCALL UpdateBrotherSnap(IFemCoreSnap *snap) = 0;
	virtual TStatus STDCALL Clone(const char *name, IFemCoreSnap *&snap) = 0;
};

// {8EA9EAEA-1A64-4174-990B-0FA886D86C47}
const MUID CLSID_IFemCore = 
{ 0x8ea9eaea, 0x1a64, 0x4174, { 0x99, 0xb, 0xf, 0xa8, 0x86, 0xd8, 0x6c, 0x47 } };

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

