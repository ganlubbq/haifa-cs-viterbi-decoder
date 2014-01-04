#include <stdlib.h>		  // srand, rand
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <bitset>         // std::bitset
#include <vector>		  // std::vector
using namespace std;

class Viterbi {
	int inputBits, outputBits, registersCount, constrainLength, parallelism;
	vector< vector<bool> > xors;
	vector<bool> memoryRegisters;
	vector<bool> encodedData;

public:
	// Constructer - will create the encoder and matching decoder
	Viterbi (int n, int k, int m, int p) {
		outputBits = n;
		inputBits = k;
		registersCount = m;
		parallelism = p;

		// calculate constrain length
		constrainLength = inputBits * registersCount;

		// initilize polynomials with random sets of bits to xor
		xors.resize(outputBits);
		srand(time(NULL));
		for (int i = 0; i < outputBits; i++)
		{
			xors[i].resize(constrainLength);
			for (int bit = 0; bit < constrainLength; bit++)
			{
				xors[i][bit] = (rand()%2 != 0);
			}
			cout << "xor" << i << ": ";
			PrintVectorBool(xors[i]);
		}

		memoryRegisters = vector<bool>(constrainLength , 0);
		// inilize the registers each with inputBits number of bits
		memoryRegisters.resize(constrainLength);
		for (int j = 0; j < constrainLength; j++)
		{
			memoryRegisters[j] = 0;
		}
	}

	void PerformXors()
	{
		for (int i = 0; i < outputBits; i++)
		{
			int bit = 0;
			// Find the first bit in memory to include in the xor
			try 
			{
				while (xors[i][bit] != 1)
				{
					bit++;
					if (bit == constrainLength) throw;
				}
			}
			catch (exception) 
			{
				cout << "BAD XORS (all zeros).\n";
			}

			bool dataOut = memoryRegisters[bit];

			for (int j = bit + 1; j < constrainLength - 1; j++)
			{
				// If bit in xor is on perform xor with previous data
				if (xors[i][j]) dataOut ^= memoryRegisters[j];
			}
			encodedData.push_back(dataOut);
		}
	}

	void PrintVectorBool(vector<bool> data)
	{
		for (int i = 0; i < data.size(); i++)
		{
			char bit = (data[i])? '1' : '0';
			cout << bit;
		}
		cout << "\n";
	}
	
	void ShiftRight(bool input)
	{
		int i;
		for (i = memoryRegisters.size()-1; i > 0 ; i--)
		{
			memoryRegisters[i] = memoryRegisters[i-1];
		}
		memoryRegisters[i] = input;
	}

	void Encode(vector<bool> data) 
	{
		for (int i = 0; i < data.size(); i = i + inputBits)
		{
			for (int j = 0; j < inputBits; j++)
				ShiftRight(data[i + j]);
			PerformXors();
		}
		cout << "Encoded Data: ";
		PrintVectorBool(encodedData);
	}

	void GenerateAutomata()
	{

	}

	void Send(vector<bool> data)
	{
		Encode(data);
		Scramble();
	}

	void Scramble()
	{

	}

	void Decode()
	{
		// do stuff...
	}
};

int main() 
{
	cout << "Create Viterbi Module\n";
	Viterbi viterbiTester = Viterbi(3, 1, 3, 0);
	
	bool data[] = {0, 0, 1, 1};
	std::vector<bool> input (data, data + sizeof(data) / sizeof(bool) );
	cout << "Input Data: ";
	viterbiTester.PrintVectorBool(input);
	
	viterbiTester.Send(input);

	return 0;
}