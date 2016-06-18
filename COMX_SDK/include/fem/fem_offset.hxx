/*HEAD fem_offset HXX ONESTEP */
#ifndef _FEM_OFFESET_NET_PART
#define _FEM_OFFESET_NET_PART

#include <base/root.hxx>
#include <fem/fem_core.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {3CC9E9A9-8CF4-4deb-B8F1-BBF2239C07F5}
const MUID IID_IFemPreciseOffset = 
{ 0x3cc9e9a9, 0x8cf4, 0x4deb, { 0xb8, 0xf1, 0xbb, 0xf2, 0x23, 0x9c, 0x7, 0xf5 } };

interface IFemPreciseOffset : public IRoot
{
	virtual TStatus STDCALL Offset(IFemCore *&pFemCoreSrc, IFemCore *&pFemCoreTarget, float distance, bool isPrecise = true) = 0;
};


// {7439892A-43BB-433d-903F-F6899D35CD11}
const MUID CLSID_IFemPreciseOffset = 
{ 0x7439892a, 0x43bb, 0x433d, { 0x90, 0x3f, 0xf6, 0x89, 0x9d, 0x35, 0xcd, 0x11 } };

} // namespace comx
} // namespace Die_maker
} // namespace KMAS



#endif