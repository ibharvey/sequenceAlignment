#pragma once

#include <string>
#include "substitutionMatrix.h"
#include "gapPenalty.h"


class alignment
{
protected:
	int _nGaps;
	int _nIdentity;
	float _nSimilarity;
	int _nTotalChars;
	int _nMatchedChars;
	substitutionMatrix _matr;
	gapPenalty _prePenalty;
	gapPenalty _regPenalty;
	gapPenalty _postPenalty;
public:
	alignment();
	//All alignments have an associated substitution matrix
	alignment(substitutionMatrix &m);
	alignment(substitutionMatrix &m, gapPenalty preP, gapPenalty regP, gapPenalty postP);
	~alignment();
};

