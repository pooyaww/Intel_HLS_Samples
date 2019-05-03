#include <stdio.h>
#include "HLS/hls.h"
#include "complex_lib.h"
// This component function takes two input streams dataInA and dataInB of complex data type and
// does the multiplication of data and puts the result back in output stream resultOut.
// There are two additional parameters outerTripCount and innerTripCount which control the
// iteration of the two loops. This makes the innerTripCount of iterations of the two
// loops dependent on the input to the component.
// There is a nested inner loop but the condition "loopCondition" is now inside
// this inner loop. So, this inner loop can run unconditionally and thus tool
// is able to pipeline the outer loop.          

component void complex_multiply(int innerTripCount, int outerTripCount, bool loopCondition,
                                ihc::stream_in<complexType > &dataInA,
                                ihc::stream_in<complexType > &dataInB,
                                ihc::stream_out<complexType > &resultOut) {

	complexType result[10][10];
	for (int row = 0; row < outerTripCount; row++) {
		for (int col = 0; col < innerTripCount; col++) {
			if (loopCondition) {
				complexType dataA = dataInA.read();
				complexType dataB = dataInB.read();
				result[row][col] = dataA*dataB;
			}
			else {
				complexType temp;
				temp.setReal(2);
				temp.setImag(2);
				result[row][col] = temp;
			}
		}
		for (int col = 0; col < innerTripCount; col++) {
			resultOut.write(result[row][col]);
		}
	}
}


int main() {
	const int num = 10;

	ihc::stream_in<complexType > inA;
	ihc::stream_in<complexType > inB;

	ihc::stream_out<complexType > resOut;

	complexType dataA;
	complexType dataB;
	complexType dataOut[num][num];

	for (int a = 0; a < num; a++) {
		for (int j = 0; j < num; j++) {
			dataA.setReal(a + j);
			dataA.setImag(a + j);
			dataB.setReal(a + j + 1);
			dataB.setImag(a + j + 1);
			inA.write(dataA);
			inB.write(dataB);
			dataOut[a][j] = dataA * dataB;
		}

	}

	complex_multiply(num, num, true, inA, inB, resOut);
	bool passed = true;
	for (int a = 0; a < num; a++) {
		for (int j = 0; j < num; j++) {
			complexType resOutData = resOut.read();
			if (dataOut[a][j].getReal() != resOutData.getReal() || dataOut[a][j].getImag() != resOutData.getImag()) {
				passed = false;
				break;
			}
		}
	}
	if (passed)
		printf("PASSED\n");
	else
		printf("FAILED\n");

	return 0;
}

