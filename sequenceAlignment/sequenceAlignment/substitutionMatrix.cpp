#include <iostream>

#include "stdafx.h"
#include "substitutionMatrix.h"


substitutionMatrix::substitutionMatrix()
{
}

int substitutionMatrix::operator[](std::pair<char, char> p){
	return _pairMatrix[p];
}

substitutionMatrix::substitutionMatrix(std::string fileName){
	//inputting the substitution matrix
	substitutionMatrix::load(fileName);
}

bool substitutionMatrix::canScore(std::string aString){
	for (int i = 0; i < aString.length(); i++){
		bool isIn = false;
		for (int j = 0; j < _alphabet.size(); j++){
			//automatically makes the character uppercase
			if (toupper(aString[i])== _alphabet[j]) isIn = true;
		}
		if (!isIn) return false;
	}
	return true;
}

bool substitutionMatrix::canScore(std::vector<std::string> aStrVec){
	bool theCheck = true;
	for (unsigned int i = 0; i < aStrVec.size(); i++){
		if (!this->canScore(aStrVec[i])) {
			theCheck = false;
			break;
		}
	}
	return theCheck;
}

bool substitutionMatrix::canScore(sequence aSeq){
	return this->canScore(aSeq.getSequence());
}

bool substitutionMatrix::canScore(std::vector<sequence> aSeqVec){
	bool theCheck = true;
	for (unsigned int i = 0; i < aSeqVec.size(); i++){
		if (!this->canScore(aSeqVec[i])){
			theCheck = false;
			break;
		}
	}
	return theCheck;
}
//Clear the current matrix
void substitutionMatrix::clear(){
	this->_alphabet.clear();
	this->_pairMatrix.clear();
}

//load in a new substitution matrix
void substitutionMatrix::load(std::string fileName){
	this->clear();
	//Try to find the substitution matrix file
	std::ifstream inFile(fileName);
	if (!inFile){
		inFile.open("Matrices/" + fileName);
	}
	if (!inFile){
		std::cerr << "Could not find specified matrix. Reminder: This is case-sensitive" << std::endl << "Defaulting to BLOSUM62" << std::endl;
		inFile.open("Matrices/BLOSUM62");
	}
	std::vector<std::string> lines;
	while (!inFile.eof()){
		std::string temp;
		std::getline(inFile, temp);
		if (temp[0] != '#') lines.push_back(temp);
	}
	//extract the alphabet for this substitution matrix
	for (unsigned int i = 0; i < lines[0].length(); i++){
		if (lines[0][i] != ' ') this->_alphabet.push_back(lines[0][i]);
	}
	//extract the matrix table
	for (unsigned int i = 0; i < _alphabet.size(); i++){
		std::istringstream ins;
		ins.str(lines[i + 1].substr(1));
		for (unsigned int j = 0; j < _alphabet.size(); j++){
			int val;
			ins >> val;
			this->_pairMatrix[std::make_pair(_alphabet[i], _alphabet[j])] = val;
		}
	}
}

void substitutionMatrix::identity(std::vector<sequence>seqs, int amplitude){
	//Get the alphabet for this identity matrix
	for (unsigned int i = 0; i < seqs.size(); i++){
		for (unsigned int j = 0; j < seqs[i].length(); j++){
			bool inAlphabet = false;
			for (unsigned int k = 0; k < this->_alphabet.size(); k++){
				if (seqs[i][j] == this->_alphabet[k])inAlphabet = true;
			}
			if (!inAlphabet)this->_alphabet.push_back(seqs[i][j]);
		}
	}
	//Generate the identity matrix
	for (unsigned int i = 0; i < this->_alphabet.size(); i++){
		for (unsigned int j = 0; j < this->_alphabet.size(); j++){
			if (i == j){
				this->_pairMatrix[std::make_pair(this->_alphabet[i], this->_alphabet[j])] = amplitude;
			}
			else{
				this->_pairMatrix[std::make_pair(this->_alphabet[i], this->_alphabet[j])] = 0;
			}
		}
	}
}

substitutionMatrix::~substitutionMatrix()
{
}
