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
	viterbi_module viterbiTester = viterbi_module(3, 1, 4, 2);

	viterbiTester.PrintProperties();

	char *input = "1100111010100101010010110101001";
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	viterbiTester.Send(input);

	viterbiTester.PrintBus();
	cout << "\n";

	//viterbiTester.PrintAutomata();
	cout << "\n";

	viterbiTester.DecodeSequential();
	
	viterbiTester.DecodeParallel();

	return 0;
}