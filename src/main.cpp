#include <iostream>
#include <cmath>
#include "../include/Bucket.h"
#include "../include/Block2.h"
#include "../include/RandomForOram.h"
#include "../include/OramReadPathEviction.h"
#include "../include/OramInterface.h"
#include "../include/RandForOramInterface.h"
#include "../include/UntrustedStorageInterface.h"
#include "../include/ServerStorage.h"
#include "../include/StashSizeSimulator.h"

using namespace std;

int BuS; //bucket_size;
int BlN; //block_numbers;

int main() {
    // cout << "Example Test: Writing fewer than reading" << endl;
    // CorrectnessTester1 * tester1 = new CorrectnessTester1();
    // tester1->runCorrectnessTest();
    
    /*
    Make sure to delete the tester and reset various static variables, 
    this allows you to seemlessly run multiple tests and simulations
    */
    // delete tester1;
    // Bucket::resetState();
    // ServerStorage::is_initialized = false;
    // ServerStorage::is_capacity_set = false;
    // RandomForOram::is_initialized = false;
    // RandomForOram::bound = -1; 

    
    /*
    Testing with a timer. Note that clock_t is not part of all implementations
    of C++, so you may have to check which clock works best for you.
    */
    // std::clock_t start = std::clock();
    // cout << "Another Sample Test: Running a long run from CorrectnessTester5" << endl;
    // CorrectnessTester5 * tester5 = new CorrectnessTester5();
    // tester5->runCorrectnessTest();
    // std::clock_t end = std::clock();
    // double total = (double)(end - start)/CLOCKS_PER_SEC;
    // cout << "total: " << total << endl;

    // delete tester5;
    // Bucket::resetState();
    // ServerStorage::is_initialized = false;
    // ServerStorage::is_capacity_set = false;
    // RandomForOram::is_initialized = false;
    // RandomForOram::bound = -1;

    /*
    Simulations for stash sizes. 
    See StashSizeSimulator.cpp for details on the StashSizeSimulator. 
    */
    // std::clock_t start = std::clock();
    // cout << "Running simulation: Z=2, Eviction Algorithm: Deterministic Eviction" << endl;
    // StashSizeSimulator * sim1 = new StashSizeSimulator(2, pow(2, 20), 500000000, 1, 1);
    // sim1->runSimulation();
    // // std::clock_t end = std::clock();
    // // double total = (double)(end - start)/CLOCKS_PER_SEC;
    // // cout << "total: " << total << endl;

    // delete sim1;
    // Bucket::resetState();
    // ServerStorage::is_initialized = false;
    // ServerStorage::is_capacity_set = false;
    // RandomForOram::is_initialized = false;
    // RandomForOram::bound = -1;
    

    // std::clock_t start = std::clock();
    // cout << "Running simulation: Z=4, Eviction Algorithm: Read Path Eviction" << endl;
    // StashSizeSimulator * sim2 = new StashSizeSimulator(4, pow(2, 20), 500000000, 2, 0);
    // sim2->runSimulation();
    // // std::clock_t end = std::clock();
    // // double total = (double)(end - start)/CLOCKS_PER_SEC;
    // // cout << "total: " << total << endl;

    // delete sim2;
    // Bucket::resetState();
    // ServerStorage::is_initialized = false;
    // ServerStorage::is_capacity_set = false;
    // RandomForOram::is_initialized = false;
    // RandomForOram::bound = -1;

    // std::clock_t start = std::clock();
    // cout << "Running simulation: Z=5, Eviction Algorithm: Read Path Eviction" << endl;
    // StashSizeSimulator * sim3 = new StashSizeSimulator(5, pow(2, 20), 500000000, 3, 0);
    // sim3->runSimulation();
    // // std::clock_t end = std::clock();
    // // double total = (double)(end - start)/CLOCKS_PER_SEC;
    // // cout << "total: " << total << endl;

    // delete sim3;
    // Bucket::resetState();
    // ServerStorage::is_initialized = false;
    // ServerStorage::is_capacity_set = false;
    // RandomForOram::is_initialized = false;
    // RandomForOram::bound = -1;


    // std::clock_t start = std::clock();
    
    BuS = 2;
    BlN = pow(2,4);
    cout << "Running simulation: Z=6, Eviction Algorithm: Read Path Eviction" << endl;
    StashSizeSimulator * sim4 = new StashSizeSimulator(BuS, BlN, 50, 4);
    sim4->runSimulation();
    // std::clock_t end = std::clock();
    // double total = (double)(end - start)/CLOCKS_PER_SEC;
    // cout << "total: " << total << endl;

    delete sim4;
    Bucket::resetState();
    //ServerStorage::is_initialized = false;
    //ServerStorage::is_capacity_set = false;
    RandomForOram::is_initialized = false;
    RandomForOram::bound = -1;

}
