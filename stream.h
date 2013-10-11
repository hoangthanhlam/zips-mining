#ifndef _STREAM_H
#define _STREAM_H
#include<vector>
#include<deque>
#include<map>
#include "sequence.h"
#include "dictionary.h"
using namespace std;


class Stream{
	public:
		vector<vector<ipair> > datastream; // data stream is either a single long sequence or a database of sequences
		Dictionary dict; // the dictionary
		void zip(int MAX); // zip MAX sequences	
	};
#endif
