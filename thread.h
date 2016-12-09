// ----------------------------------------------------------- 
// NAME : Dane Jensen                         User ID: danej
// DUE DATE : 12/09/2016                                    
// PROGRAM ASSIGNMENT #6                                   
// FILE NAME : thread.h         
// PROGRAM PURPOSE :                                           
//    	contains all the class/function prototypes nessisary 
//		for competing array multiplication using channels
// ----------------------------------------------------------- 



#ifndef quickh
	#define quickh //include guard

	//process includes
	#include <sys/types.h>
	#include <unistd.h>
	//shared memory includes
	#include <sys/ipc.h>
	#include <sys/shm.h>
	//standard includes
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	#include <iostream>
	#include <string.h>
	using namespace std;
	//thread includes
	#include "ThreadClass.h"
	#define EOD -1

	class Row:public Thread{
	public:
		Row(int* values, int size, int uid, int row, 
			SynOneToOneChannel* chan, Semaphore* PB);
	private:
		char buf[200];
		int* vals;
		int m, r;

		void ThreadFunc(void);
		SynOneToOneChannel* channel;
	};

	class Col:public Thread{
	public:
		Col(int* values, int size, int uid, int col, 
			SynOneToOneChannel* chan, Semaphore* PB);
	private:
		char buf[200];
		int* vals;
		int n,m,c;
		void ThreadFunc(void);
		SynOneToOneChannel* channel;
	};

	class Index:public Thread{
	public:
		Index(int* output, int row, int col, int uid, 
			SynOneToOneChannel* colChan, SynOneToOneChannel* colChanUp, 
			SynOneToOneChannel* rowChan, SynOneToOneChannel* rowChanLeft, Semaphore* PB);
	private:
		char buf[200];
		int row,col,m,n;
		Semaphore* pb;
		int b,e;
		int* c;
		void ThreadFunc(void);
		int value;
		SynOneToOneChannel* left;
		SynOneToOneChannel* down;
		SynOneToOneChannel* up;
		SynOneToOneChannel* right;
	};

	void printArray(int* array, int array_size);

#endif

