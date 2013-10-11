#ifndef _READER_H
#define _READER_H
#include "stream.h"


using namespace std;

class Reader{
	public:
		Stream data;
		void readDatabase(string filename); // read data stream
		void readDat(string filename);
		void readLabels(string filename); // read data label file
		void readStream(string filename);		
};


#endif
