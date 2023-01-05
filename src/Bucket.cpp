//
//
//
#include "Bucket.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

bool Bucket::is_init = false;
int Bucket::max_size = -1;

Bucket::Bucket(){
    if(!is_init){
        throw new runtime_error("Please set bucket size before creating a bucket");
    }
    blocks = vector<Block2>();
}

//Copy constructor
Bucket::Bucket(Bucket *other){
    if(other == NULL){
        throw new runtime_error("the other bucket is not malloced.");
    }
    blocks = vector<Block2>(max_size);
    for(int i = 0; i < max_size; i++){
        blocks[i] = Block2(other->blocks[i]);
    }
}

//Get block object with matching index
Block2 Bucket::getBlockByIndex(int index) {
    Block2 *copy_block = NULL;
    //for(Block b: blocks){
    for(int i = 0; i <blocks.size(); i++){
        Block2 b = blocks[i];
        if(b.index == index){
            copy_block = new Block2(b);
            break;
        }
    }
    return *copy_block;
}

void Bucket::addBlock(Block2 new_blk){
    if(blocks.size() < max_size){
        Block2 toAdd = Block2(new_blk);
        blocks.push_back(toAdd);
    }

}

bool Bucket::removeBlock(Block2 rm_blk){
    bool removed = false;
    for(int i = 0; i < blocks.size(); i++){
        if(blocks[i].index == rm_blk.index){
            blocks.erase(blocks.begin() + i);
            removed = true;
            break;
        }
    }
    return removed;
}

// Return a shallow copy.
vector<Block2> Bucket::getBlocks(){
    return this->blocks;
}

void Bucket::setMaxSize(int maximumSize){
    if(is_init == true){
        throw new runtime_error("Max Bucket Size was already set");
    }
    max_size = maximumSize;
    is_init = true;
}

int Bucket::getMaxSize() {
    return max_size;
}

void Bucket::resetState(){
    is_init = false;
}

void Bucket::printBlocks() {
    /*for (Block b: blocks) {
        b.printBlock();
    }*/
    for(int i = 0; i < blocks.size(); i++){
        blocks[i].printBlock();
    }
}
