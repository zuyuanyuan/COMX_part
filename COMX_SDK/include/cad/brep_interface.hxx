
//mesher_dlut_brep.h
#ifndef  __MESHER_BREP_HXX_26_03_2007_
#define  __MESHER_BREP_HXX_26_03_2007_

/*==============================================================================

Copyright 2006 Jilin Kingmesh Corp.
All rights reserved

================================================================================ 
File description:
-- Please append file description informations here --
Curve3d:   三维参数曲线, 是一条剪切曲线,即参数域是有界的
Surface:   三维参数曲面, 是一条剪切曲面,即参数域是有界的

B-Rep:
BrEntity:  B-Rep对象的基类, 用一个整形值来标志其唯一性;
BrVertex:  B-Rep顶点
BrEdge:    B-Rep边，每一条边和模型中的一条三维参数曲线相关联
BrLoop:    B-Rep环，除了周期性的曲面上的环外，一般来说环是封闭的，由有向边边构成。
                环有方向，且左侧为曲面待剖分部分
BrFace:    B-Rep面，面中包含多个环，所有拓扑面指向一致，即同指向体内或体外
================================================================================
Date            Name            Description of Change
2007/03/19	  Zheng guojun      Create
2007/04/30      Di ychi         completed
2007/05/16	  Zheng guojun	    Modifyed visitor interface
2010/03/12    Xu Jinting        Updata
$HISTORY$
================================================================================
*/

#include <base\root.hxx>
#include <base\type.hxx>
// #include "/comx_sdk\include/base/root.hxx"
// #include "/comx_sdk/include/base/type.hxx"

using namespace KMAS;


//! 标识拓扑对象的方向
//! BrOrientation 枚举标识两个对象之间的相对关系
//! 比如一条拓扑边和对应的几何边，如果其起点一致，
//! 那么相对关系为: Br_FORWARD,否则为Br_REVERSED
enum BrOrientation 
{ 
	Br_FORWARD  = 0,
	Br_REVERSED
};

enum BrLoopType
{
	Unclassified = -1,
	Exterior,		//外环，即参数域外边界环
	Interior,		//内环，即参数域边界内的环
	Pointer			//点环，比如圆锥顶点
};

enum ENTITYTYPE
{
	TBRVERTEX = 1, TBREDGE, TBRCOEDGE, TBRLOOP, TBRFACE, TBRWIRE, TBRSUBSHELL, TBRSHELL, 
	TBRLUMP, TBRBODY, TBRENTITYLIST, TCURVE, TSURFACE
};

namespace KMAS
{
	namespace Die_maker
	{
		namespace comx
		{
			//三维参数曲线 {9A21B3DF-76E7-4bee-8D1D-67DC88B22F8C}
			const MUID IID_ICurve3d = 
			{ 0x9a21b3df, 0x76e7, 0x4bee, { 0x8d, 0x1d, 0x67, 0xdc, 0x88, 0xb2, 0x2f, 0x8c } };

			interface ICurve3d : public IRoot
			{
				virtual TStatus STDCALL CreateLine( const point_t& pos, const unit_vector3d_t& dir ) = 0;

				virtual TStatus STDCALL CreateEllipse( const point_t& center, const unit_vector3d_t& normal, 
					                                   const vector3d_t& majorAxis, double radiusRatio = 1.0 ) = 0;

				virtual TStatus STDCALL CreateIntCurve( const ICurve3d& cur3d ) = 0;

				virtual TStatus STDCALL CreateIntCurve( int degree, bool isRational, bool isClosed, bool isPeriodic, 
					                                    int numCtrlpts, const point_t ctrlPts[], const double Weights[], 
														double pointTol, int numKnots, const double knots[], 
														double knotTol ) = 0;

				virtual TStatus STDCALL CreateCompCurve( const ICurve3d& cur3d ) = 0;

				virtual TStatus STDCALL Destroy() = 0;


				virtual TStatus STDCALL ArcLength ( const interval_t& paramInterval, double& len, bool& is_succeed ) = 0; 

				virtual TStatus STDCALL ArcLengthParam ( const double &param1, const double &len, double &param2, bool &is_succeed ) = 0;

				virtual TStatus STDCALL BoundBox ( bound_box_t & box ) = 0;

				virtual TStatus STDCALL CalculateCurvature ( const double& param, vector3d_t& cvtVector ) = 0; 

				virtual TStatus STDCALL CalculateCurvature ( const point_t& pos, vector3d_t& cvtVector ) = 0; 

				virtual TStatus STDCALL CalculatePosParameter ( const point_t &pos, double &val ) =0; 

				virtual TStatus STDCALL ClosestPointOnCurve ( const point_t &pos, point_t &clsPos ) = 0;

				virtual TStatus STDCALL Discontinuities ( const double*& ptrParam, int &nDiscont, const int &order ) =0; 

				virtual TStatus STDCALL DiscontinuousAtPos ( const double& param, int& nOrder ) =0; 

				virtual TStatus STDCALL IsClosed ( bool &closed ) =0; 

				virtual TStatus STDCALL Split ( const double& param, ICurve3d*& cur1, ICurve3d*& cur2, bool &isSucceed ) = 0;

				virtual TStatus STDCALL GetInterval ( interval_t& interv ) = 0;

				virtual TStatus STDCALL GetPointOnCurve ( const double& param, point_t& pos ) = 0;

				virtual TStatus STDCALL FirstDeriv ( double param, vector3d_t& dVector ) = 0;

				virtual TStatus STDCALL SecondDeriv ( double param, vector3d_t& dVector ) = 0;
				
				virtual TStatus STDCALL IsPeriodic ( double& period, bool& isPer ) = 0;
			};

			// {10048AE7-564C-4a96-BB5F-66EC79DDED10}
			const MUID IID_ISurface = 
			{ 0x10048ae7, 0x564c, 0x4a96, { 0xbb, 0x5f, 0x66, 0xec, 0x79, 0xdd, 0xed, 0x10 } };

			interface ISurface : public IRoot
			{
				virtual TStatus STDCALL CreatePlane( const point_t& pos, const unit_vector3d_t &normal ) = 0;

				virtual TStatus STDCALL CreateCone( const point_t& center, const unit_vector3d_t& normal, 
					        const vector3d_t& majorAxis, double radiusRatio=1, double sineAngle=0, double cosAngle=1 ) = 0;

				virtual TStatus STDCALL CreateSphere( const point_t& center, double radius ) = 0;

				virtual TStatus STDCALL CreateTorus( const point_t& center, const unit_vector3d_t& normal, 
					                                 double majorRadius, double minorRadius ) = 0;

				virtual TStatus STDCALL CreateSplineSurface( ISurface& surf ) = 0;

				virtual TStatus STDCALL CreateSurfaceApproxSurface( const ISurface* inSurface, const double tol, 
					                                                const interval_t &uRange, const interval_t& vRange ) = 0;

				virtual TStatus STDCALL Destroy() = 0;


				virtual TStatus STDCALL BoundBox ( bound_box_t &box ) = 0;	
				
				virtual TStatus STDCALL ClosestPointOnSurface( const point_t &pos, point_t &foot ) = 0;
				
				virtual TStatus STDCALL Discontinuities_u ( const double*& ptrParam, int &nDiscont, int order ) = 0;
				
				virtual TStatus STDCALL Discontinuities_v ( const double*& ptrParam, int &nDiscont, int order ) = 0;
				
				virtual TStatus STDCALL Discontinuous_at_u ( const double& uParam, int& nOrder ) = 0; 
				
				virtual TStatus STDCALL Discontinuous_at_v ( const double& vParam, int& nOrder ) = 0; 
				
				virtual TStatus STDCALL CalCrossSectionCurvature (const point2d_t &parPos, const vector3d_t &plnNormal, double& cvt) = 0; 
				
				virtual TStatus STDCALL CalCrossSectionCurvature ( const double &uParam, const double &vParam, 
					                                                const vector3d_t &plnNormal, double& cvt ) = 0; 

				virtual TStatus STDCALL CalPointOnSurface ( const double& uParam, const double& vParam, point_t& pt ) = 0;

				virtual TStatus STDCALL CalPointOnSurface ( const point2d_t& parPos, point_t& pt ) = 0;

				virtual TStatus STDCALL CalParamOfPosOnSurface ( const point_t& pos3d, point2d_t& parPos, bool& isSucceed ) = 0;
				
				virtual TStatus STDCALL CalPrinCurvature ( const point2d_t &parPos, surf_cvt& cvt ) = 0; 
				
				virtual TStatus STDCALL CalPrinCurvature ( const double& uParam, const double& vParam, surf_cvt& cvt ) = 0;

				virtual TStatus STDCALL CalSurfaceNormal ( const point2d_t &parPos, vector3d_t& surNormal ) = 0; 

				virtual TStatus STDCALL CalSurfaceNormal ( const double& uParam, const double& vParam, vector3d_t& surNormal ) = 0;

				virtual TStatus STDCALL GetInterval ( interval_t& uInterval, interval_t& vInterval ) = 0;

				virtual TStatus STDCALL GetInterval_u ( interval_t& uInterval ) = 0;

				virtual TStatus STDCALL GetInterval_v ( interval_t& vInterval ) = 0;
				
				virtual TStatus STDCALL IsClosed ( bool &closed ) =0; 
				
				virtual TStatus STDCALL IsClosed_u ( bool &closed ) = 0; 
				
				virtual TStatus STDCALL IsClosed_v ( bool &closed ) = 0; 
				
				virtual TStatus STDCALL IsPointOnSurface ( const point_t& pos3d, bool &flag, const double& tol = 1e-10 ) = 0;
				
				virtual TStatus STDCALL OutDirection ( const point2d_t &parPos, vector3d_t& dir ) = 0;  
				
				virtual TStatus STDCALL OutDirection ( const double &uParam, const double &vParam, vector3d_t& dir ) = 0;
				
				virtual TStatus STDCALL FirstDeriv ( double uParam, double vParam, vector3d_t& uDeriv, vector3d_t& vDeriv, bool& isSucceed ) = 0;

				virtual TStatus STDCALL FirstDeriv ( const point2d_t& parPos, vector3d_t& uDeriv, vector3d_t& vDeriv, bool& isSucceed ) = 0;
				
				virtual TStatus STDCALL SecondDeriv ( double uParam, double vParam, vector3d_t& uuDeriv, 
					                                  vector3d_t& uvDeriv, vector3d_t& vvDeriv, bool& isSucceed ) = 0;
				
				virtual TStatus STDCALL Periodic ( bool& isPeriod_u, double& uPeriod, bool& isPeriod_v, double& vPeriod ) = 0;

				virtual TStatus STDCALL Periodic_u ( bool& isPeriod_u, double& uPeriod ) = 0;

				virtual TStatus STDCALL Periodic_v ( bool& isPeriod_v, double& vPeriod ) = 0;
			};

			//	declare interface .
			struct IBrVertexVisitor;
			struct IBrEdgeVisitor;
			struct IBrLoopVisitor;
			struct IBrFaceVisitor;
			struct IBrWireVisitor;
			struct IBrShellVisitor;
			struct IBrLumpVisitor;
			struct IBrBodyVisitor;

			interface IBrEdge;
			interface IBrCoEdge;
			interface IBrLoop;
			interface IBrWire;
			interface IBrFace;
			interface IBrSubShell;
			interface IBrShell;
			interface IBrLump;
			interface IBrBody;

			// {71E8481D-75CD-4aa1-8D60-70AE4BFD97EB}
			const MUID IID_IBrEntity = 
			{ 0x71e8481d, 0x75cd, 0x4aa1, { 0x8d, 0x60, 0x70, 0xae, 0x4b, 0xfd, 0x97, 0xeb } };

			interface IBrEntity : public IRoot
			{
				virtual TStatus STDCALL GetIndex(int& index) = 0;
				virtual TStatus STDCALL AttachInnerEnt(unsigned long ent){assert(false); return M_FAIL;}
				virtual TStatus STDCALL GetInnerEnt(unsigned long &ent){assert(false); return M_FAIL;}
			};

			// {29346C3B-E76F-4a89-AC03-37868FE37F2B}
			const MUID IID_IBrVertex = 
			{ 0x29346c3b, 0xe76f, 0x4a89, { 0xac, 0x3, 0x37, 0x86, 0x8f, 0xe3, 0x7f, 0x2b } };

			interface IBrVertex : public IBrEntity
			{
				virtual TStatus STDCALL AddEdge ( IBrEdge *edge ) = 0;

				virtual TStatus STDCALL CountEdges ( int& nEdge ) = 0;

				virtual TStatus STDCALL CreateVertex( const point_t& pos ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL DeleteEdge ( IBrEdge *edge ) = 0;

				virtual TStatus STDCALL GetEdge ( int i, IBrEdge*& edge ) = 0; 

				virtual TStatus STDCALL GetPosition ( point_t& pt, bool& isSucceed ) = 0;

				virtual TStatus STDCALL IsEdgeLinked ( IBrEdge *edge, bool& isSucceed ) = 0;  

			};

			// {D82BDA48-91BE-4b6f-AE81-A5F7DE7613C3}
			const MUID IID_IBrEdge = 
			{ 0xd82bda48, 0x91be, 0x4b6f, { 0xae, 0x81, 0xa5, 0xf7, 0xde, 0x76, 0x13, 0xc3 } };

			interface IBrEdge : public IBrEntity
			{
				virtual TStatus STDCALL Accept ( IBrVertexVisitor* pvi ) = 0;

				virtual	TStatus STDCALL BoundBox ( bound_box_t& box ) = 0;  

				virtual TStatus STDCALL CreateEdge( IBrVertex *starVertex, IBrVertex *endVertex, ICurve3d *curv, BrOrientation sense ) = 0;

				virtual TStatus STDCALL CreateEdgeFromLine( const point_t &startPoint, const point_t &endPoint ) = 0;
				
				virtual TStatus STDCALL CreateEdgeFromSpline( int degree, bool isRational, bool isClosed, bool isPeriodic, 
					                                          int numCtrlpts, const point_t ctrlPts[], const double Weights[], 
					                                          double pointTol, int numKnots, const double knots[], 
					                                          double knotTol ) = 0;

				virtual TStatus STDCALL CreateEdgeFromSpline( int numpts, const point_t *pts, const unit_vector3d_t *start, 
					                                          const unit_vector3d_t *end, bool isApprox=TRUE, bool isPeriodic=FALSE ) = 0;

				virtual TStatus STDCALL CreateEdgeFromSpline2( int numpts, const point_t *pts, const double *params, 
					                                          const vector3d_t *start, const vector3d_t *end ) = 0;

				virtual TStatus STDCALL CreateEdgeFromCubicSpline( int npts, const point_t pts[], const unit_vector3d_t& startDir, 
					                                               const unit_vector3d_t& endDir, double fitol ) = 0;

				virtual TStatus STDCALL CreateEdgeFromArc( const point_t& center, double radius, double startAngle, 
					                                       double endAngle ) = 0;

				virtual TStatus STDCALL CreateEdgeFrom3PtArc( const point_t& pt1, const point_t& pt2, 
					                                          const point_t& pt3, bool isFull ) = 0;

				virtual TStatus STDCALL CreateEdgeFromCenterEdge( const point_t& center, const point_t& pt1, 
					                                              const point_t& pt2, const unit_vector3d_t* norm ) = 0;

				virtual TStatus STDCALL CreateEdgeFrom2PtArc( const point_t& pt1, const point_t& pt2, 
					                                            bool isFull ) = 0;

				virtual TStatus STDCALL CreateEdgeFromCubicBezier( const point_t& pt1, const point_t& pt2, 
					                                               const point_t& pt3, const point_t& pt4 ) = 0;

				virtual TStatus STDCALL CreateEdgeFromEllipse( const point_t& center, const point_t& major, 
					                                           double ratio, double startAngle, double endAngle ) = 0;

				virtual TStatus STDCALL CreateEdgeFromSpiral( const point_t& center, const vector3d_t &normal, 
					                                          const vector3d_t& startDirection, double width, double angle, 
															  bool isRight = TRUE, double startRadius = -1.0 ) = 0;

				virtual TStatus STDCALL CreateEdgeFromSpring( const point_t& axisPoint, const vector3d_t& axisVector, 
					                                          const point_t& startPosition, bool isRight, int helixCount, 
															  double* threadDistArray, double* rotationAngleArray, 
															  double* transitionHeightArray, double* transitionAngleArray ) = 0;

				virtual TStatus STDCALL CreateEdgeFromConic( const point_t& start, const unit_vector3d_t& startTan, 
					                                         const point_t& end,   const unit_vector3d_t& endTan, 
															 double rho ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL EndDerivs ( vector3d_t& deriv ) = 0;

				virtual TStatus STDCALL EndParameter ( double &param ) = 0;
				
				virtual	TStatus STDCALL EndPos ( point_t& pos ) = 0;
				
				virtual	TStatus STDCALL EndVertex ( IBrVertex*& vertex ) = 0; 
				
				virtual	TStatus STDCALL GetCurve ( ICurve3d*& cur ) = 0;
				
				virtual	TStatus STDCALL GetOrient ( BrOrientation& orient ) = 0;
				
				virtual	TStatus STDCALL GetParamRange ( interval_t& intervalParam ) = 0; 
				
				virtual	TStatus STDCALL Length ( double &len ) = 0;

				virtual TStatus STDCALL IsClosed ( bool& flag_closed ) = 0;

				virtual TStatus STDCALL IsPeriodic ( bool& isPer ) = 0;
				
				virtual	TStatus STDCALL MidDerivs ( vector3d_t& deriv ) = 0;
				
				virtual	TStatus STDCALL MidPos ( point_t& pt ) = 0;
				
				virtual TStatus STDCALL StartDerivs ( vector3d_t& deriv ) = 0;
				
				virtual TStatus STDCALL StartParameter ( double &param ) = 0;
				
				virtual	TStatus STDCALL StartPos ( point_t& pos ) = 0; 
				
				virtual	TStatus STDCALL StartVertex ( IBrVertex* vertex ) = 0;
			};

			// {5F0F2838-28B5-467b-8A06-7DF115FC3A42}
			const MUID IID_IBrCoEdge = 
			{ 0x5f0f2838, 0x28b5, 0x467b, { 0x8a, 0x6, 0x7d, 0xf1, 0x15, 0xfc, 0x3a, 0x42 } };

			interface IBrCoEdge : public IBrEntity
			{
				virtual TStatus STDCALL Accept( IBrEdgeVisitor* pvi ) = 0; 

				virtual TStatus STDCALL CreateCoEdge( IBrEdge* edge, BrOrientation sense, IBrCoEdge *prev, IBrCoEdge *next ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL EndVertex( IBrVertex*& brVertex ) = 0;

				virtual TStatus STDCALL EndParameter( double& param ) = 0;

				virtual TStatus STDCALL EndPosition( point_t& pos3d ) = 0;

				virtual TStatus STDCALL GetEdge( IBrEdge*& brEdge ) = 0;

				virtual TStatus STDCALL GetCurve( ICurve3d*& tCurve ) = 0;

				virtual	TStatus STDCALL GetOrient ( BrOrientation& orient ) = 0;

				virtual TStatus STDCALL GetParaRange( interval_t& parInterval) = 0;

				virtual TStatus STDCALL LoopPtr( IBrLoop*& brLoop ) = 0;

				virtual TStatus STDCALL NextCoEdge( IBrCoEdge*& brCoEdge ) = 0;

				virtual TStatus STDCALL PreviousCoEdge( IBrCoEdge*& brCoEdge ) = 0;

				virtual TStatus STDCALL StartVertex( IBrVertex*& brVertex ) = 0;

				virtual TStatus STDCALL StartParameter( double& param ) = 0;

				virtual TStatus STDCALL StartPosition( point_t& pos3d ) = 0;

				virtual TStatus STDCALL WirePtr( IBrWire*& wirePtr ) = 0;

				virtual TStatus STDCALL SetPrevious( IBrCoEdge *brCoEdge ) = 0;
			};

			// {72105B2F-D954-4a6a-A839-5F04A4031D23}
			const MUID IID_IBrLoop= 
			{ 0x72105b2f, 0xd954, 0x4a6a, { 0xa8, 0x39, 0x5f, 0x4, 0xa4, 0x3, 0x1d, 0x23 } };

			interface IBrLoop : public IBrEntity
			{
				virtual TStatus STDCALL Accept( IBrEdgeVisitor* pvi ) = 0;
				
				virtual TStatus STDCALL BoundBox ( bound_box_t& box ) = 0;

				virtual TStatus STDCALL CreateLoop( IBrCoEdge *coedge, IBrLoop *loop ) = 0;
				
				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL FacePtr( IBrFace*& ptrFace, bool& isSucceed ) = 0;  
				
				virtual TStatus STDCALL GetType( point_t& pt, BrLoopType& type ) = 0;

				virtual TStatus STDCALL NextLoop ( IBrLoop*& loopPtr ) = 0; 

				virtual TStatus STDCALL Start( IBrCoEdge*& coedge ) = 0;

			};

			// {78F24F59-4A54-4f4f-A053-2080CAEE100B}
			const MUID IID_IBrFace = 
			{ 0x8cfb432a, 0x48e4, 0x4ced, { 0xb9, 0x60, 0xad, 0x2f, 0x8d, 0x55, 0xf3, 0xbd } };

			interface IBrFace : public IBrEntity
			{
				virtual TStatus STDCALL Accept ( IBrLoopVisitor* pvi ) = 0;
				
				virtual TStatus STDCALL BoundBox (bound_box_t& box ) = 0;

				virtual TStatus STDCALL CreateFace( IBrLoop* loop, IBrFace* face, ISurface* surf, BrOrientation sense ) = 0;

				virtual TStatus STDCALL CreateFaceCylinderCone( const point_t& center, const vector3d_t& axisNormal, 
					                                            double botRadius, double topRadius, double startAngle, 
																double endAngle,  double ratio, const point_t *pt ) = 0; 

				virtual TStatus STDCALL CreateFacePlane( const point_t& pt, double width, double height, 
					                                     const vector3d_t *normal ) = 0;

				virtual TStatus STDCALL CreateFaceSphere( const point_t& centPos, double radius, double LongitudeStart, 
					                                      double LongitudeEnd, double LatitudeStart, double LatitudeEnd, 
														  const vector3d_t *axisNormal ) = 0; 

				virtual TStatus STDCALL CreateFaceTorus( const point_t& centPos, double major, double minor, double tubeStartAngle, 
					                                     double tubeEndAngle, double splineStartAngle, double splineEndAngle, 
														 const vector3d_t* axisNormal ) = 0;

				virtual TStatus STDCALL CreateFaceFromSurface( ISurface* fromSurface ) = 0;

				virtual TStatus STDCALL CreateFacePlanarDisk( const point_t& origin, const unit_vector3d_t& normal, double radius ) = 0;

				virtual TStatus STDCALL CreateFaceParallelogram( const point_t& origin, const point_t& left, const point_t& right ) = 0;

				virtual TStatus STDCALL CreateFaceSplineCtrlPts( int uDegree, bool isRational_U, int uForm, int uPole, int numCtrlpts_U, 
																 int vDegree, bool isRational_V, int vForm, int vPole, int numCtrlpts_V, 
																 const point_t ctrlpts[], const double weights[], double pointTol, 
																 int numKnots_u, const double knots_u[], 
																 int numKnots_v, const double knots_v[], double knotTol ) = 0; 

				virtual TStatus STDCALL CreateFaceFitSpline( double fittol, int numPts_u, int numPts_v, const point_t pts[], 
					                                         const unit_vector3d_t du_s[], const unit_vector3d_t du_e[] ) = 0;

				virtual TStatus STDCALL CreateFaceInterpolateSpline( int numPts_u, int numPts_v, const point_t pts[], 
					                                                 const unit_vector3d_t du_s[], const unit_vector3d_t du_e[],
																	 const unit_vector3d_t dv_s[], const unit_vector3d_t dv_e[] ) = 0;

				virtual TStatus STDCALL CreateFaceFromSpline( const ISurface* tSurface, const ICurve3d* lv=(const ICurve3d* )NULL, 
					                                          const ICurve3d* hv=(const ICurve3d* )NULL, const ICurve3d* lu=(const ICurve3d* )NULL, 
															  const ICurve3d* hu=(const ICurve3d* )NULL, const par_box_t* pb=( const par_box_t* )NULL ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL FacePtr( IBrFace*& facePtr ) = 0;

				virtual TStatus STDCALL FacePtrInList( IBrFace*& facePtr ) = 0;
				
				virtual TStatus STDCALL LoopPtr ( IBrLoop* loop ) = 0;	 
				
				virtual TStatus STDCALL GetSurface (ISurface*& surPtr, bool& isSucceed ) = 0;
				
				virtual TStatus STDCALL Orientation ( bool &isOrient ) = 0;

				virtual TStatus STDCALL SubShellPtr( IBrSubShell*& subShellPtr ) = 0;

				virtual TStatus STDCALL ShellPtr( IBrShell*& shellPtr ) = 0;
			};

			// {82FD8BC3-8491-41f8-8F33-B28575ADFD08}
			const MUID IID_IBrWire = 
			{ 0x82fd8bc3, 0x8491, 0x41f8, { 0x8f, 0x33, 0xb2, 0x85, 0x75, 0xad, 0xfd, 0x8 } };

			interface IBrWire : public IBrEntity
			{
				virtual TStatus STDCALL Accept( IBrEdgeVisitor *pvi ) = 0;

				virtual TStatus STDCALL BodyPtr( IBrBody *&brBody ) = 0;

				virtual TStatus STDCALL BoundBox( bound_box_t& box ) = 0;

				virtual TStatus STDCALL CoEdgePtr( IBrCoEdge*& brCoEdge ) = 0;

				virtual TStatus STDCALL CreateWire( IBrCoEdge* coedge, IBrWire* wire ) = 0;

				virtual TStatus STDCALL CreateEdgeWire( int numEdges, IBrEdge* edges[] ) = 0;

				virtual TStatus STDCALL CreateEdgeWire( int numEdges, IBrEdge *edges[], int &nBodies ) = 0;

				virtual TStatus STDCALL CreatePolygonWire( point_t center, vector3d_t start, 
					                                       vector3d_t &normal, double &side_length, int number_of_sides=6, 
														   bool oncenter=FALSE ) = 0;

				virtual TStatus STDCALL Destroy() = 0;
 
				virtual TStatus STDCALL NextWirePtr( IBrWire*& brWire ) = 0;

				virtual TStatus STDCALL NextWireInList( IBrWire*& brWire ) = 0;

				virtual TStatus STDCALL ShellPtr( IBrShell*& brShell ) = 0;

				virtual TStatus STDCALL SubShellPtr( IBrSubShell*& brSubShell ) = 0;
			};

			// {AC1C81A0-E1AA-4b7d-8EB4-B28BB3C4079C}
			const MUID IID_IBrSubShell = 
			{ 0xac1c81a0, 0xe1aa, 0x4b7d, { 0x8e, 0xb4, 0xb2, 0x8b, 0xb3, 0xc4, 0x7, 0x9c } };

			interface IBrSubShell : public IBrEntity
			{
				virtual TStatus STDCALL BoundBox( bound_box_t& box ) = 0;

				virtual TStatus STDCALL CreateSubShell( IBrFace* face, IBrSubShell* child, IBrSubShell* sibling ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL FacePtr( IBrFace*& facePtr ) = 0;

				virtual TStatus STDCALL FacePtrInList( IBrFace*& facePtr ) = 0;

				virtual TStatus STDCALL WirePtr( IBrWire*& wirePtr ) = 0;

				virtual TStatus STDCALL WirePtrInList( IBrWire*& wirePtr ) = 0;

			};

			// {87E22C15-9185-45b1-858E-FE14FB39500A}
			const MUID IID_IBrShell = 
			{ 0x87e22c15, 0x9185, 0x45b1, { 0x85, 0x8e, 0xfe, 0x14, 0xfb, 0x39, 0x50, 0xa } };

			interface IBrShell : public IBrEntity
			{
				virtual TStatus STDCALL Accept( IBrFaceVisitor *pvi ) = 0;

				virtual TStatus STDCALL Accept( IBrWireVisitor *pvi ) = 0;

				virtual TStatus STDCALL BoundBox( bound_box_t& box ) = 0;

				virtual TStatus STDCALL CreateShell( IBrFace* face, IBrSubShell* subshell, IBrShell* shell ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL FacePtr( IBrFace*& facePtr ) = 0;

				virtual TStatus STDCALL FacePtrInList( IBrFace*& facePtr ) = 0;

				virtual TStatus STDCALL IsClosed( bool& isClosed ) = 0;

				virtual TStatus STDCALL LumpPtr( IBrLump*& lumpPtr ) = 0;

				virtual TStatus STDCALL NextShellPtr( IBrShell*& shellPtr ) = 0;

				virtual TStatus STDCALL SubShellPtr( IBrSubShell*& subShellPtr ) = 0;

				virtual TStatus STDCALL WirePtr( IBrWire*& wirePtr ) = 0;

				virtual TStatus STDCALL WirePtrInList( IBrWire*& wirePtr ) = 0;
			};

			// {4CD50971-8F47-4eee-80F5-3B31FD3E5D5B}
			const MUID IID_IBrLump = 
			{ 0x4cd50971, 0x8f47, 0x4eee, { 0x80, 0xf5, 0x3b, 0x31, 0xfd, 0x3e, 0x5d, 0x5b } };

			interface IBrLump : public IBrEntity
			{
				virtual TStatus STDCALL BodyPtr( IBrBody *&brBody ) = 0;

				virtual TStatus STDCALL BoundBox( bound_box_t& box ) = 0;

				virtual TStatus STDCALL CreateLump( IBrShell* shell, IBrLump* lump ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL NextLumpPtr( IBrLump*& lumpPtr ) = 0;

				virtual TStatus STDCALL ShellPtr( IBrShell*& shellPtr ) = 0;

			};

			// {B67C14EA-5D03-44e8-A3B0-8153A557D216}
			const MUID IID_IBrBody = 
			{ 0xb67c14ea, 0x5d03, 0x44e8, { 0xa3, 0xb0, 0x81, 0x53, 0xa5, 0x57, 0xd2, 0x16 } };

			interface IBrBody : public IBrEntity
			{
				virtual TStatus STDCALL BoundBox( bound_box_t& box ) = 0;

				virtual TStatus STDCALL CreateBody( IBrLump* lump ) = 0;

				virtual TStatus STDCALL Destroy() = 0;

				virtual TStatus STDCALL CreateBody( IBrWire* wire ) = 0;

				virtual TStatus STDCALL CreateCuboid( double x, double y, double z ) = 0; 

				virtual TStatus STDCALL CreateFrustum( double height, double radius1, double radius2, double top ) = 0;

				virtual TStatus STDCALL CreatePrism( double height, double radius1, double radius2, int nsides ) = 0;

				virtual TStatus STDCALL CreatePyramid ( double height, double radius1, double radius2, double top, int nsides ) = 0;

				virtual TStatus STDCALL CreateSphere( double radius ) = 0;

				virtual TStatus STDCALL CreateBodySpline( ISurface* thisSpline ) = 0;

				virtual TStatus STDCALL CreateTorus( double majorRadius, double minorRadius ) = 0;

				virtual TStatus STDCALL CreateSheetFromFF( int numFaces, IBrFace* faces[] ) = 0;

				virtual TStatus STDCALL CreateSolidBlock( const point_t& pt1, const point_t& pt2 ) = 0;

				virtual TStatus STDCALL CreateSolidCylinderCone ( const point_t& pt1, const point_t& pt2, double majorRadius, 
					                                              double minorRadius, double topRadius, 
																  const point_t* xpt ) = 0;

				virtual TStatus STDCALL CreateSolidSphere( const point_t& center, double radius ) = 0;

				virtual TStatus STDCALL CreateSolidTorus( const point_t &center, double majorRadius, double minorRadius ) = 0;

				virtual TStatus STDCALL CreateExampleWiggle( double width, double depth, double height, 
					                                         int low_v_type, int high_v_type, int low_u_type, int high_u_type, 
															 bool height_given ) = 0;

				virtual TStatus STDCALL LumpPtr( IBrLump*& lumpPtr ) = 0;

				virtual TStatus STDCALL WirePtr( IBrWire*& wirePtr ) = 0;
			};

			// {54B673BF-5C98-441f-80F3-734D0E74A07A}
			const MUID IID_IBrEntityList = 
			{ 0x54b673bf, 0x5c98, 0x441f, { 0x80, 0xf3, 0x73, 0x4d, 0xe, 0x74, 0xa0, 0x7a } };

			interface IBrEntityList : public IBrEntity
			{
				virtual TStatus STDCALL Accept(IBrFaceVisitor* pvi) = 0;

				virtual TStatus STDCALL AddEntity ( IBrEntity* ent ) = 0;

				virtual TStatus STDCALL AddEntity ( IBrEntityList* ent ) = 0;

				virtual TStatus STDCALL BoundaryBox ( bound_box_t& box ) = 0;

				virtual TStatus STDCALL ClearList() = 0;

				virtual TStatus STDCALL Count( int& nCount ) = 0;

				virtual TStatus STDCALL Delete ( const int& index ) = 0;

				virtual TStatus STDCALL Delete ( IBrEntity* ent ) = 0;

				virtual TStatus STDCALL GetEntity( const int& index, IBrEntity*& enty ) = 0;	

				virtual TStatus STDCALL FirstEntity( IBrEntity*& enty ) = 0;

				virtual TStatus STDCALL Init() = 0;

				virtual TStatus STDCALL IterationCount( int& nCount ) = 0;

				virtual TStatus STDCALL Reverse() = 0;

				virtual TStatus STDCALL Next( IBrEntity*& enty ) = 0; 

				virtual TStatus STDCALL Load(const char *fname) = 0;

				virtual TStatus STDCALL Save(const char *fname) = 0;
			};

			// {87EAA934-1599-4f07-9148-26D5C7B7D069}
			const MUID IID_IBrFactory = 
			{ 0x87eaa934, 0x1599, 0x4f07, { 0x91, 0x48, 0x26, 0xd5, 0xc7, 0xb7, 0xd0, 0x69 } };

			interface IBrFactory : IRoot
			{
				virtual TStatus STDCALL CreateEntity( ENTITYTYPE type, void** enty ) = 0;
			};
			

			// {76036FE5-8CDD-46de-83B8-0F7B41D2A5FE}
			const MUID IID_IBrComposite = 
			{ 0x76036fe5, 0x8cdd, 0x46de, { 0x83, 0xb8, 0xf, 0x7b, 0x41, 0xd2, 0xa5, 0xfe } };

			struct IBrVertexVisitor
			{
				virtual TStatus STDCALL Visit(IBrVertex* pvertex, bool& isSucceed) = 0;
			};

			struct IBrEdgeVisitor
			{
				virtual TStatus STDCALL Visit(IBrEdge* pedge, bool& isSucceed) = 0;
			};

			struct IBrLoopVisitor
			{
				virtual TStatus STDCALL Visit(IBrLoop* ploop, bool& isSucceed) = 0;
			};

			struct IBrFaceVisitor
			{
				virtual TStatus STDCALL Visit(IBrFace* pface, bool& isSucceed) = 0;
			};

			struct IBrWireVisitor 
			{
				virtual TStatus STDCALL Visit(IBrWire* pwire, bool& isSucceed) = 0;
			};

			struct IBrShellVisitor 
			{
				virtual TStatus STDCALL Visit(IBrShell* pshell, bool& isSucceed) = 0;
			};

			struct IBrLumpVisitor 
			{
				virtual TStatus STDCALL Visit(IBrLump* plump, bool& isSucceed) = 0;
			};

			struct IBrBodyVisitor 
			{
				virtual TStatus STDCALL Visit(IBrBody* pbody, bool& isSucceed) = 0;
			};

			// {233856BE-9D19-414d-9A90-D3723D4D310D}
			const MUID CLSID_IBrComposite = 
			{ 0x233856be, 0x9d19, 0x414d, { 0x9a, 0x90, 0xd3, 0x72, 0x3d, 0x4d, 0x31, 0x1d } };

			///布尔操作
			// {9417C0C2-2536-4e17-8763-81CFA2CF2AEC}
			const MUID IID_IBrBooleans = 
			{ 0x9417c0c2, 0x2536, 0x4e17, { 0x87, 0x63, 0x81, 0xcf, 0xa2, 0xcf, 0x2a, 0xec } };
			
			interface IBrBooleans : IRoot
			{
				virtual TStatus STDCALL Unite ( IBrBody* tool, IBrBody* blank ) = 0;

				virtual TStatus STDCALL Subtract ( IBrBody* tool, IBrBody* blank ) = 0;

				virtual TStatus STDCALL Intersect ( IBrBody* tool, IBrBody* blank ) = 0;
			};

			// {D2794D88-578F-4efc-A9DC-180A23004403}
			const MUID IID_IBrTransform = 
			{ 0xd2794d88, 0x578f, 0x4efc, { 0xa9, 0xdc, 0x18, 0xa, 0x23, 0x0, 0x44, 0x3 } };

			interface IBrTransform : IRoot
			{
				virtual TStatus STDCALL RotateEntity( IBrEntity* enty, double angle, const vector3d_t& rotAxis ) = 0;

				virtual TStatus STDCALL TranslateEntity( IBrEntity* enty, const vector3d_t& trsVector ) = 0;

				virtual TStatus STDCALL ScaleEntity( IBrEntity* enty, double factor ) = 0;

				virtual TStatus STDCALL ReflectEntity ( IBrEntity* enty, const vector3d_t& normal ) = 0;
			};

		} // namespace comx
	} // namespace Die_maker
} // namespace KMAS

#endif