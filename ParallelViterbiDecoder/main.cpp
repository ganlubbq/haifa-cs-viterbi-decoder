#include <iostream>

#include "viterbi_module.h"
#include "automata.h"

using namespace std;

int main() 
{
	viterbi_module viterbiTester = viterbi_module(2, 2, 1, 0);

	viterbiTester.PrintProperties();

	char *input = "1100111010100101010010110101001";
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	viterbiTester.Send(input);

	viterbiTester.PrintBus();
	cout << "\n";

	//viterbiTester.PrintAutomata();
	cout << "\n";

	viterbiTester.Decode();

	return 0;
}