// ----------------------------------------------------------- 
// NAME : Dane Jensen                         User ID: danej
// DUE DATE : 12/09/2016                                    
// PROGRAM ASSIGNMENT #6                                   
// FILE NAME : thread-main.cpp         
// PROGRAM PURPOSE :                                           
//    	Compute the product of 2 matricies taken in as a file 
//		input in the command line.
// ----------------------------------------------------------- 

#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  main                          
//		This function is the main method of the program
//		initializes all shared components and makes sure
//		all threads have refrences to them                           
// PARAMETER USAGE :                                           
// 		argc and argv are command line arguments               
// FUNCTION CALLED :                                           
//		all constructors, thread begin and join         
// ----------------------------------------------------------- 
int main(int argc, char** argv){
	//get the arrays in
	int m,n,a,b;//the first array is mxn the other is axb
	scanf("%d", &m);
	scanf("%d", &n);
	int A[m][n];
	char buf[200];

	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			scanf("%d", &(A[i][j]));
		}
	}
	scanf("%d", &a);
	scanf("%d", &b);
	int B[a][b];
	for(int i = 0; i < a; i++){
		for(int j = 0; j < b; j++){
			scanf("%d", &(B[i][j]));
		}
	}

	//answer array will be size mxb
	if(a != n){
		cout << "The inner dimentions of the arrays must be equal\n";
		return 0;
	}

	Semaphore* pb = new Semaphore("Print Block", 1);
	int ans[m][b];
	Index* array[m][b];
	Row* row[b];
	for(int i = 0; i < m; i++){

		row[i] = new Row(&(A[i][0]), i, b, pb);
	}

	Col* col[m];
	for(int i = 0; i < b; i++){
		int values[m];
		for(int j = 0; j < a; j++){
			values[i] = B[j][i];
		}
		col[i] = new Col(values, i, m, b, pb);
	}

	for(int i = 0; i < m; i++){
		for(int j = 0; j < b; j++){
			array[i][j] = new Index(&ans[i][j], m, b, i, j, pb);
		}
	}

	for(int i = 0; i < m; i++){
		row[i]->Begin();
	}

	Col* col[m];
	for(int i = 0; i < b; i++){
		col[i]->Begin();
	}

	for(int i = 0; i < m; i++){
		for(int j = 0; j < b; j++){
			array[i][j]->Begin();
		}
	}

	for(int i = 0; i < b; i++){
		row[i]->Join();
	}

	for(int i = 0; i < m; i++){
		col[i]->Join();
	}

	for(int i = 0; i < m; i++){
		for(int j = 0; j < b; j++){
			array[i][j]->Join();
		}
	}

	//*** From main ***
	cout << "*** From main ***" << endl;
	sprintf(buf, "Matrix A: %d rows and %d columns\n", m,n);
	write(1, buf, strlen(buf));
	for(int i = 0; i < m; i++){
		printArray(A[i], n);
	}

	sprintf(buf, "Matrix B: %d rows and %d columns\n", a,b);
	write(1, buf, strlen(buf));
	for(int i = 0; i < a; i++){
		printArray(B[i], b);
	}

	sprintf(buf, "Matrix C: %d rows and %d columns\n", m,b);
	write(1, buf, strlen(buf));
	for(int i = 0; i < m; i++){
		printArray(ans[i], n);
	}

	return 0;
}


