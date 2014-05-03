#include "stdafx.h"
#include "node.h"

full::node::node(){}



full::node::node(float aScore, int nHor, int nVer, bool isSGap, bool isEGap)
{
	this->_isStart = isSGap;
	this->_isEnd = isEGap;
	this->_isGap = false;
	//This is the score to be added during dynamic programming.
	this->_score = aScore;
	//The greatest dynScore is kept during dynamic programming:
	//Therefore, the MIN dynScore will always be replaced
	this->_dynScore = -std::numeric_limits<float>::max();
	this->_horizontalIndex = nHor;
	this->_verticalIndex = nVer;
}

void full::node::matchCheck(node *p){
	if (this->_score + p->_dynScore > this->_dynScore){
		this->_dynScore = this->_score + p->_dynScore;
		this->_isGap = false;
		this->parent = p;
	}
}

void full::node::gapCheck(node *p, float aDynScore){
	if (aDynScore+p->_dynScore > _dynScore){
		this->_dynScore = aDynScore + p->_dynScore;
		this->_isGap = true;
		this->parent = p;
	}
}

full::node::~node()
{
}
