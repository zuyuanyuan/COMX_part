/*HEAD fem_util HXX ONESTEP */
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
================================================================================*/
#ifndef _KMAS_COMX_FEM_UTIL_H
#define _KMAS_COMX_FEM_UTIL_H

#include <fem/fem_core.hxx>
using namespace KMAS::Die_maker::comx;

namespace fem{

#ifndef _FEM_BASE_UTILS_FUNCTION_IMPLEMENT
#define _FEM_BASE_UTILS_FUNCTION_IMPLEMENT

/************************************************************************/
/* Template type constrain description                                  */
/*   TPoint : It should include three public members: x, y, z;          */
/*   TNormal: It should include three public members: x, y, z;          */
/************************************************************************/

template<typename TPoint , typename TNormal>
void IntersectionPointL2P(const TPoint &  NodeOnPlane,const TNormal& PlaneNormal, 
						  const TPoint&  NodeOnLine,   const TNormal&  LineDirection,
						  TPoint & intersectionPoint)
{
	double D  = PlaneNormal.x * NodeOnLine.x + PlaneNormal.y * NodeOnLine.y +PlaneNormal.z * NodeOnLine.z;
  	double NP = PlaneNormal.x * NodeOnPlane.x + PlaneNormal.y * NodeOnPlane.y + PlaneNormal.z * NodeOnPlane.z;
	double ND = PlaneNormal.x * LineDirection.x + PlaneNormal.y * LineDirection.y + PlaneNormal.z * LineDirection.z;
//	assert(fabs(ND) > 1e-8);
	double t = (NP-D)/ND;
	intersectionPoint.x = NodeOnLine.x + t * LineDirection.x;
	intersectionPoint.y = NodeOnLine.y + t * LineDirection.y;
	intersectionPoint.z = NodeOnLine.z + t * LineDirection.z;		
}

/************************************************************************/
/* Template type constrain description                                  */
/*   TPoint: It should include three public members: x, y, z;           */
/*   TShape: It should be greater than 2.                               */
/*   TTolerance: It should be 0.0~1.0                                   */
/************************************************************************/

template<typename TPoint , typename TShape , typename TTolerance>
bool IsPointInPolygon(const TPoint & pt2d , TPoint* polygon2d , TShape sides_num , TTolerance tol)
{
	int i = 0, j = 0, k = 0, l = 0;
	int status = 0;
	int indexxy[4];		//象限标识
	int xyminus[4],xymin = 0;//象限的差
	int arclength = 0;		//弧长的和
	float ff = 0.0;

	memset(indexxy, 0, sizeof(int) * 4);
	memset(xyminus, 0, sizeof(int) * 4);

	const int &nn = sides_num;

	for(i = 0; i < nn; ++i)
	{
		polygon2d[i].x = polygon2d[i].x - pt2d.x;
		polygon2d[i].y = polygon2d[i].y - pt2d.y;
	}
	//****************************************
	for(l = 0; l < nn; ++l)//原点是单元节点
	{
		if(fabs(polygon2d[l].x) < 1.0e-6 && fabs(polygon2d[l].y) < 1.0e-6)
		{
			status = 1;
			goto loop;
		}
	}
	for(j = 0; j < nn; ++j)
	{
		if(polygon2d[j].x >= 0.0)
		{
			if(polygon2d[j].y >= 0.0)
				indexxy[j] = 1;
			else
				indexxy[j] = 4;
		} 
		else if(polygon2d[j].y >= 0.0)
			indexxy[j] = 2;
		else
			indexxy[j] = 3;
	}
	//****************************************
	for(k = 0;k < nn - 1;++k)
	{
		xyminus[k] = indexxy[k+1] - indexxy[k];
	}
	xyminus[nn - 1] = indexxy[0] - indexxy[nn - 1];
	arclength = 0;
	for(l = 0; l < nn; ++l)
	{
		xymin = xyminus[l];
		if(xymin ==  0) arclength = arclength;
		if(xymin ==  1) arclength = arclength + 1;
		if(xymin == -1) arclength = arclength - 1;
		if(xymin ==  3) arclength = arclength - 1;
		if(xymin == -3) arclength = arclength + 1;

		if(abs(xymin) == 2)
		{
			if(l < nn - 1)
				ff = polygon2d[l + 1].y * polygon2d[l].x - polygon2d[l + 1].x * polygon2d[l].y;
			else
				ff = polygon2d[l + 1 - nn].y * polygon2d[l].x - polygon2d[l + 1 - nn].x * polygon2d[l].y;
			if(fabs(ff) <= tol) //在边上
			{
				status = 1;
				goto loop;
			} 
			else if(ff > tol)//f>0 ,则弧长代数和增加 
				arclength += 2;
			else //f<0 ,则弧长代数和减少 
				arclength += 2;
		}
	}
	if(arclength == 0)
		status = 0;
	else
		status = 1;
loop:
	return (status);
			
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    point_t: it should have three memebers - x, y, z;                 */
/************************************************************************/

template<typename point_t>
double MesureDistance(const point_t &pt1,const point_t &pt2)
{
     double delta_x = pt1.x - pt2.x;
     double delta_y = pt1.y - pt2.y;
     double delta_z = pt1.z - pt2.z;

     return sqrt(delta_x * delta_x + delta_y * delta_y +delta_z *delta_z);
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    normal_t: it should have three memebers - x, y, z;                */
/************************************************************************/

template<typename normal_t>
double Normalize(normal_t &normal)
{
     double model = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
     assert(fabs(model) > 1e-8);
     normal.x /= model;
     normal.y /= model;
     normal.z /= model;

     return model;
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    normal_t: it should have three memebers - x, y, z;                */
/************************************************************************/

template<typename normal_t>
void fork_product(const normal_t &first,const normal_t &second,normal_t &ret)
{
     ret.x = first.y * second.z - first.z * second.y;
     ret.y = first.z * second.x - first.x * second.z;
     ret.z = first.x * second.y - first.y * second.x;
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    point_t: it should have three memebers - x, y, z;                 */
/************************************************************************/

template<typename point_t>
double GetTriangleArea(const point_t &pt1,const point_t &pt2,const point_t &pt3)
{
     TNormal vec12(pt2.x - pt1.x , pt2.y - pt1.y , pt2.z - pt1.z);
     TNormal vec13(pt3.x - pt1.x , pt3.y - pt1.y , pt3.z - pt1.z);

     TNormal normal;
     fork_product(vec12 , vec13 , normal);

     return sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z) / 2.0;
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    real_t: it should be float or double                              */
/************************************************************************/

template<typename real_t>
void INVERT_MATRIX(real_t DALAD[3][3], real_t REDALAD[3][3])
{
     real_t AA;


     AA=DALAD[0][0]*DALAD[1][1]*DALAD[2][2]+DALAD[1][0]*DALAD[2][1]*DALAD[0][2]+DALAD[2][0]*DALAD[0][1]*DALAD[1][2]
          -DALAD[0][2]*DALAD[1][1]*DALAD[2][0]-DALAD[1][0]*DALAD[0][1]*DALAD[2][2]-DALAD[0][0]*DALAD[1][2]*DALAD[2][1];

     REDALAD[0][0]=(DALAD[1][1]*DALAD[2][2]-DALAD[1][2]*DALAD[2][1])/AA;
     REDALAD[1][0]=-(DALAD[1][0]*DALAD[2][2]-DALAD[1][2]*DALAD[2][0])/AA;
     REDALAD[2][0]=(DALAD[1][0]*DALAD[2][1]-DALAD[1][1]*DALAD[2][0])/AA;
     REDALAD[0][1]=-(DALAD[0][1]*DALAD[2][2]-DALAD[0][2]*DALAD[2][1])/AA;
     REDALAD[1][1]=(DALAD[0][0]*DALAD[2][2]-DALAD[0][2]*DALAD[2][0])/AA;
     REDALAD[2][1]=-(DALAD[0][0]*DALAD[2][1]-DALAD[0][1]*DALAD[2][0])/AA;
     REDALAD[0][2]=(DALAD[0][1]*DALAD[1][2]-DALAD[0][2]*DALAD[1][1])/AA;
     REDALAD[1][2]=-(DALAD[0][0]*DALAD[1][2]-DALAD[0][2]*DALAD[1][0])/AA;
     REDALAD[2][2]=(DALAD[0][0]*DALAD[1][1]-DALAD[0][1]*DALAD[1][0])/AA;
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    TPoint: it should have three memebers - x, y, z;                  */
/************************************************************************/

template<typename TPoint>
bool IsElemInvert(const TPoint &pt1 , const TPoint &pt2 , const TPoint &pt3)
{
    TNormal v12(pt2.x - pt1.x , pt2.y - pt1.y , pt2.z - pt1.z);
    Normalize(v12);

    TNormal v13(pt3.x - pt1.x , pt3.y - pt1.y , pt3.z - pt1.z);
    Normalize(v13);

    TNormal elem_normal;
    fork_product(v12 , v13 , elem_normal);

    return elem_normal.z < 0.0;
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    TPoint: it should have three memebers - x, y, z;                  */
/************************************************************************/

template<typename TPoint>
bool Is3PointToLine(const TPoint &pt1 , const TPoint &pt2 , const TPoint &pt3)
{
    const double side_len_12 = MesureDistance(pt1 , pt2);
    const double side_len_23 = MesureDistance(pt2 , pt3);
    const double side_len_13 = MesureDistance(pt1 , pt3);

    double max_side_len = -1.0e10;
    double perimeter = side_len_12 + side_len_13 + side_len_23;

    if(max_side_len < side_len_12) max_side_len = side_len_12;
    if(max_side_len < side_len_23) max_side_len = side_len_23;
    if(max_side_len < side_len_13) max_side_len = side_len_13;

    return fabs(perimeter - 2 * max_side_len) < max_side_len * 0.001;
}

/************************************************************************/
/*  Template type constrain description                                 */
/*    TPoint: it should have three memebers - x, y, z;                  */
/************************************************************************/

template<typename TPoint>
void MirrorPoint(TPoint &pt1 , TPoint &pt2 , TPoint &pt3)
{
    TVector v12(pt2.x - pt1.x , pt2.y - pt1.y , pt2.z - pt1.z);
    Normalize(v12);

    double l0 = v12.x;
    double m0 = v12.y;

    double x3_1 = pt3.x - pt1.x;
    double y3_1 = pt3.y - pt1.y;

    double x3_2 = x3_1 * l0 + y3_1 * m0;
    double y3_2 = - x3_1 * m0 + y3_1 * l0;

    double x4_2 = x3_2;
    double y4_2 = - y3_2;

    double x4_1 = x4_2 * l0 - y4_2 * m0;
    double y4_1 = x4_2 * m0 + y4_2 * l0;

    double x4 = x4_1 + pt1.x;
    double y4 = y4_1 + pt1.y;

    pt3.x = x4;
    pt3.y = y4;  
}

#endif

#ifndef _KMAS_FEM_CORE_CACHE_DEFINE
#define _KMAS_FEM_CORE_CACHE_DEFINE

template<typename TOwner>
class TFemCoreCacheT{
     TOwner *pOwner;
public:
     vector<local_point_t>            points;
     vector<local_element_t>          elements;
     vector< set<node_id_t> >      n2n;
     vector< set<element_id_t> >   n2e;
     vector<TNormal>                  node_normals;
     vector<TNormal>                  element_normals;
     vector< set<local_e2e_t> >    e2e;
public:
     TFemCoreCacheT() : pOwner(static_cast<TOwner*>(this)){}
     ~TFemCoreCacheT(){}
public:
     inline void Synchronize();
};

template<typename TOwner>
void TFemCoreCacheT<TOwner>::Synchronize()
{
#ifndef KMAS_COMX_FEM_CORE_IMPLEMENT
     IFemCore *&_p_fem_core_proxy = GET_EVENT_INTERFACE_OWNER(pOwner, IFemCore);
#else
     TOwner *&_p_fem_core_proxy = pOwner;
#endif

     IFemCoreLCSUtil *p_fem_core_lcs_util = NULL;
     _p_fem_core_proxy->QueryInterface(IID_IFemCoreLCSUtil, (void**)&p_fem_core_lcs_util);
     assert(p_fem_core_lcs_util);

	_p_fem_core_proxy->Clean(true);

     TNodeIterator node_begin,node_end;
     _p_fem_core_proxy->GetNode(BEGIN,node_begin);
     _p_fem_core_proxy->GetNode(END,node_end);

     n2n.clear();
     n2n.resize(node_end - node_begin + 1);

     elements.clear();
     element_normals.clear();

     TElementIterator iter_elem,begin_elem,end_elem;

     _p_fem_core_proxy->GetElement(BEGIN,begin_elem);
     _p_fem_core_proxy->GetElement(END,end_elem);

     for (iter_elem = begin_elem ; iter_elem != end_elem ; iter_elem++)
     {
          TElement &elem = *iter_elem;

          node_id_t n1 = **elem.GetNode(0);
          node_id_t n2 = **elem.GetNode(1);
          node_id_t n3 = **elem.GetNode(2);
          node_id_t n4 = -1;

          if (elem.GetNodeCount() == 4)
          {
               n4 =  **elem.GetNode(3);
          }

          local_element_t e;
          e.n1 = n1;
          e.n2 = n2;
          e.n3 = n3;
          e.n4 = n4;

          elements.push_back(e);

          TNormal enormal;
          p_fem_core_lcs_util->TransformWCS2LCS4Normal(elem.UserData(), enormal);
          element_normals.push_back(enormal);

          if (n4 != -1)
          {
               n2n[n1].insert(n2);
               n2n[n1].insert(n4);

               n2n[n2].insert(n1);
               n2n[n2].insert(n3);

               n2n[n3].insert(n2);
               n2n[n3].insert(n4);

               n2n[n4].insert(n1);
               n2n[n4].insert(n3);
          }
          else
          {
               n2n[n1].insert(n2);
               n2n[n1].insert(n3);

               n2n[n2].insert(n1);
               n2n[n2].insert(n3);

               n2n[n3].insert(n1);
               n2n[n3].insert(n2);
          }
     }

     points.clear();
     n2e.clear();
     n2e.resize(node_end - node_begin + 1);

     node_normals.clear();

     for (TNodeIterator iter_node = node_begin ; iter_node != node_end ; iter_node++)
     {
          const TPoint pt = iter_node->Point();
          const TNormal nnormal = iter_node->UserData();

          TPoint pt_new;
          p_fem_core_lcs_util->TransformWCS2LCS(pt, pt_new);

          TNormal nnormal_new;
          p_fem_core_lcs_util->TransformWCS2LCS4Normal(nnormal, nnormal_new);

          local_point_t local_pt;
          local_pt.x = pt_new.x;
          local_pt.y = pt_new.y;
          local_pt.z = pt_new.z;

          points.push_back(local_pt);
          node_normals.push_back(nnormal_new);

          node_id_t node_id = static_cast<node_id_t>(iter_node - node_begin);

          for (TNode::iterator iter_adjoin_elem = iter_node->GetAdjoinElement(BEGIN) ; 
               iter_adjoin_elem != iter_node->GetAdjoinElement(END) ; iter_adjoin_elem++)
          {
               const int &e = **iter_adjoin_elem;

               n2e[node_id].insert(e);
          }
     }

     e2e.clear();
     e2e.resize(end_elem - begin_elem + 1);

     for(int loop = 0 ; loop < static_cast<int>(elements.size()) ; loop++)
     {
          set<node_id_t> node_ids;

          element_id_t elem_id = loop;

          const node_id_t &n1 = elements[elem_id].n1;
          const node_id_t &n2 = elements[elem_id].n2;
          const node_id_t &n3 = elements[elem_id].n3;
          const node_id_t &n4 = elements[elem_id].n4;

          node_ids.insert(n1);
          node_ids.insert(n2);
          node_ids.insert(n3);
          if(n4 != -1)node_ids.insert(n4);

          set<element_id_t> adjoin_elem_ids;

          for(set<node_id_t>::iterator node_ids_iter = node_ids.begin() ; node_ids_iter != node_ids.end() ; node_ids_iter++)
          {
               const node_id_t &node_id = *node_ids_iter;

               for(set<element_id_t>::iterator node_elem_iter = n2e[node_id].begin() ; 
                   node_elem_iter != n2e[node_id].end() ; node_elem_iter++)
               {
                    const node_id_t &e = *node_elem_iter;
                    adjoin_elem_ids.insert(e);
               }
          }

          for(set<element_id_t>::iterator adjoin_elem_ids_iter = adjoin_elem_ids.begin() ; 
              adjoin_elem_ids_iter != adjoin_elem_ids.end() ; adjoin_elem_ids_iter++)
          {
               element_id_t e = *adjoin_elem_ids_iter;

               if (e == elem_id) continue;

               set<node_id_t> adjoin_elem_node_ids;

               const node_id_t &an1 = elements[e].n1;
               const node_id_t &an2 = elements[e].n2;
               const node_id_t &an3 = elements[e].n3;
               const node_id_t &an4 = elements[e].n4;

               adjoin_elem_node_ids.insert(an1);
               adjoin_elem_node_ids.insert(an2);
               adjoin_elem_node_ids.insert(an3);
               if(an4 != -1)adjoin_elem_node_ids.insert(an4);

               set<node_id_t> insection_node_ids;

               set_intersection(node_ids.begin() , node_ids.end() , 
                                adjoin_elem_node_ids.begin() , adjoin_elem_node_ids.end() ,
                                inserter(insection_node_ids,insection_node_ids.begin()));

               if (insection_node_ids.size() == 2)
               {
                    set<node_id_t>::iterator insection_node_ids_iter_begin = insection_node_ids.begin();
                    node_id_t first = *insection_node_ids_iter_begin;

                    insection_node_ids_iter_begin++;

                    node_id_t second = *insection_node_ids_iter_begin;

                    side_t side(first,second);

                    local_e2e_t adjoin_elem;

                    adjoin_elem.elem_id = e;
                    adjoin_elem.first   = first;
                    adjoin_elem.second  = second;

                    e2e[elem_id].insert(adjoin_elem);
               }
          }
     }

     p_fem_core_lcs_util->Release();
}

#endif

}//namespace fem.

#endif
