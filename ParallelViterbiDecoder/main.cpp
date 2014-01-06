#include <iostream>

#include "viterbi_module.h"
#include "automata.h"

using namespace std;

int main() 
{
	viterbi_module viterbiTester = viterbi_module(3, 1, 3, 0);

	viterbiTester.PrintProperties();

	char *input = "011011";
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	viterbiTester.Send(input);

	viterbiTester.PrintBus();
	cout << "\n";

	viterbiTester.PrintAutomata();
	cout << "\n";

	viterbiTester.Decode();

	return 0;
}