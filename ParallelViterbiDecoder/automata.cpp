/* 
*	Authors:
*	Shiran Stan-Meleh  ID: 039067608
*	Gal Keret          ID: 066547969
*/
#include "automata.h"


automata::automata(void)
{
}


automata::~automata(void)
{
}

automata::automata(int inputBits, int outputBits, int constrainLength, vector<uint32_t> xors)
{
	_inputBits = inputBits;
	_outputBits = outputBits;
	_constrainLength = constrainLength;
	_xors = xors;
	
	// Create the State Machine to be used later in decode
	GenerateAutomata();
}

map<uint32_t, vector<state>> automata::getAutomata()
{
	return _automata;
}


uint32_t automata::CalcOutput(uint32_t input)
{
	// Init temp values for calculation
	bool currXOR;
	bool first = true;
	uint32_t output = 0;
	bitset<32> buffer = bitset<32>(input);

	// Perform XORS as many as outputBits
	for (int i = 0; i < _outputBits; i++)
	{
		// Get the current xor
		bitset<32> xor = bitset<32>(_xors[i]);
		first = true;

		// Perform XOR between all bits in the buffer which has 1 in relative position on the xors vector
		for (int bit = 0; bit < _constrainLength; bit++)
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
		output = (output << 1) | (int)currXOR;
	}
	return output;
}

void automata::GenerateAutomata()
{
	// Create states as many as 2^constrainLength
	for (uint32_t newState = 0; newState < pow(2, _constrainLength); newState++)
	{
		// Temp vector to contain the valid states we can get from current state
		vector<state> tempVector;
		for (int j = 0; j < pow(2, _inputBits); j++)
		{
			// for each input bit possible create new state with output
			state tempState;
			tempState.state = (newState >> _inputBits) | (j << (_constrainLength - _inputBits));
			tempState.output = CalcOutput(tempState.state);
			tempVector.push_back(tempState);
		}
		_automata[newState] = tempVector;
	}
}

void automata::GenerateInverseAutomata()
{
	cout << "Inverse Automata:\n";
	for(std::map<uint32_t, vector<state>>::iterator iter = _automata.begin(); iter != _automata.end(); ++iter)
	{
		PrintBitSet(bitset<32>(iter->first), _constrainLength);
		cout << " -> \t";
		for (uint16_t input = 0; input < iter->second.size(); input++)
		{
			PrintBitSet(bitset<32>(input), _inputBits);
			cout << ": ";
			PrintBitSet(bitset<32>(iter->second[input].state), _constrainLength);
			cout << "/";
			PrintBitSet(bitset<32>(iter->second[input].output), _constrainLength);
			cout << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}

void automata::PrintAutomata()
{
	cout << "Automata:\n";
	for(std::map<uint32_t, vector<state>>::iterator iter = _automata.begin(); iter != _automata.end(); ++iter)
	{
		PrintBitSet(bitset<32>(iter->first), _constrainLength);
		cout << " -> \t";
		for (uint16_t input = 0; input < iter->second.size(); input++)
		{
			PrintBitSet(bitset<32>(input), _inputBits);
			cout << ": ";
			PrintBitSet(bitset<32>(iter->second[input].state), _constrainLength);
			cout << "/";
			PrintBitSet(bitset<32>(iter->second[input].output), _constrainLength);
			cout << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}