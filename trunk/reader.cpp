#include "reader.h"
using namespace std;

void Reader::readDatabase(string filename){
	ifstream file;
	string dat=filename+".dat";
	file.open(dat.c_str(),std::ios_base::in);
	if(file==NULL){
		cerr<<"File not found or could not oppened !!!"<<endl;
		return;
	}
	string s;
	char *cstr,*pch;
	while(getline(file,s)){
			cstr= new char[s.size()+1];
			strcpy(cstr,s.c_str());
			pch = strtok (cstr," ");
			int i=0;
			vector<ipair> seq;
			while(pch!=NULL){
				ipair e;
				e.id=atoi(pch);
				e.value=i;
				seq.push_back(e);
				i++;
				pch = strtok (NULL," ");
			}
			data.datastream.push_back(seq);
			delete[] cstr;
	}
}

void Reader::readDat(string filename){
	ifstream file;
	string dat=filename+".dat";
	file.open(dat.c_str(),std::ios_base::in);
	if(file==NULL){
		cerr<<"File not found or could not oppened !!!"<<endl;
		return;
	}
	string s;
	getline(file,s);
	char *cstr,*pch;
	cstr= new char[s.size()+1];
	strcpy(cstr,s.c_str());
	pch = strtok (cstr," ");
	int k=0;
	while(pch!=NULL){
		if(atoi(pch)==-1){
			cout<<endl;
			k=0;
		}else if(k==1)
			cout<<" "<<pch;
		else{
			cout<<pch;
			k=1;
		}
		pch = strtok (NULL," ");
	}
	delete[] cstr;
}

void Reader::readStream(string filename){
	ifstream file;
	string dat=filename+".dat";
	file.open(dat.c_str(),std::ios_base::in);
	if(file==NULL){
		cerr<<"File not found or could not oppened !!!"<<endl;
		return;
	}
	string s;
	getline(file,s);
	char *cstr,*pch;
	cstr= new char[s.size()+1];
	strcpy(cstr,s.c_str());
	pch = strtok (cstr," ");
	int n=0;
	while(pch!=NULL){
		if(n%100==0){
			cout<<endl;
		}else if(n%100==1)
			cout<<pch;
		else
			cout<<" "<<pch;
		n++;
		pch = strtok (NULL," ");
	}
	delete[] cstr;
}

void Reader::readLabels(string filename){
	string lab=filename+".lab";
	ifstream file;
	file.open(lab.c_str(),std::ios_base::in);
	if(file==NULL){
		cerr<<"No label file was found !!!"<<endl;
		return;
	}
	string s;
	int i=0;
	while(getline(file,s)){
			data.dict.labels[i]=s;
			i++;
	}	
}


