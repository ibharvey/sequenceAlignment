#pragma once
#include <string>

namespace full{
	class node
	{
	private:
		//score relating Seq1[i] to Seq2[j]
		float _score;
		float _dynScore;
		int _horizontalIndex;
		int _verticalIndex;
		bool _isStart;
		bool _isEnd;
		bool _isGap;
	public:
		//<willNotBeNeeded>
		node* right;
		node* diagonal;
		node* down;
		//</willNotBeNeeded>
		node* parent;
		node();
		node(float substitutionScore, int horizontalIndex, int verticalIndex, bool isStartGap = false, bool isEndGap = false);
		//Only do this for the first node (top, left) to set the start value at 0.
		void setNull(){ _dynScore = 0; }
		float dynScore(){ return _dynScore; }
		void matchCheck(node *p);
		void gapCheck(node *p, float aDynScore);
		bool isGap(){ return this->_isGap; }
		bool isStart(){ return this->_isStart; }
		bool isEnd(){ return this->_isEnd; }
		int horizontalIndex(){ return this->_horizontalIndex; }
		int verticalIndex(){ return this->_verticalIndex; }
		~node();
	};
}


//This is a todo for the future, right now the full::node is implemented
//However, the right, down and diagonal nodes are unnecessary
//I also want to see if there is other stuff I can exclude from the fast::node
namespace fast{
	class node
	{
	private:
		//score relating Seq1[i] to Seq2[j]
		float _score;
		float _dynScore;
		int _horizontalIndex;
		int _verticalIndex;
		bool _isStart;
		bool _isEnd;
		bool _isGap;
	public:
		node* parent;
		node();
		node(float substitutionScore, int horizontalIndex, int verticalIndex, bool isStartGap = false, bool isEndGap = false);
		//Only do this for the first node (top, left) to set the start value at 0.
		void setNull(){ _dynScore = 0; }
		float dynScore(){ return _dynScore; }
		void matchCheck(node *p);
		void gapCheck(node *p, float aDynScore);
		bool isGap(){ return this->_isGap; }
		bool isStart(){ return this->_isStart; }
		bool isEnd(){ return this->_isEnd; }
		int horizontalIndex(){ return this->_horizontalIndex; }
		int verticalIndex(){ return this->_verticalIndex; }
		~node();
	};
}