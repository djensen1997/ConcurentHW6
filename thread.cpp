// ----------------------------------------------------------- 
// NAME : Dane Jensen                         User ID: danej
// DUE DATE : 12/09/2016                                    
// PROGRAM ASSIGNMENT #6                                   
// FILE NAME : thread.cpp         
// PROGRAM PURPOSE :                                           
//    	Contains the implementation of array multiplication
//		using channels
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
Index::Index(int* C, int M, int N, int Row,int Col, Semaphore* PB)
:c(C), m(M), n(N), row(Row), col(Col), pb(PB){
	//compute threadid's for all threads around this one
	UserDefinedThreadID = (row+1)*m + col;
	sprintf(buf, "      Thread P[%d,%d] started\n",row+1,col+1);
	write(1,buf,strlen(buf));
	int leftID = (row+1)*m + col - 1;
	int downID = (row+1)*m + col + row;
	int upID = (row+1)*m + col - row;
	if(upID < 0){
		upID = 60 + col;
	}
	if(UserDefinedThreadID%10 == 0){
		leftID = 60 + (row+1)*10;
	}
	int rightID = (row+1)*m + col + 1;
	//make the channels
	char leftName[100];
	char downName[100];
	char rightName[100];
	char upName[100];
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
	value = 0;
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
		sprintf(buf, "      Thread P[%d,%d] ready to receive\n", row+1,col+1);
		write(1, buf, strlen(buf));
		up->Receive(&Down, sizeof(int));
		left->Receive(&Left, sizeof(int));
		sprintf(buf, "      Thread P[%d,%d] received %d from above and %d from left\n",row+1,col+1,Down,Left);
		write(1, buf, strlen(buf));
		//decide to end the loop or use data
		if(Down == EOD || Left == EOD){
			end = 1;
		}else{
			value += (Down * Left);
		}
		//pass data along
		right->Send(&Left, sizeof(int));
		down->Send(&Down, sizeof(int));
		sprintf(buf, "      Thread P[%d,%d] sent %d to below and %d to right\n",row+1,col+1,Down,Left);
		write(1,buf,strlen(buf));

	}
	*c = value;//set the array value to the computed value
	sprintf(buf, "      Thread P[%d,%d] received EOD, saved result %d and terminated\n", row,col,value);
	write(1, buf, strlen(buf));
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
	sprintf(buf, "Row thread r[%d] started\n",row);
	write(1,buf,strlen(buf));
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
	//Row thread r[3] sent 5 to P[3,1]
	Thread::ThreadFunc();
	for(int i = 0; i < m; i++){
		sprintf(buf, "Row thread r[%d] ready to send\n", r);
		write(1, buf, strlen(buf));
		channel->Send((&(vals[i])), sizeof(int));
		sprintf(buf, "Row thread r[%d] sent %d to P[%d,1]\n", r,vals[i],r);
		write(1, buf, strlen(buf));
	}
	int temp = EOD;
	channel->Send((void*)&temp, sizeof(int));
	sprintf(buf, "Row thread r[%d] sent EOD to P[%d,1] and terminated\n",r,r);
	write(1, buf, strlen(buf));
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
	sprintf(buf, "   Column thread c[%d] started\n",col);
	write(1,buf,strlen(buf));
	int downID = UserDefinedThreadID - 60;
	char downName[100];
	sprintf(downName, "Channel%d-%d", UserDefinedThreadID, downID);
	channel = new SynOneToOneChannel(downName, UserDefinedThreadID, downID);
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
void Col::ThreadFunc(void){
	Thread::ThreadFunc();
	for(int i = 0; i < n; i++){
		sprintf(buf, "   Col thread c[%d] ready to send\n", c);
		write(1, buf, strlen(buf));
		channel->Send(&(vals[i]), sizeof(int));
		sprintf(buf, "   Col thread c[%d] sent %d to P[1,%d]\n", c,vals[i],c);
		write(1, buf, strlen(buf));
	}
	int temp = EOD;
	channel->Send((void*)&temp, sizeof(int));
	sprintf(buf, "   Col thread c[%d] sent EOD to P[1,%d] and terminated\n",c,c);
	write(1, buf, strlen(buf));
	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  printarray :                         
//    prints all the integers in a givin array                            
// PARAMETER USAGE :                                           
//    array: the array of integers to print
//	  array_size: the size of the givin array
//	  rowSize: the size of the row of the array being printed out       
// FUNCTION CALLED :                                           
//    sprintf, strcat, write         
// ----------------------------------------------------------- 
void printArray(int* array, int array_size){
	//format stuff
	int ln = 20; //how many numbers can be on a line
	char out[400]; //the output buffer, nice and large
	char formatS[] = "";
	sprintf(out, "");
	strcat(out, formatS);
	//actually output the information
	for(int i = 0; i < ln; i++){
		char temp[20];
		sprintf(temp, "%4d", array[i]);
		//i is the current number
		//if i%ln is 0, that means that there
		//are alread ln numbers on a givin line,
		//so it prints out the line and array[i]
		//begins the new one
		if(((i%ln) == 0) && (i > 0)){
			sprintf(temp, "\n");
			strcat(out,temp);
			write(1, out, strlen(out));
			sprintf(out, "");
			strcat(out, formatS);
			sprintf(temp, "%4d", array[i]);
		}else{
			strcat(out, temp);
		}
	}
	//print out the last line
	strcat(out, "\n");
	write(1, out, strlen(out));
}
