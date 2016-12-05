// ----------------------------------------------------------- 
// NAME : Dane Jensen                         User ID: danej
// DUE DATE : 12/09/2016                                    
// PROGRAM ASSIGNMENT #6                                   
// FILE NAME : thread.cpp         
// PROGRAM PURPOSE :                                           
//    	
// ----------------------------------------------------------- 

#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  Index::Index
//     	inex constructor, sets basic variables for a index
//		thread
// PARAMETER USAGE :
//		C:		The global array to store the final value into
//		row:	this index's row
//		col:	this index's column
//		PB:		the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Index::Index(int** C, int M, int N, int row,int col, Semaphore* PB)
:c(C), m(M), n(N), r(row), c(col), pb(PB){
	UserDefinedThreadID = (r+1)*m + (c+1)*n;

}
