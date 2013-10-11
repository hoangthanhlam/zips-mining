#include "common.h"
using namespace std;


bool word_comp_second(const word &a, const word &b){
	return a.ids[1]>b.ids[1];
}

bool apair_comp_second(const apair &a, const apair &b){
	return a.value<b.value;
}

bool ipair_comp_second(const ipair &a, const ipair &b){
	return a.value<b.value;
}

void printword(word w){
	cout<<"[";
	for(int i=0;i<w.ids.size();i++){
		cout<<w.ids[i]<<" ";
	}
	cout<<"]"<<endl;
}


