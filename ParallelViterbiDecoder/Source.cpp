#include <stdlib.h>		  // srand, rand
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <bitset>         // std::bitset
#include <vector>		  // std::vector
using namespace std;

class Viterbi {
	int inputBits, outputBits, registersCount, constrainLength, parallelism;
	vector< vector<bool> > polynomials;
	vector< vector<bool> > memoryRegisters;	

public:
	// Constructer - will create the encoder and matching decoder
	Viterbi (int n, int k, int m, int p) {
		inputBits = k;
		outputBits = n;
		registersCount = m;
		parallelism = p;

		// calculate constrain length
		constrainLength = inputBits * (registersCount - 1);

		// initilize polynomials with random sets of bits to xor
		polynomials.resize(outputBits);
		for (int i = 0; i < outputBits; i++)
		{
			polynomials.resize(inputBits * registersCount);
			for (int bit = 0; bit < inputBits * registersCount; bit++)
			{
				polynomials[i][bit] = (rand()%2 != 0);
			}
		}

		// inilize the registers each with inputBits number of bits
		memoryRegisters.resize(registersCount);
		for (int j = 0; j < registersCount; j++)
		{
			memoryRegisters[j].resize(inputBits);
		}
	}

	void ShiftRight(bool input)
	{
		for (int i = 0; i < (inputBits * registersCount); i++)
		{
			memoryRegisters[i+1] = memoryRegisters[i];
		}
	}

	void Encode(vector<bool> data) 
	{
		for (int i = 0; i < sizeof(data); i++)
		{

		}
	}

	void GenerateAutomata()
	{

	}

	void Send()
	{
		// do stuff...
	}

	void Decode()
	{
		// do stuff...
	}
};

int main() 
{
	cout << "Create Viterbi Module\n";
	Viterbi viterbiTester = Viterbi(2, 1, 2, 0);

	cout << "foo and bar completed.\n";

	return 0;
}