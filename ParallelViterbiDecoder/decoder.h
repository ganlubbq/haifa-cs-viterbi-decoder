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
	vector<vector<vector<uint32_t>>> _metrics;
	map<uint32_t, vector<vector<uint32_t>>> _accumulatedMetrics;
	map<uint32_t, vector<uint32_t>> _vectors;
	vector<vector<vector<uint32_t>>> _results;
	mutex *_mtx;
	
	void InitMetrics();
	vector<vector<uint32_t>> MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB);
	uint32_t DecodeInputBetweenStates(uint32_t source, uint32_t dest);
	uint32_t FindSourceState(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB, uint32_t finalState);
	uint32_t FindMinState(map<uint32_t, uint32_t> states);
	void DecodeSequential(vector<uint32_t> bus);
	void DecodeParallel(vector<uint32_t> bus, int parallelism);
	void PrintVectors(vector<uint32_t> bus);
};

