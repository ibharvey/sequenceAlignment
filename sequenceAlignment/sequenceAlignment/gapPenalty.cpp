#include "stdafx.h"
#include "gapPenalty.h"


gapPenalty::gapPenalty()
{
	this->_isSet = false;
}

float* gapPenalty::getPenalties()
{
	float shsvchcv[] = { _startHor, _startVer, _contHor, _contVer };
	return shsvchcv;
}

void gapPenalty::setPenalties(float a, float b, float c, float d){
	_startHor = a;
	_startVer = b;
	_contHor = c;
	_contVer = d;
	this->_isSet = true;
	return;
}

gapPenalty::~gapPenalty()
{
}
