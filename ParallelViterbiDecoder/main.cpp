#include <iostream>

#include "viterbi_module.h"
#include "automata.h"

using namespace std;

int main() 
{
	cout << "Create Viterbi Module\n";
	viterbi_module viterbiTester = viterbi_module(3, 1, 3, 0);
	cout << "\n";

	viterbiTester.PrintXORS();
	cout << "\n";

	char *input = "011011";
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	viterbiTester.Send(input);

	viterbiTester.PrintBus();
	cout << "\n";

	viterbiTester.PrintAutomata();
	cout << "\n";

	return 0;
}