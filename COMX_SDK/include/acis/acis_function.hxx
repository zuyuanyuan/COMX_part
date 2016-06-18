#ifndef _KMAS_COMX_ACIS_FUNCTION_HXX
#define _KMAS_COMX_ACIS_FUNCTION_HXX

#include <acis/acis_function_wrap.hxx>

namespace KMAS{
namespace acis_func{

	extern "C"
	{
		typedef void (*pfn_get_vertex_coords_t)(
			const tag_t tag_vertice, 
			double *x, 
			double *y, 
			double *z);

		typedef bool (*pfn_is_pt_on_face_t)(
            const tag_t tag_face,
			double x,
			double y, 
			double z);
	}

    void get_vertex_coords(
		const tag_t tag_vertice, 
		double *x, 
		double *y, 
		double *z)
	{
        KMAS::Die_maker::comx::TAcisFunctionWrap 
			func("get_vertex_coords");

		pfn_get_vertex_coords_t pfn = 
			(pfn_get_vertex_coords_t)func.GetValue();

		(*pfn)(tag_vertice, x, y, z);
	}

	bool is_pt_on_face(
		const tag_t tag_face,
		double x,
		double y, 
		double z)
	{
		KMAS::Die_maker::comx::TAcisFunctionWrap 
			func("is_pt_on_face");

		pfn_is_pt_on_face_t pfn = 
			(pfn_is_pt_on_face_t)func.GetValue();

		return (*pfn)(tag_face, x, y, z);
	}

}// end namespace acis_func.
}// end namespace KMAS.

#endif
