#include "decoder.h"


decoder::decoder(void)
{
}


decoder::~decoder(void)
{
}

void decoder::DecodeSequential()
{/*
	// Decode data from bus == scrambledData
	vector<trellis> currStates;
	vector<trellis> nextStates;
	for (uint32_t j = 0; j < automata[0].size(); j++)
	{
		trellis temp;
		temp.state = automata[0][j].state;
		temp.hammingDist = CalcHammingDist(scrambledData[0], automata[0][j].output);
		nextStates.push_back(temp);
	}

	for (uint32_t i = 0; i < scrambledData.size(); i++)
	{
		for (uint32_t j = 0; j < nextStates.size(); j++)
		{
			for (uint32_t k = 0; k < automata[nextStates[j].state].size(); k++)
			{
				trellis temp;
				temp.state = automata[nextStates[j].state][k].state;
				temp.hammingDist = CalcHammingDist(scrambledData[i], automata[nextStates[j].state][k].output);
				nextStates.push_back(temp);
			}
		}
		currStates.empty();
	}*/
}

void decoder::DecodeParallel()
{
	// Decode data from bus == scrambledData
}