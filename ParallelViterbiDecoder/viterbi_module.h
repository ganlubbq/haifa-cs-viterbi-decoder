/* 
 *	Authors:
 *	Shiran Stan-Meleh  ID: 039067608
 *	Gal Keret          ID: 066547969
 */
#pragma once

#include "helper_functions.h"
#include "encoder.h"
#include "automata.h"
#include "decoder.h"

class viterbi_module
{
private:
	encoder _encoder;
	decoder _decoder;
	automata _automata;
	int _inputBits, _outputBits, _registersCount, _constrainLength, _parallelism;
	vector<uint32_t> _xors;
	vector<uint32_t> _encodedData;
	vector<uint32_t> _bus; // Send will save data on the bus with scarmble

public:
	viterbi_module(int outputBits, int inputBits, int registersCount, int parallelism);
	~viterbi_module(void);
	void Send(char* data);
	void Scramble(vector<uint32_t> encodedData);
	void DecodeSequential();
	void DecodeParallel();
	void PrintProperties();
	void PrintEncodedData();
	void PrintBus();
	void PrintXORS();
	void PrintAutomata();
	void PrintInverseAutomata();
};

