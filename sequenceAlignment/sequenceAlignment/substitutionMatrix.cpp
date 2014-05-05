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
ptrdiff_t substitutionMatrix::load(std::string fileName){
	this->clear();
	//Try to find the substitution matrix file
	std::ifstream inFile(fileName);
	if (!inFile){
		inFile.open("Matrices/" + fileName);
	}
	if (!inFile){
		inFile.open("Matrices/BLOSUM62");
		if (!inFile){
			std::cerr << "Error: Could not find " << fileName << " or the default Matrices/ directory. Terminating." << std::endl;
			return -1;
		}
		else{
			std::cerr << "Error: Could not find " << fileName << std::endl << "Reminder: This is case-sensitive" << std::endl << "Defaulting to BLOSUM62" << std::endl;
		}
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
	return 0;
}

ptrdiff_t substitutionMatrix::save(std::string fileName){
	//open file
	std::ofstream outFile(fileName);
	//put alphabet on first line
	for (size_t i = 0; i <= this->_alphabet.size(); i++){
		if (i == 0){
			outFile << "  ";
		}
		else{
			char temp = this->_alphabet[i - 1];
			outFile << ' ' << (char)this->_alphabet[i-1] << ' ';
		}

	}
	outFile << std::endl;
	for (size_t i = 0; i < this->_alphabet.size(); i++){
		outFile << this->_alphabet[i] << ' ';
		for (size_t j = 0; j < (char)this->_alphabet.size(); j++){
			size_t score = this->_pairMatrix[std::make_pair(this->_alphabet[i], this->_alphabet[j])];
			if (score < 10)outFile << " ";
			outFile << score << " ";
		}
		outFile << std::endl;
	}
	return 0;
}

void substitutionMatrix::identity(std::vector<sequence>seqs, ptrdiff_t match_val, ptrdiff_t mismatch_val){
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
				this->_pairMatrix[std::make_pair(this->_alphabet[i], this->_alphabet[j])] = match_val;
			}
			else{
				this->_pairMatrix[std::make_pair(this->_alphabet[i], this->_alphabet[j])] = mismatch_val;
			}
		}
	}
}

substitutionMatrix::~substitutionMatrix()
{
}
