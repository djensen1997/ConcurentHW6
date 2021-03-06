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
	Row* row[m];
	SynOneToOneChannel* rowChannel[m+1][b+1];
	SynOneToOneChannel* colChannel[m+1][b+1];
	for(int i = 0; i < m; i++){
		int userid = (i+1)*(b+1);
		char name[100];
		sprintf(name, "Channel%d-%d", userid, userid + 1);
		rowChannel[i+1][0] = new SynOneToOneChannel(name, userid, userid + 1);
		row[i] = new Row(&(A[i][0]), n, userid,i, rowChannel[i+1][0], pb);
	}

	Col* col[b];
	for(int i = 0; i < b; i++){
		int userid = i+1;
		int* values = (int*)malloc(sizeof(int) * a);
		for(int j = 0; j < a; j++){
			values[j] = B[j][i];
		}
		char name[100];
		sprintf(name, "Channel%d-%d", userid, (b+1) + userid);
		colChannel[0][i+1] = new SynOneToOneChannel(name, userid, (b+1) + (userid));
		col[i] = new Col(values, a, userid,i, colChannel[0][i+1],pb);
	}

	for(int i = 0; i < m; i++){
		for(int j = 0; j < b; j++){
			int row = i+1;
			int col = j+1;
			int userid = (i+1)*(b+1) + (j+1);
			if(j+1 == b && i+1 == m){
				array[i][j] = new Index(&ans[i][j], userid, i, j, NULL, colChannel[row-1][col],
					NULL,rowChannel[row][col-1],pb);
			}else if(j+1 == b){
				char downName[100];
				sprintf(downName, "Channel%d-%d", userid, userid + (b+1));
				colChannel[row][col] = new SynOneToOneChannel(downName, userid, userid + (b+1));
				array[i][j] = new Index(&ans[i][j], userid, i, j, colChannel[row][col], colChannel[row-1][col],
					NULL,rowChannel[row][col-1],pb);
			}else if(i+1 == m){
				char rightName[100];
				sprintf(rightName, "Channel%d-%d", userid, userid + 1);
				rowChannel[row][col] = new SynOneToOneChannel(rightName, userid, userid + 1);
				array[i][j] = new Index(&ans[i][j], userid, i, j, NULL, colChannel[row-1][col],
					rowChannel[row][col],rowChannel[row][col-1],pb);
			}else{
				char rightName[100];
				sprintf(rightName, "Channel%d-%d", userid, userid + 1);
				rowChannel[row][col] = new SynOneToOneChannel(rightName, userid, userid + 1);
				char downName[100];
				sprintf(downName, "Channel%d-%d", userid, userid + (b+1));
				colChannel[row][col] = new SynOneToOneChannel(downName, userid, userid + (b+1));
				array[i][j] = new Index(&ans[i][j], userid, i, j, colChannel[row][col], colChannel[row-1][col],
					rowChannel[row][col],rowChannel[row][col-1],pb);
			}
		}
	}

	for(int i = 0; i < m; i++){
		row[i]->Begin();
	}
	for(int i = 0; i < b; i++){
		col[i]->Begin();
	}
	for(int i = 0; i < m; i++){
		for(int j = 0; j < b; j++){
			array[i][j]->Begin();
		}
	}

	for(int i = 0; i < m; i++){
		row[i]->Join();
	}

	for(int i = 0; i < b; i++){
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


