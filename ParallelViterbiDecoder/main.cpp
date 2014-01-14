/* 
*	Authors:
*	Shiran Stan-Meleh  ID: 039067608
*	Gal Keret          ID: 066547969
*/

#include <iostream>

#include "viterbi_module.h"
#include "automata.h"

using namespace std;

int main() 
{
	viterbi_module viterbiTester = viterbi_module(10, 1, 3, 5);

	viterbiTester.PrintProperties();

	char *input = "01100101111010101010010100101010010101001010010101010101010100010101";
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	viterbiTester.Send(input);

	viterbiTester.PrintEncodedData();
	cout << "\n";

	viterbiTester.PrintBus();
	cout << "\n";

	//viterbiTester.PrintAutomata();
	//cout << "\n";

	//viterbiTester.PrintInverseAutomata();
	//cout << "\n";

	viterbiTester.DecodeSequential();

	viterbiTester.DecodeParallel();

	return 0;
}