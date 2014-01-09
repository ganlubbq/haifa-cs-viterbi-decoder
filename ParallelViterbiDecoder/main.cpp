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
	viterbi_module viterbiTester = viterbi_module(2, 1, 2, 1);

	viterbiTester.PrintProperties();

	char *input = "110110100111";
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