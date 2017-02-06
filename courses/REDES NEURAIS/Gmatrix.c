/**********************************************************************************************
***********************************************************************************************
***  File:			GMatrix.c
***	 Author:		Geovany Araujo Borges
***	 Contents:		Implementation of Matrix type and related functions for use in C and C++.
***					
***********************************************************************************************
**********************************************************************************************/
//#include "stdafx.h" // MSVC6.0 may require this.
#include "Gmatrix.h"

/**********************************************************************************************
***** GMatrix: Includes.
**********************************************************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/**********************************************************************************************
***** GMatrix: Error Handling
**********************************************************************************************/
#define GMATRIX_ERROR(msg) {GMATRIX_PRINTCOMMAND("\n%s",msg); GMATRIX_ABORT_FUNCTION;}

#define GMATRIX_ASSERT(CallingFunction,TestFailCode)\
	if(TestFailCode){ \
		GMATRIX_PRINTCOMMAND("\n*** Warning ( %s ) in function %s",#TestFailCode,CallingFunction);\
		GMATRIX_ABORT_FUNCTION;\
	}

/**********************************************************************************************
***** GMatrix: Dynamic allocation
**********************************************************************************************/
#if GMATRIX_DEFINE_ALLOC
PGMATRIX PGMATRIX_ALLOC(int Nr, int Nc)
{
	PGMATRIX pMatrix;

	pMatrix = (GMATRIX*) malloc(sizeof(GMATRIX));
	GMATRIX_ASSERT("PGMATRIX_ALLOC",pMatrix==NULL);	

	pMatrix->Data = (double*) malloc(Nr*Nc*sizeof(double));
	GMATRIX_ASSERT("PGMATRIX_ALLOC",pMatrix->Data==NULL);	

	pMatrix->Nr = Nr;
	pMatrix->Nc = Nc;
	pMatrix->MaxSize = Nr*Nc;

	return(pMatrix);
}

void PGMATRIX_FREE(PGMATRIX pMatrix)
{
	free(pMatrix->Data);
	free(pMatrix);
}
#endif

/**********************************************************************************************
***** GMatrix: Utilities
**********************************************************************************************/
/** 
 *  PGMATRIX_SETSIZE é uma função...
 */

#if GMATRIX_DEFINE_SETSIZE
void PGMATRIX_SETSIZE(PGMATRIX pMat,int Nll, int Ncc)
{
	GMATRIX_ASSERT("PGMATRIX_SETSIZE",pMat->MaxSize < (Nll*Ncc));	
	pMat->Nr = Nll; pMat->Nc = Ncc; 
}
#endif

#if GMATRIX_DEFINE_PRINT
void PGMATRIX_PRINT_NAMED(char* NameString,PGMATRIX pMat)
{
	int i,j;

	GMATRIX_ASSERT("_PGMATRIX_PRINT",pMat == NULL);	
	GMATRIX_PRINTCOMMAND("\n\t%s:",NameString);	
	for(i=1;i<=pMat->Nr;++i){	
		GMATRIX_PRINTCOMMAND("\n\t   ");	
		for(j=1;j<=pMat->Nc;++j){	
			GMATRIX_PRINTCOMMAND("%c%4.5f ",GMATRIXMACRO_SIGNCHAR(PGMATRIX_DATA(pMat,i,j)),fabs(PGMATRIX_DATA(pMat,i,j)));	
		}	
	}
	GMATRIX_PRINTCOMMAND("\n");	
}
#endif

#if GMATRIX_DEFINE_PRINT_MATLABFORM
void PGMATRIX_PRINT_MATLABFORM_NAMED(char* NameString,PGMATRIX pMat)
{
	int i,j,nc,ncmax,columninc = 4,n;

	GMATRIX_ASSERT("_PGMATRIX_PRINT_MATLABFORM",pMat == NULL);	
	GMATRIX_PRINTCOMMAND("\n%s = ",NameString);
	nc = 1;
	while (nc<=pMat->Nc){
		ncmax = nc + columninc - 1;
		if (ncmax >	pMat->Nc){
			ncmax = pMat->Nc;
		}
		GMATRIX_PRINTCOMMAND("\n  Columns %i through %i\n  ",nc,ncmax);
		for(i=1;i<=pMat->Nr;++i){
			for(j=nc;j<=ncmax;++j){
				if (fabs(PGMATRIX_DATA(pMat,i,j))>=1.0){
					GMATRIX_PRINTCOMMAND(" ");
				}
				if (fabs(PGMATRIX_DATA(pMat,i,j))>0.0){
					for(n=0;n<(4-log10(fabs(PGMATRIX_DATA(pMat,i,j))));++n) GMATRIX_PRINTCOMMAND(" ");
				}
				GMATRIX_PRINTCOMMAND("%c%4.14f ",GMATRIXMACRO_SIGNCHAR(PGMATRIX_DATA(pMat,i,j)),fabs(PGMATRIX_DATA(pMat,i,j)));	
			}	
			if(i!=pMat->Nr){
				GMATRIX_PRINTCOMMAND("\n  ");
			}
		}
		nc += columninc;
	}
	GMATRIX_PRINTCOMMAND("\n");	
}
#endif

#if GMATRIX_DEFINE_PRINT_EXP
void PGMATRIX_PRINT_EXP_NAMED(char* NameString,PGMATRIX pMat)
{
	int i,j;

	GMATRIX_ASSERT("_PGMATRIX_PRINT_EXP",pMat == NULL);	
	GMATRIX_PRINTCOMMAND("\n%s (%i x %i):",NameString,pMat->Nr,pMat->Nc);	
	for(i=1;i<=pMat->Nr;++i){	
		GMATRIX_PRINTCOMMAND("\n   ");	
		for(j=1;j<=pMat->Nc;++j){	
			GMATRIX_PRINTCOMMAND("%c%4.14e ",GMATRIXMACRO_SIGNCHAR(PGMATRIX_DATA(pMat,i,j)),fabs(PGMATRIX_DATA(pMat,i,j)));	
		}	
	}
	GMATRIX_PRINTCOMMAND("\n");	
}
#endif

#if GMATRIX_DEFINE_PRINTROW
void PGMATRIX_PRINTROW_NAMED(char* NameString,PGMATRIX pMat,int i)
{
	int j;

	GMATRIX_ASSERT("_PGMATRIX_PRINTROW",pMat == NULL);	
	GMATRIX_PRINTCOMMAND("\nRow %i of %s (%i x %i):",i,NameString,pMat->Nr,pMat->Nc);	
	GMATRIX_PRINTCOMMAND("\n   ");	
	for(j=1;j<=pMat->Nc;++j){	
		GMATRIX_PRINTCOMMAND("%c%4.5f ",GMATRIXMACRO_SIGNCHAR(PGMATRIX_DATA(pMat,i,j)),fabs(PGMATRIX_DATA(pMat,i,j)));	
	}
	GMATRIX_PRINTCOMMAND("\n");	
}
#endif

#if GMATRIX_DEFINE_INFO
void PGMATRIX_INFO_NAMED(char* NameString,PGMATRIX pMat)
{
	GMATRIX_PRINTCOMMAND("\n%s: Nr = %i, Nc = %i, MaxSize = %i",NameString,pMat->Nr,pMat->Nc,pMat->MaxSize);	
}
#endif

/**********************************************************************************************
***** GMatrix: Special Matrices
**********************************************************************************************/
#if GMATRIX_DEFINE_ZEROES
void PGMATRIX_ZEROES(PGMATRIX pMat)
{
	int i,j;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) = 0.0;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_SUMLINCOL
void PGMATRIX_SUMLINCOL(PGMATRIX pMat)
{
	int i,j;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) = i+j;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_ONES
void PGMATRIX_ONES(PGMATRIX pMat)
{
	int i,j;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) = 1.0;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_RAND
void PGMATRIX_RAND(PGMATRIX pMat)
{
	int i,j;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) = (((double)(rand()))/RAND_MAX);	
		}	\
	}
}
#endif

#if GMATRIX_DEFINE_RANDN
void PGMATRIX_RANDN(PGMATRIX pMat)
{
	int i,j,k;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) = 0.0;
			for(k=1;k<=12;++k){	
				PGMATRIX_DATA(pMat,i,j) += ((((double)(rand()))/RAND_MAX)-0.5);	
			}
		}	
	}
}
#endif

#if GMATRIX_DEFINE_IDENTITY
void PGMATRIX_IDENTITY(PGMATRIX pMat) 
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_IDENTITY",pMat->Nr != pMat->Nc);	
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) = 0.0;	
		}	
		PGMATRIX_DATA(pMat,i,i) = 1.0;	
	}
}
#endif

#if GMATRIX_DEFINE_WILKINSON
void PGMATRIX_WILKINSON(PGMATRIX pMat)
{
	int m,i,j;

	GMATRIX_ASSERT("PGMATRIX_WILKINSON",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("In PGMATRIX_WILKINSON, the matrix must have an odd number of columns",(pMat->Nr % 2) == 0);	

	// m = (n-1)/2;
	m = (pMat->Nr-1)/2;
	// e = ones(n-1,1);
	// W = diag(abs(-m:m)) + diag(e,1) + diag(e,-1);
	PGMATRIX_ZEROES(pMat);
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			if (i==j){
				PGMATRIX_DATA(pMat,i,i) = fabs(i-m-1);
			}
			else{
				if ((i==j-1) && (j > 1) ){
					PGMATRIX_DATA(pMat,i,j) = 1;
				}
				if ((j==i-1) && (i > 1) ){
					PGMATRIX_DATA(pMat,i,j) = 1;
				}
			}
		}
	}
}
#endif

#if GMATRIX_DEFINE_ORTHOGONAL
void PGMATRIX_ORTHOGONAL(PGMATRIX pMat,int Type) 
{
	/* Orthogonal matrix according to Type and the matrix order N:
    Type = 1:  Q(i,j) = SQRT(2/(N+1)) * SIN( i*j*PI/(N+1) )
            Symmetric eigenvector matrix for second difference matrix.
            This is the default Type.
    Type = 2:  Q(i,j) = 2/(SQRT(2*N+1)) * SIN( 2*i*j*PI/(2*N+1) )
            Symmetric.
    Type = -1: Q(i,j) = COS( (i-1)*(j-1)*PI/(N-1) )
            Chebyshev Vandermonde-like matrix, based on extrema 
            of T(N-1).
    Type = -2: Q(i,j) = COS( (i-1)*(j-1/2)*PI/N) )
            Chebyshev Vandermonde-like matrix, based on zeros of T(N).*/
	// Extracted from gallery MATLAB function

	double N;
	int i,j;

	N = (double)(pMat->Nr);
	GMATRIX_ASSERT("PGMATRIX_ORTHOGONAL",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("PGMATRIX_ORTHOGONAL, Type is not a valid value",(Type!=1)&&(Type!=2)&&(Type!=-1)&&(Type!=-2));	
	PGMATRIX_ZEROES(pMat);
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			switch(Type){
			case 1:
				PGMATRIX_DATA(pMat,i,j) = sqrt(2.0/(N+1.0)) * sin( i*j*GMATRIXCONST_PI/(N+1.0) );
				break;
			case 2:
				PGMATRIX_DATA(pMat,i,j) = 2.0/(sqrt(2.0*N+1.0)) * sin( 2.0*i*j*GMATRIXCONST_PI/(2.0*N+1.0) );
				break;
			case -1:
				PGMATRIX_DATA(pMat,i,j) = cos( (i-1.0)*(j-1.0)*GMATRIXCONST_PI/(N-1.0) );
				break;
			case -2:
				// m = (0:n-1)'*(.5:n-.5) * (pi/n)
				PGMATRIX_DATA(pMat,i,j) = cos( (i-1.0)*(j-1.0/2.0)*GMATRIXCONST_PI/N );
				break;
			}
		}
	}
}
#endif

/**********************************************************************************************
***** GMatrix: COPY
**********************************************************************************************/
#if GMATRIX_DEFINE_COPY
void PGMATRIX_COPY(PGMATRIX pMatResult, PGMATRIX pMat)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_COPY",pMatResult->MaxSize < (pMat->Nr*pMat->Nc));	
	PGMATRIX_SETSIZE(pMatResult,pMat->Nr,pMat->Nc);
	for(i=1;i<=pMatResult->Nr;++i){	
		for(j=1;j<=pMatResult->Nc;++j){	
			PGMATRIX_DATA(pMatResult,i,j) = PGMATRIX_DATA(pMat,i,j); 
		}	
	}
}
#endif

#if GMATRIX_DEFINE_COPY_COLUMN
void PGMATRIX_COPY_COLUMN(PGMATRIX pMatResult, int ColDest, PGMATRIX pMat, int ColOrigin)
{
	int i;
	GMATRIX_ASSERT("PGMATRIX_COPY_COLUMN",pMatResult->Nr != pMat->Nr);	
	for(i=1;i<=pMatResult->Nr;++i){	
		PGMATRIX_DATA(pMatResult,i,ColDest) = PGMATRIX_DATA(pMat,i,ColOrigin); 
	}
}
#endif

#if GMATRIX_DEFINE_COPY_ROW
void PGMATRIX_COPY_ROW(PGMATRIX pMatResult, int RowDest, PGMATRIX pMat, int RowOrigin)
{
	int i;
	GMATRIX_ASSERT("PGMATRIX_COPY_ROW",pMatResult->Nc != pMat->Nc);	
	for(i=1;i<=pMatResult->Nc;++i){	
		PGMATRIX_DATA(pMatResult,RowDest,i) = PGMATRIX_DATA(pMat,RowOrigin,i); 
	}
}
#endif

/**********************************************************************************************
***** GMatrix: Transposition
**********************************************************************************************/
#if GMATRIX_DEFINE_TRANSPOSE_COPY
void PGMATRIX_TRANSPOSE_COPY(PGMATRIX pMatTranspose, PGMATRIX pMat) 
{
	int i,j;
	PGMATRIX_SETSIZE(pMatTranspose,pMat->Nc,pMat->Nr);	
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMatTranspose,j,i) = PGMATRIX_DATA(pMat,i,j);	
		}	
	}
}
#endif

/**********************************************************************************************
***** GMatrix: Trace
**********************************************************************************************/
#if GMATRIX_DEFINE_TRACE
double PGMATRIX_TRACE(PGMATRIX pMat)
{
	double Trace;

	int i;
	GMATRIX_ASSERT("GMATRIX_TRACE",pMat->Nr != pMat->Nc);

	for (i=1,Trace=0.0; i<=pMat->Nr; ++i){ 
		Trace += PGMATRIX_DATA(pMat,i,i); 
	} 

	return Trace;
}
#endif

/**********************************************************************************************
***** GMatrix: Sum entries
**********************************************************************************************/
#if GMATRIX_DEFINE_SUMENTRIES
double PGMATRIX_SUMENTRIES(PGMATRIX pMat)
{
	double Sum = 0.0;
	int i,j;

	for (i=1; i<=pMat->Nr; ++i){ 
		for (j=1; j<=pMat->Nc; ++j){ 
			Sum += PGMATRIX_DATA(pMat,i,j); 
		}
	} 

	return Sum;
}
#endif

/**********************************************************************************************
***** GMatrix: Sum absolute entries
**********************************************************************************************/
#if GMATRIX_DEFINE_SUMABSOLUTEENTRIES
double PGMATRIX_SUMABSOLUTEENTRIES(PGMATRIX pMat)
{
	double Sum = 0.0;
	int i,j;

	for (i=1; i<=pMat->Nr; ++i){ 
		for (j=1; j<=pMat->Nc; ++j){ 
			Sum += fabs(PGMATRIX_DATA(pMat,i,j)); 
		}
	} 

	return Sum;
}
#endif

/**********************************************************************************************
***** GMatrix: LU Decomposition
**********************************************************************************************/
#if GMATRIX_DEFINE_LUDCMP
double PGMATRIX_LUDCMP(PGMATRIX pMatLU, PGMATRIX pMat)
{
	double  big,dum,sum,temp;
	double	d;
	double  *vv;
	int i,imax,j,k;

	GMATRIX_ASSERT("GMATRIX_LUDCMP",pMat->Nr != pMat->Nc);	

	PGMATRIX_COPY(pMatLU, pMat); 

	vv = (double*) malloc((pMat->Nr+1)*sizeof(double));
	
	d=1.0;	
	for (i=1;i<=pMatLU->Nr;i++)  {
		big=0.0;	
		for (j=1;j<=pMatLU->Nr;j++)
			if ((temp=fabs(PGMATRIX_DATA(pMatLU,i,j))) > big) big=temp;
		if (big == 0.0) GMATRIX_ASSERT("PGMATRIX_LUDCMP: Singular matrix",1);
		vv[i]=1.0/big;
	} 
	for (j=1;j<=pMatLU->Nr;j++)  {
		for (i=1;i<j;i++) {
			sum=PGMATRIX_DATA(pMatLU,i,j);
			for (k=1;k<i;k++) sum -= PGMATRIX_DATA(pMatLU,i,k)*PGMATRIX_DATA(pMatLU,k,j);
			PGMATRIX_DATA(pMatLU,i,j)=sum;
		}
		big=0.0;
		for (i=j;i<=pMatLU->Nr;i++) {
			sum=PGMATRIX_DATA(pMatLU,i,j);
			for (k=1;k<j;k++)
				sum -= PGMATRIX_DATA(pMatLU,i,k)*PGMATRIX_DATA(pMatLU,k,j);
			PGMATRIX_DATA(pMatLU,i,j)=sum;
			if ( (dum=vv[i]*fabs(sum)) >= big) {
				big=dum;
				imax=i;
			}
		}
		if (j != imax) {
			for (k=1;k<=pMatLU->Nr;k++) {
				dum=PGMATRIX_DATA(pMatLU,imax,k);
				PGMATRIX_DATA(pMatLU,imax,k)=PGMATRIX_DATA(pMatLU,j,k);
				PGMATRIX_DATA(pMatLU,j,k)=dum;
			}
			d = -(d);
			vv[imax]=vv[j];
		}
		if (PGMATRIX_DATA(pMatLU,j,j) == 0.0) PGMATRIX_DATA(pMatLU,j,j)=1.0e-20;
		if (j != pMatLU->Nr) {
			dum=1.0/(PGMATRIX_DATA(pMatLU,j,j));
			for (i=j+1;i<=pMatLU->Nr;i++) PGMATRIX_DATA(pMatLU,i,j) *= dum;
		}
	}

	free(vv);

	return d;

}
#endif

/**********************************************************************************************
***** GMatrix: Gauss-Jordan Elimination
***** Abstract: Given matrices A and B from A*X=B, computes inv(A) and X. 
*****			It is applied only to determined systems, i.e. A is a square matrix.
***** Note: if the pointer to the X or B matrices is NULL, it performs only inv(A).
**********************************************************************************************/

#if GMATRIX_DEFINE_GAUSSJORDAN
void PGMATRIX_GAUSSJORDAN(PGMATRIX pAInverse, PGMATRIX pX, PGMATRIX pA, PGMATRIX pB)
{
	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv,temp;
	int *indxc,*indxr,*ipiv;
	int flaginverseonly = 1;

	GMATRIX_ASSERT("PGMATRIX_GAUSSJORDAN",pA->Nr != pA->Nc);
	if ((pX!=NULL) && (pB!=NULL)){
		flaginverseonly = 0;
		GMATRIX_ASSERT("PGMATRIX_GAUSSJORDAN",pA->Nr != pB->Nr);
	}

	if (pAInverse!=pA){
		PGMATRIX_COPY(pAInverse, pA);
	}
	if (flaginverseonly==0){
		PGMATRIX_COPY(pX, pB);
	}

	indxc = (int*) malloc((pA->Nr+1)*sizeof(int));
	indxr = (int*) malloc((pA->Nr+1)*sizeof(int));
	ipiv  = (int*) malloc((pA->Nr+1)*sizeof(int));

	for (j=1;j<=pA->Nr;j++) ipiv[j]=0;
	for (i=1;i<=pA->Nr;i++) {
		big=0.0;
		for (j=1;j<=pA->Nr;j++)
			if (ipiv[j] != 1)
				for (k=1;k<=pA->Nr;k++) {
					if (ipiv[k] == 0) {
						if (fabs(PGMATRIX_DATA(pAInverse,j,k)) >= big) {
							big=fabs(PGMATRIX_DATA(pAInverse,j,k));
							irow=j;
							icol=k;
						}
					} else if (ipiv[k] > 1) GMATRIX_ASSERT("PGMATRIX_GAUSSJORDAN: Singular matrix (step 1)",1);
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=1;l<=pA->Nr;l++) GMATRIXMACRO_SWAP(PGMATRIX_DATA(pAInverse,irow,l),PGMATRIX_DATA(pAInverse,icol,l));
			if (flaginverseonly==0){
				for (l=1;l<=pB->Nc;l++) GMATRIXMACRO_SWAP(PGMATRIX_DATA(pX,irow,l),PGMATRIX_DATA(pX,icol,l));
			}
		}
		indxr[i]=irow;
		indxc[i]=icol;
		if (PGMATRIX_DATA(pAInverse,icol,icol) == 0.0){
			GMATRIX_ASSERT("PGMATRIX_GAUSSJORDAN: Singular matrix (step 2)",1);
		}
		pivinv=1.0/PGMATRIX_DATA(pAInverse,icol,icol);
		PGMATRIX_DATA(pAInverse,icol,icol)=1.0;
		for (l=1;l<=pA->Nr;l++) PGMATRIX_DATA(pAInverse,icol,l) *= pivinv;
		if (flaginverseonly==0){
			for (l=1;l<=pB->Nc;l++) PGMATRIX_DATA(pX,icol,l) *= pivinv;
		}
		for (ll=1;ll<=pA->Nr;ll++){
			if (ll != icol) {
				dum=PGMATRIX_DATA(pAInverse,ll,icol);
				PGMATRIX_DATA(pAInverse,ll,icol)=0.0;
				for (l=1;l<=pA->Nr;l++) PGMATRIX_DATA(pAInverse,ll,l) -= PGMATRIX_DATA(pAInverse,icol,l)*dum;
				if (flaginverseonly==0){
					for (l=1;l<=pB->Nc;l++) PGMATRIX_DATA(pX,ll,l) -= PGMATRIX_DATA(pX,icol,l)*dum;
				}
			}
		}
	}
	
	for (l=pA->Nr;l>=1;l--) {
		if (indxr[l] != indxc[l]){
			for (k=1;k<=pA->Nr;k++){
				GMATRIXMACRO_SWAP(PGMATRIX_DATA(pAInverse,k,indxr[l]),PGMATRIX_DATA(pAInverse,k,indxc[l]));
			}
		}
	}

	free(ipiv);
	free(indxr);
	free(indxc);
}
#endif

/**********************************************************************************************
***** GMatrix: Cholesky decomposition
***** Abstract: Given matrice A symmetric and positive definite, computes a lower diagonal
*****			matrix L such that L*(L)' = A. 
**********************************************************************************************/
#if GMATRIX_DEFINE_CHOLESKY
void PGMATRIX_CHOLESKY(PGMATRIX pL, PGMATRIX pA)
{
	int i,j,k;
	double sum,*p;

	GMATRIX_ASSERT("PGMATRIX_CHOLESKY",pA->Nr != pA->Nc);
	PGMATRIX_COPY(pL,pA);	
	p = (double*) malloc((pA->Nr+1)*sizeof(double));

	for (i=1;i<=pL->Nr;i++) {
		for (j=i;j<=pL->Nr;j++) {
			for (sum=PGMATRIX_DATA(pL,i,j),k=i-1;k>=1;k--){
				sum -= PGMATRIX_DATA(pL,i,k)*PGMATRIX_DATA(pL,j,k);
			}
			if (i == j) {
				if (sum == 0.0){ // For singular matrices (incomplet Cholesky form);
					sum = 1e-50;
				}
				if (sum <= 0.0){
//				if (sum <= 0.0){
					free(p);
					GMATRIX_ASSERT("PGMATRIX_CHOLESKY: Failed (Singular or non-positive definite matrix",1);
					return;
				}
				p[i]=sqrt(sum);
			} else PGMATRIX_DATA(pL,j,i)=sum/p[i];
		}
	}
	for (i=1;i<=pL->Nr;i++) {
		for (j=i;j<=pL->Nc;j++) {
			if (i == j) {
				PGMATRIX_DATA(pL,i,j)=p[i];
			}
			else{
				PGMATRIX_DATA(pL,i,j)=0.0;
			}
		}
	}
	free(p);
}
#endif

/**********************************************************************************************
***** GMatrix: Determinant
**********************************************************************************************/
#if GMATRIX_DEFINE_DETERMINANT2
double PGMATRIX_DETERMINANT2(PGMATRIX pMat)
{
	double Determinant = 0.0;

	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr == 0);	
	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr != 2);	
	
	Determinant = PGMATRIX_DATA(pMat,2,2)*PGMATRIX_DATA(pMat,1,1)-PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,2,1);
	
	return Determinant;
}
#endif

#if GMATRIX_DEFINE_DETERMINANT3
double PGMATRIX_DETERMINANT3(PGMATRIX pMat)
{
	double Determinant = 0.0;

	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr == 0);	
	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr != 3);	

	Determinant = (PGMATRIX_DATA(pMat,1,1)*PGMATRIX_DATA(pMat,2,2)*PGMATRIX_DATA(pMat,3,3)-PGMATRIX_DATA(pMat,1,1)*PGMATRIX_DATA(pMat,2,3)*PGMATRIX_DATA(pMat,3,2)-PGMATRIX_DATA(pMat,2,1)*PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,3,3)+PGMATRIX_DATA(pMat,2,1)*PGMATRIX_DATA(pMat,1,3)*PGMATRIX_DATA(pMat,3,2)+PGMATRIX_DATA(pMat,3,1)*PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,2,3)-PGMATRIX_DATA(pMat,3,1)*PGMATRIX_DATA(pMat,1,3)*PGMATRIX_DATA(pMat,2,2));

	return Determinant;
}
#endif

#if GMATRIX_DEFINE_DETERMINANT
double PGMATRIX_DETERMINANT(PGMATRIX pMat, PGMATRIX pMatDummy)
{
	double Determinant = 0.0;
	int j;

	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("PGMATRIX_DETERMINANT",pMat->Nr == 0);	

	Determinant = PGMATRIX_LUDCMP(pMatDummy,pMat); // MatDummy is the LU decomposition of Mat  
	for (j=1; j<=pMatDummy->Nr; ++j){ 
		Determinant *= PGMATRIX_DATA(pMatDummy,j,j); 
	} 

	return Determinant;
}
#endif

/**********************************************************************************************
***** GMatrix: Eigenvalues
**********************************************************************************************/
/*#define GMATRIX_EIGENVALUES2(Eigenvalue1, Eigenvalue1, Mat) PGMATRIX_EIGENVALUES2(&Eigenvalue1,&Eigenvalue2, &Mat)

void PGMATRIX_EIGENVALUES2(double* pEigenvalue1, double* pEigenvalue2, PGMATRIX pMat) 
{
	GMATRIX_ASSERT("PGMATRIX_EIGENVALUES2",pMat->Nr != 2);
	GMATRIX_ASSERT("PGMATRIX_EIGENVALUES2",pMat->Nr != pMat->Nc);	

	*pEigenvalue1 = 0.5*(M[0][0] + M[1][1] + sqrt( (M[0][0] - M[1][1])*(M[0][0] - M[1][1]) + 4*M[1][0]*M[0][1] ));
	*pEigenvalue2 = 0.5*(M[0][0] + M[1][1] - sqrt( (M[0][0] - M[1][1])*(M[0][0] - M[1][1]) + 4*M[1][0]*M[0][1] ));

}*/

/**********************************************************************************************
***** GMatrix: Inverse
**********************************************************************************************/
#if GMATRIX_DEFINE_INVERSE2_COPY
void PGMATRIX_INVERSE2_COPY(PGMATRIX pMatInverse, PGMATRIX pMat) 
{
	double Det;

	GMATRIX_ASSERT("PGMATRIX_INVERSE2_COPY",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("PGMATRIX_INVERSE2_COPY",pMat->Nr == 0);	
	GMATRIX_ASSERT("PGMATRIX_INVERSE2_COPY",pMat->Nr != 2);
	PGMATRIX_SETSIZE(pMatInverse,pMat->Nr, pMat->Nc);
	Det = PGMATRIX_DETERMINANT2(pMat);

	if ( fabs(Det) >= 1e-12 ){ 
		PGMATRIX_DATA(pMatInverse,1,1) =  PGMATRIX_DATA(pMat,2,2)/Det; 
		PGMATRIX_DATA(pMatInverse,1,2) = -PGMATRIX_DATA(pMat,1,2)/Det; 
		PGMATRIX_DATA(pMatInverse,2,1) = -PGMATRIX_DATA(pMat,2,1)/Det; 
		PGMATRIX_DATA(pMatInverse,2,2) =  PGMATRIX_DATA(pMat,1,1)/Det; 
	}	
	else{ 
		PGMATRIX_GAUSSJORDAN(pMatInverse, NULL, pMat, NULL);
	}
}
#endif
      
#if GMATRIX_DEFINE_INVERSE3_COPY
void PGMATRIX_INVERSE3_COPY(PGMATRIX pMatInverse, PGMATRIX pMat) 
{
	double Det;

	GMATRIX_ASSERT("PGMATRIX_INVERSE3_COPY",pMat->Nr != pMat->Nc);	
	GMATRIX_ASSERT("PGMATRIX_INVERSE3_COPY",pMat->Nr == 0);	
	GMATRIX_ASSERT("PGMATRIX_INVERSE3_COPY",pMat->Nr != 3);	
	PGMATRIX_SETSIZE(pMatInverse,pMat->Nr, pMat->Nc);
	Det = PGMATRIX_DETERMINANT3(pMat);

	if ( fabs(Det) >= 1e-12 ){ 
    	PGMATRIX_DATA(pMatInverse,1,1) =  (PGMATRIX_DATA(pMat,2,2)*PGMATRIX_DATA(pMat,3,3)-PGMATRIX_DATA(pMat,2,3)*PGMATRIX_DATA(pMat,3,2))/Det; 
	    PGMATRIX_DATA(pMatInverse,1,2) = -(PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,3,3)-PGMATRIX_DATA(pMat,1,3)*PGMATRIX_DATA(pMat,3,2))/Det; 
    	PGMATRIX_DATA(pMatInverse,1,3) =  (PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,2,3)-PGMATRIX_DATA(pMat,1,3)*PGMATRIX_DATA(pMat,2,2))/Det; 
	    PGMATRIX_DATA(pMatInverse,2,1) = -(PGMATRIX_DATA(pMat,2,1)*PGMATRIX_DATA(pMat,3,3)-PGMATRIX_DATA(pMat,2,3)*PGMATRIX_DATA(pMat,3,1))/Det; 
    	PGMATRIX_DATA(pMatInverse,2,2) =  (PGMATRIX_DATA(pMat,1,1)*PGMATRIX_DATA(pMat,3,3)-PGMATRIX_DATA(pMat,1,3)*PGMATRIX_DATA(pMat,3,1))/Det; 
	    PGMATRIX_DATA(pMatInverse,2,3) = -(PGMATRIX_DATA(pMat,1,1)*PGMATRIX_DATA(pMat,2,3)-PGMATRIX_DATA(pMat,1,3)*PGMATRIX_DATA(pMat,2,1))/Det; 
    	PGMATRIX_DATA(pMatInverse,3,1) =  (PGMATRIX_DATA(pMat,2,1)*PGMATRIX_DATA(pMat,3,2)-PGMATRIX_DATA(pMat,2,2)*PGMATRIX_DATA(pMat,3,1))/Det; 
	    PGMATRIX_DATA(pMatInverse,3,2) = -(PGMATRIX_DATA(pMat,1,1)*PGMATRIX_DATA(pMat,3,2)-PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,3,1))/Det; 
    	PGMATRIX_DATA(pMatInverse,3,3) =  (PGMATRIX_DATA(pMat,1,1)*PGMATRIX_DATA(pMat,2,2)-PGMATRIX_DATA(pMat,1,2)*PGMATRIX_DATA(pMat,2,1))/Det; 
	} 
	else{ 
		PGMATRIX_GAUSSJORDAN(pMatInverse, NULL, pMat, NULL);
	}
}
#endif

#if GMATRIX_DEFINE_INVERSE_COPY
void PGMATRIX_INVERSE_COPY(PGMATRIX pMatInverse, PGMATRIX pMat) 
{
	GMATRIX_ASSERT("PGMATRIX_INVERSE_COPY",pMat->Nr != pMat->Nc);
	GMATRIX_ASSERT("PGMATRIX_INVERSE_COPY",pMat->Nr == 0);	

	if (pMat->Nc == 1){	
		GMATRIX_ASSERT("PGMATRIX_INVERSE_COPY",pMat->Nr != pMat->Nc);	
		if (fabs(PGMATRIX_DATA(pMat,1,1)) >= 1e-16){
			PGMATRIX_SETSIZE(pMatInverse,pMat->Nr, pMat->Nc);
			PGMATRIX_DATA(pMatInverse,1,1) = 1.0/PGMATRIX_DATA(pMat,1,1); 
		}
		else{
			GMATRIX_ASSERT("PGMATRIX_INVERSE_COPY: Singular matrix",1);	
		}
	}
	if (pMat->Nc == 2){	
		PGMATRIX_INVERSE2_COPY(pMatInverse,pMat);
	} 
	if (pMat->Nc == 3){	
		PGMATRIX_INVERSE3_COPY(pMatInverse,pMat);
	} 
	if (pMat->Nc > 3){	
		PGMATRIX_GAUSSJORDAN(pMatInverse, NULL, pMat, NULL);
	}
}
#endif

#if GMATRIX_DEFINE_INVERSE
void PGMATRIX_INVERSE(PGMATRIX pMat) 
{
	GMATRIX_ASSERT("PGMATRIX_INVERSE_COPY",pMat->Nr != pMat->Nc);
	GMATRIX_ASSERT("PGMATRIX_INVERSE_COPY",pMat->Nr == 0);	
	PGMATRIX_GAUSSJORDAN(pMat, NULL, pMat, NULL);
}
#endif

/**********************************************************************************************
***** GMatrix: Row and Column swapping
**********************************************************************************************/
#if GMATRIX_DEFINE_SWAP_ROW
void PGMATRIX_SWAP_ROW(PGMATRIX pMat,int i, int j)
{
	double temp;
	int k;

	GMATRIX_ASSERT("PGMATRIX_SWAP_ROW",pMat->Nr <= 0);
	GMATRIX_ASSERT("PGMATRIX_SWAP_ROW",pMat->Nr < i);
	GMATRIX_ASSERT("PGMATRIX_SWAP_ROW",pMat->Nr < j);
	GMATRIX_ASSERT("PGMATRIX_SWAP_ROW",i <= 0);
	GMATRIX_ASSERT("PGMATRIX_SWAP_ROW",j <= 0);

	if(i==j) return;
	for(k=1;k<=pMat->Nc;++k){
		GMATRIXMACRO_SWAP(PGMATRIX_DATA(pMat,i,k),PGMATRIX_DATA(pMat,j,k));
	}
}
#endif

#if GMATRIX_DEFINE_SWAP_COLUMN
void PGMATRIX_SWAP_COLUMN(PGMATRIX pMat,int i, int j)
{
	double temp;
	int k;

	GMATRIX_ASSERT("PGMATRIX_SWAP_COLUMN",pMat->Nc <= 0);
	GMATRIX_ASSERT("PGMATRIX_SWAP_COLUMN",pMat->Nc < i);
	GMATRIX_ASSERT("PGMATRIX_SWAP_COLUMN",pMat->Nc < j);
	GMATRIX_ASSERT("PGMATRIX_SWAP_COLUMN",i <= 0);
	GMATRIX_ASSERT("PGMATRIX_SWAP_COLUMN",j <= 0);

	if(i==j) return;
	for(k=1;k<=pMat->Nr;++k){
		GMATRIXMACRO_SWAP(PGMATRIX_DATA(pMat,k,i),PGMATRIX_DATA(pMat,k,j));
	}
}
#endif

/**********************************************************************************************
***** GMatrix: Singular Valor Decomposition: Mat = U*S*Transpose(V)
**********************************************************************************************/
#if GMATRIX_DEFINE_SVD
double dpythag(double a, double b)
{
	double absa,absb;
	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb) return absa*sqrt(1.0+GMATRIXMACRO_SQR(absb/absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+GMATRIXMACRO_SQR(absa/absb)));
}

void PGMATRIX_SVD(PGMATRIX pU,PGMATRIX pS,PGMATRIX pV,PGMATRIX pMat, unsigned char FlagSorted)
{
	int ir;
	double cS;
	int flag,i,its,j,jj,k,l,nm;
	double anorm,c,f,g,h,s,scale,x,y,z;
	PGMATRIX pRV1,pcU,pcV;

	GMATRIX_ASSERT("PGMATRIX_SVD",pMat->Nr <= 0);
	GMATRIX_ASSERT("PGMATRIX_SVD",pMat->Nc <= 0);

	PGMATRIX_SETSIZE(pU,pMat->Nr,pMat->Nc);
	PGMATRIX_SETSIZE(pS,pMat->Nc,pMat->Nc);
	PGMATRIX_SETSIZE(pV,pMat->Nc,pMat->Nc);

	pRV1 = PGMATRIX_ALLOC(pMat->Nc,1);
	PGMATRIX_COPY(pU,pMat);
	PGMATRIX_ZEROES(pS);

	g=scale=anorm=0.0;
	for (i=1;i<=pMat->Nc;i++) {
		l=i+1;
		PGMATRIX_DATA(pRV1,i,1)=scale*g;
		g=s=scale=0.0;
		if (i <= pMat->Nr) {
			for (k=i;k<=pMat->Nr;k++) scale += fabs(PGMATRIX_DATA(pU,k,i));
			if (scale) {
				for (k=i;k<=pMat->Nr;k++) {
					PGMATRIX_DATA(pU,k,i) /= scale;
					s += PGMATRIX_DATA(pU,k,i)*PGMATRIX_DATA(pU,k,i);
				}
				f=PGMATRIX_DATA(pU,i,i);
				// SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
				// g = -SIGN(sqrt(s),f);
				g = -1.0 * sqrt(s) * GMATRIXMACRO_SIGN(f);
				h = f*g-s;
				PGMATRIX_DATA(pU,i,i)=f-g;
				for (j=l;j<=pMat->Nc;j++) {
					for (s=0.0,k=i;k<=pMat->Nr;k++) s += PGMATRIX_DATA(pU,k,i)*PGMATRIX_DATA(pU,k,j);
					f=s/h;
					for (k=i;k<=pMat->Nr;k++) PGMATRIX_DATA(pU,k,j) += f*PGMATRIX_DATA(pU,k,i);
				}
				for (k=i;k<=pMat->Nr;k++) PGMATRIX_DATA(pU,k,i) *= scale;
			}
		}
		PGMATRIX_DATA(pS,i,i) = scale * g;
		g=s=scale=0.0;
		if (i <= pMat->Nr && i != pMat->Nc) {
			for (k=l;k<=pMat->Nc;k++) scale += fabs(PGMATRIX_DATA(pU,i,k));
			if (scale) {
				for (k=l;k<=pMat->Nc;k++) {
					PGMATRIX_DATA(pU,i,k) /= scale;
					s += PGMATRIX_DATA(pU,i,k)*PGMATRIX_DATA(pU,i,k);
				}
				f=PGMATRIX_DATA(pU,i,l);
				// SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
				// g = -SIGN(sqrt(s),f);
				g = -1.0 * sqrt(s) * GMATRIXMACRO_SIGN(f);
				h=f*g-s;
				PGMATRIX_DATA(pU,i,l)=f-g;
				for (k=l;k<=pMat->Nc;k++) PGMATRIX_DATA(pRV1,k,1)=PGMATRIX_DATA(pU,i,k)/h;
				for (j=l;j<=pMat->Nr;j++) {
					for (s=0.0,k=l;k<=pMat->Nc;k++) s += PGMATRIX_DATA(pU,j,k)*PGMATRIX_DATA(pU,i,k);
					for (k=l;k<=pMat->Nc;k++) PGMATRIX_DATA(pU,j,k) += s*PGMATRIX_DATA(pRV1,k,1);
				}
				for (k=l;k<=pMat->Nc;k++) PGMATRIX_DATA(pU,i,k) *= scale;
			}
		}
		anorm = GMATRIXMACRO_MAX(anorm,(fabs(PGMATRIX_DATA(pS,i,i))+fabs(PGMATRIX_DATA(pRV1,i,1))));
	}
	for (i=pMat->Nc;i>=1;i--) {
		if (i < pMat->Nc) {
			if (g) {
				for (j=l;j<=pMat->Nc;j++) PGMATRIX_DATA(pV,j,i)=(PGMATRIX_DATA(pU,i,j)/PGMATRIX_DATA(pU,i,l))/g;
				for (j=l;j<=pMat->Nc;j++) {
					for (s=0.0,k=l;k<=pMat->Nc;k++) s += PGMATRIX_DATA(pU,i,k)*PGMATRIX_DATA(pV,k,j);
					for (k=l;k<=pMat->Nc;k++) PGMATRIX_DATA(pV,k,j) += s*PGMATRIX_DATA(pV,k,i);
				}
			}
			for (j=l;j<=pMat->Nc;j++) PGMATRIX_DATA(pV,i,j)=PGMATRIX_DATA(pV,j,i)=0.0;
		}
		PGMATRIX_DATA(pV,i,i)=1.0;
		g=PGMATRIX_DATA(pRV1,i,1);
		l=i;
	}
	for (i=GMATRIXMACRO_MIN(pMat->Nr,pMat->Nc);i>=1;i--) {
		l=i+1;
		g=PGMATRIX_DATA(pS,i,i);
		for (j=l;j<=pMat->Nc;j++) PGMATRIX_DATA(pU,i,j)=0.0;
		if (g) {
			g=1.0/g;
			for (j=l;j<=pMat->Nc;j++) {
				for (s=0.0,k=l;k<=pMat->Nr;k++) s += PGMATRIX_DATA(pU,k,i)*PGMATRIX_DATA(pU,k,j);
				f=(s/PGMATRIX_DATA(pU,i,i))*g;
				for (k=i;k<=pMat->Nr;k++) PGMATRIX_DATA(pU,k,j) += f*PGMATRIX_DATA(pU,k,i);
			}
			for (j=i;j<=pMat->Nr;j++) PGMATRIX_DATA(pU,j,i) *= g;
		} else for (j=i;j<=pMat->Nr;j++) PGMATRIX_DATA(pU,j,i)=0.0;
		++PGMATRIX_DATA(pU,i,i);
	}
	for (k=pMat->Nc;k>=1;k--) {
		for (its=1;its<=30;its++) {
			flag=1;
			for (l=k;l>=1;l--) {
				nm=l-1;
				if ((double)(fabs(PGMATRIX_DATA(pRV1,l,1))+anorm) == anorm) {
					flag=0;
					break;
				}
				if ((double)(fabs(PGMATRIX_DATA(pS,nm,nm))+anorm) == anorm) break;
			}
			if (flag) {
				c=0.0;
				s=1.0;
				for (i=l;i<=k;i++) {
					f=s*PGMATRIX_DATA(pRV1,i,1);
					PGMATRIX_DATA(pRV1,i,1)=c*PGMATRIX_DATA(pRV1,i,1);
					if ((double)(fabs(f)+anorm) == anorm) break;
					g=PGMATRIX_DATA(pS,i,i);
					h=dpythag(f,g);
					PGMATRIX_DATA(pS,i,i)=h;
					h=1.0/h;
					c=g*h;
					s = -f*h;
					for (j=1;j<=pMat->Nr;j++) {
						y=PGMATRIX_DATA(pU,j,nm);
						z=PGMATRIX_DATA(pU,j,i);
						PGMATRIX_DATA(pU,j,nm)=y*c+z*s;
						PGMATRIX_DATA(pU,j,i)=z*c-y*s;
					}
				}
			}
			z=PGMATRIX_DATA(pS,k,k);
			if (l == k) {
				if (z < 0.0) {
					PGMATRIX_DATA(pS,k,k) = -z;
					for (j=1;j<=pMat->Nc;j++) PGMATRIX_DATA(pV,j,k) = -PGMATRIX_DATA(pV,j,k);
				}
				break;
			}
			if (its >= 30){
				GMATRIX_ASSERT("PGMATRIX_SVD: no convergence in 30 iterations",1);	
			}

			x=PGMATRIX_DATA(pS,l,l);
			nm=k-1;
			y=PGMATRIX_DATA(pS,nm,nm);
			g=PGMATRIX_DATA(pRV1,nm,1);
			h=PGMATRIX_DATA(pRV1,k,1);
			f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
			g=dpythag(f,1.0);
			// SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
			// f=((x-z)*(x+z)+h*((y/(f+SIGN(g,f)))-h))/x;
			f=((x-z)*(x+z)+h*((y/(f+g*GMATRIXMACRO_SIGN(f)))-h))/x;
			c=s=1.0;
			for (j=l;j<=nm;j++) {
				i=j+1;
				g=PGMATRIX_DATA(pRV1,i,1);
				y=PGMATRIX_DATA(pS,i,i);
				h=s*g;
				g=c*g;
				z=dpythag(f,h);
				PGMATRIX_DATA(pRV1,j,1)=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g = g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=1;jj<=pMat->Nc;jj++) {
					x=PGMATRIX_DATA(pV,jj,j);
					z=PGMATRIX_DATA(pV,jj,i);
					PGMATRIX_DATA(pV,jj,j)=x*c+z*s;
					PGMATRIX_DATA(pV,jj,i)=z*c-x*s;
				}
				z=dpythag(f,h);
				PGMATRIX_DATA(pS,j,j)=z;
				if (z) {
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (jj=1;jj<=pMat->Nr;jj++) {
					y=PGMATRIX_DATA(pU,jj,j);
					z=PGMATRIX_DATA(pU,jj,i);
					PGMATRIX_DATA(pU,jj,j)=y*c+z*s;
					PGMATRIX_DATA(pU,jj,i)=z*c-y*s;
				}
			}
			PGMATRIX_DATA(pRV1,l,1)=0.0;
			PGMATRIX_DATA(pRV1,k,1)=f;
			PGMATRIX_DATA(pS,k,k)=x;
		}
	}
	PGMATRIX_FREE(pRV1);

	// Sort singular value matrix in descending order if requested:
	if((FlagSorted!=0) && (pS->Nr>1)){
		pcU = PGMATRIX_ALLOC(pU->Nr,1);
		pcV = PGMATRIX_ALLOC(pV->Nr,1);

		l = (pS->Nr >> 1) + 1;
		ir = pS->Nr;
		while(1){
			if (l > 1) {
				--l;
				cS = PGMATRIX_DATA(pS,l,l);
				PGMATRIX_COPY_COLUMN(pcU,1,pU,l);
				PGMATRIX_COPY_COLUMN(pcV,1,pV,l);
			} 
			else {
				cS = PGMATRIX_DATA(pS,ir,ir);
				PGMATRIX_COPY_COLUMN(pcU,1,pU,ir);
				PGMATRIX_COPY_COLUMN(pcV,1,pV,ir);

				PGMATRIX_DATA(pS,ir,ir) = PGMATRIX_DATA(pS,1,1);
				PGMATRIX_COPY_COLUMN(pU,ir,pU,1);
				PGMATRIX_COPY_COLUMN(pV,ir,pV,1);
				if (--ir == 1) {
					PGMATRIX_DATA(pS,1,1) = cS;
					PGMATRIX_COPY_COLUMN(pU,1,pcU,1);
					PGMATRIX_COPY_COLUMN(pV,1,pcV,1);
					break;
				}
			}
			i=l;
			j=l+l;
			while (j <= ir) {
				if (j < ir && PGMATRIX_DATA(pS,j,j) > PGMATRIX_DATA(pS,j+1,j+1)) j++;
				if (cS > PGMATRIX_DATA(pS,j,j)) {
					PGMATRIX_DATA(pS,i,i) = PGMATRIX_DATA(pS,j,j);
					PGMATRIX_COPY_COLUMN(pU,i,pU,j);
					PGMATRIX_COPY_COLUMN(pV,i,pV,j);
					i=j;
					j <<= 1;
				} else j=ir+1;
			}
			PGMATRIX_DATA(pS,i,i)=cS;
			PGMATRIX_COPY_COLUMN(pU,i,pcU,1);
			PGMATRIX_COPY_COLUMN(pV,i,pcV,1);
		}

		PGMATRIX_FREE(pcU);
		PGMATRIX_FREE(pcV);
	}
}
#endif

/**********************************************************************************************
***** GMatrix: Arithmetic with constants
**********************************************************************************************/
#if GMATRIX_DEFINE_ADD_CONST
void PGMATRIX_ADD_CONST(PGMATRIX pMat,double Value)
{
	int i,j;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) += Value;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_CONST
void PGMATRIX_MULTIPLY_CONST(PGMATRIX pMat,double Value)
{
	int i,j;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMat,i,j) *= Value;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_CONST_COPY
void PGMATRIX_MULTIPLY_CONST_COPY(PGMATRIX pMatResult, PGMATRIX pMat,double Value)
{
	int i,j;
	PGMATRIX_SETSIZE(pMatResult,pMat->Nr,pMat->Nc);
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMatResult,i,j) =  PGMATRIX_DATA(pMat,i,j) * Value;
		}	
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_CONST_ADD
void PGMATRIX_MULTIPLY_CONST_ADD(PGMATRIX pMatResult, PGMATRIX pMat,double Value)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_CONST_ADD",pMatResult->Nr != pMat->Nr);	
	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_CONST_ADD",pMatResult->Nc != pMat->Nc);	
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			PGMATRIX_DATA(pMatResult,i,j) +=  PGMATRIX_DATA(pMat,i,j) * Value;
		}	
	}
}
#endif

/**********************************************************************************************
***** GMatrix: Arithmetic with matrices
**********************************************************************************************/
#if GMATRIX_DEFINE_ADD_COPY
void PGMATRIX_ADD_COPY(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_ADD_COPY",pMatA->Nr != pMatB->Nr);	
	GMATRIX_ASSERT("PGMATRIX_ADD_COPY",pMatA->Nc != pMatB->Nc);	
	PGMATRIX_SETSIZE(pMatResult,pMatA->Nr, pMatA->Nc);
	for(i=1;i<=pMatA->Nr;++i){	
		for(j=1;j<=pMatA->Nc;++j){	
			PGMATRIX_DATA(pMatResult,i,j) = PGMATRIX_DATA(pMatA,i,j) + PGMATRIX_DATA(pMatB,i,j);
		}	
	}
}
#endif

#if GMATRIX_DEFINE_ADD
void PGMATRIX_ADD(PGMATRIX pMatA, PGMATRIX pMatB)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_ADD",pMatA->Nr != pMatB->Nr);	
	GMATRIX_ASSERT("PGMATRIX_ADD",pMatA->Nc != pMatB->Nc);	
	for(i=1;i<=pMatB->Nr;++i){	
		for(j=1;j<=pMatB->Nc;++j){	
			PGMATRIX_DATA(pMatA,i,j) += PGMATRIX_DATA(pMatB,i,j);
		}	
	}
}
#endif

#if GMATRIX_DEFINE_SUBSTRACT_COPY
void PGMATRIX_SUBSTRACT_COPY(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB)
{
	int i,j;
	GMATRIX_ASSERT("GMATRIX_SUBSTRACT_COPY",pMatA->Nr != pMatB->Nr);	
	GMATRIX_ASSERT("GMATRIX_SUBSTRACT_COPY",pMatA->Nc != pMatB->Nc);	
	PGMATRIX_SETSIZE(pMatResult,pMatA->Nr, pMatA->Nc);
	for(i=1;i<=pMatA->Nr;++i){	
		for(j=1;j<=pMatA->Nc;++j){	
			PGMATRIX_DATA(pMatResult,i,j) = PGMATRIX_DATA(pMatA,i,j) - PGMATRIX_DATA(pMatB,i,j); 
		}	
	}
}
#endif

#if GMATRIX_DEFINE_SUBSTRACT_IDENTITY_COPY
void PGMATRIX_SUBSTRACT_IDENTITY_COPY(PGMATRIX pMatResult, PGMATRIX pMat)
{
	int i,j;
	GMATRIX_ASSERT("GMATRIX_SUBSTRACT_COPY",pMat->Nr != pMat->Nc);	
	PGMATRIX_SETSIZE(pMatResult,pMat->Nr, pMat->Nc);
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){
			if (j==i){
				PGMATRIX_DATA(pMatResult,i,j) = PGMATRIX_DATA(pMat,i,j) - 1.0;
			}
			else{
				PGMATRIX_DATA(pMatResult,i,j) = PGMATRIX_DATA(pMat,i,j);
			}
		}	
	}
}
#endif

#if GMATRIX_DEFINE_SUBSTRACT_IDENTITY
void PGMATRIX_SUBSTRACT_IDENTITY(PGMATRIX pMat)
{
	int i,j;
	GMATRIX_ASSERT("GMATRIX_SUBSTRACT_COPY",pMat->Nr != pMat->Nc);	
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){
			if (j==i){
				PGMATRIX_DATA(pMat,i,j) = PGMATRIX_DATA(pMat,i,j) - 1.0;
			}
		}	
	}
}
#endif

#if GMATRIX_DEFINE_SUBSTRACT
void PGMATRIX_SUBSTRACT(PGMATRIX pMatA, PGMATRIX pMatB)
{
	int i,j;
	GMATRIX_ASSERT("GMATRIX_SUBSTRACT_COPY",pMatA->Nr != pMatB->Nr);	
	GMATRIX_ASSERT("GMATRIX_SUBSTRACT_COPY",pMatA->Nc != pMatB->Nc);	
	for(i=1;i<=pMatA->Nr;++i){	
		for(j=1;j<=pMatA->Nc;++j){	
			PGMATRIX_DATA(pMatA,i,j) -= PGMATRIX_DATA(pMatB,i,j); 
		}	
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_COPY
void PGMATRIX_MULTIPLY_COPY(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB) 
{
	int i,j;
	int k;
	double Acc;

	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_COPY",pMatA->Nc != pMatB->Nr);	
	PGMATRIX_SETSIZE(pMatResult,pMatA->Nr, pMatB->Nc);
	for(i=1;i<=pMatA->Nr;++i){	
		for(j=1;j<=pMatB->Nc;++j){	
			Acc = 0.0;
			for(k=1;k<=pMatA->Nc;++k){	
				Acc += PGMATRIX_DATA(pMatA,i,k)*PGMATRIX_DATA(pMatB,k,j);	
			}	
			PGMATRIX_DATA(pMatResult,i,j) = Acc;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_COPY_EXTENDED
void PGMATRIX_MULTIPLY_COPY_EXTENDED(PGMATRIX pMatResult, PGMATRIX pMatA, int FlagTransposeA, PGMATRIX pMatB, int FlagTransposeB) 
{
	int k;
	int i,j;
	double Acc;

	if ( !FlagTransposeA && !FlagTransposeB ){
		GMATRIX_ASSERT("PGMATRIX_MULTIPLY_COPY_EXTENDED",pMatA->Nc != pMatB->Nr);	
		PGMATRIX_SETSIZE(pMatResult,pMatA->Nr, pMatB->Nc);
		for(i=1;i<=pMatA->Nr;++i){	
			for(j=1;j<=pMatB->Nc;++j){	
				Acc = 0.0;
				for(k=1;k<=pMatA->Nc;++k){	
					Acc += PGMATRIX_DATA(pMatA,i,k)*PGMATRIX_DATA(pMatB,k,j);	
				}	
				PGMATRIX_DATA(pMatResult,i,j) = Acc;	
			}	
		}
	}
	if ( !FlagTransposeA && FlagTransposeB ){
		GMATRIX_ASSERT("PGMATRIX_MULTIPLY_COPY_EXTENDED (pMatB is transposed)",pMatA->Nc != pMatB->Nc);	
		PGMATRIX_SETSIZE(pMatResult,pMatA->Nr, pMatB->Nr);
		for(i=1;i<=pMatA->Nr;++i){	
			for(j=1;j<=pMatB->Nr;++j){	
				Acc = 0.0;
				for(k=1;k<=pMatA->Nc;++k){	
					Acc += PGMATRIX_DATA(pMatA,i,k)*PGMATRIX_DATA(pMatB,j,k);	
				}	
				PGMATRIX_DATA(pMatResult,i,j) = Acc;	
			}	
		}
	}
	if ( FlagTransposeA && !FlagTransposeB ){
		GMATRIX_ASSERT("PGMATRIX_MULTIPLY_COPY_EXTENDED (pMatA is transposed)",pMatA->Nr != pMatB->Nr);	
		PGMATRIX_SETSIZE(pMatResult,pMatA->Nc, pMatB->Nc);
		for(i=1;i<=pMatA->Nc;++i){	
			for(j=1;j<=pMatB->Nc;++j){	
				Acc = 0.0;
				for(k=1;k<=pMatA->Nr;++k){	
					Acc += PGMATRIX_DATA(pMatA,k,i)*PGMATRIX_DATA(pMatB,k,j);	
				}	
				PGMATRIX_DATA(pMatResult,i,j) = Acc;	
			}	
		}
	}
	if ( FlagTransposeA && FlagTransposeB ){
		GMATRIX_ASSERT("PGMATRIX_MULTIPLY_COPY_EXTENDED (pMatA and pMatB are transposed)",pMatA->Nr != pMatB->Nc);	
		PGMATRIX_SETSIZE(pMatResult,pMatA->Nc, pMatB->Nr);
		for(i=1;i<=pMatA->Nc;++i){	
			for(j=1;j<=pMatB->Nr;++j){	
				Acc = 0.0;
				for(k=1;k<=pMatA->Nr;++k){	
					Acc += PGMATRIX_DATA(pMatA,k,i)*PGMATRIX_DATA(pMatB,j,k);	
				}	
				PGMATRIX_DATA(pMatResult,i,j) = Acc;	
			}	
		}
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_ADD
void PGMATRIX_MULTIPLY_ADD(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB) 
{
	int k;
	int i,j;
	double Acc;

	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_ADD",pMatA->Nc != pMatB->Nr);	
	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_ADD",pMatResult->Nr != pMatA->Nr);	
	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_ADD",pMatResult->Nc != pMatB->Nc);	
	for(i=1;i<=pMatA->Nr;++i){	
		for(j=1;j<=pMatB->Nc;++j){	
			Acc = 0.0;
			for(k=1;k<=pMatA->Nc;++k){	
				Acc += PGMATRIX_DATA(pMatA,i,k)*PGMATRIX_DATA(pMatB,k,j);	
			}	
			PGMATRIX_DATA(pMatResult,i,j) += Acc;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_MULTIPLY_SUBSTRACT
void PGMATRIX_MULTIPLY_SUBSTRACT(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB) 
{
	int k;
	int i,j;
	double Acc;

	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_SUBSTRACT",pMatA->Nc != pMatB->Nr);	
	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_SUBSTRACT",pMatResult->Nr != pMatA->Nr);	
	GMATRIX_ASSERT("PGMATRIX_MULTIPLY_SUBSTRACT",pMatResult->Nc != pMatB->Nc);	
	for(i=1;i<=pMatA->Nr;++i){	
		for(j=1;j<=pMatB->Nc;++j){	
			Acc = 0.0;
			for(k=1;k<=pMatA->Nc;++k){	
				Acc += PGMATRIX_DATA(pMatA,i,k)*PGMATRIX_DATA(pMatB,k,j);	
			}	
			PGMATRIX_DATA(pMatResult,i,j) -= Acc;	
		}	
	}
}
#endif

#if GMATRIX_DEFINE_TRIPLEMULTIPLY_COPY
void PGMATRIX_TRIPLEMULTIPLY_COPY(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB, PGMATRIX pMatC, PGMATRIX pMatDummy) 
{
	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_COPY",pMatA->Nc != pMatB->Nr);	
	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_COPY",pMatB->Nc != pMatC->Nr);	
	PGMATRIX_MULTIPLY_COPY(pMatDummy, pMatB, pMatC);	
	PGMATRIX_MULTIPLY_COPY(pMatResult, pMatA, pMatDummy);
}
#endif

#if GMATRIX_DEFINE_TRIPLEMULTIPLY_COPY_EXTENDED
void PGMATRIX_TRIPLEMULTIPLY_COPY_EXTENDED(PGMATRIX pMatResult, PGMATRIX pMatA, BOOL FlagTransposeA, PGMATRIX pMatB, BOOL FlagTransposeB, PGMATRIX pMatC, BOOL FlagTransposeC, PGMATRIX pMatDummy) 
{
//	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_COPY",pMatA->Nc != pMatB->Nr);	
//	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_COPY",pMatB->Nc != pMatC->Nr);	
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy, pMatB, FlagTransposeB, pMatC, FlagTransposeC);	
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatResult, pMatA, FlagTransposeA, pMatDummy, FALSE);
}
#endif

#if GMATRIX_DEFINE_TRIPLEMULTIPLY_ADD
void PGMATRIX_TRIPLEMULTIPLY_ADD(PGMATRIX pMatResult, PGMATRIX pMatA, PGMATRIX pMatB, PGMATRIX pMatC, PGMATRIX pMatDummy) 
{
	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_ADD",pMatA->Nc != pMatB->Nr);	
	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_ADD",pMatB->Nc != pMatC->Nr);	
	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_ADD",pMatResult->Nr != pMatA->Nr); 
	GMATRIX_ASSERT("PGMATRIX_TRIPLEMULTIPLY_ADD",pMatResult->Nc != pMatC->Nc); 
	PGMATRIX_MULTIPLY_COPY(pMatDummy, pMatB, pMatC);	
	PGMATRIX_MULTIPLY_ADD(pMatResult, pMatA, pMatDummy);
}
#endif

#if GMATRIX_DEFINE_COVARIANCE_PROPAGATION_COPY
void PGMATRIX_COVARIANCE_PROPAGATION_COPY(PGMATRIX pMatCovResult, PGMATRIX pMatGain, PGMATRIX pMatCov, PGMATRIX pMatDummy)
{
	GMATRIX_ASSERT("PGMATRIX_COVARIANCE_PROPAGATION_COPY",pMatGain->Nc != pMatCov->Nr); 
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy, pMatCov, 0, pMatGain, 1);	
	PGMATRIX_MULTIPLY_COPY(pMatCovResult, pMatGain, pMatDummy);
}
#endif

#if GMATRIX_DEFINE_COVARIANCE_PROPAGATION_ADD
void PGMATRIX_COVARIANCE_PROPAGATION_ADD(PGMATRIX pMatCovResult, PGMATRIX pMatGain, PGMATRIX pMatCov, PGMATRIX pMatDummy)
{
	GMATRIX_ASSERT("PGMATRIX_COVARIANCE_PROPAGATION_ADD",pMatGain->Nc != pMatCov->Nr); 
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy, pMatCov, 0, pMatGain, 1);	
	PGMATRIX_MULTIPLY_ADD(pMatCovResult, pMatGain, pMatDummy);
}
#endif

#if GMATRIX_DEFINE_MAHALANOBIS_DISTANCE
double PGMATRIX_MAHALANOBIS_DISTANCE(PGMATRIX pMatResidual, PGMATRIX pMatCovResidual, PGMATRIX pMatDummy1, PGMATRIX pMatDummy2)
{
	GMATRIX_ASSERT("PGMATRIX_MAHALANOBIS_DISTANCE",pMatCovResidual->Nc != pMatCovResidual->Nr); 
	GMATRIX_ASSERT("PGMATRIX_MAHALANOBIS_DISTANCE",pMatResidual->Nr != pMatCovResidual->Nr); 
	GMATRIX_ASSERT("PGMATRIX_MAHALANOBIS_DISTANCE",pMatResidual->Nc != 1); 
//	PGMATRIX_COPY(pMatDummy1, pMatCovResidual); // Inverse using gauss_jordan decomposition.
//	PGMATRIX_INVERSE(pMatDummy1);	// pMatDummy1 = inv(pMatCovResidual);
	PGMATRIX_INVERSE_COPY(pMatDummy1,pMatCovResidual);	// pMatDummy1 = inv(pMatCovResidual);
	PGMATRIX_MULTIPLY_COPY(pMatDummy2, pMatDummy1, pMatResidual); // pMatDummy2 = inv(pMatCovResidual)*pMatResidual;
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy1, pMatResidual,1,pMatDummy2,0); // pMatDummy1 = pMatResidual'*inv(pMatCovResidual)*pMatResidual;
	return(PGMATRIX_DATA(pMatDummy1,1,1));
}
#endif

/**********************************************************************************************
***** GMatrix: Submatrices
**********************************************************************************************/
#if GMATRIX_DEFINE_SUBMATRIX_COPY
void PGMATRIX_SUBMATRIX_COPY(PGMATRIX pMat, int nl, int nc, PGMATRIX pMatOrigin)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY",nl<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY",nc<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY",pMat->Nr < (nl+pMatOrigin->Nr-1));	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY",pMat->Nc < (nc+pMatOrigin->Nc-1));	
    for(i=nl;(i<=(nl+pMatOrigin->Nr-1)) && (i<=pMat->Nr);++i){ 
		for(j=nc;(j<=(nc+pMatOrigin->Nc-1)) && (j<=pMat->Nc);++j){ 
			PGMATRIX_DATA(pMat,i,j) = PGMATRIX_DATA(pMatOrigin,i-nl+1,j-nc+1); 
		} 
    }
}
#endif

#if GMATRIX_DEFINE_SUBMATRIX_COPY_FROM_ORIGIN
void PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN(PGMATRIX pMat, int nrs, int ncs, PGMATRIX pMatOrigin)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN",nrs<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN",ncs<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN",pMat->Nr < nrs);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN",pMat->Nc < ncs);
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN",nrs > pMatOrigin->Nr);
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_FROM_ORIGIN",ncs > pMatOrigin->Nc);
    for(i=1;i<=nrs;i++){ 
		for(j=1;j<=ncs;j++){ 
			PGMATRIX_DATA(pMat,i,j) = PGMATRIX_DATA(pMatOrigin,i,j); 
		} 
    }
}
#endif

#if GMATRIX_DEFINE_SUBMATRIX_ADD
void PGMATRIX_SUBMATRIX_ADD(PGMATRIX pMat, int nl, int nc, PGMATRIX pMatOrigin)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_ADD",nl<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_ADD",nc<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_ADD",pMat->Nr < (nl+pMatOrigin->Nr-1));	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_ADD",pMat->Nc < (nc+pMatOrigin->Nc-1));	
    for(i=nl;(i<=(nl+pMatOrigin->Nr-1)) && (i<=pMat->Nr);++i){ 
		for(j=nc;(j<=(nc+pMatOrigin->Nc-1)) && (j<=pMat->Nc);++j){ 
			PGMATRIX_DATA(pMat,i,j) += PGMATRIX_DATA(pMatOrigin,i-nl+1,j-nc+1); 
		} 
    }
}
#endif

#if GMATRIX_DEFINE_SUBMATRIX_COPY_EXTENDED
void PGMATRIX_SUBMATRIX_COPY_EXTENDED(PGMATRIX pMat, int nl, int nc, PGMATRIX pMatOrigin, int FlagTransposeOrigin)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_EXTENDED",nl<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_EXTENDED",nc<1);
	if (!FlagTransposeOrigin){
		GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_EXTENDED (pMatOrigin is not transposed)",pMat->Nr < (nl+pMatOrigin->Nr-1));	
		GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_EXTENDED (pMatOrigin is not transposed)",pMat->Nc < (nc+pMatOrigin->Nc-1));	
	    for(i=nl;(i<=(nl+pMatOrigin->Nr-1)) && (i<=pMat->Nr);++i){ 
			for(j=nc;(j<=(nc+pMatOrigin->Nc-1)) && (j<=pMat->Nc);++j){ 
				PGMATRIX_DATA(pMat,i,j) = PGMATRIX_DATA(pMatOrigin,i-nl+1,j-nc+1); 
			} 
		}
	}
	else{
		GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_EXTENDED (pMatOrigin is transposed)",pMat->Nr < (nl+pMatOrigin->Nc-1));	
		GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_COPY_EXTENDED (pMatOrigin is transposed)",pMat->Nc < (nc+pMatOrigin->Nr-1));	
	    for(i=nl;(i<=(nl+pMatOrigin->Nc-1)) && (i<=pMat->Nr);++i){ 
			for(j=nc;(j<=(nc+pMatOrigin->Nr-1)) && (j<=pMat->Nc);++j){ 
				PGMATRIX_DATA(pMat,i,j) = PGMATRIX_DATA(pMatOrigin,j-nc+1,i-nl+1); 
			} 
		}
	}
}
#endif

#if GMATRIX_DEFINE_SUBMATRIX_FILL
void PGMATRIX_SUBMATRIX_FILL(PGMATRIX pMat,int nlb,int nle,int ncb,int nce,double Value)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_FILL",nlb<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_FILL",ncb<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_FILL",pMat->Nr < nle);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_FILL",pMat->Nc < nce);	
    for(i=nlb;(i<=nle) && (i<=pMat->Nr);++i){ 
		for(j=ncb;(j<=nce) && (j<=pMat->Nc);++j){ 
			PGMATRIX_DATA(pMat,i,j) = Value; 
		} 
    }
}
#endif

#if GMATRIX_DEFINE_SUBMATRIX_MULTIPLY_CONST
void PGMATRIX_SUBMATRIX_MULTIPLY_CONST(PGMATRIX pMat,int nlb,int nle,int ncb,int nce,double Value)
{
	int i,j;
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_MULTIPLY_CONST",nlb<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_MULTIPLY_CONST",ncb<1);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_MULTIPLY_CONST",pMat->Nr < nle);	
	GMATRIX_ASSERT("PGMATRIX_SUBMATRIX_MULTIPLY_CONST",pMat->Nc < nce);	
    for(i=nlb;(i<=nle) && (i<=pMat->Nr);++i){ 
		for(j=ncb;(j<=nce) && (j<=pMat->Nc);++j){ 
			PGMATRIX_DATA(pMat,i,j) *= Value; 
		} 
    }
}
#endif

/**********************************************************************************************
***** GMatrix: Pseudoinverse using SVD:
***** Abstract: Apinv is the pseudoinverse of A.
***** Note: Apinv == NULL, put result on A matrix.
**********************************************************************************************/
#if GMATRIX_DEFINE_PSEUDOINVERSE
void PGMATRIX_PSEUDOINVERSE(PGMATRIX pApinv, PGMATRIX pA, PGMATRIX pMatDummy)
{
	int n;
	double tol, norm;
	PGMATRIX pU;
	PGMATRIX pS;
	PGMATRIX pV;

	GMATRIX_ASSERT("PGMATRIX_PSEUDOINVERSE",pA->Nr <= 0);
	GMATRIX_ASSERT("PGMATRIX_PSEUDOINVERSE",pA->Nc <= 0);
	GMATRIX_ASSERT("PGMATRIX_PSEUDOINVERSE",pMatDummy->MaxSize < pA->Nr*pA->Nc);

	pU = PGMATRIX_ALLOC(pA->Nr,pA->Nc);
	pS = PGMATRIX_ALLOC(pA->Nc,pA->Nc);
	pV = PGMATRIX_ALLOC(pA->Nc,pA->Nc);

	// Compute SVD:
	PGMATRIX_SVD(pU,pS,pV,pA,FALSE);
	// Compute norm:
	norm = fabs(PGMATRIX_DATA(pS,1,1));
	for(n=2;n<=pA->Nc;++n){
		norm = GMATRIXMACRO_MAX(norm,fabs(PGMATRIX_DATA(pS,n,n)));
	}
	// Compute tolerance:
	tol = (GMATRIXMACRO_MAX(pA->Nr,pA->Nc)) * norm * 1e-12;

	// Compute inv(S):
	for(n=1;n<=pA->Nc;++n){
		if (fabs(PGMATRIX_DATA(pS,n,n))<=tol){
			PGMATRIX_DATA(pS,n,n) = 0.0;
		}
		else{
			PGMATRIX_DATA(pS,n,n) = 1.0 / PGMATRIX_DATA(pS,n,n);
		}
	}
	// Compute pinv:
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy,pS,FALSE,pU,TRUE);
	if (pApinv==NULL){
		PGMATRIX_MULTIPLY_COPY_EXTENDED(pA,pV,FALSE,pMatDummy,FALSE);
	}
	else{
		PGMATRIX_MULTIPLY_COPY_EXTENDED(pApinv,pV,FALSE,pMatDummy,FALSE);
	}

	PGMATRIX_FREE(pU);
	PGMATRIX_FREE(pS);
	PGMATRIX_FREE(pV);
}
#endif

/**********************************************************************************************
***** GMatrix: Moore-Penrose Pseudoinverses
**********************************************************************************************/
#if GMATRIX_DEFINE_LEFT_PSEUDOINVERSE_COPY
void PGMATRIX_LEFT_PSEUDOINVERSE_COPY(PGMATRIX pApinv, PGMATRIX pA, PGMATRIX pMatDummy)
{
	GMATRIX_ASSERT("PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE",pApinv->Nr!=pA->Nc);	
	GMATRIX_ASSERT("PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE",pApinv->Nc!=pA->Nr);	

	// Spinv = inv(S'*S)*S'
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy, pA, 1, pA, 0);
	PGMATRIX_INVERSE(pMatDummy);
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pApinv, pMatDummy, 0, pA, 1);
}
#endif

/**********************************************************************************************
***** GMatrix: Derivative computations
**********************************************************************************************/
#if GMATRIX_DEFINE_DERIVATIVE_LEFT_PSEUDOINVERSE
void PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE(PGMATRIX pdApinvdx, PGMATRIX pdAdx, PGMATRIX pA, PGMATRIX pApinv, PGMATRIX pMatDummy1, PGMATRIX pMatDummy2)
{
	GMATRIX_ASSERT("PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE",pdAdx->Nr!=pA->Nr);	
	GMATRIX_ASSERT("PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE",pdAdx->Nc!=pA->Nc);	
	GMATRIX_ASSERT("PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE",pApinv->Nr!=pA->Nc);	
	GMATRIX_ASSERT("PGMATRIX_DERIVATIVE_LEFT_PSEUDOINVERSE",pApinv->Nc!=pA->Nr);	
	PGMATRIX_SETSIZE(pdApinvdx,pApinv->Nr,pApinv->Nc);	

	// Apinv = inv(A'*A)*A'
	// dApinvdx = -inv(A'*A)*dAdx'*(A*Apinv-I)-Apinv*dAdx*Apinv.

	PGMATRIX_TRIPLEMULTIPLY_COPY(pdApinvdx,pApinv,pdAdx,pApinv,pMatDummy1); // dApinvdx = Apinv*dAdx*Apinv
	PGMATRIX_MULTIPLY_COPY(pMatDummy1,pA,pApinv); // MatDummy1 = A*Apinv;
	PGMATRIX_SUBSTRACT_IDENTITY(pMatDummy1); // MatDummy1 = A*Apinv-I;

	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy2,pdAdx,1,pMatDummy1,0); // MatDummy2 = dAdx'*(A*Apinv-I);
	PGMATRIX_MULTIPLY_COPY_EXTENDED(pMatDummy1,pA,1,pA,0); // MatDummy1 = A'*A
	PGMATRIX_INVERSE(pMatDummy1); // MatDummy1 = inv(A'*A);
	PGMATRIX_MULTIPLY_ADD(pdApinvdx,pMatDummy1,pMatDummy2); // dApinvdx = inv(A'*A)*dAdx'*(A*Apinv-I) + Apinv*dAdx*Apinv;
	PGMATRIX_MULTIPLY_CONST(pdApinvdx,-1.0); // dApinvdx = -inv(A'*A)*dAdx'*(A*Apinv-I) - Apinv*dAdx*Apinv;
}
#endif

/**********************************************************************************************
***** GMatrix: Rank of a matrix
**********************************************************************************************/
#if GMATRIX_DEFINE_RANK_FROMSVD
int PGMATRIX_RANK_FROMSVD(PGMATRIX pU, PGMATRIX pS, PGMATRIX pV)
{
	double tol,maxabs;
	int rank,i;

	GMATRIX_ASSERT("PGMATRIX_RANK",pS->Nr==0);	
	GMATRIX_ASSERT("PGMATRIX_RANK",pS->Nc==0);	

	// Compute tol:
	maxabs = -1.0;
	for(i=1;i<=pS->Nr;++i){
		maxabs = GMATRIXMACRO_MAX(maxabs,PGMATRIX_DATA(pS,i,i));
	}
	tol = GMATRIXMACRO_MAX(pU->Nr,pV->Nr) * maxabs * 1e-16;

	// Compute the rank:
	for(i=1,rank=0;i<=pS->Nr;++i){
		if(PGMATRIX_DATA(pS,i,i) > tol){
			++rank;
		}
	}

	return(rank);
}
#endif

#if GMATRIX_DEFINE_RANK
int PGMATRIX_RANK(PGMATRIX pMat)
{
	int rank;
	PGMATRIX pU;
	PGMATRIX pS;
	PGMATRIX pV;

	GMATRIX_ASSERT("PGMATRIX_RANK",pMat->Nr==0);	
	GMATRIX_ASSERT("PGMATRIX_RANK",pMat->Nc==0);	

	// Alloc matrices:
	pU = PGMATRIX_ALLOC(pMat->Nr,pMat->Nc);
	pS = PGMATRIX_ALLOC(pMat->Nc,pMat->Nc);
	pV = PGMATRIX_ALLOC(pMat->Nc,pMat->Nc);

	// Compute SVD:
	PGMATRIX_SVD(pU,pS,pV,pMat,FALSE);

	// Compute rank:
	rank = PGMATRIX_RANK_FROMSVD(pU,pS,pV);

	// Free matrices:
	PGMATRIX_FREE(pU);
	PGMATRIX_FREE(pS);
	PGMATRIX_FREE(pV);

	return(rank);
}
#endif

/**********************************************************************************************
***** GMatrix: Statistics
**********************************************************************************************/

#if GMATRIX_DEFINE_MEAN
double PGMATRIX_MEAN(PGMATRIX pMat)
{
	int i,j;
	double sum = 0.0;
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			sum += PGMATRIX_DATA(pMat,i,j);
		}	
	}
	return(sum/(pMat->Nr*pMat->Nc));
}
#endif

#if GMATRIX_DEFINE_VARIANCE
double PGMATRIX_VARIANCE(PGMATRIX pMat)
{
	int i,j;
	double sum = 0.0;
	double mean;

	mean = PGMATRIX_MEAN(pMat);
	for(i=1;i<=pMat->Nr;++i){	
		for(j=1;j<=pMat->Nc;++j){	
			sum += GMATRIXMACRO_SQR(PGMATRIX_DATA(pMat,i,j)-mean);
		}	
	}
	return(sum/(pMat->Nr*pMat->Nc-1));
}
#endif

/**********************************************************************************************
***** GMatrix: CMEX
**********************************************************************************************/

#ifdef GMATRIX_CMEX
PGMATRIX PGMATRIX_ALLOC_FROM_MXARRAY(const mxArray *pmxarray)
{
	PGMATRIX mat;
	double *pData;
	int i,j;

	pData = mxGetPr(pmxarray);
	mat = PGMATRIX_ALLOC(mxGetM(pmxarray),mxGetN(pmxarray));
	for(i=1;i<=mat->Nr;++i){
		for(j=1;j<=mat->Nc;++j){
			PGMATRIX_DATA(mat,i,j) = pData[(i-1)+(j-1)*mat->Nr];
		}
	}

	return(mat);
}

mxArray *PGMATRIX_COPY_TO_MXARRAY(PGMATRIX pMat)
{
	mxArray *pmxarray;
	double *pData;
	int i,j;

	pmxarray = mxCreateDoubleMatrix(pMat->Nr,pMat->Nc,mxREAL);

	pData = mxGetPr(pmxarray);
	for(i=1;i<=pMat->Nr;++i){
		for(j=1;j<=pMat->Nc;++j){
			pData[(i-1)+(j-1)*pMat->Nr] = PGMATRIX_DATA(pMat,i,j);
		}
	}

	return(pmxarray);
}
#endif
