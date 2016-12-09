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
//     	index constructor, sets basic variables for a index
//		thread
// PARAMETER USAGE :
//		C:		The reference to the index this thread will put its value into
//		row:	this index's row
//		col:	this index's column
//		PB:		the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Index::Index(int* C, int M, int N, int row,int col, Semaphore* PB)
:c(C), m(M), n(N), r(row), c(col), pb(PB){
	//compute threadid's for all threads around this one
	UserDefinedThreadID = (r+1)*m + col;
	int leftID = (r+1)*m + col - 1;
	int downID = (r+1)*m + col + r;
	int upID = (r+1)*m + col - r;
	if(upID < 0){
		upID = 60 + col;
	}
	if(UserDefinedThreadID%10 == 0){
		leftID = 60 + (r+1)*10;
	}
	int rightID = (r+1)*m + col + 1;
	//make the channels
	char leftName[100];
	char downName[100];
	sprintf(leftName, "Channel%d-%d", leftID,UserDefinedThreadID);
	sprintf(downName, "Channel%d-%d", UserDefinedThreadID, downID);
	sprintf(rightName, "Channel%d-%d", UserDefinedThreadID, rightID);
	sprintf(upName, "Channel%d-%d", upID, UserDefinedThreadID);
	if(row + 1 != M){
		right = new SynOneToOneChannel(rightName, UserDefinedThreadID, rightID);
	}

	if(col + 1 != N){
		down = new SynOneToOneChannel(downName, UserDefinedThreadID, downID);
	}
	
	up = new SynOneToOneChannel(upName, upID, UserDefinedThreadID);
	left = new SynOneToOneChannel(leftName, leftID, UserDefinedThreadID);
	
}


// ----------------------------------------------------------- 
// FUNCTION  Index::ThreadFunc
//     	runs the index thread.  Takes a value from the thread
//		up and right of this one, passes those down and left 
//		respectivly and then multiplies those values together
// PARAMETER USAGE :
//		N/A
// FUNCTION CALLED :                                           
// 		sprintf, srand, Cannel.send, Cannel.recieve
// ----------------------------------------------------------- 
void Index::ThreadFunc(void){
	Thread::ThreadFunc();
	//temp variables to hold data
	int Down = 0;
	int Left = 0;
	//termination condition
	int end = 0;
	while(end == 0){
		//get information
		up->recieve(Down, sizeof(int));
		left->recieve(Left, sizeof(int));
		//decide to end the loop or use data
		if(down == EOD || left == EDO){
			end = 1;
		}else{
			value += (up * right);
		}
		//pass data along
		right->send(&Left, sizeof(int));
		down->send(&Down, sizeof(int));
	}
	*c = value;//set the array value to the computed value
	Exit();//terminate the thread
}


// ----------------------------------------------------------- 
// FUNCTION  Row::Row
//     	row constructor, sets basic variables for a row
//		thread
// PARAMETER USAGE :
//		values:		values to pass through
//		row:		this row thread's row number
//		M:			The Number of col's in the array
//		PB:			the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Row::Row(int* values, int row, int M, Semaphore* PB)
:vals(values), r(row), m(M), pb(PB){
	UserDefinedThreadID = 60 + (r+1)*10;
	int rightID = (r+1)*m;
	char rightName[100];
	sprintf(rightName, "Channel%d-%d", UserDefinedThreadID, rightID);
	channel = new SynOneToOneChannel(rightName, UserDefinedThreadID, rightID);
}


// ----------------------------------------------------------- 
// FUNCTION  Row::ThreadFunc
//     	row thead function, passes a list of values right,
//		then terminates
// PARAMETER USAGE :
//		N/A
// FUNCTION CALLED :                                           
// 		sprintf, srand, Cannel.send, Cannel.recieve
// ----------------------------------------------------------- 
void Row::ThreadFunc(void){
	Thread::ThreadFunc();
	for(int i = 0; i < m; i++){
		channel->send(vals[i], sizeof(int));
	}
	channel->send(EOD, sizeof(int));
	Exit();
}


// ----------------------------------------------------------- 
// FUNCTION  Col::Col
//     	Col constructor, sets basic variables for a Col
//		thread
// PARAMETER USAGE :
//		values:		values to pass through
//		col:		this col thread's col number
//		N:			The Number of rows's in the array
//		PB:			the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Col::Col(int* values, int col, int N, Semaphore* PB)
:vals(values), c(col), n(N), pb(PB){
	UserDefinedThreadID = 60 + col;
	int downID = UserDefinedThreadID - 60;
	char downName[100];
	sprintf(downName, "Channel%d-%d", UserDefinedThreadID, downID);
	channel = new SynOneToOneChannel(rightName, UserDefinedThreadID, downID);
}


// ----------------------------------------------------------- 
// FUNCTION  Row::ThreadFunc
//     	col thead function, passes a list of values right,
//		then terminates
// PARAMETER USAGE :
//		N/A
// FUNCTION CALLED :                                           
// 		sprintf, srand, Cannel.send, Cannel.recieve
// ----------------------------------------------------------- 
void Row::ThreadFunc(void){
	Thread::ThreadFunc();
	for(int i = 0; i < n; i++){
		channel->send(vals[i], sizeof(int));
	}
	channel->send(EOD, sizeof(int));
	Exit();
}
