#pragma once

#include <math.h>       /* log2 */
#include "helper_functions.h"

class decoder
{
private:
	int _intputBits, _constrainLength;
	map<uint32_t, vector<state>> _automata;
	vector<vector<vector<uint32_t>>> metrics;
public:
	decoder(void);
	~decoder(void);
	decoder(int intputBits,int constrainLength, map<uint32_t, vector<state>> automata);

	void InitMetrics();
	vector<vector<uint32_t>> MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB);
	uint32_t FindMinState(map<uint32_t, uint32_t> states);
	void DecodeSequential(vector<uint32_t> bus);
	void DecodeParallel();
};

