//
//
//

#ifndef PORAM_BUCKET_H
#define PORAM_BUCKET_H


#include "Block2.h"
#include <vector>
#include <stdexcept>

class Bucket {

public:
    Bucket();
    Bucket(Bucket *other);
    Block2 getBlockByIndex(int index);
    void addBlock(Block2 new_blk);
    bool removeBlock(Block2 rm_blk);
    vector<Block2> getBlocks();
    static void setMaxSize(int maximumSize);
    static void resetState();
    static int getMaxSize();
    void printBlocks();

private:
    static bool is_init; //should be initially false
    static int max_size; //should be initially -1
    vector<Block2> blocks;
};


#endif //PORAM_BUCKET_H
