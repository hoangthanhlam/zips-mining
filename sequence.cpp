
#include "sequence.h"

using namespace std;



word Sequence::getBestMatch(word w){
	vector<list<int> > pos; 
	word match;
	// get the linked lists of positions
	for(int i=0;i<w.ids.size();i++){
		if(mapseq.find(w.ids[i])==mapseq.end())
			return match;
		else
			pos.push_back(mapseq[w.ids[i]]);
	}
	
	vector<vector<word> > matrix;
	matrix.resize(w.ids.size());
	for(int i=0;i<w.ids.size();i++){
		if(i==0){
			for(list<int>::iterator it=pos[0].begin();it!=pos[0].end();it++){
				word ww;
				ww.ids.push_back(0);
				ww.ids.push_back(*it);
				ww.ids.push_back(0);
				matrix[i].push_back(ww);
			}
		} else {
			for(list<int>::iterator it=pos[i].begin();it!=pos[i].end();it++){
				int index=0,min=INT_MAX,mini=0;
				while(index<matrix[i-1].size()&&matrix[i-1][index].ids[1]<*it){
					if(matrix[i-1][index].ids[0]==INT_MAX){
						index++;
						continue;
					}
					int g=matrix[i-1][index].ids[0]+*it-matrix[i-1][index].ids[1];
					if(g<min){
						min=g;
						mini=index;
					}
					index++;
					
				}
				word ww;
				ww.ids.push_back(min);
				ww.ids.push_back(*it);
				ww.ids.push_back(mini);
				matrix[i].push_back(ww);				
			}
		}
		
	}
	
	int min=INT_MAX,mini=0;
	for(int i=0;i<matrix.back().size();i++){
		if(min>matrix.back().at(i).ids[0]){
			min=matrix.back().at(i).ids[0];
			mini=i;
		}
	}
	/*for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++){
			cout<<"["<<matrix[i][j].ids[0]<<" "<<matrix[i][j].ids[1]<<" "<<matrix[i][j].ids[2]<<"] ";
		}
			cout<<endl;
	}*/
	if(min==INT_MAX)
			return match;
	//trace back to get the best match
	for(int i=matrix.size()-1;i>=0;i--){
		match.ids.push_front(matrix[i][mini].ids[1]);
		mini=matrix[i][mini].ids[2];		
	}
	return match;
}

word Sequence::getFirstBestMatch(word w){
	vector<list<int> > pos; 
	word match;
	// get the linked lists of positions
	for(int i=0;i<w.ids.size();i++){
		if(mapseq.find(w.ids[i])==mapseq.end())
			return match;
		else
			pos.push_back(mapseq[w.ids[i]]);
	}
	
	// the first list must contain only the position corresponding to the first character of the sequence
	while(pos.front().size()>1){
		pos.front().pop_back();
	}

	vector<vector<word> > matrix;
	matrix.resize(w.ids.size());
	for(int i=0;i<w.ids.size();i++){
		if(i==0){
			for(list<int>::iterator it=pos[0].begin();it!=pos[0].end();it++){
				word ww;
				ww.ids.push_back(0);
				ww.ids.push_back(*it);
				ww.ids.push_back(0);
				matrix[i].push_back(ww);
			}
		} else {
			for(list<int>::iterator it=pos[i].begin();it!=pos[i].end();it++){
				int index=0,min=INT_MAX,mini=0;
				while(index<matrix[i-1].size()&&matrix[i-1][index].ids[1]<*it){
					if(matrix[i-1][index].ids[0]==INT_MAX){
						index++;
						continue;
					}
					int g=matrix[i-1][index].ids[0]+elias(*it-matrix[i-1][index].ids[1]);
					if(g<min){
						min=g;
						mini=index;
					}
					index++;
					
				}
				word ww;
				ww.ids.push_back(min);
				ww.ids.push_back(*it);
				ww.ids.push_back(mini);
				matrix[i].push_back(ww);				
			}
		}
		
	}
	
	int min=INT_MAX,mini=0;
	for(int i=0;i<matrix.back().size();i++){
		if(min>matrix.back().at(i).ids[0]){
			min=matrix.back().at(i).ids[0];
			mini=i;
		}
	}
	/*for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++){
			cout<<"["<<matrix[i][j].ids[0]<<" "<<matrix[i][j].ids[1]<<" "<<matrix[i][j].ids[2]<<"] ";
		}
			cout<<endl;
	}*/
	if(min==INT_MAX)
			return match;
	//trace back to get the best match
	for(int i=matrix.size()-1;i>=0;i--){
		match.ids.push_front(matrix[i][mini].ids[1]);
		mini=matrix[i][mini].ids[2];		
	}
	return match;
}

word Sequence::firstBestMatch(word w){
	vector<list<int> > pos; 
	word match;
	// get the linked lists of positions
	for(int i=0;i<w.ids.size();i++){
		if(mapseq.find(w.ids[i])==mapseq.end())
			return match;
		else
			pos.push_back(mapseq[w.ids[i]]);
	}
	
	// the first list must contain only the position corresponding to the first character of the sequence
	while(pos.front().size()>1){
		pos.front().pop_back();
	}

	vector<vector<word> > matrix;
	matrix.resize(w.ids.size());
	for(int i=0;i<w.ids.size();i++){
		if(i==0){
			for(list<int>::iterator it=pos[0].begin();it!=pos[0].end();it++){
				word ww;
				ww.ids.push_back(-sequence[*it].g_cost);
				ww.ids.push_back(*it);
				ww.ids.push_back(0);
				matrix[i].push_back(ww);
			}
		} else {
			for(list<int>::iterator it=pos[i].begin();it!=pos[i].end();it++){
				int index=0,min=INT_MAX,mini=0;
				while(index<matrix[i-1].size()&&matrix[i-1][index].ids[1]<*it){
					if(matrix[i-1][index].ids[0]==INT_MAX){
						index++;
						continue;
					}
					int g=matrix[i-1][index].ids[0]+elias(*it-matrix[i-1][index].ids[1])-sequence[*it].g_cost;
					if(g<min){
						min=g;
						mini=index;
					}
					index++;
					
				}
				word ww;
				ww.ids.push_back(min);
				ww.ids.push_back(*it);
				ww.ids.push_back(mini);
				matrix[i].push_back(ww);				
			}
		}
		
	}
	
	int min=INT_MAX,mini=0;
	for(int i=0;i<matrix.back().size();i++){
		if(min>matrix.back().at(i).ids[0]){
			min=matrix.back().at(i).ids[0];
			mini=i;
		}
	}
	/*for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++){
			cout<<"["<<matrix[i][j].ids[0]<<" "<<matrix[i][j].ids[1]<<" "<<matrix[i][j].ids[2]<<"] ";
		}
			cout<<endl;
	}*/
	if(min==INT_MAX)
			return match;
	//trace back to get the best match
	for(int i=matrix.size()-1;i>=0;i--){
		match.ids.push_front(matrix[i][mini].ids[1]);
		mini=matrix[i][mini].ids[2];		
	}
	return match;
}
void Sequence::removeMatch(word w, word mpos){
	for(int j=0;j<mpos.ids.size();j++){
		   	list<int>::iterator li=lower_bound(mapseq[w.ids[j]].begin(),mapseq[w.ids[j]].end(),mpos.ids[j]);
			if(li==mapseq[w.ids[j]].end()||((*li)!=mpos.ids[j])){
				cout<<w.ids[j]<<" "<<mpos.ids[j]<<" "<<j<<" "<<w.ids.size()<<endl;
				cerr<<"Error: match was not found!"<<endl;			
			}else{
				mapseq[w.ids[j]].erase(li);
			}
	}
		
}

void Sequence::print(){
	for(int j=0;j<sequence.size();j++)
			cout<<sequence[j].id<<" ";
	cout<<endl;
}

void Sequence::createMap(){
	this->mapseq.clear();
	for(vector<myevent>::iterator i=sequence.begin();i!=sequence.end();i++){
		this->mapseq[i->id].push_back(i->ts);
	}
}

ipair Sequence::getNextEvent(int pos){
	ipair ip;
	ip.id=-1;
	ip.value=INT_MAX;
	for(map<int,list<int> >::iterator it=mapseq.begin();it!=mapseq.end();it++){
		list<int>::iterator li=upper_bound(it->second.begin(),it->second.end(),pos);
		if(li==it->second.end())
				continue;
		else{
			if(*li<ip.value){
				ip.value=*li;
				ip.id=it->first;
			}
		}
	}
	if (ip.id==-1)
		ip.value=-1;
	return ip;
}


