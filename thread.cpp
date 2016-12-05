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
// FUNCTION  Canible::Canible
//     	canible constructor, sets basic variables for a canible
//		thread
// PARAMETER USAGE :                                           
// 		n:		the number of this canible
//		B:		a reference to the boat monitor
//		*END:	used to terminate at the end of the game
//		PB:		the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Canible::Canible(int n, Boat* B, int* END, Semaphore* PB)
:uid(n), b(B), end(END), printBlock(PB){
	srand(time(NULL));
	//adds spaces to the format string
	formatS = std::string(uid, ' ');
	out = std::string();
	sprintf(buf, "Canible %d starts\n", uid);
	out = formatS + buf;
	printBlock->Wait();
	cout << out;
	printBlock->Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  Canible::ThreadFunc
//     	the thread function for a canible, handles canible's
//		actions outside of the monitor
// PARAMETER USAGE :                                           
// 		n/a
// FUNCTION CALLED :                                           
//  	Boat::cabibleArrives, sprintf, Delay
// ----------------------------------------------------------- 
void Canible::ThreadFunc(void){
	Thread::ThreadFunc();
	while(*end == 0){
		int temp = rand() % 200;
		for(int i = 0; i < temp; i++)
			Delay();
		sprintf(buf, "Canible %d arrives\n", uid);
		out = formatS + buf;
		printBlock->Wait();
		cout << out;
		printBlock->Signal();
		b->canibleArrives(uid);
	}
	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  Missionary::Missionary
//     	missionary constructor, sets basic variables for a missionary
//		thread
// PARAMETER USAGE :                                           
// 		n:		the number of this missionary
//		B:		a reference to the boat monitor
//		*END:	used to terminate at the end of the game
//		PB:		the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Missionary::Missionary(int n, Boat* B, int* END, Semaphore* PB)
:uid(n), b(B), end(END), printBlock(PB){
	srand(time(NULL));
	formatS = std::string(uid, ' ');
	sprintf(buf, "Missionary %d starts\n", uid);
	out = formatS + buf;
	printBlock->Wait();
	cout << out;
	printBlock->Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  Missionary::ThreadFunc
//     	the thread function for a missionary, handles missionary's
//		actions outside of the monitor
// PARAMETER USAGE :                                           
// 		n/a
// FUNCTION CALLED :                                           
//  	Boat::missionaryArrives, sprintf, Delay
// ----------------------------------------------------------- 
void Missionary::ThreadFunc(void){
	Thread::ThreadFunc();
	while(*end == 0){
		int temp = rand() % 200;
		for(int i = 0; i < temp; i++)
			Delay();
		sprintf(buf, "Missionary %d arrives\n", uid);
		out = formatS + buf;
		printBlock->Wait();
		cout << out;
		printBlock->Signal();
		b->missionaryArrives(uid);
	}
	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  Ships::Ships
//     	ship constructor, sets basic variables for a ship
//		thread
// PARAMETER USAGE :                                           
// 		trips:	the number of trips this boat will do
//		B:		a reference to the boat monitor
//		*END:	used to terminate at the end of the game
//		PB:		the output mutex
// FUNCTION CALLED :                                           
// 		sprintf, srand
// ----------------------------------------------------------- 
Ship::Ship(int T, Boat* B, int* END, Semaphore* PB)
:trips(T), b(B), end(END), printBlock(PB){
	srand(time(NULL));
	formatS = "***** ";
	sprintf(buf, "Boat thread starts\n");
	out = formatS + buf;
	printBlock->Wait();
	cout << out;
	printBlock->Signal();
	load = 0;
}

// ----------------------------------------------------------- 
// FUNCTION  Ship::ThreadFunc
//     	the thread function for a ship, handles ship's
//		actions outside of the monitor
// PARAMETER USAGE :                                           
// 		n/a
// FUNCTION CALLED :                                           
//  	Boat::boatReady, Boat::boatDone, sprintf, Delay
// ----------------------------------------------------------- 
void Ship::ThreadFunc(void){
	Thread::ThreadFunc();
	while(load < trips){
		int temp = rand() % 200;
		for(int i = 0; i < temp; i++)
			Delay();
		out = formatS + "The boat is ready\n";
		printBlock->Wait();
		cout << out;
		printBlock->Signal();
		b->boatReady(p);
		sprintf(buf, "Boat load (%d): Passenger list (%s, %s, %s)\n", load+1,p[0],p[1],p[2]);
		out = formatS + buf;
		printBlock->Wait();
		cout << out;
		printBlock->Signal();
		temp = rand() % 200;
		for(int i = 0; i < temp; i++)
			Delay();
		b->boatDone();
		sprintf(buf, "Boat load (%d): Completed\n", load+1);
		out = formatS + buf;
		printBlock->Wait();
		cout << out;
		printBlock->Signal();
		load++;
		temp = rand() % 200;
		for(int i = 0; i < temp; i++)
			Delay();
	}
	b->boatClosed();
	Exit();
}

