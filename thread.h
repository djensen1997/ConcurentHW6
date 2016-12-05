// ----------------------------------------------------------- 
// NAME : Dane Jensen                         User ID: danej
// DUE DATE : 12/09/2016                                    
// PROGRAM ASSIGNMENT #6                                   
// FILE NAME : thread.h         
// PROGRAM PURPOSE :                                           
//    	
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
	#include <string>
	using namespace std;
	//thread includes
	#include "ThreadClass.h"
	#define EOD -1

	class Row:public Thread{
	public:
		Row(int* values, int m, Semaphore* PB);
	private:
		Semaphore* pb;
		int values[];
		void TheadFunc(void);
		SynOneToOneChannel* channel;
	};

	class Col:public Thread{
	public:
		Col(int* values, int n);
	private:
		Semaphore* pb;
		int values[];
		void TheadFunc(void);
		SynOneToOneChannel* channel;
	};

	class Index:public Thread{
	public:
		Index(int** C, int M, int N, int row,int col, Semaphore* PB);
	private:
		int row,col,m,n;
		Semaphore* pb;
		int** c;
		void TheadFunc(void);
		int value = 0;
		SynOneToOneChannel* left,down;
	};

#endif

