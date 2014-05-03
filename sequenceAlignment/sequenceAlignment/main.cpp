// main.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <vector>

#include "getopter.h"

#include "node.h"
#include "gapPenalty.h"
#include "substitutionMatrix.h"
//Automatically includes pairwiseAlignment.h
#include "msa.h"

#define TEST true

//_tmain() is required for getopt to function correctly
int _tmain(int argc, TCHAR** argv)
{
	getopter opts = getopter(argc, argv);
	
	//Put testing stuff here
	if (TEST){
		//Gap Penalties:	1) startHor		2) startVer		3) contHor		4) contVer
		gapPenalty regPenalties = gapPenalty(-.3, -.2, -1.1, -1.2);
		gapPenalty prePenalties = gapPenalty(0, -14.2, 0, -3.9);
		gapPenalty postPenalties = gapPenalty(0, 0, 0, 0);
		pairwiseAlignment compareTwo = pairwiseAlignment(opts.getSequences()[0], opts.getSequences()[1], opts.getMatrix(), prePenalties, regPenalties, postPenalties);
		compareTwo.slowAlign();
		compareTwo.save();
	}
	//Put code for performing the functions specified in the opts
	else{
		//Need to interpret which gap penalties are set in 'opts' to build with the correct alignment constructor
	}
	
	
	return 0;
}
