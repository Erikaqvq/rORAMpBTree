
#include "StashSizeSimulator.h"
#include "Bucket.h"
#include "ServerStorage.h"
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "RandomForOram.h"
#include "UntrustedStorageInterface.h"
#include "OramReadPathEviction.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>


using namespace std;

StashSizeSimulator::StashSizeSimulator(int bucketSize, int numBlocks, int numAccesses, int simNumber) {
	this->numAccesses = numAccesses; 
	this->numBlocks = numBlocks;
	this->bucketSize = bucketSize;
	//int tmp = log10(numBlocks)/log10(2);
	//tmp = tmp * tmp;
	//this->num_Orams = log10(ceil(numBlocks / tmp)) / log10(2) + 1;
    this->num_Orams = ceil(log10(numBlocks / ((log10(numBlocks)/log10(2)) * (log10(numBlocks)/log10(2))))/log10(2));

	/*
	this->stashSizes = new int[numBlocks+2]; 
	this->stashSizes[0] = numAccesses; //?
	this->simNumber = simNumber; //?
	
	for (int i=1; i < numBlocks+2; i++) {
		this->stashSizes[i] = 0;
	}
	*/

}	

/*
int* StashSizeSimulator::sampleData(int i) {
	int* newArray = new int[Block::BLOCK_SIZE];
	for (int j = 0; j < Block::BLOCK_SIZE; ++j) {
		newArray[j] = i;
	}
	return newArray;
}
*/

/*
void StashSizeSimulator::updateStashSizes(int recorded, int times) {
	for (int i = 1; i<recorded+1;i++){
		this->stashSizes[i] += times;
	}
}
*/

void StashSizeSimulator::writeSimulation() {
	ofstream sim;
	sim.open("simulations/simulation"+to_string(simNumber)+".txt");
	for (int i = 0; i<numBlocks+2; i++) {
		if (this->stashSizes[i] == 0) {
			break;
		}
		sim << to_string(i-1) << "," << to_string(this->stashSizes[i]) << endl;
	}
	sim.close();
}

void StashSizeSimulator::runSimulation() {


	cout<<"This is runSimulation"<<endl;
	int bSize = this->bucketSize;
	int nBlocks = this->numBlocks;
	int nAccesses = this->numAccesses;
	int nOrams = this->num_Orams;
	int num_levels = ceil(log10(nBlocks) / log10(2)) + 1;
	int num_leaves = pow(2, num_levels-1);
	
	Bucket::resetState();
	Bucket::setMaxSize(bSize);	
	//Block2::ini_blocks(nBlocks);
	//cout<<"num_orams="<<num_Orams<<endl;

	//cout<<"Finish ini datas"<<endl;

	//OramInterface::iniPM iniOrams[nOrams + 1]; // Used to initialize the position maps and store them into (l+1) oram trees.
	//OramInterface::iniPM *p = iniOrams;
	

	/*
	int* recSizes = new int[nBlocks+1];

	for (int i=0; i < nBlocks+1; i++) {
		recSizes[i] = 0;
	}
	*/


	// Create (l+1) Path Orams -- PO_0 --> PO_l l = nOrams
	UntrustedStorageInterface* storages[nOrams+1];
	RandForOramInterface* random = new RandomForOram();
	OramInterface* orams[nOrams + 1];
	int ness[nOrams + 1]; // How many blocks' pos should be stored in Oram_i
	btree_t* btrees[nOrams + 1];

	cout<<"Begin ini PORAMs"<<endl;

	vector<int>PMs[nOrams + 1];

	for(int i = 0; i < nOrams + 1; i++){
		storages[i] = new ServerStorage(i);
		//storages[i] = storage;
		//cout<<i<<":"<<storage<<endl;
			
		// When initializing oram, PositionMaps will be recorded.
		ness[i] = ceil(nBlocks / pow(2 , i));
		//int PositionMaps[ness[i]];
		int* PositionMap = (int *)malloc(sizeof(int) * ness[i]);
		OramInterface* oram;
		orams[i] = new OramReadPathEviction(storages[i], random, bSize, nBlocks, i, PositionMap, ness[i]);
		//orams[i] = oram;
		//cout<<i<<" "<<oram<<endl;
		//iniOrams[i].head = PositionMaps;
		//cout<<i<<" "<<*(iniOrams[i].head +5)<<endl;
		for(int j = 0;j<=ness[i];j++) PMs[i].push_back(PositionMap[j]);

		//build_tree
		btrees[i] = btree_ini(i, PositionMap, ness[i]);
	}
	//for(int i = 0; i<= 7 ;i++) cout<<orams[i]<<endl;
	
	cout << "Warming up the stash by writing blocks" << endl;
	int iniDatas[nBlocks];

	// then give datas
	vector<int> pmpm[nBlocks];
	for (int i = 0; i < nBlocks; i++) iniDatas[i] = i;
	//int tmp;
	//cout<<nBlocks<<endl;
	//cout<<"nlevels"<<num_levels<<endl;
	for(int i = 0; i< nBlocks; i++){
		cout<<"get positionmap for:"<<i<<endl;
		//int tmpPM[nOrams + 1];
		for(int j = 0; j < nOrams + 1; j++){
			int len = pow(2, j);
			if((i % len) == 0) {
				pmpm[i].push_back(PMs[j][i/len]);
				//tmpPM[j] = PMs[j][i/len];
				//cout<<i<<" "<<tmpPM[j]<<endl;
				//cout<<tmpPM[j]<<endl;
				//tmp = pmpm[i][j];
			}
			else{
				//cout<<"rvs test"<<i<<" "<<j<<endl;
				//cout<<tmp<<endl;
				int tmpj=i/len;
				int tmp=pmpm[tmpj][j];
				int rvs = reverse_bits(tmp, num_levels, num_leaves);
				//cout<<rvs<<endl;
				rvs = rvs + (i % len);
				//cout<<rvs<<endl;
				rvs = reverse_bits(rvs, num_levels, num_leaves);
				//cout<<rvs<<endl;
				pmpm[i].push_back(rvs);
				//tmpPM[j] = rvs;
			}
			//cout<<"block="<<i<<" oram="<<j<<" pm="<<tmpPM[j]<<endl;
		}//cout<<"hi";
		for(int j = 0; j < nOrams + 1; j++){
			//cout<<"work in oram"<<j<<endl;
			//cout<<orams[j]<<endl;
			//cout<<"hi";
			orams[j]->iniaccess(i, iniDatas[i], j, btrees[j], pmpm[i], pmpm[i][j]);
			int* re;
			//cout<<"hello";
			//orams[j]->access(0,1,NULL,j,btrees[j],re);
			//cout<<"hi";
		}
	}
	for(int i = 0; i<= 7 ;i++) cout<<orams[i]<<endl;
	for(int i = 0; i < nAccesses; i++){
		bool op;
		int StID, len;
		int *D=new int[101];
		cout<<"Please input op, startid, access length."<<endl;
		cin>>op>>StID>>len;
		int l = ceil(log10(len) / log10(2));
	    int L = pow(2, l);
	    int a1 = (StID / L) * L;
	    int a2 = ((StID+len-1)/L) * L;
		cout<<L<<" "<<a1<<" "<<a2<<endl;
		//vector<int> rets1;
		//vector<int> rets2;
	    if(op == 0){ // read
		//cout<<1<<endl;
		//cout<<orams[l]<<endl;
		//cout<<"ret1="<<ret1<<endl;
		vector<int> ret1;
		ret1 = orams[l]->access(0, a1, D, l, btrees[l]);
		cout<<"rets1finish"<<endl;
		//int* testl=new int();
		//btrees[l] = btree_search(btrees[l],1, testl);
		//cout<<"test"<<*testl<<endl;
		vector<int> ret2 = orams[l]->access(0, a2, D, l, btrees[l]);
	    }
	    else{ // write
		    for(int j = 0; j <len; j++) cin>>D[j];
			vector<int> ret1=orams[l]->access(0, a1, D, l, btrees[l]);
			vector<int> ret2=orams[l]->access(0, a2, D, l, btrees[l]);
			//output
	    }
	}

/*
	for(int i = 0; i < nAccesses; i++){
		int* accessed = oram->access(0, i % nBlocks, NULL, , btii);
		int rec = oram->getStashSize();
		recSizes[rec]++;
		if (i%1000000 == 0) {
			cout << "Accessed " << to_string(i) << " Recorded size = " << to_string(rec) << endl;
		}
		
	}*/
	

	/*
	cout << "Writing results" << endl;
	for (int i=0; i<nBlocks+1;i++) {
		updateStashSizes(i, recSizes[i]);
	}
	this->writeSimulation();
	*/
    //cout<<"Finish runSimulation"<<endl;
}

int StashSizeSimulator::reverse_bits(int g, int num_levels, int num_leaves) {

    /*
    INPUT: Integers g and bits_length.
    OUTPUT: Integer reverse of length bits_length consisting of reversed bits of g.
    To be used to traverse the leaves and run eviction in reverse lexicographical order.
    */
    int bits_length = num_levels - 1;
    int g_mod = g % num_leaves;
    int reverse = 0;
    while(g_mod) {
        reverse <<= 1;
        reverse |= g_mod & 1;
        g_mod >>= 1;
        bits_length--;
    }
    
    reverse <<= bits_length;
    return reverse;
}
