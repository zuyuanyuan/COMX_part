// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A0C633A2_2240_4D20_9E6F_68B2BA1DA843__INCLUDED_)
#define AFX_STDAFX_H__A0C633A2_2240_4D20_9E6F_68B2BA1DA843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <base/comx_js_ext.hxx>
#include <base/type_cast.hxx>

using namespace KMAS::type;

#ifdef SUPPORT_OPENGL

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#endif

#ifdef SUPPORT_ACIS

// Include Acis Head Files
#include "acis.hxx"
#include "api.hxx"
#include "cstrapi.hxx"
#include "kernapi.hxx"
#include "af_api.hxx"
#include "getowner.hxx"
#include "fct_utl.hxx"
#include "intrapi.hxx"
#include "curve.hxx"
#include "curdef.hxx"
#include "edge.hxx"
#include "intsect.hxx"
#include "pcurve.hxx"
#include "surface.hxx"
#include "interval.hxx"
#include "acistype.hxx"
#include "point.hxx"
#include "vertex.hxx"

//#include "acisiges_api.hxx"
#include "traverse2.hxx"
#include "face.hxx"
#include "eulerapi.hxx"
#include "heal_api.hxx"
#include "ctapi.hxx"
#include "ptlist.hxx"
#include "rnd_api.hxx"

#endif

#ifdef SUPPORT_COMX_COMPONENT

#include <base\service.hxx>
#include <base\factory.hxx>
#include <base\prop.hxx>
#include <base\debug.hxx>
#include <base\libkulform.hxx>
#include <base\function.hxx>

#ifdef SUPPORT_ACIS
#define USE_ACIS_UI_COMPONENT
#endif

#include <base\util.hxx>
using namespace KMAS::util;

#include <ui\ui_toolbar_parser.hxx>
#include <ui\plugin_toolbar_func.hxx>
#include <ui\plugin.hxx>

using namespace KMAS::Die_maker::comx;

#ifdef SUPPORT_OPENGL

#include <gl\gl_content.hxx>
#include <gl\gl_pickup.hxx>
#include <gl\gl_wcs.hxx>
#include <gl\gl_drag.hxx>
#include <gl\gl_light.hxx>
#include <gl\gl_material.hxx>
#include <gl\gl_sensor.hxx>
#include <gl\gl_toolkit.hxx>
#include <gl\gl_data_base.hxx>

#endif

#endif



// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A0C633A2_2240_4D20_9E6F_68B2BA1DA843__INCLUDED_)
