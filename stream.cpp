#include "stream.h"
using namespace std;

void Stream::zip(int MAX){
	vector<int> characters;
	int benefit;
	time_t start,end;
	time(&start);
	int nevent=0;
	int uncompress=0;
	for(int i=0;i<datastream.size()&&i<MAX;i++){
		Sequence seq;
		for(int j=0;j<datastream[i].size();j++){
			myevent me;
			me.id=datastream[i][j].id;
			me.ts=datastream[i][j].value;
			seq.sequence.push_back(me);
		}
		// go through the data and get the g_cost information
		for(int j=0;j<seq.sequence.size();j++){
			if(seq.sequence[j].id>=characters.size()){ // for the first time the next event has been seen and its id is larger than the maximum event id seen so far
				for(int i=characters.size();i<seq.sequence[j].id+1;i++){
					characters.push_back(-1);
				}
				characters.back()=nevent;
				uncompress+=elias(1); 
				seq.sequence[j].g_cost=elias(1);			
			} else if(characters[seq.sequence[j].id]==-1) { // for the first time the next event has been seen and its id is less than the maximum event id seen so far
				characters[seq.sequence[j].id]=nevent;
				uncompress+=elias(1); 
				seq.sequence[j].g_cost=elias(1);		
			} else{ // the event has been seen so far, just need to update its g_cost
				uncompress+=elias(nevent-characters[seq.sequence[j].id]); 
				seq.sequence[j].g_cost=elias(nevent-characters[seq.sequence[j].id]);
				characters[seq.sequence[j].id]=nevent;					
			}		
			nevent++;
		}
	
		seq.createMap();
		//benefit=dict.encode(seq);
		benefit=dict.lzw_encode(seq);
		dict.nsequence=i;
		//dict.print();
		if(i%100==0){
			time(&end);
			cout<<i<<" "<<dict.wordlist.size()<<" "<<difftime(end,start)<<" "<<benefit<<" "<<uncompress<<" "<<endl;
			//dict.getPatterns(100);
		}

	}
	int compress=dict.getPatterns(100);
	cout<<"We save: "<<benefit<<" (bits), uncompressed size: " <<uncompress <<" (bits), compression ratio: "<<uncompress/(uncompress-benefit+0.0)<<endl;
	cout<<dict.single_index<<" "<<dict.pair_index<<" "<<nevent<<endl;
}

