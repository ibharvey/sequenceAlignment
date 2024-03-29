#pragma once

#include <string>
#include <map>
#include <sstream>

#include <fstream>
#include <vector>

#include "sequence.h"

//Gap Penalties:	1) startHor		2) startVer		3) contHor		4) contVer
class substitutionMatrix
{
private:
	std::vector<char> _alphabet;
	//Using pair instead of set: P(A->G) is not necessarily = P(G->A)
	std::map<std::pair<char, char>, ptrdiff_t> _pairMatrix;
public:
	substitutionMatrix();
	substitutionMatrix(std::string fileName);
	//use the operator [pair(i,j)] to specify a substitution
	int operator[](std::pair<char, char>);

	//returns whether this matrix's alphabet contains all letters in a given sequence
	bool canScore(std::string aString);
	bool canScore(std::vector<std::string> aStrVec);
	bool canScore(sequence aSeq);
	bool canScore(std::vector<sequence> aSeqVec);
	bool exists(){ return (this->_alphabet.size() > 0 && this->_pairMatrix.size() > 0); }
	void clear();

	size_t length()const{ return this->_alphabet.size(); }
	TCHAR getResidue(size_t i)const{ return this->_alphabet[i]; }

	ptrdiff_t load(std::string fileName);
	ptrdiff_t save(std::string fileName);

	//creates an identity matrix based off the sequences provided
	void identity(std::vector<sequence>seqs, ptrdiff_t match_val= 1, ptrdiff_t mismatch_val= -1);
	~substitutionMatrix();
};
