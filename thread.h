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
	#include "boat-monitor.h"

	class Canible:public Thread{
	public:
		Canible(int n, Boat* B, int* END, Semaphore* PB);//constructor
		char* getName();
	private:
		std::string out;
		char buf[400];
		char* name;
		void ThreadFunc(void);
		Boat* b;
		int uid;
		std::string formatS;
		int* end;
		Semaphore* printBlock;
	};

	class Missionary:public Thread{
	public:
		Missionary(int n, Boat* B, int* END, Semaphore* PB);//constructor
		char* getName();
	private:
		std::string out;
		char buf[400];
		void ThreadFunc(void);
		char* name;
		Boat* b;
		int uid;
		std::string formatS;
		int* end;
		Semaphore* printBlock;
	};

	class Ship:public Thread{
	public:
		Ship(int b, Boat* B, int* END, Semaphore* PB);
	private:
		std::string out;
		char buf[400];
		void ThreadFunc(void);
		Boat* b;
		int trips;
		std::string formatS;
		int* end;
		Semaphore* printBlock;
		char* p[3];		//the passenger list
		int load;
	};

#endif

