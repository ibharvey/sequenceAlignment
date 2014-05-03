#include "stdafx.h"
#include "alignment.h"

alignment::alignment(substitutionMatrix &m){
	alignment::alignment();
	_matr = m;
	//What should the default gap penalties be?
	//Gap Penalties:	1) startHor		2) startVer		3) contHor		4) contVer
	_regPenalty= gapPenalty(-10, -10, -4, -4);
	_prePenalty = gapPenalty(0, 0, 0, 0);
	_postPenalty = gapPenalty(0, 0, 0, 0);
}
//
alignment::alignment(substitutionMatrix &m, gapPenalty preP, gapPenalty regP, gapPenalty postP){
	alignment::alignment();
	_matr = m;
	_prePenalty = preP;
	_regPenalty = regP;
	_postPenalty = postP;
}

alignment::alignment()
{
	_nIdentity = 0;
	_nSimilarity = 0;
	_nGaps = 0;
}

alignment::~alignment()
{

}
