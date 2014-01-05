#include <stdlib.h>		  // srand, rand
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <bitset>         // std::bitset
#include <vector>		  // std::vector
#include <map>			  // std:map
#include <string>		  // std:string
using namespace std;

struct DictionaryValue {
	uint32_t state;
	uint32_t output;
}typedef state;

class Viterbi {
	int inputBits, outputBits, registersCount, constrainLength, parallelism;
	vector<uint32_t> xors;
	uint32_t memoryRegisters;
	vector<uint32_t> encodedData;
	vector<uint32_t> scrambledData;
	map<uint32_t, vector<state>> automata;

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
			uint32_t numOfBits = pow(2, constrainLength);
			xors[i] = rand() % numOfBits;
			cout << "XOR" << i << ": ";
			PrintBitSet(xors[i], constrainLength);
			cout << "\n";
		}

		// init memoryRegisters
		memoryRegisters = 0;

		// Create the State Machine to be used later in decode
		GenerateAutomata();
	}

	void Encode(char *data)
	{
		// Init pointer to iterate on input data
		char *dataPtr = data;

		// Move over char input with jumps of size inputbits
		for (int i = 0; i < strlen(data); i = i + inputBits)
		{
			// Take the next Block to encode with size of InputBits
			char* block = new char[inputBits]();
			strncpy_s(block, sizeof(block), dataPtr, inputBits);
			// Shift Right the bits within the block
			for (int j = inputBits - 1; j >= 0; j--) 
			{
				ShiftRight((block[j] == '1')? 1 : 0);
			}
			// Calculate the output using XORS * outputBits and Push the encoded result to encoded output
			encodedData.push_back(CalcOutput(memoryRegisters));
			// Move to start of next Block
			dataPtr += inputBits;
		}

		// Print out the encoded data in blocks (each for one input symbol)
		cout << "Encoded Data:\n";
		for (int i = 0; i < encodedData.size(); i++)
		{
			PrintBitSet(encodedData[i], constrainLength);
		}
		cout << "\n";
	}

	void ShiftRight(bool input)
	{
		// Each time shift right by 1 bit and put the next input bit on left (reading wise)
		// We always use bitset of 32 (assumed to be max of constrain length) but we only refer to constrain length
		memoryRegisters = (memoryRegisters >> 1) | (input << constrainLength - 1);
	}

	uint32_t CalcOutput(uint32_t input)
	{
		// Init temp values for calculation
		bool currXOR;
		bool first = true;
		uint32_t output = 0;
		bitset<32> buffer = bitset<32>(input);

		// Perform XORS as many as outputBits
		for (int i = 0; i < outputBits; i++)
		{
			// Get the current xor
			bitset<32> xor = bitset<32>(xors[i]);
			first = true;

			// Perform XOR between all bits in the buffer which has 1 in relative position on the xors vector
			for (int bit = 0; bit < constrainLength; bit++)
			{
				if ((xor[bit] == 1) && first)
				{
					currXOR = buffer[bit];
					first = false;
				}
				else if (xor[bit] == 1) 
					currXOR ^= buffer[bit];
			}

			// Put the result in output in consistent order (left to right)
			output = (output << 1) | currXOR;
		}
		return output;
	}

	void PrintBitSet(bitset<32> data, int count)
	{
		// Print bitset from left to right (for clearer reading)- only the actual part used
		for (int i = count - 1; i >= 0; i--)
		{
			char bit = (data[i])? '1' : '0';
			cout << bit;
		}
	}

	void GenerateAutomata()
	{
		// Create states as many as 2^constrainLength
		for (uint32_t newState = 0; newState < pow(2, constrainLength); newState++)
		{
			// Temp vector to contain the valid states we can get from current state
			vector<state> tempVector;
			for (int j = 0; j < pow(2, inputBits); j++)
			{
				// for each input bit possible create new state with output
				state tempState;
				tempState.state = (newState >> inputBits) | (j << constrainLength - inputBits);
				tempState.output = CalcOutput(tempState.state);
				tempVector.push_back(tempState);
			}
			automata[newState] = tempVector;
		}
	}

	void PrintAutomata()
	{
		cout << "Automata:\n";
		for(std::map<uint32_t, vector<state>>::iterator iter = automata.begin(); iter != automata.end(); ++iter)
		{
			PrintBitSet(bitset<32>(iter->first), constrainLength);
			cout << " -> \t";
			for (int input = 0; input < iter->second.size(); input++)
			{
				PrintBitSet(bitset<32>(input), inputBits);
				cout << ": ";
				PrintBitSet(bitset<32>(iter->second[input].state), constrainLength);
				cout << "/";
				PrintBitSet(bitset<32>(iter->second[input].output), constrainLength);
				cout << "\t";
			}
			cout << "\n";
		}
		cout << "\n";
	}

	void Send(char* data)
	{
		// First encode the data
		Encode(data);
		// Filp some bits to simulate bus interference
		Scramble();
	}

	void Scramble()
	{
		scrambledData = encodedData;
		srand(time(NULL));
		for (int i = 0; i < outputBits; i = i + rand() % scrambledData.size())
		{
			bitset<32> block = bitset<32>(scrambledData[i]);
			block[rand() % outputBits].flip();
			scrambledData[i] = block.to_ullong();
		}
		cout << "Scarmbled Data:\n";
		for (int i = 0; i < scrambledData.size(); i++)
		{
			PrintBitSet(scrambledData[i], constrainLength);
		}
		cout << "\n";
	}

	void Decode()
	{
		// Decode data from bus == scrambledData
	}
};

int main() 
{
	cout << "Create Viterbi Module\n";
	Viterbi viterbiTester = Viterbi(3, 2, 3, 0);

	char *input = "011011";
	cout << "Input Data:\n" << input << "\n";

	viterbiTester.Send(input);
	viterbiTester.PrintAutomata();

	return 0;
}