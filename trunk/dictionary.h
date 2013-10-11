#ifndef _DICTIONARY_H
#define _DICTIONARY_H
#include "common.h"
#include "sequence.h"
using namespace std;



class Dictionary{
	public:
		bool mode;// mode=true explore, mode=false if precise
		int  MAX_DICT_SIZE; // the maximum size of the dictionary. When the dictionary is full with more than MAX_DICT_SIZE non-singleton, the non-singletons at leaves of the tree with lowest compression benefit are removed  
		int benefit; //compresion benefit so far
		int pair_index; // the pair index of the most recent encoded word
		int single_index;// the single index of the most recent encoded word
		int nsequence; // the number of sequence has been processed so far
		int nnonsingleton; // the number of nonsingleton words
		map<int, map<int,int> > paths; // the set of reference paths

		vector<int> characters; //  all the characters have been seen so far, map from event id to event index in the word list, also store the single_index of the character
		map<int,int> d_characters; // all the characters seen so far but removed from the dictionary
		map<int,string> labels; // map from event id to event labels, label stored in [input_file_name].lab. if the file was not found, the  label map is kept empty
		vector<node> wordlist; // the list of words in the dictionary
		vector<int> nonsingleton_indices; // the indices of all nonsingleton words 


		word getWord(int index); // return the word with the index
		void insert(node extension);  // insert a new candiate to the dictionary
		void remove(map<int,int> indices); // remove words with index stored in the indices map 
		bool exist(node n); // return true if the word exists, false otherwise
		int getPatterns(int K); // print all Top-K patterns with highest compression benefit
		void print(); // print the dictionary content
		int encode(Sequence seq); // encode a sequence, return total compression benefit
		int lzw_encode(Sequence seq); // encode a sequence, return total compression benefit
		void printWord(int index); // print a word with index in the dictionary
		void setMaxDictSize(int); // to set MAX_DICT_SIZE
		bool isLoop(deque<int> match, int index); // check if there is a loop in the set of reference path if the set of matches is added

		Dictionary(){ //constructor
			pair_index=0;
			single_index=0;
			nsequence=0;
			nnonsingleton=0;
			benefit=0;
			mode=true; //default is explore, onde can change it to precise if need more precise results
		}
		

};


#endif
