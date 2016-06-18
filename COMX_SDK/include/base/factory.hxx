/*HEAD factory HXX ONESTEP */
#ifndef _FACTORY_H
#define _FACTORY_H

#pragma warning(disable:4786)

#include <base/root.hxx>

#include <vector>
#include <string>
using namespace std;

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {B28FEE75-D734-4248-A5EB-604A61A87B38}
const MUID IID_IFactory =
    {
        0xb28fee75, 0xd734, 0x4248, { 0xa5, 0xeb, 0x60, 0x4a, 0x61, 0xa8, 0x7b, 0x38 }
    };

interface IFactory : public IRoot
{
    virtual TStatus STDCALL CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv) = 0;
    virtual TStatus STDCALL Register(const char *szDLL) = 0;
    virtual TStatus STDCALL Dispose(void) = 0;
    virtual TStatus STDCALL Initialize() = 0;
    virtual TStatus STDCALL QueryService(MREFTYPEID rclsid,MREFIID riid,void **ppv) = 0;
    virtual TStatus STDCALL LoadScript(const char *xmlFilename, const bool debug = true) = 0;
};

// {849E46BC-B600-40f4-81C6-E051F84D87F5}
const MUID IID_IFactory2 = 
{ 0x849e46bc, 0xb600, 0x40f4, { 0x81, 0xc6, 0xe0, 0x51, 0xf8, 0x4d, 0x87, 0xf5 } };

interface IFactory2 : public IFactory
{
    virtual TStatus STDCALL CreateInstanceEx(IRoot *pRootOuter, MREFTYPEID rclsid,MREFIID riid,void **ppv) = 0;
};

// {44D5DC46-3427-4016-90DE-E1A059DD1576}
const MUID IID_IFactoryGlobalSetting = 
{ 0x44d5dc46, 0x3427, 0x4016, { 0x90, 0xde, 0xe1, 0xa0, 0x59, 0xdd, 0x15, 0x76 } };

interface IFactoryGlobalSetting : public IRoot
{
	virtual TStatus STDCALL GetHomePath(char *path) = 0;
	virtual TStatus STDCALL SetHomePath(const char *path) = 0;
	virtual TStatus STDCALL GetRootPath(char *path) = 0;
	virtual TStatus STDCALL SetRootPath(const char *path) = 0;
	virtual TStatus STDCALL AppendScriptPath(const char *path) = 0;
	virtual TStatus STDCALL GetScriptPathNum(int &num) = 0;
	virtual TStatus STDCALL GetScriptPath(const int index, char *path) = 0;
	virtual TStatus STDCALL AppendDataPath(const char *path) = 0;
	virtual TStatus STDCALL AppendModulePath(const char *path) = 0;
	virtual TStatus STDCALL SearchDataFilePath(char *fname, char *path) = 0;
	virtual TStatus STDCALL SearchModuleFilePath(char *fname, char *path) = 0;
	virtual TStatus STDCALL LoadProductInfo(const char *fname) = 0;
	virtual TStatus STDCALL GetPackageFileName(char *fname) = 0;
};

// {4ACD9015-1971-4471-ADC4-3D05DB4E166E}
const MUID IID_IFactoryUiConfig = 
{ 0x4acd9015, 0x1971, 0x4471, { 0xad, 0xc4, 0x3d, 0x5, 0xdb, 0x4e, 0x16, 0x6e } };

interface IFactoryUiConfig : public IRoot
{
    virtual TStatus STDCALL IsMaxMainWindow(bool &flag) = 0;
	virtual TStatus STDCALL IsLockToolbar(bool &flag) = 0;
	virtual TStatus STDCALL GetTitleOfMainWindow(char *title) = 0;
	virtual TStatus STDCALL GetPkgCLSID(char *clsid) = 0;
};

// {1DDA09E8-D961-4d2e-8B61-507ECE40AC55}
const MUID IID_IFactoryUiConfig2 = 
{ 0x1dda09e8, 0xd961, 0x4d2e, { 0x8b, 0x61, 0x50, 0x7e, 0xce, 0x40, 0xac, 0x55 } };

interface IFactoryUiConfig2 : public IFactoryUiConfig
{
	virtual TStatus STDCALL IsEnableView2D(bool &flag) = 0;
	virtual TStatus STDCALL GetView2DTitle(char *title) = 0;
	virtual TStatus STDCALL GetView2DXTitle(char *title) = 0;
	virtual TStatus STDCALL GetView2DYTitle(char *title) = 0;
	virtual TStatus STDCALL IsEnableView2DScale(bool &flag) = 0;
	virtual TStatus STDCALL IsEnableView2DCaptureLine(bool &flag) = 0;
};

// {224A6D9E-140A-4b87-99FA-90C6BA623AC2}
const MUID IID_IFactoryUiConfig3 = 
{ 0x224a6d9e, 0x140a, 0x4b87, { 0x99, 0xfa, 0x90, 0xc6, 0xba, 0x62, 0x3a, 0xc2 } };

interface IFactoryUiConfig3 : public IFactoryUiConfig2
{
	virtual TStatus STDCALL IsEnableConsole(bool &flag) = 0;
};

// {B4E0F997-B110-47a3-8C20-9A21034CE20B}
const MTYPEID CLSID_IFactory =
    {
        0xb4e0f997, 0xb110, 0x47a3, { 0x8c, 0x20, 0x9a, 0x21, 0x3, 0x4c, 0xe2, 0xb }
    };

//DECLARE_CREATE_INSTANCE()
extern "C" DLL_API TStatus CreateFactory(MREFTYPEID rclsid,MREFIID riid,void **ppv);

class TFactory
{
    IFactory *i_factory;
	IFactoryGlobalSetting *p_factory_gs;
	IFactoryUiConfig *p_factory_uc;
    bool _is_ref;
	string _package_name;
public:
     enum TSolutionType
     {
          KDEBUG,
          KRELEASE
     };
public:
    inline TFactory(bool ref = true);
    inline ~TFactory();
public:
    inline TStatus Register(const char *szDll);
    inline TStatus CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv);
    inline TStatus Initialize();
    inline TStatus QueryService(MREFTYPEID rclsid,MREFIID riid,void **ppv);
    inline TStatus LoadScript(const char *xmlFilename, const TSolutionType stype);
public:
	inline TStatus CreateInstanceEx(IRoot *pRootOuter,MREFTYPEID rclsid,MREFIID riid,void **ppv);
public:
	inline TStatus GetHomePath(char *path);
	inline TStatus SetHomePath(const char *path);
	inline TStatus GetRootPath(char *path);
	inline TStatus SetRootPath(const char *path);
	inline TStatus AppendScriptPath(const char *path);
	inline TStatus GetScriptPathNum(int &num);
	inline TStatus GetScriptPath(const int index, char *path);
	inline TStatus AppendDataPath(const char *path);
	inline TStatus AppendModulePath(const char *path);
	inline TStatus SearchDataFilePath(char *fname, char *path);
	inline TStatus SearchModuleFilePath(char *fname, char *path);
	inline TStatus LoadProductInfo(const char *fname_cfg);
	inline TStatus GetPackageName(char *fname);
public:
	inline TStatus STDCALL IsMaxMainWindow(bool &flag);
	inline TStatus STDCALL IsLockToolbar(bool &flag);
	inline TStatus STDCALL GetTitleOfMainWindow(char *title);
	inline TStatus STDCALL GetPkgCLSID(char *title);
public:
	inline TStatus STDCALL IsEnableView2D(bool &flag);
	inline TStatus STDCALL GetView2DTitle(char *title);
	inline TStatus STDCALL GetView2DXTitle(char *title);
	inline TStatus STDCALL GetView2DYTitle(char *title);
	inline TStatus STDCALL IsEnableView2DScale(bool &flag);
	inline TStatus STDCALL IsEnableView2DCaptureLine(bool &flag);
public:
	inline TStatus STDCALL IsEnableConsole(bool &flag);
};

TFactory::TFactory(bool ref) : i_factory(NULL) , _is_ref(ref)
{
    KMAS::Die_maker::comx::CreateFactory(CLSID_IFactory,IID_IFactory,(void**)&i_factory);
    assert(i_factory);

	i_factory->QueryInterface(IID_IFactoryGlobalSetting, (void**)&p_factory_gs);
	assert(p_factory_gs);

	p_factory_uc = NULL;
	i_factory->QueryInterface(IID_IFactoryUiConfig, (void**)&p_factory_uc);
}

TFactory::~TFactory()
{
    if ( i_factory )
    {
        if (!_is_ref)
        {
            i_factory -> Dispose();
        }

        i_factory -> Release();
    }

	if (p_factory_gs)
	{
		p_factory_gs->Release();
	}
}

TStatus TFactory::Register(const char *szDll)
{
    return i_factory -> Register(szDll);
}

TStatus TFactory::CreateInstance(MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
    return i_factory -> CreateInstance(rclsid , riid , ppv);
}

TStatus TFactory::CreateInstanceEx(IRoot *pRootOuter,MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
	IFactory2 *p_factory_2 = NULL;
	i_factory->QueryInterface(IID_IFactory2, (void**)&p_factory_2);
	
	if (p_factory_2)
	{
		p_factory_2->CreateInstanceEx(pRootOuter,rclsid,riid,ppv);
		p_factory_2->Release();

		return M_OK;
	}
	else
	{
		return M_FAIL;
	}
}

TStatus TFactory::Initialize()
{
    return i_factory -> Initialize();
}

TStatus TFactory::QueryService(MREFTYPEID rclsid,MREFIID riid,void **ppv)
{
     return i_factory -> QueryService(rclsid , riid , ppv);
}

TStatus TFactory::LoadScript(const char *xmlFilename, const TSolutionType stype)
{
     if (stype == KDEBUG)
     {
          return i_factory->LoadScript(xmlFilename, true);
     }
     else
     {
          return i_factory->LoadScript(xmlFilename, false);
     }
}

TStatus TFactory::GetHomePath(char *path)
{
	return p_factory_gs->GetHomePath(path);
}

TStatus TFactory::SetHomePath(const char *path)
{
	return p_factory_gs->SetHomePath(path);
}

TStatus TFactory::GetRootPath(char *path)
{
	return p_factory_gs->GetRootPath(path);
}

TStatus TFactory::SetRootPath(const char *path)
{
	return p_factory_gs->SetRootPath(path);
}

TStatus TFactory::AppendScriptPath(const char *path)
{
	return p_factory_gs->AppendScriptPath(path);
}

TStatus TFactory::GetScriptPathNum(int &num)
{
	return p_factory_gs->GetScriptPathNum(num);
}

TStatus TFactory::GetScriptPath(const int index, char *path)
{
	return p_factory_gs->GetScriptPath(index, path);
}

TStatus TFactory::AppendDataPath(const char *path)
{
	return p_factory_gs->AppendDataPath(path);
}

TStatus TFactory::AppendModulePath(const char *path)
{
	return p_factory_gs->AppendModulePath(path);
}

TStatus TFactory::SearchDataFilePath(char *fname, char *path)
{
	return p_factory_gs->SearchDataFilePath(fname, path);
}

TStatus TFactory::SearchModuleFilePath(char *fname, char *path)
{
	return p_factory_gs->SearchModuleFilePath(fname, path);
}

TStatus TFactory::LoadProductInfo(const char *fname)
{
	return p_factory_gs->LoadProductInfo(fname);
}

TStatus TFactory::GetPackageName(char *fname)
{
	return p_factory_gs->GetPackageFileName(fname);
}

TStatus STDCALL TFactory::IsMaxMainWindow(bool &flag)
{
	if (p_factory_uc != NULL)
	{
		return p_factory_uc->IsMaxMainWindow(flag);
	}

	return M_FAIL;
}

TStatus STDCALL TFactory::IsLockToolbar(bool &flag)
{
	if (p_factory_uc != NULL)
	{
		return p_factory_uc->IsLockToolbar(flag);
	}

	return M_FAIL;
}

TStatus STDCALL TFactory::GetTitleOfMainWindow(char *title)
{
	if (p_factory_uc != NULL)
	{
		return p_factory_uc->GetTitleOfMainWindow(title);
	}

	return M_FAIL;
}

TStatus STDCALL TFactory::GetPkgCLSID(char *title)
{
	if (p_factory_uc != NULL)
	{
		return p_factory_uc->GetPkgCLSID(title);
	}

	return M_FAIL;
}

TStatus STDCALL TFactory::IsEnableView2D(bool &flag)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig2, p_factory_uc2);
	if (p_factory_uc2 != NULL)
	{
		p_factory_uc2->IsEnableView2D(flag);
		p_factory_uc2->Release();
	}

	return M_OK;
}

TStatus STDCALL TFactory::GetView2DTitle(char *title)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig2, p_factory_uc2);
	if (p_factory_uc2 != NULL)
	{
		p_factory_uc2->GetView2DTitle(title);
		p_factory_uc2->Release();
	}
	return M_OK;
}

TStatus STDCALL TFactory::GetView2DXTitle(char *title)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig2, p_factory_uc2);
	if (p_factory_uc2 != NULL)
	{
		p_factory_uc2->GetView2DXTitle(title);
		p_factory_uc2->Release();
	}

	return M_OK;
}

TStatus STDCALL TFactory::GetView2DYTitle(char *title)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig2, p_factory_uc2);
	if (p_factory_uc2 != NULL)
	{
		p_factory_uc2->GetView2DYTitle(title);
		p_factory_uc2->Release();
	}

	return M_OK;
}

TStatus STDCALL TFactory::IsEnableView2DScale(bool &flag)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig2, p_factory_uc2);
	if (p_factory_uc2 != NULL)
	{
		p_factory_uc2->IsEnableView2DScale(flag);
		p_factory_uc2->Release();
	}
	
	return M_OK;
}

TStatus STDCALL TFactory::IsEnableView2DCaptureLine(bool &flag)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig2, p_factory_uc2);
	if (p_factory_uc2 != NULL)
	{
		p_factory_uc2->IsEnableView2DCaptureLine(flag);
		p_factory_uc2->Release();
	}
	
	return M_OK;
}

TStatus STDCALL TFactory::IsEnableConsole(bool &flag)
{
	QUERY_INTERFACE(p_factory_uc, IFactoryUiConfig3, p_factory_uc3);
	if (p_factory_uc3 != NULL)
	{
		p_factory_uc3->IsEnableConsole(flag);
		p_factory_uc3->Release();
	}
	
	return M_OK;
}

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif /*_FACTORY_H*/
