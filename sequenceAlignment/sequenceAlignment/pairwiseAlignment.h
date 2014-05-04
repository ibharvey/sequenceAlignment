#pragma once

#include <iostream>
#include <fstream>

#include "alignment.h"
#include "sequence.h"
#include "substitutionMatrix.h"
#include "node.h"


class pairwiseAlignment : public alignment
{
private:
	//Unaligned template
	sequence _template;
	//Unaligned query
	sequence _query;
	//Aligned template
	sequence _algnTemplate;
	//Aligned query
	sequence _algnQuery;
	//Creates & configures the table for dynamic programming
	std::vector<std::vector<full::node>> vectorNodes(bool check = true);
	//Performs the dynamic programming for sequence alignment
	std::vector<std::vector<full::node>> relateNodes(std::vector<std::vector<full::node>> &curNodes);
	//Recursively finds the alignment given the last node of the dynamic programming table and both sequences
	std::pair<std::string, std::string> recurAlign(full::node aNode);
public:
	pairwiseAlignment();
	/*
	Use default gap penalty values
		Non-standard gaps: All 0
		Standard gaps: start: -10 && continue: -4
	*/
	pairwiseAlignment(sequence a, sequence b, substitutionMatrix m);
	//Set all gap penalties to one gap penalty type
	pairwiseAlignment(sequence a, sequence b, substitutionMatrix m, gapPenalty p);
	//Set non-standard gap penalties to the first and standard gap penalties to the second 
	pairwiseAlignment(sequence a, sequence b, substitutionMatrix m, gapPenalty nRegP, gapPenalty regP);
	//Give separate pre penalties, regular penalties, and post penalties of a dynamic programming alignment
	pairwiseAlignment(sequence a, sequence b, substitutionMatrix m, gapPenalty preP, gapPenalty regP, gapPenalty postP);
	pairwiseAlignment(std::pair < sequence, sequence> seqPair, substitutionMatrix m);
	sequence getTemplate(){ return _algnTemplate;}
	sequence getQuery(){ return _algnQuery; }
	void setTemplate(sequence a){ _template = a; }
	void setQuery(sequence b){ _query = b; }
	//Used to test new changes to see what can optimize the alignment
	void checkSpeed();
	//I feel this takes too long
	void slowAlign();
	//Finds %-identity & %-similarity
	void findStats();
	//Print the sequences to the console
	void print();
	//Save the sequence alignment to a file
	void save();
	void save(std::ofstream &outFile);
	void save(std::string outFileStr);
	~pairwiseAlignment();
};

