//
//
//

#ifndef PORAM_ORAMINTERFACE_H
#define PORAM_ORAMINTERFACE_H
#include "Block2.h"
#include "btree.h"
#include <vector>

class OramInterface {
public:
    //enum Operation {READ,WRITE};
    //read=0 write=1
    //struct iniPM{
	//		vector<int> pms;
	//};
    virtual vector<int> access(bool op, int blockIndex,
                                    int newdata[], int oramid, btree_t* btii) { 
                                    int dummy = 1000;
                                    int* dummy_ptr = &dummy;
                                    return vector<int>(); };
    virtual void iniaccess(int blockIndex, int newdata, int oramid, btree_t* btii, vector<int> PoM, int pp) { };
    virtual int P(int leaf, int level) { return 0; };
    virtual int* getPositionMap() { int dummy = 1000;
                                    int* dummy_ptr = &dummy;
                                    return 0; };
    virtual vector<Block2> getStash() { return vector<Block2>(); };
    virtual int getStashSize() { return 0; };
    virtual int getNumLeaves() { return 0; };
    virtual int getNumLevels() { return 0; };
    virtual int getNumBlocks() { return 0; };
    virtual int getNumBuckets() { return 0; };
};


#endif //PORAM_ORAMINTERFACE_H
