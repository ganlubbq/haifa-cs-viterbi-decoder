/* 
 *	Authors:
 *	Shiran Stan-Meleh  ID: 039067608
 *	Gal Keret          ID: 066547969
 */
#pragma once

#include "helper_functions.h"

class encoder
{
private:
	int _inputBits, _outputBits, _constrainLength;
	vector<uint32_t> _xors;
	uint32_t _memoryRegisters;

public:
	encoder(void);
	~encoder(void);
	encoder(int inputBits, int outputBits, int constrainLength, vector<uint32_t> xors);

	vector<uint32_t> Encode(char *data);
	void ShiftRight(bool input);
	uint32_t CalcOutput(uint32_t input);
};

