#pragma once

#include <vector>
#include <map>

#include "substitutionMatrix.h"
#include "pairwiseAlignment.h"
#include "sequence.h"

class msa : public alignment
{
private:
	std::vector<sequence>_seqs;
	std::vector<sequence>_algnSeqs;
public:
	msa();
	msa(std::vector<sequence>s, substitutionMatrix matr);
	msa(std::vector<sequence>s, substitutionMatrix matr, gapPenalty preP, gapPenalty regP, gapPenalty postP);
	//Star aligns the 0-index sequence against all other sequences given
	void star();
	~msa();
};

