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

decoder::decoder(int intputBits,int constrainLength, map<uint32_t, vector<state>> automata)
{
	_intputBits = intputBits;
	_constrainLength = constrainLength;
	_automata = automata;
	_mtx = new mutex();

	InitMetrics();
}

void decoder::InitMetrics()
{
	for (uint32_t input = 0; input < pow(2, _intputBits); input++)
	{
		vector<vector<uint32_t>> rows;
		for (uint32_t ps = 0; ps < pow(2, _constrainLength); ps++)
		{
			vector<uint32_t> cols;
			for (uint32_t ns = 0; ns < pow(2, _constrainLength); ns++)
			{
				if (_automata[ps][input].state == ns)
					cols.push_back(CalcHammingDist(input, _automata[ps][input].output));
				else
					cols.push_back(0xFFFFFFFF);
			}
			rows.push_back(cols);
		}
		_metrics.push_back(rows);
	}
}

vector<vector<uint32_t>> decoder::MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB)
{
	vector<vector<uint32_t>> metC;
	uint32_t minimum = 0xFFFFFFFF;

	if (metA.size() != metB.size())
	{
		cout << "Metrics Size not match!\n";
		exit(1);
	}

	for (uint32_t i = 0; i < metA.size(); i++)
	{
		vector<uint32_t> cols;
		for (uint32_t j = 0; j < metA.size(); j++)
		{
			for (uint32_t k = 0; k < metA.size(); k++)
			{
				minimum = min(minimum, metA[i][k] + metB[k][j]);
			}
			cols.push_back(minimum);
			minimum = 0xFFFFFFFF;
		}
		metC.push_back(cols);
	}
	return metC;
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
	cout << "Most Likely Data:\n";
	int outputBits = log( _automata.size() ) / log( 2 );
	bool bad = true;
	for (uint16_t i = 0; i < path.size() - 1; i++)
	{
		for (uint16_t inputBits = 0; inputBits < _automata[0].size(); inputBits++)
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

// This static function will run in thread and perform the sub metric calculations
static void ThreadWorker(decoder *_decoder, uint16_t start, uint16_t end, vector<uint32_t> bus)
{
	vector<vector<uint32_t>> result = _decoder->_metrics[bus[start]];
	_decoder->_mtx->lock();
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
}

void decoder::DecodeParallel(vector<uint32_t> bus, int parallelism)
{
	int sizeOfPart = bus.size()/parallelism;
	
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
}