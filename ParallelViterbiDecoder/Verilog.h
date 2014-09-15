#pragma once
#include <string>
#include <fstream>
#include <math.h>
using namespace std;
class Verilog
{
public:
	Verilog(void);
	~Verilog(void);
	static void GenerateQuadMinimum();
	static void GenerateViterbiDecoderTester();
	static void GenerateViterbiDecoder();
	static void GenerateTrallisP();
	static void GenerateTrallisPTester();

	static void GenerateTrallisS();
	static void GenerateTrallisSTester();

	static void GenerateCorrect();
	static void GenerateHammingDistance();
	static void GenerateCorrectTester();

	static void GenerateMinimumIndex();
	static void GenerateMatrixMultiply();

private:
	static const string quadMinimum;
	static const string viterbiDecoderTester;
	static const string viterbiDecoder;
	static const string trallisP;
	static const string trallisPTester;
	static const string correct;
	static const string correctTester;
	static const string hammingDistance;
	static const string trallisS;
	static const string trallisSTester;
	static const string minimumIndex;
	static const string matrixMultiply;

	static void Println(int num, ...);
	static void Println(string line);
	static void NewLine();
	static void Print(int num, ...);
	static void Print(string line);
};

static string folder = "C:\\test\\";
static bool isParallel = true;
static int inputLength = 6; //input length in bits.
static int seqLength = 2; //sequence length in bits.