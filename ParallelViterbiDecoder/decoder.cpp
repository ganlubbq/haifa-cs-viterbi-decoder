/* 
*	Authors:
*	Shiran Stan-Meleh  ID: 039067608
*	Gal Keret          ID: 066547969
*/
#include "decoder.h"


decoder::decoder(void)
{
}


decoder::~decoder(void)
{
}

decoder::decoder(int intputBits, int outputBits, int constrainLength, map<uint32_t, vector<state>> automata, map<uint32_t, map<uint32_t, state>> inverseAutomata)
{
	_intputBits = intputBits;
	_outputBits = outputBits;
	_constrainLength = constrainLength;
	_automata = automata;
	_inverseAutomata = inverseAutomata;
	_mtx = new mutex();

	InitMetrics();
}

void decoder::InitMetrics()
{
	// Loop over all possible symbols values
	for (uint32_t symbol = 0; symbol < pow(2, _outputBits); symbol++)
	{
		vector<vector<uint32_t>> rows;
		// Loop over all states from which we go to next states
		for (uint32_t ps = 0; ps < pow(2, _constrainLength); ps++)
		{
			vector<uint32_t> cols;
			// Loop over the next states and check ouput if path is valid
			for (uint32_t ns = 0; ns < pow(2, _constrainLength); ns++)
			{
				bool found = false;
				// Since Automata is sorted by input check for each sybol all inputs and outputs and calc hamming dist
				for (uint32_t input = 0; input < pow(2, _intputBits); input++)
				{
					if (_automata[ps][input].state == ns)
					{
						cols.push_back(CalcHammingDist(symbol, _automata[ps][input].output));
						found = true;
					}
				}
				// If path is not valid put the max value as inifinty
				if (!found)
					cols.push_back(0xFFFFFFFF);
			}
			rows.push_back(cols);
		}
		_adjacentMetrics.push_back(rows);
	}
}

uint32_t decoder::DecodeInputBetweenStates(uint32_t sourceState, uint32_t finalState)
{
	for(std::map<uint32_t, state>::iterator iter = _inverseAutomata[sourceState].begin(); iter != _inverseAutomata[sourceState].end(); ++iter)
	{
		if (iter->second.state == finalState)
			return iter->second.output;
	}

	return -1;
}

uint32_t decoder::FindSourceState(vector<vector<uint32_t>> metB, vector<vector<uint32_t>> metM, uint32_t finalState)
{
	// int value
	uint32_t sourceState = 0;
	uint32_t currMin = metB[0][0] + metM[0][finalState];

	// Go over the overall paths from states in B to final State in M and find the min path
	for (uint32_t k = 1; k < metB.size(); k++)
	{
		if (metB[0][k] + metM[k][finalState] < currMin)
		{
			currMin = metB[0][k] + metM[k][finalState];
			sourceState = k;
		}
	}
	return sourceState;
}

uint32_t decoder::FindMinState(map<uint32_t, uint32_t> states)
{
	// Find the minimum state in a vector of states in trellis
	map<uint32_t, uint32_t>::iterator iter;
	iter = states.begin();
	uint32_t minState = iter->first;
	uint32_t minHammingDist = iter->second;

	// Loop over all states and save minimum
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
	cout << "Most Likely Data:\n";
	bool bad = true;
	for (uint16_t i = 0; i < path.size() - 1; i++)
	{
		for (uint16_t inputBits = 0; inputBits < _automata[0].size(); inputBits++)
		{
			if (_automata[path[i]][inputBits].state == path[i+1])
			{
				PrintBitSet(inputBits, _intputBits);
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

void decoder::DecodeParallel(vector<uint32_t> bus, int parallelism)
{
	int sizeOfPart;
	if (parallelism == 0) sizeOfPart = bus.size();
	else sizeOfPart = bus.size()/parallelism;

	// Create p threads and pass each the start and end of input needed to be worked on
	uint16_t subsetID = 0;
	uint16_t start = 0;
	while (start < bus.size())
	{
		uint16_t end = start + sizeOfPart - 1;
		uint16_t diff = bus.size() - 1 - end;
		// bus input has residule when divided by p - attach it to final part
		if (diff < sizeOfPart)
		{
			end += diff;
		}

		thread* newThread = new thread(CalcAccummlatedMetrices, this, subsetID, start, end, bus);
		_workers.push_back( newThread );

		if (diff < sizeOfPart)
		{
			break;
		}

		start += sizeOfPart;
		subsetID++;
	}

	// wait for all theards to finish
	for (uint16_t threadID = 0; threadID < _workers.size(); threadID++) 
	{
		if (_workers[threadID]->joinable()) {
			_workers[threadID]->join();
		}
	}

	// Calculate Bs and Ms subsets in 2 threads to Find the final state between each subset of metrics
	vector<vector<vector<uint32_t>>> Bs;
	vector<vector<vector<uint32_t>>> Ms;
	vector<uint32_t> finalStates;
	thread BsThread = thread(CalcBs, this->_subsetMetrics, &Bs, &finalStates);
	thread MsThread = thread(CalcMs, this->_subsetMetrics, &Ms);
	BsThread.join();
	MsThread.join();

	for (int i = 0; i < parallelism - 1; i++)
	{
		finalStates.push_back(FindSourceState(Bs[i+1], Ms[i], finalStates[i]));
	}

	// Create p threads and pass each the start and end of input needed to be worked on
	start = 0;
	subsetID = 0;
	_workers.empty();
	while (start < bus.size())
	{
		uint16_t end = start + sizeOfPart - 1;
		uint16_t diff = bus.size() - 1 - end;
		// bus input has residule when divided by p - attach it to final part
		if (diff < sizeOfPart)
		{
			end += diff;
		}

		thread* newThread = new thread(TraceBack, this, start, end, finalStates[subsetID], bus);
		_workers.push_back( newThread );

		if (diff < sizeOfPart)
		{
			break;
		}

		start += sizeOfPart;
		subsetID++;
	}

	// Wait for all theards to finish
	for (uint16_t threadID = 0; threadID < _workers.size(); threadID++) 
	{
		if (_workers[threadID]->joinable()) {
			_workers[threadID]->join();
		}
	}

	// Print result
	cout << "Most Likely Data:\n";
	for (uint32_t index = 0; index < _decodedData.size(); index++)
	{
		PrintBitSet(_decodedData[index], _intputBits);
	}
	cout << "\n";
}

void decoder::PrintVectors(vector<uint32_t> bus)
{
	/*
	cout << "Trellis Hamming Distances:\n";
	for (uint16_t states = 0; states < pow(2, _constrainLength); states++)
	{
	PrintBitSet(bitset<32>(states), _constrainLength);
	cout << ":\t";
	for (uint16_t i = 0; i < _accumulatedMetrics.size(); i++)
	{
	if (_accumulatedMetrics[i][states] == 0xFFFFFFFF)
	cout << "inf\t";
	else cout << _accumulatedMetrics[i][states] << "\t";
	}
	cout << "\n";
	}
	cout << "\n";
	*/
}

/************************************ Thread Functions ***********************************/
static void CalcAccummlatedMetrices(decoder *_decoder, uint16_t subsetID, uint16_t start, uint16_t end, vector<uint32_t> bus)
{
	vector<vector<uint32_t>> result = _decoder->_adjacentMetrics[bus[start]];

	_decoder->_mtx->lock();
	_decoder->_accumulatedMetrics[start] = result;
	_decoder->_mtx->unlock();

	// if last part is not size of input length / parallelism then adjust it
	if (end > bus.size()) end = bus.size() - 1;

	// Perform sequential metric calculation from start of part recieved to end of part
	for (int inputIndex = start + 1; inputIndex <= end; inputIndex++)
	{
		uint32_t symbol = bus[inputIndex];

		result = MultiplyMetrics(result, _decoder->_adjacentMetrics[symbol]);

		_decoder->_mtx->lock();
		_decoder->_accumulatedMetrics[inputIndex] = result;
		_decoder->_mtx->unlock();
	}
	_decoder->_mtx->lock();
	_decoder->_subsetMetrics[subsetID] = result;
	_decoder->_mtx->unlock();
}

static void CalcBs(map<uint32_t, vector<vector<uint32_t>>> subsetMetrics, vector<vector<vector<uint32_t>>> *Bs, vector<uint32_t> *finalStates)
{
	vector<vector<uint32_t>> B = subsetMetrics[0];
	Bs->push_back(B);
	// start from first subset and multiply the next subset with current each time
	for (uint16_t b = 1; b < subsetMetrics.size(); b++)
	{
		B = MultiplyMetrics(B, subsetMetrics[b]);
		Bs->push_back(B);
	}

	// while already calculating the whole metrics multipliers get the final state
	finalStates->push_back(VectorMin(B[0]));
	// to ease the finding of source states later on
	reverse(Bs->begin(),Bs->end());
}

static void CalcMs(map<uint32_t, vector<vector<uint32_t>>> subsetMetrics, vector<vector<vector<uint32_t>>> *Ms)
{
	vector<vector<uint32_t>> M = subsetMetrics[subsetMetrics.size() - 1];
	Ms->push_back(M);
	// start from end subset and multiply the previous subset with current each time
	for (int m = subsetMetrics.size() - 2; m >= 0 ; m--)
	{
		M = MultiplyMetrics(M, subsetMetrics[m]);
		Ms->push_back(M);
	}
}

static void TraceBack(decoder *decoder, uint16_t start, uint16_t end, uint32_t finalState, vector<uint32_t> bus)
{
	uint32_t sourceState;
	for (uint32_t index = end; index > start; index--)
	{
		sourceState = decoder->FindSourceState(decoder->_accumulatedMetrics[index-1], decoder->_adjacentMetrics[bus[index]], finalState);
		decoder->_mtx->lock();
		decoder->_decodedData[index] = decoder->DecodeInputBetweenStates(sourceState, finalState);
		decoder->_mtx->unlock();
		finalState = sourceState;
	}
}