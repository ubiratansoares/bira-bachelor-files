#include <stdio.h>
#include <meschach/matrix.h>

void print_mat(MAT* A){
	int i, j;
   	for ( i = 0; i < A->m; i++ ) 
		for ( j = 0; j < A->n; j++ ){ 
			printf("%lf\n",A->me[i][j]); 
		}
}

void print_vet(VEC* V){
	int i;
	for(i=0;i< V->dim ; i++)
		printf("%lf\n",V->ve[i]);		
}

int main(){
	int i,j;
   	MAT* H_t;
	MAT* W;
	MAT* aux;
	MAT* aux_inv;
	MAT* pre_w;
	MAT* H;
	VEC* y;
    H = m_get(4,2);
	aux =  m_get(2,2);
	aux_inv = m_get(2,2);
	pre_w = m_get(2,4);
	y = v_get(4);
	W = v_get(2);
    	for(i=0;i< H->m ; i++){
		for(j=0;j < H->n ; j++){
			scanf("%lf",&H->me[i][j]);
		}
	}

	H_t = m_transp(H,MNULL);
	for(i=0; i < y->dim ; i++)
		scanf("%lf",&y->ve[i]);
	mtrm_mlt(H,H,aux);
	m_inverse(aux,aux_inv);
	mmtr_mlt(aux_inv,H,pre_w);
	print_mat(pre_w);
	printf("passou\n");
	print_vet(y);
	printf("passou1\n");
	W = mv_mlt(pre_w,y,VNULL);
	printf("passou2\n");
	print_vet(W);
	return 0;
}
