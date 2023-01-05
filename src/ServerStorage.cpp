
#include "ServerStorage.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//bool ServerStorage::is_initialized = false;
//bool ServerStorage::is_capacity_set = false;

ServerStorage::ServerStorage(int oramid) {
	
	//if (this->is_initialized) {
	//	throw new runtime_error("ONLY ONE ServerStorage CAN BE USED AT A TIME IN THIS IMPLEMENTATION");
	//}
	//this->ServerStorage::rOram[oramid].is_initialized = true;
}

void ServerStorage::setCapacity(int totalNumOfBuckets, int OramID) {
	/*if (this->ServerStorage::rOram[OramID].is_capacity_set) {
		cout<<"hello"<<endl;
		throw new runtime_error("Capacity of ServerStorage cannot be changed");
	}*/
	//this->ServerStorage::rOram[OramID].is_capacity_set = true;
	this->capacity = totalNumOfBuckets;
	//rOrams *p = this->rOram;
	//p+= OramID;
	this->buckets.assign(totalNumOfBuckets, Bucket());
	//rOram[OramID]->buckets.assign(totalNumOfBuckets, Bucket());
}

Bucket ServerStorage::ReadBucket(int OramID, int position) {
	/*if (!this->ServerStorage::rOram[OramID].is_capacity_set) {
		throw new runtime_error("Please call setCapacity before reading or writing any block");
	}*/

	if (position >= this->capacity || position < 0) {
		//std::ostringstream positionStream;
		//positionStream << position;
		throw new runtime_error("You are trying to access Bucket " + to_string(position) + ", but this Server contains only " + to_string(this->capacity) + " buckets.");
	}
	//rOrams *p = this->rOram;
	//p+=OramID;
	return this->buckets.at(position);
	//return this->rOram[OramID]->buckets.at(position);
}

void ServerStorage::WriteBucket(int OramID, int position, const Bucket& bucket_to_write) {
	/*if (!this->ServerStorage::rOram[OramID].is_capacity_set) {
		throw new runtime_error("Please call setCapacity before reading or writing any block");
	}*/

	if (position >= this->capacity || position < 0) {
		throw new runtime_error("You are trying to access Bucket " + to_string(position) + ", but this Server contains only " + to_string(this->capacity) + " buckets.");
	}

	//rOrams *p = this-> rOram;
	//p+=OramID;
	this->buckets.at(position) = bucket_to_write;
	return;
}

void ServerStorage::WriteBtreePM(int OramID, btree_t *btree) {
	//rOrams *p = this->rOram;
	//p+=OramID;
	this->btree = btree;
}