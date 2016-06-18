/*HEAD fem_core_campatible HXX ONESTEP */
#ifndef _KMAS_FEM_CORE_COMPATIBLE_H
#define _KMAS_FEM_CORE_COMPATIBLE_H

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {376C0A86-BA2A-4889-89B1-F81E035A4214}
const MUID IID_IFemCore = 
{ 0x376c0a86, 0xba2a, 0x4889, { 0x89, 0xb1, 0xf8, 0x1e, 0x3, 0x5a, 0x42, 0x14 } };

interface IFemCore : public IRoot{
	virtual TStatus STDCALL InsertNode(fem::POSITION pos , fem::TNode &node) = 0;
	virtual TStatus STDCALL InsertNode(fem::TNodeIterator pos , fem::TNode &node) = 0;
	virtual TStatus STDCALL InsertNode(fem::POSITION pos , fem::TNodeIterator first , fem::TNodeIterator last) = 0;
	virtual TStatus STDCALL InsertNode(fem::TNodeIterator pos , fem::TNodeIterator first , fem::TNodeIterator last) = 0;
	virtual TStatus STDCALL InsertElement(fem::POSITION pos , int n1 , int n2 , int n3 , int n4 = -1) = 0;
	virtual TStatus STDCALL InsertElement(fem::TElementIterator pos , int n1 , int n2 , int n3 , int n4 = -1) = 0;
	virtual TStatus STDCALL GetNode(fem::POSITION pos,fem::TNodeIterator &iter) = 0;
	virtual TStatus STDCALL GetNode(int pos,fem::TNodeIterator &iter) = 0;
	virtual TStatus STDCALL GetElement(fem::POSITION pos,fem::TElementIterator &iter) = 0;
	virtual TStatus STDCALL GetElement(int pos,fem::TElementIterator &iter) = 0;
	virtual TStatus STDCALL DeleteNode(fem::TNodeIterator pos) = 0;
	virtual TStatus STDCALL DeleteNode(fem::TNodeIterator first,fem::TNodeIterator last) = 0;
	virtual TStatus STDCALL DeleteElement(fem::TElementIterator pos) = 0;
	virtual TStatus STDCALL DeleteElement(fem::TElementIterator first,fem::TElementIterator last) = 0;
	virtual TStatus STDCALL Clean(bool is_force = false) = 0;
	virtual TStatus STDCALL Clear() = 0;
	virtual TStatus STDCALL IsInitialize(bool &is_init) = 0;
	virtual TStatus STDCALL GetRotateCenter(fem::TPoint &center) = 0;
	virtual TStatus STDCALL GetScale(double &scale) = 0;
	virtual TStatus STDCALL GetSide(fem::POSITION pos, fem::TSideIterator &iter) = 0;
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif