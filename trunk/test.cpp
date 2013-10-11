#include "reader.h"
using namespace std;

void test_data(int l,int E){
	int N=5;
	int L=100;
	vector<int> next;
	for(int i=0;i<N;i++)
		next.push_back(N*i);
	for(int i=1;i<=l*L;i++){
		int j=rand()%N;
		if(i%L==0)
			//cout<<N*j+next[j]<<" -1 ";
			cout<<N*j+next[j]<<endl;
		else
			cout<<N*j+next[j]<<" ";
		next[j]=(next[j]+1)%N;		
		if(i%N==0){
			cout<<rand()%E+N<<" ";
		}
	}
	/*for(int i=0;i<N+E;i++)
		cout<<i<<endl;*/
}



int main(int argc,char* argv[]){
	try
  	{
		cerr<<endl;cerr<<endl;
		cerr<<"******************************************************************************"<<endl;
		cerr<<"Usage: command dataname [DICT_SIZE] [#_SEQ] "<<endl;
		cerr<<"  * dataname: the datafile is [dataname].dat, the label file is [dataname].lab"<<endl;
		cerr<<"  * DICT_SIZE (optional): the maximum size of the dictionary (default=1000)"<<endl;
		cerr<<"  * #_SEQ (optional): stop  the program after processing #_SEQ sequences"<<endl;
		cerr<<"******************************************************************************"<<endl;
		cerr<<endl;

		if(argc<2){
			cerr<<"The number of agguments must be at least 1, see the usage information above!!!!"<<endl;
			return 1;
		}
		srand ( time(NULL) );
		Reader r;
		//r.readDat(argv[1]);
		//r.readStream(argv[1]);
		time_t start,end;
		time(&start);
		cerr<<"Reading data..."<<endl;
		r.readDatabase(argv[1]);
		cerr<<"Reading labels (optional)..."<<endl;
		r.readLabels(argv[1]);
		//cerr<<"Printing data..."<<endl;
		//r.printData();
		cerr<<"Zips is zipping the data ..., take a coffee and enjoy the results"<<endl;
		if(argc<3){
			r.data.dict.setMaxDictSize(1000);		
		}else
			r.data.dict.setMaxDictSize(atoi(argv[2]));

		if(argc<4)
			r.data.zip(INT_MAX); // run up to INT_MAX sequence
		else
			r.data.zip(atoi(argv[3]));

		time(&end);
		cout<<"Running time: "<<difftime(end,start)<<endl;		
	}
 	 catch (bad_alloc& ba)
  	{
    		cerr << "bad_alloc caught: " << ba.what() << endl;
  	}	
	return 1;
}

