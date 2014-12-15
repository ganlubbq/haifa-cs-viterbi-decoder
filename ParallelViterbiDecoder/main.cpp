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
	if (parallelism > 1) Verilog::isParallel = true;
	else Verilog::isParallel = false;
	
	Verilog::inputLength = inputBits; // parameter n
	Verilog::seqLength = outputBits; // parameter k

	Verilog::quadMinimum = Verilog::folder + "quadMin.v";
	Verilog::viterbiDecoderTester = Verilog::folder + "viterbiDecoderTester.v";
	Verilog::viterbiDecoder = Verilog::folder + "viterbiDecoder.v";
	Verilog::trallisP = Verilog::folder + "Trallies.v";
	Verilog::trallisPTester = Verilog::folder + "TrallisTester.v";
	Verilog::hammingDistance = Verilog::folder + "HammingDistance.v";
	Verilog::correctTester = Verilog::folder + "CorrectionTester.v";
	Verilog::correct = Verilog::folder + "Correct.v";
	Verilog::trallisS = Verilog::folder + "TralliesS.v";
	Verilog::trallisSTester = Verilog::folder + "TralliesSTester.v";
	Verilog::minimumIndex = Verilog::folder + "MinimumIndex.v";
	Verilog::matrixMultiply = Verilog::folder + "matrixMultiply.v";

	Verilog::GenerateTrallisP();
	Verilog::GenerateHammingDistance();
	Verilog::GenerateViterbiDecoder(parallelism);
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

	int inputBits, outputBits, registersCount, parallelism, inputLength , sequenceLength = 2;
	string test;
	cout << "please enter verilog files output folder:\n";
	cin >> Verilog::folder;

	//Verilog::folder = "C:\\Users\\galk\\Desktop\\ViterbiProject\\";

	cout << "Please Input start parameters for the Viterbi module:\n";
	cout << "int outputBits\n";
	cin >> outputBits;
	cout << "int inputBits\n";
	cin >> inputBits;
	cout << "int registersCount\n";
	cin >> registersCount;
	cout << "int parallelism\n";
	cin >> parallelism;
	cout << "\n";

	viterbi_module viterbiTester = viterbi_module(outputBits, inputBits, registersCount, parallelism);

	viterbiTester.PrintProperties();
	cout << "Please enter input in binary ONLY (010100010...):\n";
	cin >> test;
	char *input = (char*)test.c_str();
	cout << "Input Data:\n" << input << "\n";
	cout << "\n";

	// Create the verilog files
	cout << "Please insert length of input to decode in verilog (>6)\n";
	cin >> inputLength;
	cout << "Please insert length of sequence length\n";
	cin >> sequenceLength;

	cout << "Generating Verilog Files...";
	GenerateVerilog(sequenceLength, inputLength, registersCount, parallelism);
	cout << "Done\n\n";

	cout << "****The following output is pure c++ implementation, verliog files are generated to " << Verilog::folder << "****\n\n";

	viterbiTester.Send(input);

	viterbiTester.PrintEncodedData();
	cout << "\n";

	viterbiTester.PrintBus();
	cout << "\n";

	viterbiTester.PrintAutomata();
	viterbiTester.PrintAutomataToFile(Verilog::folder);
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