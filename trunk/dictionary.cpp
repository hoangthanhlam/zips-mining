#include "dictionary.h"
using namespace std;


void Dictionary::insert(node candidate){
	//insert new sequences
	int k=0, min=0;
	if(!exist(candidate)){ // the candidate does not exist
		wordlist.push_back(candidate); // insert the candidate 
			if(candidate.prefix_index!=-1){ // the candidate is not a character, add a link from its parrent
				wordlist[candidate.prefix_index].children[candidate.last_symbol]=wordlist.size()-1;
				nnonsingleton++;
			}
			k++;
	}
	vector<ipair> vp;
	vector<int> vps;
	map<int,int> indices;
	for(int i=0;i<wordlist.size()-k;i++){
			if(wordlist[i].prefix_index!=-1&&wordlist[i].children.size()==0){ // non-singleton word at the leaves of the prefix tree
				ipair p;
				p.id=i;
				p.value=wordlist[i].comp_benefit;
				vp.push_back(p);
			} else if(wordlist[i].children.size()==0){
				vps.push_back(i);
			}
	}
	sort(vp.begin(),vp.end(),ipair_comp_second);
	if(vp.size()==0)
		min=0;
	else 
		min=vp.front().value;
	
	int K=nnonsingleton-k-MAX_DICT_SIZE;
	if(K>0){ // dictionary is full, removing un-important nonsingleton words
		for(int i=wordlist.size()-1;i>=wordlist.size()-k;i--){
			wordlist[i].comp_benefit=min;
			wordlist[i].m_comp_benefit=min;
		}
		for(int i=0;i<K&&i<vp.size();i++){
			indices[vp[i].id]=1;
		}
		nnonsingleton-=indices.size();
		for(int i=0;i<vps.size();i++)
			indices[vps[i]]=1;

		if(!indices.empty()){
			remove(indices);			
		}		
	} 

	//update nonsingleton indices
	nonsingleton_indices.clear();
	for(int i=0;i<wordlist.size();i++){
		if(wordlist[i].prefix_index!=-1)
			nonsingleton_indices.push_back(i);
	}
}

void Dictionary::remove(map<int,int> indices){
	vector<int> new_index;
	int index=0;
    for(int i=0;i<wordlist.size();i++){
		new_index.push_back(index);
		if(indices.find(i)==indices.end())			
			index++;
	}
	
	// remap children list and suffix index
	for(int i=0;i<wordlist.size();i++){
		if(indices.find(i)==indices.end()){
			if(wordlist[i].prefix_index!=-1){
				wordlist[i].prefix_index=new_index[wordlist[i].prefix_index];
			}
			map<int,int>::iterator it=wordlist[i].children.begin();
			while(it!=wordlist[i].children.end()){
				if(indices.find(it->second)!=indices.end())
					wordlist[i].children.erase(it++);
				else{
					wordlist[i].children[it->first]=new_index[it->second];
					it++;
				}
			}
		}
	}
		
	
	//remove words
	vector<node>::iterator vi=wordlist.begin();
	index=0;
	while(vi!=wordlist.end()){
		if(indices.find(index)!=indices.end()){
			if(vi->prefix_index==-1){
				d_characters[vi->last_symbol]=vi->single_index;
			}
			vi=wordlist.erase(vi);			
		}else
			vi++;
		index++;
	
	}
}

word Dictionary::getWord(int index){
	word w;
	int next=index;
	while(next!=-1){
		w.ids.push_front(wordlist[next].last_symbol);
		next=wordlist[next].prefix_index;
	}
	return w;
}

bool Dictionary::isLoop(deque<int> m,int index){
	int i=0;
	for(i=0;i<m.size();i++){
		if(paths[wordlist[index].last_match[i]].find(m[i])!=paths[wordlist[index].last_match[i]].end())
			break;
	}
	if(i<m.size())
		return true;
	else
		return false;
}

bool Dictionary::exist(node n){
	if(this->wordlist[n.prefix_index].children.find(n.last_symbol)==this->wordlist[n.prefix_index].children.end())
		return false;
	else
		return true;
}

int Dictionary::getPatterns(int K){
	int compress=0;
	vector<word> v;
	for(int i=0;i<wordlist.size();i++){
		if(wordlist[i].prefix_index==-1)
			continue;
		word w;
		w.ids.push_back(i);
		w.ids.push_back(wordlist[i].comp_benefit-wordlist[i].m_comp_benefit);
		//w.ids.push_back(wordlist[i].comp_benefit);
		v.push_back(w);
	}
	
	sort(v.begin(),v.end(),word_comp_second);
	int k=0;
	cout<<"id,group,area,color"<<endl;
	while(k<v.size()&&v[k].ids[1]>0){
		if(k<K){
			word w=getWord(v[k].ids[0]);
			cout<<k<<",";
			if(labels.size()>0){
				for(int i=0;i<w.ids.size();i++){
					cout<<labels[w.ids[i]]<<" ";
				}
				cout<<","<<v[k].ids[1]<<","<<v[k].ids[1]<<endl;	
			
			} else{
				for(int i=0;i<w.ids.size();i++){
					cout<<w.ids[i]<<" ";
				}
				cout<<","<<v[k].ids[1]<<","<<v[k].ids[1]<<endl;			
			}
		}
		compress+=v[k].ids[1];
		k++;		
	}
	cout<<"The number of patterns: "<<k<<endl;
	return compress;
}

void Dictionary::print(){
	for(int i=0;i<wordlist.size();i++){
		printWord(i);
		cout<<wordlist[i].comp_benefit<<endl;
	}
	cout<<"*********************************"<<endl;
}

void Dictionary::printWord(int index){
	word w=getWord(index);
	for(int i=0;i<w.ids.size();i++){
		if(labels.size()>0){
			cout<<labels[w.ids[i]]<<" ";
		} else{
			cout<<w.ids[i]<<" ";
		}		
	}
}

int Dictionary::encode(Sequence seq){
	// encode the sequence
	while(true){
		ipair ap=seq.getNextEvent(-1); // get the id and the position of first event
		if(ap.id==-1&&ap.value==-1)// the sequence is exhausted, no more event to be encoded
			return benefit;
		// first time see the next event
		if(ap.id>=characters.size()){
			for(int i=characters.size();i<ap.id;i++){
				characters.push_back(-1);
			}
			node singleton; // a singleton to be added to the dictionary
			singleton.freq=0;
			singleton.comp_benefit=0;
			singleton.prefix_index=-1;
			singleton.last_symbol=ap.id;
			singleton.pair_index=-1; // no pair index for this singleton
			characters.push_back(single_index);		
			d_characters[ap.id]=single_index;
			wordlist.push_back(singleton); // add the singleton to the dictionary					
		} else if(characters[ap.id]==-1) {
			node singleton; // a singleton to be added to the dictionary
			singleton.freq=0;
			singleton.comp_benefit=0;
			singleton.prefix_index=-1;
			singleton.last_symbol=ap.id;
			singleton.pair_index=-1; // no pair index for this singleton
			characters[ap.id]=single_index;
			d_characters[ap.id]=single_index;
			wordlist.push_back(singleton); // add the singleton to the dictionary				
		} else if(d_characters.find(ap.id)==d_characters.end()){
			node singleton; // a singleton to be added to the dictionary
			singleton.freq=0;
			singleton.comp_benefit=0;
			singleton.prefix_index=-1;
			singleton.last_symbol=ap.id;
			singleton.pair_index=-1; // no pair index for this singleton
			d_characters[ap.id]=wordlist.size();
			wordlist.push_back(singleton); // add the singleton to the dictionary	
		}
		word max_word;int max=INT_MIN, max_index;	word max_match; double max_f=INT_MIN+0.0;
		max_word.ids.push_back(ap.id);
		max_match.ids.push_back(ap.value);
		max_index=0;
		time_t start,end;
		time(&start);
	   	int index=0;
		int ni=0;
		while(ni<nonsingleton_indices.size()){
				index=nonsingleton_indices[ni];
				word w=getWord(index); // get the next word from the dictionary
				if(wordlist[index].prefix_index==-1||w.ids.front()!=ap.id){ // skip singleton or dictionary word that does not start with the first character of the sequence 
					ni++;
					continue;
				}
				word match=seq.getFirstBestMatch(w); // get the best match of w in the sequence
				if(match.ids.empty()){ //the match is empty, continue
					ni++;
					continue;
				}else{ // the match is not empty
					int g_cost=0,c=0,uc=0;
					for(int i=0;i<match.ids.size()-1;i++){
						g_cost+=elias(match.ids[i+1]-match.ids[i]);						
					}
					//calculate the cost of encoding the current match with the current word 
					c=g_cost;
					if(pair_index+1-wordlist[index].pair_index<single_index+1-wordlist[index].single_index)
						c+=2+elias(pair_index+1-wordlist[index].pair_index);
					else
						c+=1+elias(single_index+1-wordlist[index].single_index);
					// calculate the cost of encoding the current match with separate characters
					for(int i=0;i<match.ids.size();i++)
						uc+=seq.sequence[match.ids[i]].g_cost;
					if((max_f<(uc/(c+0.0)))&&uc>c){ // update max_word 
							max_f=uc/(c+0.0);
							max=uc-c;
							max_word=w;
							max_match=match;
							max_index=index;
					}	
					/*if((max_f<(uc-c+0.0))&&uc>c){ // update max_word 
							max_f=(uc-c+0.0);
							max=uc-c;
							max_word=w;
							max_match=match;
							max_index=index;
					}*/
				}
				ni++;				
		}
		time(&end);
		
		ipair app=seq.getNextEvent(max_match.ids.back()); //get the next event right after the best match
		if(app.id!=-1&&app.value!=-1){ // if there is an event being right after the match, put it together with the match
			max_match.ids.push_back(app.value);
			max_word.ids.push_back(app.id);			
		}
		seq.removeMatch(max_word,max_match); // remove the match and also the event right after the match if it exists	
		
		if(max>0){ // if the encoding of the max_match with the max_word gives positive compression benefit
			single_index++;
			benefit+=max;
			wordlist[max_index].comp_benefit+=max; // update compression benefit
			wordlist[max_index].freq++;
			pair_index++;
			wordlist[max_index].single_index=single_index;			
		}else {// no word give positive compression benefit, encode the first event of the sequence
			single_index++;
			pair_index++;
			benefit+=seq.sequence[max_match.ids.front()].g_cost-elias(single_index-characters[ap.id]); 
			characters[ap.id]=single_index;
		}
	  	//add an extension to the dictionary or update the dictionary if the extension already exists
		if(app.id!=-1&&app.value!=-1){ // there is an extension
			//update the dictionary with the next event
			if(app.id>=characters.size()){ // for the first time the next event has been seen and its id is larger than the maximum event id seen so far
				for(int i=characters.size();i<app.id;i++){
					characters.push_back(-1);
				}
				node singleton; // an extension to be added to the dictionary
				singleton.freq=0;
				singleton.comp_benefit=0;
				singleton.prefix_index=-1;
				singleton.last_symbol=app.id;
				singleton.pair_index=-1; // no pair index for this singleton
				single_index++;
				characters.push_back(single_index);
				d_characters[app.id]=single_index;
				//add compression benefit of encoding the event right after the max_match
				benefit+=seq.sequence[app.value].g_cost-elias(1);  
				wordlist.push_back(singleton); // add the singleton to the dictionary		
			} else if(characters[app.id]==-1) { // for the first time the next event has been seen and its id is smaller than the maximum event id seen so far
				node singleton; // an extension to be added to the dictionary
				singleton.freq=0;
				singleton.comp_benefit=0;
				singleton.prefix_index=-1;
				singleton.last_symbol=app.id;
				singleton.pair_index=-1; // no pair index for this singleton
				single_index++;
			   	//add compression benefit of encoding the event right after the max_match
				benefit+=seq.sequence[app.value].g_cost-elias(1); 
				characters[app.id]=single_index;
				d_characters[app.id]=single_index;
				wordlist.push_back(singleton); // add the singleton to the dictionary		
			} else{ // the next singleton already exists, just need to update its single_index
				single_index++;
				//add compression benefit of encoding the event right after the max_match
				benefit+=seq.sequence[app.value].g_cost-elias(single_index-characters[app.id]); 
				characters[app.id]=single_index;
				if(d_characters.find(app.id)==d_characters.end()){
					node singleton; // a singleton to be added to the dictionary
					singleton.freq=0;
					singleton.comp_benefit=0;
					singleton.prefix_index=-1;
					singleton.last_symbol=app.id;
					singleton.pair_index=-1; // no pair index for this singleton
					d_characters[app.id]=1;
					wordlist.push_back(singleton); // add the singleton to the dictionary	
				}
			}
					
			//update the dictionary with the extension
			if(max>0){
				if(wordlist[max_index].children.find(app.id)==wordlist[max_index].children.end()){ // the extension does not exist in the dictionary
					node extension; // an extension to be added to the dictionary
					extension.freq=0;
					extension.comp_benefit=0;
					extension.m_comp_benefit=0;
					extension.prefix_index=max_index;
					extension.last_symbol=app.id;
					extension.pair_index=pair_index;
					extension.single_index=-1; // no single index for this word				
					insert(extension); // add the extension to the dictionary
				} else { //the extension already exists in the dictionay, update its pair_index
					wordlist[wordlist[max_index].children[app.id]].pair_index=pair_index;
				}
			} else {
				for(int ii=0;ii<wordlist.size();ii++){
					if(wordlist[ii].prefix_index==-1&&wordlist[ii].last_symbol==ap.id){
						max_index=ii;
						break;
					}
				}
				if(wordlist[max_index].children.find(app.id)==wordlist[max_index].children.end()){ // the extension does not exist in the dictionary
					node extension; // an extension to be added to the dictionary
					extension.freq=0;
					extension.comp_benefit=0;
					extension.m_comp_benefit=0;
					extension.prefix_index=max_index;
					extension.last_symbol=app.id;
					extension.pair_index=pair_index;
					extension.single_index=-1; // no single index for this word				
					insert(extension); // add the extension to the dictionary
				} else { //the extension already exists in the dictionay, update its pair_index
					wordlist[wordlist[max_index].children[app.id]].pair_index=pair_index;
				}				
			}			
		}		
	}
	return benefit;
}

int Dictionary::lzw_encode(Sequence seq){
	paths.clear(); // clear the paths for the new sequence
	// encode the sequence
	while(true){
		ipair ap=seq.getNextEvent(-1); // get the id and the position of first event
		if(ap.id==-1&&ap.value==-1)// the sequence is exhausted, no more event to be encoded
			return benefit;
		// first time see the next event
		if(ap.id>=characters.size()){ //its id is greater than any id has been seen so far
			for(int i=characters.size();i<ap.id;i++){
				characters.push_back(-1);
			}
			node singleton; // a singleton to be added to the dictionary
			singleton.freq=0;
			singleton.comp_benefit=0;
			singleton.prefix_index=-1;
			singleton.last_symbol=ap.id;
			singleton.single_index=single_index;
			characters.push_back(single_index);	
			wordlist.push_back(singleton); // add the singleton to the dictionary					
		} else if(characters[ap.id]==-1) {//its id is less than an id has been seen so far but never seen before
			node singleton; // a singleton to be added to the dictionary
			singleton.freq=0;
			singleton.comp_benefit=0;
			singleton.prefix_index=-1;
			singleton.last_symbol=ap.id;
			singleton.single_index=single_index;
			characters[ap.id]=single_index;
			wordlist.push_back(singleton); // add the singleton to the dictionary				
		} else if(d_characters.find(ap.id)!=d_characters.end()){  // it has been seen but removed from the dictionary
			node singleton; // a singleton to be added to the dictionary
			singleton.freq=0;
			singleton.comp_benefit=0;
			singleton.prefix_index=-1;
			singleton.last_symbol=ap.id;
			singleton.single_index=single_index;
			d_characters[ap.id]=wordlist.size();
			wordlist.push_back(singleton); // add the singleton to the dictionary			
		}
		word max_word;int max=INT_MIN, max_index;	word max_match; double max_f=INT_MIN+0.0;
		max_word.ids.push_back(ap.id);
		max_match.ids.push_back(ap.value);
		max_index=0;
		
	   	int index=0;
		int ni=0;
		while(ni<nonsingleton_indices.size()){
				index=nonsingleton_indices[ni];
				word w=getWord(index); // get the next word from the dictionary
				if(wordlist[index].prefix_index==-1||w.ids.front()!=ap.id){ // skip singleton or dictionary word that does not start with the first character of the sequence 
					ni++;
					continue;
				}
				word match=seq.getFirstBestMatch(w); // get the best match of w in the sequence
				//word match=seq.firstBestMatch(w); // get the best match of w in the sequence
				if(match.ids.empty()){ //the match is empty, continue
					ni++;
					continue;
				}else if(this->nsequence==wordlist[index].match_seq&&isLoop(match.ids,index)){ //there is a loop, skip this word
					ni++;
					continue;
				}else{ // the match is not empty
					int g_cost=0,c=0,uc=0;
					for(int i=0;i<match.ids.size()-1;i++){
						g_cost+=elias(match.ids[i+1]-match.ids[i]);						
					}
					//calculate the cost of encoding the current match with the current word 
					c=g_cost;
					c+=elias(single_index+1-wordlist[index].single_index)+1; 
					// calculate the cost of encoding the current match with separate characters
					for(int i=0;i<match.ids.size();i++)
						uc+=seq.sequence[match.ids[i]].g_cost;
					
					if(mode){
						if((max_f<(uc/(c+0.0)))&&uc>c){ // update max_word 
							max_f=uc/(c+0.0);
							max=uc-c;
							max_word=w;
							max_match=match;
							max_index=index;
						}
					} else {
						if((max_f<(uc/(w.ids.size()*c+0.0)))&&uc>c){ // update max_word 
							max_f=uc/(w.ids.size()*c+0.0);
							max=uc-c;
							max_word=w;
							max_match=match;
							max_index=index;
						}
					}
				}
				ni++;				
		}
		
		ipair app=seq.getNextEvent(max_match.ids.back()); //get the next event right after the best match
		seq.removeMatch(max_word,max_match); // remove the match
		
		
		if(max>0){ // if the encoding of the max_match with the max_word gives positive compression benefit
			single_index++;
			//max+=max_word.ids.size();
			benefit+=max;
			wordlist[max_index].comp_benefit+=max_f; // update compression benefit
			wordlist[max_index].freq++;
			wordlist[max_index].single_index=single_index;
			// update paths
			if (wordlist[max_index].match_seq==this->nsequence){
				for(int i=0;i<max_match.ids.size();i++){
					map<int,int> m;
					m.insert(paths[wordlist[max_index].last_match[i]].begin(),paths[wordlist[max_index].last_match[i]].end());
					m[max_match.ids[i]]=1;
					paths.erase(wordlist[max_index].last_match[i]);
					paths[max_match.ids[i]]=m;
				}
			} else{
				for(int i=0;i<max_match.ids.size();i++){
					map<int,int> m;
					m[max_match.ids[i]]=1;
					paths[max_match.ids[i]]=m;
				}
			}
			wordlist[max_index].match_seq=this->nsequence;
			wordlist[max_index].last_match.assign(max_match.ids.begin(),max_match.ids.end());			
		}else {// no word give positive compression benefit, encode the first event of the sequence
			single_index++;
			benefit+=seq.sequence[max_match.ids.front()].g_cost-elias(single_index-characters[ap.id]); 
			characters[ap.id]=single_index;
		}

		if(app.id!=-1&&app.value!=-1){ // if there is an event being right after the match, put it together with the match
			max_match.ids.push_back(app.value);
			max_word.ids.push_back(app.id);			
		}		

	  	//add an extension to the dictionary or update the dictionary if the extension already exists
		if(app.id!=-1&&app.value!=-1){ // there is an extension
			//update the dictionary with the extension
			if(max>0){
				if(wordlist[max_index].children.find(app.id)==wordlist[max_index].children.end()){ // the extension does not exist in the dictionary
					node extension; // an extension to be added to the dictionary
					extension.freq=0;
					extension.comp_benefit=0;
					extension.m_comp_benefit=0;
					extension.prefix_index=max_index;
					extension.last_symbol=app.id;
					extension.single_index=single_index; 
					extension.match_seq=this->nsequence;	
					extension.last_match.assign(max_match.ids.begin(),max_match.ids.end());
					insert(extension); // add the extension to the dictionary
				} else { //the extension already exists in the dictionay
					//do nothing
				}
			} else {
				for(int ii=0;ii<wordlist.size();ii++){
					if(wordlist[ii].prefix_index==-1&&wordlist[ii].last_symbol==ap.id){
						max_index=ii;
						break;
					}
				}
				if(wordlist[max_index].children.find(app.id)==wordlist[max_index].children.end()){ // the extension does not exist in the dictionary
					node extension; // an extension to be added to the dictionary
					extension.freq=0;
					extension.comp_benefit=0;
					extension.m_comp_benefit=0;
					extension.prefix_index=max_index;
					extension.last_symbol=app.id;
					extension.single_index=single_index; 	
					extension.match_seq=this->nsequence;
					extension.last_match.assign(max_match.ids.begin(),max_match.ids.end());
					insert(extension); // add the extension to the dictionary					
				} else { //the extension already exists in the dictionay
					//do nothing
				}				
			}			
		}
		//print();
	}
	return benefit;
}

void Dictionary::setMaxDictSize(int D){
	MAX_DICT_SIZE=D;
}