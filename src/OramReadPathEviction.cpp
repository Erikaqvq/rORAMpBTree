//
//
//

#include "OramReadPathEviction.h"
#include "btree.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string.h>
#include <vector>

using std::vector;
using std::count;

bool cmp(Block2 &a, Block2 &b){
    return a.index < b.index;
}
int getPpos(int pleaf, int lev){
    return (pleaf % (1<<lev) + (1<<lev) - 1);
}

int getrealp(int pleaf, int lev){
    return pleaf + (1<<(lev-1));
}
OramReadPathEviction::OramReadPathEviction(UntrustedStorageInterface* storage, RandForOramInterface* rand_gen,
                                           int bucket_size, int num_blocks, int OramNum, int* PM, int PMsize) {
    this->storage = storage;
    this->rand_gen = rand_gen;
    this->bucket_size = bucket_size;
    this->num_blocks = num_blocks;
    this->num_levels = ceil(log10(num_blocks) / log10(2)) + 1; 
    this->num_buckets = pow(2, num_levels) - 1;
    this->num_Orams = OramNum;
    if (this->num_buckets*this->bucket_size < this->num_blocks) //deal with precision loss
    {
        cout<<"precision error"<<endl;
        throw new runtime_error("Not enough space for the acutal number of blocks.");
    }
    this->num_leaves = pow(2, num_levels-1);
    //if(num_Orams == 0){
    //    Bucket::resetState();
    //    Bucket::setMaxSize(bucket_size);
    //}
    this->rand_gen->setBound(num_leaves);
    this->storage->setCapacity(num_buckets, OramNum);
    this->stash = vector<Block2>();

    int eff = pow(2, OramNum);
    int cnt = 0;
    for (int i = 0; i < this->num_blocks; i++){
        if(i % eff == 0){
            PM[cnt++] = rand_gen->getRandomLeaf();
        }
    }
    // 以下加了空的blocks
    for(int i = 0; i < num_buckets; i++){
        Bucket init_bkt = Bucket();
         for(int j = 0; j < bucket_size; j++){
            init_bkt.addBlock(Block2());
        }
        storage->WriteBucket(num_Orams, i, init_bkt); 
    }
}

void OramReadPathEviction::iniaccess(int blockIndex, int newdata, int OramID, btree_t *btree, vector<int> PoM, int pp) {

    int realp = pp + pow(2, num_levels - 1);
    for (int i = 0; i < num_levels; i++) {
        int nowl = pow(2,i); // total nodes in level
        int tmpp = getPpos(realp, i);
        vector<Block2> blocks = storage->ReadBucket(OramID, tmpp).getBlocks();
        for(int o = 0; o < blocks.size(); o++){
            Block2 b = blocks[o];
            if (b.index == -1) continue;
            this->stash.push_back(Block2(b));
        }
    }
    vector<Block2> newstash;
    for (int i = 0; i < this->stash.size(); i++) {
        Block2 b = this->stash[i];
        if(b.index == -1) continue;
        newstash.push_back(b);
    }
    Block2 thisblock;
    thisblock.data = newdata;
    thisblock.allPM = PoM;
    thisblock.leaf_id = pp;
    thisblock.index = blockIndex;
    newstash.push_back(thisblock);
    this->stash.swap(newstash); 
    // Eviction steps: write to the same path that was read from.
    for (int l = num_levels - 1; l >= 0; l--) {
        if(stash.size() == 0) break;
        vector<int> bid_evicted = vector<int>();
        Bucket bucket = Bucket();
        int Pxl = getPpos(realp, l);
        int counter = 0; // clean all the blocks in the bucket
        for(int ct = 0; ct < this->stash.size(); ct++){
            Block2 b_instash = this->stash[ct];
            if (counter >= bucket_size) break;
            Block2 be_evicted = b_instash;
            cout<<be_evicted.index<<endl;
            if (Pxl == getPpos(getrealp(be_evicted.leaf_id, num_levels - 1), l)){
                bucket.addBlock(be_evicted);
                bid_evicted.push_back(be_evicted.index);
                counter++;
            }
        }
        //remove from the stash
        for(int i = 0; i < bid_evicted.size(); i++){   
            for(int j=0; j<this->stash.size(); j++){
                Block2 b_instash = this->stash.at(j);
                if(b_instash.index == bid_evicted.at(i)){   
                    this->stash.erase(this->stash.begin() + j);
                    break;
                }
            }
        }
        while(counter < bucket_size){
            bucket.addBlock(Block2()); //dummy block
            counter++;
        }
        storage->WriteBucket(num_Orams, Pxl, bucket);
    }
}

vector<int> OramReadPathEviction::access(bool op, int blockIndex, int newdata[], int OramID, btree_t *btree) {
    int operLength = pow(2, OramID); // Number of consecutive blocks
    int* oldLeaf=new int();
    btree = btree_search(btree, blockIndex, oldLeaf);
    btree = btree_delete(btree, blockIndex);
    int newLeaf = rand_gen->getRandomLeaf();
    btree = btree_insert(btree, blockIndex, newLeaf);
    int* testleaf=new int();
    btree = btree_search(btree,blockIndex,testleaf);
    int realp = getrealp(*oldLeaf, num_levels - 1);
    for (int i = 0; i < num_levels; i++) {
        int nowl = pow(2,i);
        for(int j = realp; j < realp + min(operLength, nowl); j++){
            int tmppos = getPpos(j,i);
            vector<Block2> blocks = storage->ReadBucket(OramID, tmppos).getBlocks();
            int count = blocks.size();
            for(int o = 0; o < count; o++){
                Block2 b = blocks[o];
                if (b.index == -1) continue;
                stash.push_back(Block2(b));
            }
        }
    }
    vector<Block2> bresult;
    vector<Block2> newstash;
    vector<Block2> bupdate;
    vector<int> stashbook;
    stashbook.clear();
    for (int i = 0; i < stash.size(); i++){
        Block2 b = stash[i];
        if(std::find(stashbook.begin(), stashbook.end(), b.index) != stashbook.end()){
            if(b.index == -1) continue;
            stashbook.push_back(b.index);
            if(b.index < blockIndex || b.index >= blockIndex + operLength){
            newstash.push_back(b);
            continue;
            }
        bresult.push_back(b);
        }
    }
    sort(bresult.begin(), bresult.end(), cmp);
    vector<int> retdata =vector<int>();
    for(int i = 0; i < operLength; i++){
        Block2 tmpBlock = bresult[i];
        vector<int> newpom = tmpBlock.allPM;
        newpom[OramID] = (newLeaf + i) % (1<<(num_levels - 1));
        Block2 newBlock;
        if(op == 1) { // write
            newBlock = Block2(newLeaf, blockIndex+i, newdata[i], newpom);
        }
        else { // read
            retdata.push_back(tmpBlock.data);
            newBlock = Block2(newLeaf, blockIndex+i, tmpBlock.data, newpom);
        }
        newstash.push_back(newBlock); 
        bupdate.push_back(newBlock); 
    }
    // Eviction steps: write to the same path that was read from.
    for (int l = num_levels - 1; l >= 0; l--) {
        int nowl = pow(2, l);
        for(int j = realp; j < realp + min(operLength, nowl); j++){
            vector<int> bid_evicted = vector<int>();
            Bucket bucket = Bucket();
            int Pxl = getPpos(realp, l);
            int counter = 0;
            for(int kk = 0; kk < stash.size();kk++){ 
                Block2 b_instash = stash[kk];          
                if (counter >= bucket_size) break;
                Block2 be_evicted = b_instash;
                if (Pxl == getPpos(getrealp(be_evicted.leaf_id, num_levels - 1), l)){
                    bucket.addBlock(be_evicted);
                    bid_evicted.push_back(be_evicted.index);
                    counter++;
                    }
            }
            //remove from the stash
            for(int i = 0; i < bid_evicted.size(); i++) {            
                for(int jj = 0; jj < stash.size(); jj++) {
                    Block2 b_instash = stash.at(jj);
                    if(b_instash.index == bid_evicted.at(i)){   
                        this->stash.erase(this->stash.begin() + jj);
                        stashbook[b_instash.index] = 0;
                        break;
                    }
                }
            }
            while(counter < bucket_size) {
                bucket.addBlock(Block2()); //dummy block
                counter++;
            }
            storage->WriteBucket(OramID, Pxl, bucket);
        }
    }
    for(int i = 0; i<bupdate.size(); i++){
        for(int r = 0; r < num_Orams + 1; r++){
            vector<int> newpom = bupdate[i].allPM;
            int tar = newpom[r];
            OramReadPathEviction::updateOrams(r, tar, bupdate[i]);   
        }
    }
    return retdata;
}

void OramReadPathEviction::updateOrams(int OramID, int leafID, Block2 newBlock){
    for(int l = num_levels - 1; l >= 0; l--){
        vector<Block2> blocks = storage->ReadBucket(OramID, OramReadPathEviction::P(leafID, l)).getBlocks();
        for(int k = 0; k < blocks.size(); k++){
            if(blocks[k].leaf_id == -1){
                cout<<"lev="<<l<<" k="<<k<<endl;
                Bucket newBucket = Bucket();//cout<<"orz"<<endl;
                for(int j = 0; j < blocks.size(); j ++){
                    if(j == k) newBucket.addBlock(newBlock);
                    else newBucket.addBlock(blocks[j]);
                }//
                cout<<"orz1"<<endl;
                storage->WriteBucket(OramID, P(leafID, l) , newBucket);
                //
                cout<<"orz2"<<endl;
                return;
            }
        }
    }
}

int OramReadPathEviction::P(int leaf, int level) {
    /*
    * This function should be deterministic. 
    * INPUT: leaf in range 0 to num_leaves - 1, level in range 0 to num_levels - 1. 
    * OUTPUT: Returns the location in the storage of the bucket which is at the input level and leaf.
    */
    return (1<<level) - 1 + (leaf >> (this->num_levels - level - 1));
}



/*
The below functions are to access various parameters, as described by their names.
INPUT: No input
OUTPUT: Value of internal variables given in the name.
*/

/*int* OramReadPathEviction::getPositionMap() {
    return this->position_map;
}*/

vector<Block2> OramReadPathEviction::getStash() {
    return this->stash;
}
    
int OramReadPathEviction::getStashSize() {
    return (this->stash).size();
}
    
int OramReadPathEviction::getNumLeaves() {
    return this->num_leaves;

}

int OramReadPathEviction::getNumLevels() {
    return this->num_levels;

}

int OramReadPathEviction::getNumBlocks() {
    return this->num_blocks;

}

int OramReadPathEviction::getNumBuckets() {
    return this->num_buckets;

}

int OramReadPathEviction::ReverseBits(int g) {
    /*
    INPUT: Integers g and bits_length.
    OUTPUT: Integer reverse of length bits_length consisting of reversed bits of g.
    To be used to traverse the leaves and run eviction in reverse lexicographical order.
    */
    int bits_length = num_levels - 1;
    int g_mod = g%num_leaves;
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
