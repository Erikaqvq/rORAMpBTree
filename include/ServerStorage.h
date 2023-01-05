//
//
//

#ifndef PORAM_SERVERSTORAGE_H
#define PORAM_SERVERSTORAGE_H
#include "OramInterface.h"
#include "RandForOramInterface.h"
#include "UntrustedStorageInterface.h"
#include "btree.h"
#include <cmath>

class ServerStorage : public UntrustedStorageInterface {
public:
    
    //Bucket* buckets;
    
    
    std::vector<Bucket> buckets;
    btree_t *btree;
        //static bool is_initialized;
        //static bool is_capacity_set;
 
    //int rOram[10001];
    //std::vector<Bucket> buckets;
    
    ServerStorage(int OramID);
    void setCapacity(int totalNumOfBuckets, int oramid);
    Bucket ReadBucket(int OramID, int position);
    void WriteBucket(int OramID, int position, const Bucket& bucket_to_write);
    void WriteBtreePM(int OramID, btree_t *btree);

private: 

    int capacity;

};


#endif //PORAM_ORAMREADPATHEVICTION_H
