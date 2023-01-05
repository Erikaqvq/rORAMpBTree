//
//
//

#ifndef PORAM_ORAMREADPATHEVICTION_H
#define PORAM_ORAMREADPATHEVICTION_H
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "UntrustedStorageInterface.h"
#include "StashSizeSimulator.h"
#include <cmath>

class OramReadPathEviction : public OramInterface {
public:
    UntrustedStorageInterface* storage;
    RandForOramInterface* rand_gen;

    int bucket_size;
    int num_levels;
    int num_leaves;
    int num_blocks;
    int num_buckets;
    int num_Orams;
    //struct iniPM{
	//		int *head;
	//};

    //int *position_map; //array    EDIT BY ERU
    vector<Block2> stash;
    //vector<int>stashbook;

    OramReadPathEviction(UntrustedStorageInterface* storage,
            RandForOramInterface* rand_gen, int bucket_size, int num_blocks, int num_oram, int*pm, int pmsize);
    void iniaccess(int blockIndex, int newdata, int oramid, btree_t* btii, vector<int> PoM, int pp);
    vector<int> access(bool op, int blockIndex, int newdata[], int oramid, btree_t* btii);
    int P(int leaf, int level);
    //int* getPositionMap();
    vector<Block2> getStash();
    int getStashSize();
    int getNumLeaves();
    int getNumLevels();
    int getNumBlocks();
    int getNumBuckets();
    int ReverseBits(int g);
    void updateOrams(int OramID, int leafID, Block2 newBlock);
};


#endif 
