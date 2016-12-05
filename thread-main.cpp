// ----------------------------------------------------------- 
// NAME : Dane Jensen                         User ID: danej
// DUE DATE : 12/09/2016                                    
// PROGRAM ASSIGNMENT #6                                   
// FILE NAME : thread-main.cpp         
// PROGRAM PURPOSE :                                           
//    	
// ----------------------------------------------------------- 

#include "thread.h"
#include "boat-monitor.h"

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
	//   ./prog5 c m b
	//num canibles, num missionaries, num crossings
	if(argc < 4){
		return 0;
	}
	int c = atoi(argv[1]);
	if(c == 0){
		c = 8;
	}
	int m = atoi(argv[2]);
	if(m == 0){
		m = 8;
	}
	int b = atoi(argv[3]);
	if(b == 0){
		b = 5;
	}
	int end = 0;

	Semaphore* printBlock = new Semaphore("Print Block", 1);

	//init
	Boat* B = new Boat("Boat Monitor", &end, printBlock, b);
	Ship* s = new Ship(b, B, &end, printBlock);
	s->Begin();
	Missionary* M[m];
	for(int i = 0; i < m; i++){
		M[i] = new Missionary(i+1, B, &end, printBlock);
		M[i]->Begin();
	}
	Canible* C[c];
	for(int i = 0; i < c; i++){
		C[i] = new Canible(i+1, B, &end, printBlock);
		C[i]->Begin();
	}

	//cleanup
	s->Join();
	return 0;
}
