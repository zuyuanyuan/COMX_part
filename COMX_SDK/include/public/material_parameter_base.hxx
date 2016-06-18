/*HEAD fem_material_para_lib HXX COMX_COMPONENT_MODULE */

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
#ifndef _KMAS_COMX_MATERIAL_PARAMETER_H_200712922221
#define _KMAS_COMX_MATERIAL_PARAMETER_H_200712922221

namespace KMAS
{
namespace Die_maker
{
namespace comx
{

struct material_parameter
{
	material_parameter(float _elasticmodules = 0.0, float _density = 0.0, float _poisson_ratio = 0.0, float _thickness = 0.0, 
		float _yielding_stress = 0.0, float _hardening_index = 0.0, float _hardening_factor = 0.0, float _fracture_criteria = 0.0,
		float _R0 = 0.0, float _R45 = 0.0, float _R90 = 0.0,
		float _in_plane_yielding_fun_index = 0.0, float _anisophy_coefficient = 0.0, float _thickness_yielding_fun_index = 0.0,
		float _sensitivity_index = 0.0, float _initial_strain_rate = 0.0, float _initial_strain = 0.0)
	{
		elasticmodules = _elasticmodules;
		density = _density;
		poisson_ratio = _poisson_ratio;
		thickness = _thickness;
		yielding_stress = _yielding_stress;
		hardening_index = _hardening_index;
		hardening_factor = _hardening_factor;
		fracture_criteria = _fracture_criteria;
		R0 = _R0;
		R45 = _R45;
		R90 = _R90;
		in_plane_yielding_fun_index = _in_plane_yielding_fun_index;
		anisophy_coefficient = _anisophy_coefficient;
		thickness_yielding_fun_index = _thickness_yielding_fun_index;
		sensitivity_index = _sensitivity_index;
		initial_strain_rate = _initial_strain_rate;
		initial_strain = _initial_strain;
	}
	material_parameter& operator=(const material_parameter& right)
	{
		elasticmodules = right.elasticmodules;
		density = right.density;
		poisson_ratio = right.poisson_ratio;
		thickness = right.thickness;
		yielding_stress = right.yielding_stress;
		hardening_index = right.hardening_index;
		hardening_factor = right.hardening_factor;
		fracture_criteria = right.fracture_criteria;
		R0 = right.R0;
		R45 = right.R45;
		R90 = right.R90;
		in_plane_yielding_fun_index = right.in_plane_yielding_fun_index;
		anisophy_coefficient = right.anisophy_coefficient;
		thickness_yielding_fun_index = right.thickness_yielding_fun_index;
		sensitivity_index = right.sensitivity_index;
		initial_strain_rate = right.initial_strain_rate;
		initial_strain = right.initial_strain;
		
		return *this;
	}
public:
	float elasticmodules;
	float density;
	float poisson_ratio;
	float thickness;
	float yielding_stress;
	float hardening_index;
	float hardening_factor;
	float fracture_criteria;
	float R0;
	float R45;
	float R90;
	float in_plane_yielding_fun_index;
	float anisophy_coefficient;
	float thickness_yielding_fun_index;
	float sensitivity_index;
	float initial_strain_rate;
    float initial_strain;
};

}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif