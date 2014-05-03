#include "stdafx.h"
#include "sequence.h"

sequence::sequence()
{
	_name = "";
	_sequence = "";
}

sequence::sequence(std::string n, std::string s){
	_name = n;
	_sequence = s;
	//TODO: Try to determine if protein or dna de novo
}

sequence::sequence(std::string n, TCHAR* s){
	_name = n;
	//Turning the TCHAR* argv into a string
	std::wstring ws(s);
	std::string strArgs(ws.begin(), ws.end());
	_sequence = strArgs;
}

sequence::sequence(std::string n, std::string s, bool isProt){
	sequence::sequence(n, s);
	this->_isProtein = isProt;
}

void sequence::insert(int i, std::string aSeq){
	this->_sequence.insert(i, aSeq);
}

void sequence::append(std::string aSeq){
	//remove '\n' and '\r'
	char chars[] = "\n\r";
	//From http://stackoverflow.com/questions/5891610/how-to-remove-characters-from-a-string
	for (unsigned int i = 0; i < strlen(chars); ++i)
	{
		// you need include <algorithm> to use general algorithms like std::remove()
		aSeq.erase(std::remove(aSeq.begin(), aSeq.end(), chars[i]), aSeq.end());
	}
	_sequence.append(aSeq);
}

void sequence::clear(){
	this->_isProtein = NULL;
	this->_name = "";
	this->_sequence = "";
}

bool sequence::operator==(sequence bSeq){
	return (this->_name == bSeq._name && this->_sequence == bSeq._sequence && this->_isProtein == bSeq._isProtein);
}

sequence::~sequence()
{
}
