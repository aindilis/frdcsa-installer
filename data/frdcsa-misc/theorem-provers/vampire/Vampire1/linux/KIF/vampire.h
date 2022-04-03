//
//  file vampire.h
//  main file for the big vampire
//

#ifndef __vampire__
#define __vampire__

char run (VampireKernel& kernel, 
	  long timeLimit,
	  VampireKernel::Options& options, 
	  Problem& problem
	  ); 

void passOptions (const Options& vampire, 
		  VampireKernel::Options& kernel, 
		  const char* jobId);

const char* generateJobId();
ofstream* createTab (Options& opts, const char* jobId);

#endif
