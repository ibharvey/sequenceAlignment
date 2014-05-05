/*
Purpose:
	Getopter is an object to organize getopt-based argument handling

This version of getopt was written by:
	Ludvik Jerabek
	http://www.codeproject.com/Articles/157001/Full-getopt-Port-for-Unicode-and-Multibyte-Microso
*/
#pragma once

//Basic getopt functionality
#include "getopt.h"
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

//Processing arguments
#include <iostream>
#include <vector>

//io file stuffs
#include <fstream>
#include <sstream>

#include "sequence.h"
#include "substitutionMatrix.h"
#include "gapPenalty.h"

class getopter
{
private:
	/*Variables*/
	int _c;
	int _verboseFlag;
	int _quietFlag;
	//Perform all possible pairwise alignments
	int _makePairs;
	//The type of MSA to use (eg "star")
	std::string _msaId;
	//Substitution matrix object
	substitutionMatrix _matrix;
	/*Gap Penalties*/
	gapPenalty _prePenalty;
	gapPenalty _regPenalty;
	gapPenalty _postPenalty;
	//A vector of input sequences 
	std::vector<sequence>_sequences;

	/*Functions*/
	//Method to generate a vector of sequences from a FASTA file
	std::vector<sequence>parseSeqs(std::string aFile);
	//Trying to parse multiple formats that a user might input for gap penalties
	void parseGap(gapPenalty& penalty, std::string strArg);
public:
	getopter();
	substitutionMatrix getMatrix() const { return this->_matrix; }
	std::string getMSAId() const { return this->_msaId; }
	gapPenalty getPrePenalty() const { return this->_prePenalty; }
	gapPenalty getRegPenalty() const { return this->_regPenalty; }
	gapPenalty getPostPenalty() const { return this->_postPenalty; }
	std::vector<sequence>getSequences()const{ return this->_sequences; }
	sequence getASequence(size_t index)const{ return this->_sequences[index]; }
	int iteratePairs()const{ return this->_makePairs; }
	getopter::getopter(int argc, TCHAR* argv[]);
	~getopter();
};

