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
	#include <string>
	using namespace std;
	//thread includes
	#include "ThreadClass.h"
	#define EOD -1

	class Row:public Thread{
	public:
		Row(int* values, int row, int M, Semaphore* PB);
	private:
		char buf[200];
		Semaphore* pb;
		int* vals;
		int m, r;
		void TheadFunc(void);
		SynOneToOneChannel* channel;
	};

	class Col:public Thread{
	public:
		Col(int* values, int col, int N, Semaphore* PB);
	private:
		char buf[200];
		Semaphore* pb;
		int* vals;
		int n,c;
		void TheadFunc(void);
		SynOneToOneChannel* channel;
	};

	class Index:public Thread{
	public:
		Index(int* output, int row, int col, int M, int N, Semaphore* PB);
	private:
		char buf[200];
		int row,col,m,n;
		Semaphore* pb;
		int* c;
		void TheadFunc(void);
		int value = 0;
		SynOneToOneChannel* left,down,up,right;
	};

	void printArray(int* array, int array_size);

#endif

