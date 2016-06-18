/*HEAD comx_console HXX COMX_COMPONENT_MODULE */

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

#ifndef _KMAS_COMX_COMX_CONSOLE_H_2010821114728
#define _KMAS_COMX_COMX_CONSOLE_H_2010821114728

#include "base\root.hxx"

#include <sstream>
#include <string>
using namespace std;

#include <base/factory.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

// {12D74C88-2CF5-4e5f-91E1-704B3C0D1BF7}
 const MUID IID_IComxConsole = 
{ 0x12d74c88, 0x2cf5, 0x4e5f, { 0x91, 0xe1, 0x70, 0x4b, 0x3c, 0xd, 0x1b, 0xf7 } };

interface IComxConsole : public IRoot
{
     virtual TStatus STDCALL Startup() = 0;
     virtual TStatus STDCALL Closure() = 0;
     virtual TStatus STDCALL Input(char* buffer) = 0;
     virtual TStatus STDCALL Output(const char* buffer) = 0;
	 virtual TStatus STDCALL IsEnable(bool &flag) = 0;
};

// {D384C7A7-4EAB-4b00-8F5F-898542311DE8}
 const MTYPEID CLSID_IComxConsole = 
{ 0xd384c7a7, 0x4eab, 0x4b00, { 0x8f, 0x5f, 0x89, 0x85, 0x42, 0x31, 0x1d, 0xe8 } };

const static class kout_type
{
	IComxConsole *_p_comx_console;
	mutable bool _is_console_enable;
	void _get_console_status() const
	{
		static bool flag = false;
		if (!flag)
		{
			if (_p_comx_console != NULL)
			{
				bool bInnerFlag = false;
				_p_comx_console->IsEnable(_is_console_enable);
			}

			flag = true;
		}
	}
public:
	kout_type()
	{
		TFactory factory;
		QUERY_SERVICE(factory, CLSID_IComxConsole, IComxConsole, p_comx_console);

		_is_console_enable = false;
		if (p_comx_console != NULL)
		{
			_p_comx_console = p_comx_console;
			p_comx_console->Release();
		}
	}
public:
	template<typename T>
	const kout_type &operator <<(T &t) const
	{
		_get_console_status();

		if (_is_console_enable)
		{
			stringstream str_stream;
			str_stream << t;
			
			char buf[1024] = "";
			str_stream.getline(buf, 1024);
			
			_p_comx_console->Output(buf);
		}
		
		return *this;
	}
	
	const kout_type &operator <<(char * t) const
	{
		_get_console_status();

		if (_is_console_enable)
		{
			_p_comx_console->Output(t);
		}
		
		return *this;
	}

	const kout_type &operator <<(string t) const 
	{
		_get_console_status();

		if (_is_console_enable)
		{
			_p_comx_console->Output(t.c_str());
		}
		
		return *this;
	}
}kout;

const static class kin_type
{
	IComxConsole *_p_comx_console;
	mutable bool _is_console_enable;
	void _get_console_status() const 
	{
		static bool flag = false;
		if (!flag)
		{
			if (_p_comx_console != NULL)
			{
				_p_comx_console->IsEnable(_is_console_enable);
			}
			
			flag = true;
		}
	}
public:
	kin_type()
	{
		TFactory factory;
		QUERY_SERVICE(factory, CLSID_IComxConsole, IComxConsole, p_comx_console);
		
		_is_console_enable = false;
		if (p_comx_console != NULL)
		{
			_p_comx_console = p_comx_console;
			p_comx_console->Release();
		}
	}
public:
	template<typename T>
	const kin_type &operator >>(T &t) const 
	{
		_get_console_status();

		if (_is_console_enable)
		{
			char buf[1024] = "";
			_p_comx_console->Input(buf);
			stringstream str_stream(buf);
			str_stream >> t;
		}
		
		return *this;
	}
}kin;

static const string kendl = "\n";

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif

