#ifndef _KMAS_COMX_XML_SERIALIZE_H
#define _KMAS_COMX_XML_SERIALIZE_H

#include <xml/xml_stream.hxx>

namespace KMAS
{
namespace Die_maker
{
namespace comx
{
// {8794FC7B-1435-409c-85D8-5FB04F9CAB67}
const MUID IID_IXmlSerialize = 
{ 0x8794fc7b, 0x1435, 0x409c, { 0x85, 0xd8, 0x5f, 0xb0, 0x4f, 0x9c, 0xab, 0x67 } };

interface IXmlSerialize : public IRoot
{
	virtual TStatus STDCALL In(IXmlStream *&pXmlStream) = 0;
	virtual TStatus STDCALL Out(IXmlStream *&pXmlStream) = 0;
};

} // namespace comx
} // namespace Die_maker
} // namespace KMAS

#endif