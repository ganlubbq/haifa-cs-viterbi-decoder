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
	int _intputBits, _constrainLength;
	vector<thread*> _workers;

public:
	decoder(void);
	~decoder(void);
	decoder(int intputBits,int constrainLength, map<uint32_t, vector<state>> automata);
	
	map<uint32_t, vector<state>> _automata;
	vector<vector<vector<uint32_t>>> _metrics;
	map<uint32_t, vector<vector<uint32_t>>> _accumulatedMetrics;
	map<uint32_t, vector<uint32_t>> _vectors;
	mutex *_mtx;
	
	void InitMetrics();
	vector<vector<uint32_t>> MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB);
	uint32_t FindMinState(map<uint32_t, uint32_t> states);
	void DecodeSequential(vector<uint32_t> bus);
	void DecodeParallel(vector<uint32_t> bus, int parallelism);
};

