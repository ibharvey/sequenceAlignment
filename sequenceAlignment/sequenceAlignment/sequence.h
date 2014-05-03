#pragma once

#include <string>
#include <algorithm>

class sequence
{
private:
	std::string _name;
	std::string _sequence;
	bool _isProtein;
public:
	sequence();
	sequence(std::string name, std::string seq);
	sequence(std::string name, TCHAR* seq);
	sequence(std::string name, std::string seq, bool isProt);
	void setName(std::string aName){ _name = aName; }
	void setSequence(std::string aSeq){ _sequence = aSeq; }
	void setProtein(bool b){ _isProtein = b; }
	std::string getName() const { return this->_name; }
	std::string getSequence() const { return this->_sequence; }
	bool isProtein() const { return _isProtein; }
	bool isBigger(sequence two){ return this->_sequence > two._sequence; }
	/*Extensions to easily manipulate the actual sequence of the object*/

	void append(std::string aSeq);
	void insert(int i, std::string aSeq);
	void clear();
	char operator[](unsigned int i){ return this-> _sequence[i]; }
	sequence operator+(char c){ return sequence(this->_name,this->_sequence + c); }
	bool operator==(sequence bSeq);
	unsigned int length() const { return this->_sequence.length(); }
	bool empty(){ return this->_sequence.empty(); }

	/*End Extensions*/
	~sequence();
};

