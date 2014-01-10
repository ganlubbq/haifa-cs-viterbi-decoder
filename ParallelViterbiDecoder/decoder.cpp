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

decoder::decoder(int intputBits, int outputBits, int constrainLength, map<uint32_t, vector<state>> automata)
{
	_intputBits = intputBits;
	_outputBits = outputBits;
	_constrainLength = constrainLength;
	_automata = automata;
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
		_metrics.push_back(rows);
	}
}

vector<vector<uint32_t>> decoder::MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB)
{
	// Init the result metric and the temp minium used un calculations
	vector<vector<uint32_t>> metC;
	uint32_t minimum;

	// Make sure matric size comply
	if (metA.size() != metB.size())
	{
		cout << "Metrics Size not match!\n";
		exit(1);
	}

	for (uint32_t i = 0; i < metA.size(); i++)
	{
		vector<uint32_t> cols;
		for (uint32_t j = 0; j < metB.size(); j++)
		{
			minimum = 0xFFFFFFFF;
			for (uint32_t k = 0; k < metA.size(); k++)
			{
				// avoid overflow
				if (metA[i][k] == 0xFFFFFFFF || metB[k][j] == 0xFFFFFFFF)
					minimum = min(minimum, 0xFFFFFFFF);
				else
					minimum = min(minimum, metA[i][k] + metB[k][j]);
			}
			cols.push_back(minimum);
		}
		metC.push_back(cols);
	}
	return metC;
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

// This static function will run in each thread and perform the sub metric calculations
static void ThreadWorker(decoder *_decoder, uint16_t start, uint16_t end, vector<uint32_t> bus)
{
	vector<vector<uint32_t>> result = _decoder->_metrics[bus[start]];

	_decoder->_mtx->lock();
	_decoder->_vectors[start] = result[0];
	_decoder->_accumulatedMetrics[0] = result;
	_decoder->_mtx->unlock();

	// if last part is not size of input length / parallelism then adjust it
	if (end > bus.size()) end = bus.size() - 1;

	// Perform sequential metric calculation from start of part recieved to end of part
	for (int inputIndex = start + 1; inputIndex <= end; inputIndex++)
	{
		uint32_t symbol = bus[inputIndex];

		result = _decoder->MultiplyMetrics(result, _decoder->_metrics[symbol]);

		_decoder->_mtx->lock();
		_decoder->_vectors[inputIndex] = result[0];
		_decoder->_accumulatedMetrics[inputIndex] = result;
		_decoder->_mtx->unlock();
	}
	_decoder->_mtx->lock();
	_decoder->_results.push_back(result);
	_decoder->_mtx->unlock();
}

void decoder::DecodeParallel(vector<uint32_t> bus, int parallelism)
{
	int sizeOfPart;
	if (parallelism == 0) sizeOfPart = bus.size();
	else sizeOfPart = bus.size()/parallelism;

	// Create p threads and pass each the start and end of input needed to be worked on
	for (uint16_t start = 0; start < bus.size(); start += sizeOfPart)
	{
		thread* newThread = new thread(ThreadWorker, this, start, start + sizeOfPart - 1, bus);
		_workers.push_back( newThread );
	}

	// wait for all theards to finish
	for (uint16_t threadID = 0; threadID < _workers.size(); threadID++) 
	{
		if (_workers[threadID]->joinable()) {
			_workers[threadID]->join();
		}
	}

	// Multiply the accumulated metrics returned from each thread
	vector<vector<uint32_t>> currMetric = _results[0];
	for (uint16_t i = 1; i < _results.size(); i++)
	{
		currMetric = MultiplyMetrics(currMetric, _results[i]);
	}

	// Just for testing
	PrintVectors(bus);

	//print most likely data
	cout << "Most Likely Data:\n";
	for (uint16_t inputBits = 0; inputBits < _automata[0].size(); inputBits++)
	{
		if (_automata[0][inputBits].state == VectorMin(_vectors[0]))
		{
			PrintBitSet(inputBits, _intputBits);
		}
	}

	for (uint16_t i = 0; i < _vectors.size() - 1; i++)
	{
		uint32_t ps = VectorMin(_vectors[i]);
		uint32_t ns = VectorMin(_vectors[i+1]);
		for (uint16_t inputBits = 0; inputBits < _automata[0].size(); inputBits++)
		{
			if (_automata[ps][inputBits].state == ns)
			{
				PrintBitSet(inputBits, _intputBits);
			}
		}
	}
	cout << "\n";
}

void decoder::PrintVectors(vector<uint32_t> bus)
{
	cout << "Trellis Hamming Distances:\n";
	for (uint16_t states = 0; states < pow(2, _constrainLength); states++)
	{
		PrintBitSet(bitset<32>(states), _constrainLength);
		cout << ":\t";
		for (uint16_t i = 0; i < _vectors.size(); i++)
		{
			if (_vectors[i][states] == 0xFFFFFFFF)
				cout << "inf\t";
			else cout << _vectors[i][states] << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}