//
//
//

#ifndef PORAM_UNTRUSTEDSTORAGEINTERFACE_H
#define PORAM_UNTRUSTEDSTORAGEINTERFACE_H
#include "Bucket.h"

class UntrustedStorageInterface {
    //You are required to call setCapacity BEFORE reading or writing any buckets. You are not allowed to change the capacity after you set it.
    //You should only use ONE UntrustedStorage in your ORAM.
public:
    virtual void setCapacity(int totalNumOfBuckets, int oramid) {};

    virtual Bucket ReadBucket(int OramID, int position) { return Bucket(); };

    virtual void WriteBucket(int OramID, int position, const Bucket& bucket_to_write) {};

    virtual void WriteBtreePM(int OramID, int pom[]) {};
};


#endif //PORAM_UNTRUSTEDSTORAGEINTERFACE_H
