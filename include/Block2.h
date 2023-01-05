//
//
//

#ifndef PORAM_BLOCK2_H
#define PORAM_BLOCK2_H

#include <algorithm>
#include <vector>
using namespace std;

class Block2 {

public:
    Block2();
    Block2(int leaf_id, int index, int data, vector<int> pm);
    virtual ~Block2();
    int index;
    int leaf_id;
    vector<int> allPM;
    int data;
    void printBlock();
    

};

#endif //PORAM_BLOCK_H
