#include <cstdio>
#include <cstring>

int main() {
	int n;

	scanf("%d", &n);

	double a[n][n], l[n][n], u[n][n], m[n][n], s;
	double y[n], b[n];

	for (int i = 0; i < n; i++) 
		for (int j = 0; j < n; j++)
			scanf("%lf", &a[i][j]);

	memset(l, 0, sizeof(l));
	memset(u, 0, sizeof(u));

	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			s = 0;
			for (int k = 0; k <= i-1; k++)
				s += l[i][k]*u[k][j];
			u[i][j] = a[i][j] - s;
		} 
		
		for (int j = i; j < n; j++) {
			s = 0;
			for (int k = 0; k <= i-1; k++) s += l[j][k]*u[k][i];
			l[j][i] = (a[j][i] - s)/u[i][i];
		}
	}

	for (int t = 0; t < n; t++) {
		memset(b, 0, sizeof(b));
		b[t] = 1; // coluna da identidade

		/* Obtem L y = b */
		for( int i = 0; i < n; i++ ) {
			s = 0;
			for( int j = 0; j < i; j++ ) s += l[i][j]*y[j];
			y[i] = (b[i] - s)/l[i][i];
		}

		/* Obtem U x = y */
		for( int i = n-1; i >= 0; i-- ) {
			s = 0;
			for( int j = i+1; j < n; j++ ) s += u[i][j]*m[j][t];
			m[i][t] = (y[i] - s)/u[i][i];
		}
	}

	printf("L: \n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%lf ", l[i][j]);
		printf("\n");
	}

	printf("\nU: \n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%lf ", u[i][j]);
		printf("\n");
	}
	
	printf("\nInversa: \n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%lf ", m[i][j]);
		printf("\n");
	}

	return 0;
}
