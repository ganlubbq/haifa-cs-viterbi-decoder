#pragma once
#include "helper_functions.h"

class automata
{
private:
	int _inputBits, _outputBits, _constrainLength;
	vector<uint32_t> _xors;
	map<uint32_t, vector<state>> _automata;

public:
	automata(void);
	~automata(void);
	automata(int inputBits, int outputBits, int constrainLength, vector<uint32_t> xors);

	map<uint32_t, vector<state>> getAutomata();
	uint32_t CalcOutput(uint32_t input);
	void GenerateAutomata();
	void GenerateInverseAutomata();
	void PrintAutomata();
};

