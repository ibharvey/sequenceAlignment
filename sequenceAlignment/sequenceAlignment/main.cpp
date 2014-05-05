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

enum saErrors{NoArguments=-1};

void interpretPenalties(getopter &opts);
void runAlignments(getopter &opts);
void runAlignments(getopter &opts, gapPenalty one);
void runAlignments(getopter &opts, gapPenalty one, gapPenalty two);
void runAlignments(getopter &opts, gapPenalty one, gapPenalty two, gapPenalty three);

//_tmain() is required for getopt to function correctly
int _tmain(int argc, TCHAR** argv)
{
	if (argc <= 1){
		std::cout << "Error: No arguments given" << std::endl;
		return saErrors::NoArguments;
	}

	getopter opts = getopter(argc, argv);

	//Put testing stuff here
	if (TEST){
		//Gap Penalties:	1) startHor		2) startVer		3) contHor		4) contVer
		gapPenalty regPenalties = gapPenalty(-.3, -.2, -1.1, -1.2);
		gapPenalty prePenalties = gapPenalty(0, -14.2, 0, -3.9);
		gapPenalty postPenalties = gapPenalty(0, 0, 0, 0);
		pairwiseAlignment compareTwo = pairwiseAlignment(opts.getASequence(0), opts.getASequence(1), opts.getMatrix(), opts.getPrePenalty(), opts.getRegPenalty(), opts.getPostPenalty());
		compareTwo.slowAlign();
		compareTwo.save();
	}
	else{
		//Need to interpret which gap penalties are set in 'opts' to build with the correct alignment constructor
		interpretPenalties(opts);
	}
	
	return 0;
}

void runAlignments(getopter &opts){
	if (opts.getMSAId() != ""){
		msa multi = msa(opts.getSequences(), opts.getMatrix());
		if (opts.getMSAId() == "star"){
			multi.star();
		}
		//output the msa
	}
	if (opts.iteratePairs()==1){
		std::vector<pairwiseAlignment>fullList;
		for (size_t i = 0; i < opts.getSequences().size(); i++){
			for (size_t j = i + 1; j < opts.getSequences().size(); j++){
				pairwiseAlignment temp = pairwiseAlignment(opts.getASequence(i), opts.getASequence(j), opts.getMatrix());
				temp.slowAlign();
				fullList.push_back(temp);
			}
		}
		//output all pairwise alignments
	}
}

void runAlignments(getopter &opts, gapPenalty one){
	if (opts.getMSAId() != ""){
		msa multi = msa(opts.getSequences(), opts.getMatrix(),one,one,one);
		if (opts.getMSAId() == "star"){
			multi.star();
		}
		//output the msa
	}
	if (opts.iteratePairs()==1){
		std::vector<pairwiseAlignment>fullList;
		for (size_t i = 0; i < opts.getSequences().size(); i++){
			for (size_t j = i + 1; j < opts.getSequences().size(); j++){
				pairwiseAlignment temp = pairwiseAlignment(opts.getASequence(i), opts.getASequence(j), opts.getMatrix(), one);
				temp.slowAlign();
				fullList.push_back(temp);
			}
		}
		//output all pairwise alignments
	}
}

void runAlignments(getopter &opts, gapPenalty one, gapPenalty two){
	if (opts.getMSAId() != ""){
		msa multi = msa(opts.getSequences(), opts.getMatrix(), one, two, one);
		if (opts.getMSAId() == "star"){
			multi.star();
		}
		//output the msa
	}
	if (opts.iteratePairs()==1){
		std::vector<pairwiseAlignment>fullList;
		for (size_t i = 0; i < opts.getSequences().size(); i++){
			for (size_t j = i + 1; j < opts.getSequences().size(); j++){
				pairwiseAlignment temp = pairwiseAlignment(opts.getASequence(i), opts.getASequence(j), opts.getMatrix(), one, two);
				temp.slowAlign();
				fullList.push_back(temp);
			}
		}
		//output all pairwise alignments
	}
}

void runAlignments(getopter &opts, gapPenalty one, gapPenalty two, gapPenalty three){
	if (opts.getMSAId() != ""){
		msa multi = msa(opts.getSequences(), opts.getMatrix(), one, two, three);
		if (opts.getMSAId() == "star"){
			multi.star();
		}
		//output the msa
	}
	if (opts.iteratePairs()==1){
		std::vector<pairwiseAlignment>fullList;
		for (size_t i = 0; i < opts.getSequences().size(); i++){
			for (size_t j = i + 1; j < opts.getSequences().size(); j++){
				pairwiseAlignment temp = pairwiseAlignment(opts.getASequence(i), opts.getASequence(j), opts.getMatrix(), one, two, three);
				temp.slowAlign();
				fullList.push_back(temp);
			}
		}
		//output all pairwise alignments
	}
}

/*-----------------------------------------------------------------------------------
2^3 = 8 possible combinations of gap penalties exists
	Currently, the code is written to handle all 8, albeit strangely
	The protocols may change with time to:
		Alter how partial combinations are handled
		Spitt out errors, if preferred
 ---------------------------------------------------------------------------------*/
void interpretPenalties(getopter &opts){
	if (opts.getRegPenalty().isSet()){
		if (opts.getPrePenalty().isSet()){
			if (opts.getPostPenalty().isSet()){
				//Pre && Post && Reg
				runAlignments(opts, opts.getPrePenalty(), opts.getRegPenalty(), opts.getPostPenalty());
			}
			else{
				//Pre && !Post && Reg
				runAlignments(opts, opts.getPrePenalty(), opts.getRegPenalty());
			}
		}
		else if (opts.getPostPenalty().isSet()){
			//!Pre && Post && Reg
			runAlignments(opts, opts.getPostPenalty(), opts.getRegPenalty());
		}
		else{
			//!Pre && !Post && Reg
			runAlignments(opts, opts.getRegPenalty());
		}
	}
	else if (opts.getPrePenalty().isSet()){
		if (opts.getPostPenalty().isSet()){
			//Pre && Post && !Reg
			//I'm going to average the pre and post penalties to make a regular penalty.
			//If there is a better way to do handle this, please let me know.
			float sH = (opts.getPostPenalty().getHStart() + opts.getPrePenalty().getHStart()) / 2;
			float sV = (opts.getPostPenalty().getVStart() + opts.getPrePenalty().getVStart()) / 2;
			float cH = (opts.getPostPenalty().getHCont() + opts.getPrePenalty().getHCont()) / 2;
			float cV = (opts.getPostPenalty().getVCont() + opts.getPrePenalty().getVCont()) / 2;
			gapPenalty avg = gapPenalty(sH, sV, cH, cV);
			runAlignments(opts, opts.getPrePenalty(), avg, opts.getPostPenalty());
		}
		else{
			//Pre && !Post && !Reg
			runAlignments(opts, opts.getPrePenalty());
		}
	}
	else if (opts.getPostPenalty().isSet()){
		//!Pre && Post && !Reg
		runAlignments(opts, opts.getPostPenalty());
	}
	else{
		//!Pre && !Post && !Reg
		runAlignments(opts);
	}
}