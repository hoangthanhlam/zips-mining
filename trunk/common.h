/*******************************************************************/
/* To have a very practical  compressing patterns miner on stream  */
/*			Project starts on 9/10/2012							   */
/*			Written by Hoang Thanh Lam							   */
/*			TU/e Eindhoven, Netherlands							   */	
/*******************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include<vector>
#include<map>
#include <list>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <deque>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <cstring>
#include <sstream>

 
using namespace std;


struct myevent{
	int id; //id of the event
	int ts; //timestamp
	int g_cost; // cost of the gap to the previous occurence of the event
};

struct word{
	deque<int> ids; // list if  character ids
};


struct node{
	int prefix_index; // the prefix index
	int last_symbol; // the last symbol
	double comp_benefit; // estimate of compression benefit
	double m_comp_benefit; // estimate of compression benefit, when the word is inserted into the dictionary, this value is initiated to the minimum compression benefit of the words in the dictionary (used for Space-Saving algorithm)
	int freq; // the number of time the word was used to encode data
	map<int,int> children; //map from the last symbols of  children to indice of all the children
	int pair_index; // the pair index of the most recent occurence
	int single_index; // the single index of the most recent occurence
	vector<int> last_match; // the last match position
	int match_seq; // the id of the sequence in which the last match is found
};

struct apair{
	int id;
	double value;
};

struct ipair{
	int id;
	int value;
};


inline int log2( int n ){
	int l=0;
	while(n>>=1)
		l++;
	return l;  
}

inline int elias(int n){ // return the number of bits used to encode an integer in the Elias code 
	if(n<0)
		return 0;
	//return 2*(int)(log2(n))+1;
	//return (int)(log2(n)+1);
	return (int)(log2(n)+2*log2(log2(n)+1))+1;
}



bool apair_comp_second(const apair &a, const apair &b);

bool ipair_comp_second(const ipair &a, const ipair &b); 

bool word_comp_second(const word &a, const word &b); //compare two words by the second field

void printword(word);

#endif
