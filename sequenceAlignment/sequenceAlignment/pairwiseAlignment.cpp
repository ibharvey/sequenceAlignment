#include "stdafx.h"
#include "pairwiseAlignment.h"
#include <time.h>

pairwiseAlignment::pairwiseAlignment() : alignment()
{
	_template.clear();
	_query.clear();
	_algnTemplate.clear();
	_algnQuery.clear();
}

pairwiseAlignment::pairwiseAlignment(sequence a, sequence b, substitutionMatrix m) : alignment(m){
	_template = a;
	_query = b;
	_algnTemplate.clear();
	_algnTemplate.setName(a.getName());
	_algnTemplate.setProtein(a.isProtein());
	_algnQuery.clear();
	_algnQuery.setName(b.getName());
	_algnQuery.setProtein(b.isProtein());
}
pairwiseAlignment::pairwiseAlignment(sequence a, sequence b, substitutionMatrix m, gapPenalty p){
	pairwiseAlignment::pairwiseAlignment(a, b, m, p, p, p);
}

pairwiseAlignment::pairwiseAlignment(sequence a, sequence b, substitutionMatrix m, gapPenalty nRegP, gapPenalty regP){
	pairwiseAlignment::pairwiseAlignment(a, b, m, nRegP, regP, nRegP);
}

pairwiseAlignment::pairwiseAlignment(sequence a, sequence b, substitutionMatrix m, gapPenalty preP, gapPenalty regP, gapPenalty postP) : alignment(m, preP, regP, postP){
	_template = a;
	_query = b;
	_algnTemplate.clear();
	_algnTemplate.setName(a.getName());
	_algnTemplate.setProtein(a.isProtein());
	_algnQuery.clear();
	_algnQuery.setName(b.getName());
	_algnQuery.setProtein(b.isProtein());
}

pairwiseAlignment::pairwiseAlignment(std::pair<sequence, sequence> seqPair, substitutionMatrix m){
	pairwiseAlignment(seqPair.first, seqPair.second, m);
}

void pairwiseAlignment::print(){
	std::cout << _algnTemplate.getSequence() << std::endl << _algnQuery.getSequence() << std::endl;
}

void pairwiseAlignment::save(){
	std::ofstream outFile("output.txt");
	pairwiseAlignment::save(outFile);
}

void pairwiseAlignment::save(std::ofstream &outFile){
	outFile << this->_algnTemplate.getName() << std::endl << this->_algnTemplate.getSequence() << std::endl
		<< this->_algnQuery.getName() << std::endl << this->_algnQuery.getSequence() << std::endl;
}

void pairwiseAlignment::save(std::string outFileStr){
	std::ofstream outFile(outFileStr);
	pairwiseAlignment::save(outFile);
}

//Caveat: Not well tested yet
void pairwiseAlignment::findStats(){
	if (_algnTemplate.empty() || _algnQuery.empty()){
		//Trying to find statistics without first running the alignment
		this->slowAlign();
	}
	_nTotalChars = _algnTemplate.length();
	for (unsigned int i = 0; i < _algnTemplate.length(); i++){
		if ((_algnTemplate[i] != '-') && (_algnQuery[i] != '-')){
			//Identity must be a perfect match
			if (_algnTemplate[i] == _algnQuery[i]){
				_nIdentity += 1;
			}
			//Similarity is just a positive matrix score
			if (_matr[std::make_pair(_algnTemplate[i], _algnQuery[i])]>0){
				_nSimilarity += 1;
			}
		}
		else{
			//This index has a gap in one of the sequences
			_nGaps += 1;
		}
	}
	_nMatchedChars = _nTotalChars - _nGaps;
}

void pairwiseAlignment::slowAlign(){
	std::vector<std::vector<full::node>> allNodes = vectorNodes();
	this->relateNodes(allNodes);
	std::pair<std::string, std::string> pairAlign = this->recurAlign(allNodes[_template.length() + 1][_query.length() + 1]);
	_algnTemplate.setSequence(pairAlign.first);
	_algnQuery.setSequence(pairAlign.second);
}

//When calling this function, use the last node in the table to find the best alignment
std::pair<std::string, std::string> pairwiseAlignment::recurAlign(full::node aNode){
	if (aNode.parent->horizontalIndex() == -1 && aNode.parent->verticalIndex() == -1){
		return std::make_pair(std::string(), std::string());
	}
	else{
		char one, two;
		if (aNode.horizontalIndex() < _query.length()){
			if (aNode.parent->horizontalIndex() == aNode.horizontalIndex()){
				one = '-';
			}
			else{
				one = _query[aNode.horizontalIndex()];
			}
		}
		else{
			one = '-';
		}
		if (aNode.verticalIndex() < _template.length()){
			if (aNode.parent->verticalIndex() == aNode.verticalIndex()){
				two = '-';
			}
			else{
				two = _template[aNode.verticalIndex()];
			}
		}
		else{
			two = '-';
		}

		std::pair<std::string, std::string> recurHolder = recurAlign(*aNode.parent);
		//If both are a gap, skip this alignment point
		if (one == '-' && two == '-'){
			return std::make_pair(recurHolder.first, recurHolder.second);
		}
		else{
			return std::make_pair(recurHolder.first + one, recurHolder.second + two);
		}
	}
}



std::vector<std::vector<full::node>> pairwiseAlignment::relateNodes(std::vector<std::vector<full::node>> &curNodes){
	curNodes[0][0].setNull();
	for (unsigned int i = 0; i < curNodes.size(); i++){
		for (unsigned int j = 0; j < curNodes[i].size(); j++){
			if (i + 1 < curNodes.size()){
				curNodes[i][j].down = &curNodes[i + 1][j];
				//Find the correct gap penalty values to use:
				gapPenalty curPenalty;
				float valPenalty;
				if (j == 0) {
					curPenalty = _prePenalty;
				}
				else if (j == curNodes[0].size() - 1){
					curPenalty = _postPenalty;
				}
				else{
					curPenalty = _regPenalty;
				}
				//Use vertical penalty
				if (curNodes[i][j].isGap()){
					//'continue' gap penalty
					valPenalty = curPenalty.getVCont();
				}
				else{
					//'start' gap penalty
					valPenalty = curPenalty.getVStart();
				}
				//Check whether this is the best option for curNodes[i+1][j]
				curNodes[i + 1][j].gapCheck(&curNodes[i][j], valPenalty);
			}
			if (j + 1 < curNodes[i].size()){
				curNodes[i][j].right = &curNodes[i][j + 1];
				//Find the correct gap penalty values to use:
				//Use horizontal penalty
				gapPenalty curPenalty;
				float valPenalty;
				if (i == 0) {
					curPenalty = _prePenalty;
				}
				else if (i == curNodes.size() - 1){
					curPenalty = _postPenalty;
				}
				else{
					curPenalty = _regPenalty;
				}
				//Use vertical penalty
				if (curNodes[i][j].isGap()){
					//'continue' gap penalty
					valPenalty = curPenalty.getHCont();
				}
				else{
					//'start' gap penalty
					valPenalty = curPenalty.getHStart();
				}
				//Check whether this is the best option for curNodes[i][j+1]
				curNodes[i][j + 1].gapCheck(&curNodes[i][j], valPenalty);
			}
			if (i + 1<curNodes.size() && j + 1<curNodes[i].size()){
				curNodes[i][j].diagonal = &curNodes[i + 1][j + 1];
				//No diagonal gap penalties exist
				curNodes[i + 1][j + 1].matchCheck(&curNodes[i][j]);
			}
		}
	}
	return curNodes;
}

void pairwiseAlignment::checkSpeed(){
	unsigned int max = 10;
	time_t start, stop;
	std::cout << "Using the make_pair matrix: ";
	for (int i = 0; i < max; i++){
		time(&start);
		pairwiseAlignment::vectorNodes();
		time(&stop);
		if (i == max - 1)std::cout << "and ";
		std::cout << difftime(stop, start) << " secs";
		if (i == max - 1){
			std::cout << "." << std::endl;
		}
		else{
			std::cout << ", ";
		}
	}

	std::cout << "Using the [][] matrix:" << std::endl;
	for (int i = 0; i < max; i++){
		time(&start);
		pairwiseAlignment::vectorNodes(false);
		time(&stop);
		if (i == max - 1)std::cout << "and ";
		std::cout << difftime(stop, start) << " secs";
		if (i == max - 1){
			std::cout << "." << std::endl;
		}
		else{
			std::cout << ", ";
		}
	}
}

//The boolean check argument exists because I am trying to optimize this method.
//It is the bottleneck of the alignment.
std::vector<std::vector<full::node>> pairwiseAlignment::vectorNodes(bool check){
	//Check both sequences to make sure they are only letters of the alphabet
	if (!_matr.canScore(_template)) std::cerr << _template.getName() << " contains characters not in the substitution matrix" << std::endl;
	if (!_matr.canScore(_query)) std::cerr << _query.getName() << " contains characters not in the substitution matrix" << std::endl;

	//Generate the table of nodes for dynamic programming
	std::vector<std::vector<full::node>>allNodes;
	//This puts a 'pre' gap on the top of the table
	std::vector<full::node>topGaps;
	for (unsigned int i = 0; i <= _query.length() + 1; i++){
		topGaps.push_back(full::node(0, i - 1, -1, true));
	}
	allNodes.push_back(topGaps);
	for (unsigned int i = 0; i < _template.length(); i++){
		std::vector<full::node> lineNodes;
		//This puts the 'pre' gap on the left side of the table
		lineNodes.push_back(full::node(0, -1, i, true));
		for (unsigned int j = 0; j < _query.length(); j++){
			if (check){
				lineNodes.push_back(full::node(_matr[std::make_pair(_template[i], _query[j])], j, i));
			}
		}
		//This puts the gap on the right side of the table
		lineNodes.push_back(full::node(0, _query.length(), i, false, true));
		allNodes.push_back(lineNodes);
	}
	//This puts the gap on the bottom of the table
	std::vector<full::node>bottomGaps;
	for (unsigned int i = 0; i <= _query.length() + 1; i++){
		bottomGaps.push_back(full::node(0, i - 1, _template.length(), false, true));
	}
	allNodes.push_back(bottomGaps);
	return allNodes;
}

pairwiseAlignment::~pairwiseAlignment()
{

}
