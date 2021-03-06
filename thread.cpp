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
Index::Index(int* output, int uid, int Row, int Col, 
	SynOneToOneChannel* colChan, SynOneToOneChannel* colChanUp, 
	SynOneToOneChannel* rowChan, SynOneToOneChannel* rowChanLeft, Semaphore* PB)
:c(output), row(Row), col(Col), up(colChanUp), left(rowChanLeft), pb(PB){
	if(colChan == NULL){
		b = 1;
	}else{
		b = 0;
		down = colChan;
	}
	if(rowChan == NULL){
		e = 1;
	}else{
		e = 0;
		right = rowChan;
	}
	//compute threadid's for all threads around this one
	UserDefinedThreadID = uid;
	
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
	pb->Wait();
	sprintf(buf, "      Thread P[%d,%d] started\n",row+1,col+1);
	write(1,buf,strlen(buf));
	pb->Signal();
	//temp variables to hold data
	int Down = 0;
	int Left = 0;
	//termination condition
	int end = 0;
	while(end == 0){
		//get information
		up->Receive(&Down, sizeof(int));
		left->Receive(&Left, sizeof(int));
		pb->Wait();
		sprintf(buf, "      Thread P[%d,%d] received %d from above and %d from left\n",row+1,col+1,Down,Left);
		write(1, buf, strlen(buf));
		pb->Signal();
		//decide to end the loop or use data
		if(Down == EOD && Left == EOD){
			end = 1;
		}else{
			value += (Down * Left);
		}
		//pass data along
		if(e == 1 && b == 1){

		}else if(e != 0){
			pb->Wait();
			sprintf(buf, "      Thread P[%d,%d] sent %d to below\n",row+1,col+1,Down);
			write(1,buf,strlen(buf));
			pb->Signal();
			down->Send(&Down, sizeof(int));
		}else if(b != 0){
			pb->Wait();
			sprintf(buf, "      Thread P[%d,%d] sent %d to right\n",row+1,col+1,Left);
			write(1,buf,strlen(buf));
			pb->Signal();
			right->Send(&Left, sizeof(int));
		}else{
			pb->Wait();
			sprintf(buf, "      Thread P[%d,%d] sent %d to below and %d to right\n",row+1,col+1,Down,Left);
			write(1,buf,strlen(buf));
			pb->Signal();
			right->Send(&Left, sizeof(int));
			down->Send(&Down, sizeof(int));
		}
		
	}
	*c = value;//set the array value to the computed value
	pb->Wait();
	sprintf(buf, "      Thread P[%d,%d] received EOD, saved result %d and terminated\n", row+1,col+1,value);
	write(1, buf, strlen(buf));
	pb->Signal();
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
Row::Row(int* values, int size, int uid, int row, SynOneToOneChannel* chan, Semaphore* PB)
:vals(values), m(size), r(row), channel(chan), pb(PB){
	UserDefinedThreadID = uid;
	
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
	pb->Wait();
	sprintf(buf, "Row thread r[%d] started\n",r+1);
	write(1,buf,strlen(buf));
	pb->Signal();
	for(int i = 0; i < m; i++){
		channel->Send((&(vals[i])), sizeof(int));
		pb->Wait();
		sprintf(buf, "Row thread r[%d] sent %d to P[%d,1]\n", r+1,vals[i],r+1);
		write(1, buf, strlen(buf));
		pb->Signal();
	}
	int temp = EOD;
	channel->Send(&temp, sizeof(int));
	pb->Wait();
	sprintf(buf, "Row thread r[%d] sent EOD to P[%d,1] and terminated\n",r+1,r+1);
	write(1, buf, strlen(buf));
	pb->Signal();
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
Col::Col(int* values, int size, int uid, int col, SynOneToOneChannel* chan, Semaphore* PB)
:vals(values), n(size), c(col), channel(chan), pb(PB){
	UserDefinedThreadID = col+1;
	
	
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
	pb->Wait();
	sprintf(buf, "   Column thread c[%d] started\n",c+1);
	write(1,buf,strlen(buf));
	pb->Signal();
	for(int i = 0; i < n; i++){
		channel->Send(&(vals[i]), sizeof(int));
		pb->Wait();
		sprintf(buf, "   Col thread c[%d] sent %d to P[1,%d]\n", c+1,vals[i],c+1);
		write(1, buf, strlen(buf));
		pb->Signal();
	}
	int temp = EOD;
	channel->Send(&temp, sizeof(int));
	pb->Wait();
	sprintf(buf, "   Col thread c[%d] sent EOD to P[1,%d] and terminated\n",c+1,c+1);
	write(1, buf, strlen(buf));
	pb->Signal();
	free(vals);
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
	int useln = 0;//used to escape code that isn't aplicible to this application
	char formatS[] = "";
	sprintf(out, "");
	strcat(out, formatS);
	//actually output the information
	for(int i = 0; i < array_size; i++){
		char temp[20];
		sprintf(temp, "%d ", array[i]);
		//i is the current number
		//if i%ln is 0, that means that there
		//are alread ln numbers on a givin line,
		//so it prints out the line and array[i]
		//begins the new one
		if(((i%ln) == 0) && (i > 0) && useln == 1){
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
