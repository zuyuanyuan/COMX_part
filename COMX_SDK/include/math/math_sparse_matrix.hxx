/*HEAD math_sparse_matrix HXX ONESTEP */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --

	  The mainly reason for this component is for wrapping some iterative 
	algorithm that is used to solve large sparse matrix. In other words, 
	it is used to solve Ax = b.


================================================================================
Date            Name            Description of Change
12/22/2006      ZhangXK         Created.
$HISTORY$
================================================================================
*/

#ifndef _KMAS_COMX_MATH_SPARSE_MATRIX_H_200612222247
#define _KMAS_COMX_MATH_SPARSE_MATRIX_H_200612222247

#include "base\root.hxx"


namespace KMAS
{
namespace Die_maker
{
namespace comx
{

enum TMathSparseMatrixSolverPrecond
{
	 PRECOND_LDLT               =       0,
     PRECOND_CHOLESKY			=		1,
     PRECOND_SSOR				=		2,
     PRECOND_ILU				=		3,
     PRECOND_ILUT				=		4,
	 PRECOND_NIL                =       5
};

enum TMathSparseMatrixSolverType
{
     SOLVER_CG			=		0,
     SOLVER_GMRES		=		1,
     SOLVER_BICG		=		2,
     SOLVER_QMR			=		3,
     SOLVER_TFQMR		=		4,
     SOLVER_CGS			=		5,
     SOLVER_BICGSTAB	=		6,
     SOLVER_GCR			=		7,
     SOLVER_RICHARDSON	=		8,
	 SOLVER_SOR         =       9
};

enum TMathSparseMatrixType
{
     UNSYMMETRIC	=		0,
     SYMMETRIC		=		1,
};

struct TMathSparseMatrixSolverStatus
{
     int iteration_step;
     double residual;
     int error_code;
};

// {1D3BE6F5-C0A6-435c-9C00-D8E3B1ED8717}
const MUID IID_IMathSparseMatrix = 
{ 0x1d3be6f5, 0xc0a6, 0x435c, { 0x9c, 0x0, 0xd8, 0xe3, 0xb1, 0xed, 0x87, 0x17 } };

interface IMathSparseMatrix : public IRoot
{
     virtual TStatus STDCALL Create(const TMathSparseMatrixType& type, const int& n) = 0;
     virtual TStatus STDCALL Destroy() = 0;
     virtual TStatus STDCALL Clean() = 0;
     virtual TStatus STDCALL Delta(const double val, const int& i, const int& j) = 0;
     virtual TStatus STDCALL Deltas(double* vals, int* i, int* j, const int len) = 0;
	 virtual TStatus STDCALL Deltas(double* vals, int* j, int* iindex, const int vlen,const int iilen) = 0; 
     virtual TStatus STDCALL Update(const double val, const int i, const int j) = 0;
     virtual TStatus STDCALL Updates(double* vals, int* i, int* j, const int len) = 0;
	 virtual TStatus STDCALL Updates(double* vals, int* j, int* iindex, const int vlen,const int iilen) = 0; 
     virtual TStatus STDCALL IsInit(bool& flag) = 0;
};

// {6311FD19-7730-4611-A553-17A0473FD717}
const MUID IID_IMathSparseMatrix2 = 
{ 0x6311fd19, 0x7730, 0x4611, { 0xa5, 0x53, 0x17, 0xa0, 0x47, 0x3f, 0xd7, 0x17 } };

interface IMathSparseMatrix2 : public IMathSparseMatrix
{
	virtual TStatus STDCALL GetValue(double *vals, int *i, int *j, const int len) = 0;
};

// {C2224AED-2FC4-4ffa-A2EC-ED25B8E87938}
const MUID IID_IMathSparseMatrixSolver = 
{ 0xc2224aed, 0x2fc4, 0x4ffa, { 0xa2, 0xec, 0xed, 0x25, 0xb8, 0xe8, 0x79, 0x38 } };

interface IMathSparseMatrixSolver : public IRoot
{
     virtual TStatus STDCALL Solver(const TMathSparseMatrixSolverType& solver_type, const TMathSparseMatrixSolverPrecond& precond_type, double* b, double* x) = 0;
     virtual TStatus STDCALL SetMaxIterationStep(const int& step) = 0;
     virtual TStatus STDCALL SetTolerance(const double tol) = 0;
     virtual TStatus STDCALL GetStatus(TMathSparseMatrixSolverStatus& status) = 0;
};

// {99BB6E3D-FF6C-4b5c-BDB6-1828A08F0CB0}
const MUID IID_IMathSparseMatrixSolver2 = 
{ 0x99bb6e3d, 0xff6c, 0x4b5c, { 0xbd, 0xb6, 0x18, 0x28, 0xa0, 0x8f, 0xc, 0xb0 } };

interface IMathSparseMatrixSolver2 : public IMathSparseMatrixSolver
{
	virtual TStatus STDCALL EnableProgressbar(const bool flag) = 0;
	virtual TStatus STDCALL SetLogFile(const char *fname) = 0;
};

// {945A6464-7E11-4b86-A732-6A45B902D74B}
const MTYPEID CLSID_IMathSparseMatrix = 
{ 0x945a6464, 0x7e11, 0x4b86, { 0xa7, 0x32, 0x6a, 0x45, 0xb9, 0x2, 0xd7, 0x4b } };


}//namespace comx
}//namespace Die_maker
}//namespace KMAS

#endif


