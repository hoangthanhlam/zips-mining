#ifndef _SEQUENCE_H
#define _SEQUENCE_H
#include "common.h"


using namespace std;
class Sequence{
	public:
		vector<myevent> sequence; // a sequence of events;
		map<int,list<int> > mapseq; // the sequence but it is stored in a form of inverted lists
		void createMap(); // create inverted lists, i.e.  a map from id to a linked list containing positions of the corresponding event  in the sequence
		void removeMatch(word w, word mpos); // remove a match of w in the sequence at position stored in mpos
		word getBestMatch(word w); // get the best match of the given word
		word getFirstBestMatch(word w); // get the best match of the given word (min gap cost), the first character of the match must be the first character of the sequence
		word firstBestMatch(word w); //get the best match of the given word, the first character of the match must be the first character of the sequence
		ipair getNextEvent(int pos); // return id and the position of the  event in the current sequence right after the position pos, if pos=-1, it returns the first event, if pos is out of position range it return (-1,-1) 
		void print();//print sequence

};

#endif