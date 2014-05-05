#include "stdafx.h"
#include "msa.h"

msa::msa()
{
}

msa::msa(std::vector<sequence> s, substitutionMatrix matr) : alignment(matr)
{
	this->_seqs = s;
}

msa::msa(std::vector<sequence> s, substitutionMatrix matr, gapPenalty preP, gapPenalty regP, gapPenalty postP) : alignment(matr, preP, regP, postP)
{
	this->_seqs = s;
}

//This is called 'star' because it has one main/central sequence that all others 'branch' off from
void msa::star() {
	//The map object's first object is the 0-index sequence-types
	//I've decided to implement the map object using std::string-keys to reduce redundancy
	std::map<std::string, std::vector<sequence>> allPairs;
	for (int i = 1; i < _seqs.size(); i++){
		std::cout << "Aligning sequence " << i+1 << " out of " << _seqs.size() << "." << std::endl;
		pairwiseAlignment temp = pairwiseAlignment(_seqs[0], _seqs[i], _matr);
		temp.slowAlign();
		allPairs[temp.getTemplate().getSequence()].push_back(sequence(_seqs[i].getName(),temp.getQuery().getSequence()));
	}
	//The tricky part is how to maintain proper alignment if/when the template changes
	//loop through the main sequence types (amount & length of gaps)
	/*Pseudo code:
	for i in range(0,len(template1)):
		if res1 = '-':
			create a '-' in template2 and all aligned seqs2
			
	*/

	std::string sCombined;
	std::vector < sequence > vCombined;
	for (std::map<std::string, std::vector<sequence>>::iterator it = allPairs.begin();
		it != allPairs.end(); it++){
		//If this is the first one, just set it as the combined
		if (it == allPairs.begin()){
			sCombined = it->first;
			vCombined = it->second;
		}
		/*If this template is not the first template gap type, 
		combine with s/vCombined
		*/
		else{
			std::string sLong, sShort;
			std::vector<sequence> vLong, vShort;
			if (sCombined.length() > it->first.length()){
				sLong = sCombined;
				vLong = vCombined;
				sShort = it->first;
				vShort = it->second;
			}
			else{
				sLong = it->first;
				vLong = it->second;
				sShort = sCombined;
				vShort = vCombined;
			}
			//these keep track of the last character of the template that was seen
			int nCharL = 0;
			int nCharS = 0;
			//for every character in the long sequence
			for (int i = 0; i < sLong.length(); i++){
				if (sLong[i] != '-' && sShort[i] != '-'){
					if (nCharL == nCharS){
						//do nothing to the sequences that match them
						nCharL++;
						nCharS++;
					}
					else{
						std::cerr <<
							"The template indeces don't match up." << std::endl;
					}
				}
				else if (sLong[i] == '-' && sShort[i] != '-'){
					 //insert a '-' at this position for all vShort strings
					for (int j = 0; j < vShort.size(); j++){
						vShort[j].insert(i, "-");
					}
					//sShort = sShort.insert(i, "-");
					nCharL++;
				}
				else if (sLong[i] != '-' && sShort[i] == '-'){
					//insert a '-' at this position for all vLong strings
					for (int j = 0; j < vLong.size(); j++){
						vLong[j].insert(i, "-");
					}
					//sLong = sLong.insert(i, "-");
					nCharS++;
				}
				else if (sLong[i] == '-' && sShort[i] == '-'){
					//do nothing to the sequences where neither matches
					nCharL++;
					nCharS++;
				}
			}
			if (sShort == sLong){
				sCombined = sShort;
				vShort.insert(vShort.begin(), vLong.begin(), vLong.end());
				vCombined = vShort;
			}
			else{
				std::cerr << "The final msaStar template seqs are not matching:" << 
					std::endl << "algorithm must be wrong" << std::endl;
			}
		}
	}
	vCombined.insert(vCombined.begin(), sequence(this->_seqs[0].getName(),sCombined));
	this->_algnSeqs = vCombined;
}

msa::~msa()
{
}
