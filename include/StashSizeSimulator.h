

#ifndef PORAM_STASHSIZESIMULATOR
#define PORAM_STASHSIZESIMULATOR

#include "OramInterface.h"
#include "OramReadPathEviction.h"
#include "UntrustedStorageInterface.h"
#include "ServerStorage.h"

#include <vector>


class StashSizeSimulator {
	public:

		StashSizeSimulator(int bSize, int nBlocks, int nAccesses, int sNumber);
		int* sampleData(int i);
		void runSimulation();
		void writeSimulation();
		void updateStashSizes(int recorded, int times);
		int reverse_bits(int g, int num_levels, int num_leaves);
		int* recordedSizes;
		int* stashSizes;
		int numBlocks;
		int bucketSize;
		int numAccesses;
		int simNumber;
		int type;
		int num_Orams;
		//struct iniPM{
		//	int *head;
		//};

};

#endif