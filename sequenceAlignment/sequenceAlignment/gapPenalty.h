#pragma once

class gapPenalty
{
private:
	float _startHor;
	float _startVer;
	float _contHor;
	float _contVer;
	bool _isSet;
public:
	bool isSet(){ return this->_isSet; }
	//Get all penalties at once
	float* getPenalties();
	//Set all penalties at once
	void setPenalties(float a, float b, float c, float d);

	/*Getting individual penalty values*/
	//Get horizontal start penalty
	float getHStart(){ return _startHor; }
	//Get horizontal continue penalty
	float getHCont(){ return _contHor; }
	//Get vertical start penalty
	float getVStart(){ return _startVer; }
	//Get vertical continue penalty
	float getVCont(){ return _contVer; }

	/*Setting individual penalty values*/
	//Set horizontal start penalty
	void setHStart(float a){ _startHor = a; }
	//Set horizontal continue penalty
	void setHCont(float a){ _contHor = a; }
	//Set vertical start penalty
	void setVStart(float a){ _startVer = a; }
	//Set vertical continue penalty
	void setVCont(float a){ _contVer = a; }

	gapPenalty();
	gapPenalty(float a, float b, float c, float d){
		setPenalties(a, b, c, d);
	}
	
	~gapPenalty();
};

