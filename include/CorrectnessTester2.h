

#include "OramInterface.h"
#include "OramReadPathEviction.h"
#include "UntrustedStorageInterface.h"
#include "ServerStorage.h"

#include <vector>


class CorrectnessTester2 {
	public:

		CorrectnessTester2();
		int sampleData(int i);
		void runCorrectnessTest();

};