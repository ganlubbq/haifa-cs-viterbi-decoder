#include "decoder.h"


decoder::decoder(void)
{
}


decoder::~decoder(void)
{
}

decoder::decoder(map<uint32_t, vector<state>> automata)
{
	_automata = automata;
}

uint32_t decoder::FindMinState(map<uint32_t, uint32_t> states)
{
	map<uint32_t, uint32_t>::iterator iter;
	iter = states.begin();
	uint32_t minState = iter->first;
	uint32_t minHammingDist = iter->second;

	for (iter = states.begin(); iter != states.end(); iter++)
	{
		if (iter->second < minHammingDist)
		{
			minState = iter->first;
			minHammingDist = iter->second;
		}
	}
	return minState;
}

void decoder::DecodeSequential(vector<uint32_t> bus)
{
	map<uint32_t, uint32_t>::iterator iter;
	map<uint32_t, uint32_t> currStates;
	map<uint32_t, uint32_t> nextStates;
	vector<uint32_t> path;

	// Init first Trellis states
	currStates[0] = 0;
	path.push_back(0);

	// Go over each symbol of encoded (scrambled) data
	for (uint32_t i = 0; i < bus.size(); i++)
	{
		// Go over each of the current states and calulate next states and hamming distance
		for (iter = currStates.begin(); iter != currStates.end(); iter++)
		{
			uint32_t currState = iter->first;
			// iterate over the possible inputs to generate the next states from each state of the current list
			for (uint32_t inputBits = 0; inputBits < _automata[currState].size(); inputBits++)
			{
				state newState = _automata[currState][inputBits];
				uint32_t hammingDist = CalcHammingDist(bus[i], newState.output);
				
				if (nextStates.count(newState.state) > 0) {
					if ( (iter->second + hammingDist) < nextStates[newState.state])
						nextStates[newState.state] = iter->second + hammingDist;
				}
				else
					nextStates[newState.state] = iter->second + hammingDist;
			}
		}

		path.push_back(FindMinState(nextStates));

		currStates = nextStates;
		nextStates.clear();
	}

	//print most likely data
	int outputBits = log( _automata.size() ) / log( 2 );
	bool bad = true;
	for (int i = 0; i < path.size() - 1; i++)
	{
		for (int inputBits = 0; inputBits < _automata[0].size(); inputBits++)
		{
			if (_automata[path[i]][inputBits].state == path[i+1])
			{
				PrintBitSet(inputBits, log( _automata[0].size() ) / log( 2 ));
				bad = false;
			}
		}
		if (bad)
		{
			cout << "\nERROR TOO LARGE: Can't correct\n";
			break;
		}
		else bad = true;
	}
	cout << "\n";
}

void decoder::DecodeParallel()
{
	// Decode data from bus == scrambledData
}