#include "stdafx.h"
#include "getopter.h"

getopter::getopter(){}

getopter::getopter(int argc, TCHAR* argv[])
{
	//Initialize
	this->_makePairs = 0;
	this->_verboseFlag = 0;
	this->_quietFlag = 0;
	this->_msaId = "";
	//Iterate through command line arguments
	while (1){
		static struct option longOptions[]=
		{
			//TODO: Implement verbose/quiet
			/*
			I'm thinking the best way is to implement functions
			in getopter for cout and cerr that just wrap around
			the verbose and quiet flags
			*/
			{ _T("verbose"),	ARG_NONE, &this->_verboseFlag, 1 },
			{ _T("quiet"),		ARG_NONE, &this->_quietFlag, 1 },
			/*Pairwise alignment:
				Iterate through and align all pairs of sequences
			*/
			{ _T("pair"),		ARG_NONE, &this->_makePairs, 1 },
			{ _T("pairwise"),	ARG_NONE, &this->_makePairs, 1 },
			{ _T("pairwisealignment"), 
								ARG_NONE, &this->_makePairs, 1 },
			/*Multiple sequence alignment:
				Star - all _sequences are aligned to first sequence
			*/
			{ _T("msa"),		ARG_REQ, 0, _T('m') },
			{ _T("multi"),		ARG_REQ, 0, _T('m') },
			{ _T("multiple"),	ARG_REQ, 0, _T('m') },
			{ _T("multiplesequencealignment"),
								ARG_REQ, 0, _T('m') },
			//Substitution matrix
			{ _T("matrix"),		ARG_REQ, 0, _T('x') },

			//Gap penalties
			{ _T("before"),		ARG_REQ, 0, _T('b') },
			{ _T("after"),		ARG_REQ, 0, _T('a') },
			{ _T("regular"),	ARG_REQ, 0, _T('r') },
			
			//IO information
			{ _T("input"),		ARG_REQ, 0, _T('i') },
			{ _T("output"),		ARG_REQ, 0, _T('o') },
			{ _T("format"),		ARG_REQ, 0, _T('f') }, //Default is FASTA
			//Last line of longOptions
			{ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL}
		};

		int optionIndex = 0;
		this->_c = getopt_long(argc, argv, _T("b:a:r:qvpm:i:o:x:f:"), longOptions, &optionIndex);

		//Check for end of operation or error
		if (this->_c == -1) break;

		//Convert the argument to a string
		std::string lowArgs;
		std::string strArgs;
		if (optarg){
			std::wstring ws(optarg);
			strArgs.assign(ws.begin(), ws.end());
			for (int i = 0; strArgs[i]; i++){
				lowArgs.push_back(tolower(strArgs[i]));
			}
		}

		//Handle options
		switch (this->_c){
		case 0:
			//Long option
			//If this option set a flag, do nothing else now.
			if (longOptions[optionIndex].flag != 0) {
				break;
			}
			else{
				//TODO: Determine what would end up here
				break;
			}
		case _T('b'):
			//prePenalties (before)
			this->parseGap(this->_prePenalty, strArgs);
			break;
		case _T('a'):
			//postPenalties (after)
			this->parseGap(this->_postPenalty, strArgs);
			break;
		case _T('r'):
			//regPenalties (regular)
			this->parseGap(this->_regPenalty, strArgs);
			break;
		case _T('v'):
			//Verbose
			this->_verboseFlag = 1;
			break;
		case _T('q'):
			//Quiet
			this->_quietFlag = 1;
			break;
		case _T('p'):
			//User specified to do pairwise alignments
			this->_makePairs = 1;
			break;
		case _T('m'):
			this->_msaId = lowArgs;
			break;
		case _T('i'):
			//Disregard the non-option ARGV-elements
			this->_sequences = this->parseSeqs(strArgs);
			break;
		case _T('o'):
			//TODO: Make an object for outputing alignments

			break;
		case _T('x'):
			//Try to find the given matrix name
			this->_matrix = substitutionMatrix(strArgs);
			break;
		case '?':
			//getopt_long printed an error message
			break;
		default:
			this->_c = -1;
		}
	}
	/*------------------------------------------------------------------------------
	This if-statement checks whether the user specified both quiet and verbose.
	If they do, we are going to have those two cancel eachother out.
	---------------------------------------------------------------------------------*/
	if (this->_verboseFlag && this->_quietFlag){
		std::cerr << "Cannot be both verbose and quiet: Executing as neither" << std::endl;
		this->_quietFlag = !this->_quietFlag;
		this->_verboseFlag = !this->_verboseFlag;
	}
	/*------------------------------------------------------------------------------
	This if-statement allows the user to input sequences as the final arguments,
	after all other options have been chosen by the user. This only works if a
	sequence file has not already been specified and processed.
	---------------------------------------------------------------------------------*/
	if (optind < argc){
		//If there is an input file, the user isn't also inputing _sequences manually
		if (this->_sequences.empty()){
			//try to make _sequences out of the ending arguments
			for (int i = 1; optind < argc; i++){
				std::string baseStr = ">Sequence ";
				baseStr.append(std::to_string(i));
				this->_sequences.push_back(sequence(baseStr, argv[optind++]));
			}
		}
	}
	/*--------------------------------------------------------------------------------------------
	This if-statement attempts to find a substitution matrix if one has not yet been provided. 
	It starts with very specific matrices and gradually increases the complexity of the matrix:
		1) DNA identity matrix
		2) RNA identity matrix
		3) BLOSUM62 matrix
		4) Dynamically generating identity matrix for the given set of sequences
	----------------------------------------------------------------------------------------------*/
	//if there is a substitution matrix:
	if (this->_matrix.exists()){
		//Check if this substitution matrix works for the sequences
		if (!this->_matrix.canScore(this->_sequences)){
			std::cerr << "This substitution matrix cannot represent all characters in the given sequences" << std::endl;
		}
	}
	//If there is no substitution matrix try to default to DNA
	else{
		this->_matrix.load("Matrices/DNA");
		if (this->_matrix.canScore(this->_sequences)){
			std::cout << "No substitution matrix specified, using a DNA identity matrix." << std::endl;
		}
		else{
			//If DNA doesn't work, try RNA
			this->_matrix.load("Matrices/RNA");
			if (this->_matrix.canScore(this->_sequences)){
				std::cout << "No substitution matrix specified, using an RNA identity matrix." << std::endl;
			}
			else{
				//If not DNA or RNA, try standard protein
				this->_matrix.load("Matrices/BLOSUM62");
				if (this->_matrix.canScore(this->_sequences)){
					std::cout << "No substitution matrix specified, using BLOSUM62. " << std::endl;
				}
				else{
					std::cout << "Input contained non-standard characters, generating an identity matrix" << std::endl;
					//This sequence is not standard DNA, RNA or protein
					//Load an identity matrix for the sequences provided
					this->_matrix.identity(this->_sequences);
				}
			}
		}
	}
	
}

void getopter::parseGap(gapPenalty& penalty, std::string strArgs){
	//Should be formatted as startHor,startVer,contHor,contVer 
	/*
	Turn strArgs into a set of integers:
		Iterate through the string:
			Replace any non-number && non '-' with ' '
	*/ 
	std::stringstream ss;
	std::string temp;
	float sH, sV, cH, cV;
	for (unsigned int i = 0; i < strArgs.length(); i++){
		if ((strArgs[i] >= '0' && strArgs[i] <= '9') || strArgs[i]=='.'){
			temp.push_back(strArgs[i]);
		}
		else{
			//If '-', check whether the previous char was a number of '.'
			if (strArgs[i] == '-'){
				if (i>0 && ((strArgs[i - 1] >= '0' && strArgs[i - 1] <= '9') || strArgs[i-1]=='.')){
					//If negative penalties are clumped without a spacer, this will separate them
					temp.push_back(' ');
				}
				//Either way, add the '-' char to the string
				temp.push_back(strArgs[i]);
			}
			else{
				//If the character was not a '.', '-' or number: replace is with a space for the stringstream
				temp.push_back(' ');
			}
		}
	}
	ss << temp;
	ss >> sH >> sV >> cH >> cV;
	penalty.setPenalties(sH, sV, cH, cV);
}

//TODO: Turn this function into a class(es) to import arbitrary sequence formats
//Assumes input file is FASTA format currently
std::vector<sequence> getopter::parseSeqs(std::string aFile){
	std::ifstream inFile(aFile);
	std::vector<sequence>seqs;
	sequence sTemp;
	while (!inFile.eof()){
		std::string temp;
		std::getline(inFile, temp);
		if (!seqs.empty()){
			//if not the first sequence
			if (temp[0] == '>'){
				//if starting a new sequence...
				//1)Put the current sTemp into the vector
				seqs.push_back(sTemp);
				//2)Clear the sTemp variable
				sTemp.clear();
				//3)Set the new name value
				sTemp.setName(temp);
			}
			else{
				//if any sequence > first...
				sTemp.append(temp);
			}
		}
		else{
			if (temp[0] == '>'){
				if (sTemp.getName() == ""){
					//if this is the first line...
					sTemp.setName(temp);
				}
				else{
					//if this is the second sequence...
					seqs.push_back(sTemp);
					sTemp.clear();
					sTemp.setName(temp);
				}
				
			}
			else{
				//if this is the first sequence...
				sTemp.append(temp);
			}
		}
	}
	seqs.push_back(sTemp);
	return seqs;
}

getopter::~getopter()
{
}
