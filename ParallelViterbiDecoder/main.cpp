/* 
*	Authors:
*	Shiran Stan-Meleh  ID: 039067608
*	Gal Keret          ID: 066547969
*/

#include <iostream>
#include <string>

#include "viterbi_module.h"
#include "automata.h"
#include "Verilog.h"

using namespace std;

void GenerateVerilog(int outputBits, int inputBits, int registersCount, int parallelism)
{
	folder = "C:\\test\\"; //doesn't work yet
	(parallelism > 1)? isParallel = true : false;
	inputLength = inputBits; // parameter n
	seqLength = outputBits; // parameter k

	Verilog::GenerateTrallisP();
	Verilog::GenerateHammingDistance();
	Verilog::GenerateViterbiDecoder();
	Verilog::GenerateViterbiDecoderTester();
	
	Verilog::GenerateCorrect();

	Verilog::GenerateTrallisSTester();
	Verilog::GenerateQuadMinimum();
	Verilog::GenerateTrallisS();
	

	Verilog::GenerateMinimumIndex();
	
	Verilog::GenerateMatrixMultiply();
}


int main()
{
	int inputBits, outputBits, registersCount, parallelism;
	string test;
	cout << "Please Input start parameters for the Viterbi module:\n";
	cout << "int outputBits\n";
	cin >> outputBits;
	cout << "int inputBits\n";
	cin >> inputBits;
	cout << "int registersCount\n";
	cin >> registersCount;
	cout << "int parallelism\n";
	cin >> parallelism;

	viterbi_module viterbiTester = viterbi_module(outputBits, inputBits, registersCount, parallelism);
	GenerateVerilog(outputBits, inputBits, registersCount, parallelism);

	viterbiTester.PrintProperties();
	cout << "Please enter input in binary ONLY (010100010...):\n";
	cin >> test;
	char *input = (char*)test.c_str();
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	// Print Input
	ofstream f;
	f.open("input.txt");
	f << input;
	f.close();

	viterbiTester.Send(input);

	viterbiTester.PrintEncodedData();
	cout << "\n";

	viterbiTester.PrintBus();
	cout << "\n";

	viterbiTester.PrintAutomata();
	viterbiTester.PrintAutomataToFile(folder);
	cout << "\n";

	//viterbiTester.PrintInverseAutomata();
	//cout << "\n";

	viterbiTester.DecodeSequential();

	viterbiTester.DecodeParallel();

	// Don't close window right away
	std::cout << std::endl << "Press Enter key to Close...";
	getchar();
	getchar();

	return 0;
}