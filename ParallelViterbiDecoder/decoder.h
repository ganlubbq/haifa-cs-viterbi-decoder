/* 
 *	Authors:
 *	Shiran Stan-Meleh  ID: 039067608
 *	Gal Keret          ID: 066547969
 */
#pragma once

#include <math.h>       /* log2 */
#include <mutex>
#include "helper_functions.h"

class decoder
{
private:
	int _intputBits, _outputBits, _constrainLength;
	vector<thread*> _workers;

public:
	decoder(void);
	~decoder(void);
	decoder(int intputBits, int outputBits, int constrainLength, map<uint32_t, vector<state>> automata, map<uint32_t, map<uint32_t, state>> inverseAutomata);
	map<uint32_t, vector<state>> _automata;
	map<uint32_t, map<uint32_t, state>> _inverseAutomata;
	vector<vector<vector<uint32_t>>> _adjacentMetrics;
	map<uint32_t, vector<vector<uint32_t>>> _accumulatedMetrics;
	map<uint32_t, vector<vector<uint32_t>>> _subsetMetrics;
	map<uint32_t, uint32_t> _decodedData;
	mutex *_mtx;
	
	void InitMetrics();
	uint32_t DecodeInputBetweenStates(uint32_t source, uint32_t dest);
	uint32_t FindSourceState(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB, uint32_t finalState);
	uint32_t FindMinState(map<uint32_t, uint32_t> states);
	void DecodeSequential(vector<uint32_t> bus);
	void DecodeParallel(vector<uint32_t> bus, int parallelism);
	void PrintVectors(vector<uint32_t> bus);
};

/************************************ Thread Functions ***********************************/
void CalcAccummlatedMetrices(decoder *_decoder, uint16_t subsetID, uint16_t start, uint16_t end, vector<uint32_t> bus);
static void CalcBs(map<uint32_t, vector<vector<uint32_t>>> _results, vector<vector<vector<uint32_t>>> *Bs,  vector<uint32_t> *finalStates);
static void CalcMs(map<uint32_t, vector<vector<uint32_t>>> _results, vector<vector<vector<uint32_t>>> *Ms);
static void TraceBack(decoder *_decoder, uint16_t start, uint16_t end, uint32_t finalState);
