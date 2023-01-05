//
//
//
#include "Block2.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


Block2::Block2(){
	this->leaf_id = -1;
    this->index = -1;
}

Block2::Block2(int leaf_id, int index, int data, vector<int> allPom) : leaf_id(leaf_id), index(index), data(data)
{
   this->allPM = allPom;
}

Block2::~Block2()
{
    //dtor
}

void Block2::printBlock(){
	string data_holder = "";
	data_holder += to_string(this->data);
	data_holder += " ";
	cout << "index: " << to_string(this->index) << " leaf id: " << to_string(this->leaf_id) << " data: " << data_holder << endl;
}