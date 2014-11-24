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
	static void GenerateViterbiDecoder(int parallelism);
	static void GenerateTrallisP();
	static void GenerateTrallisPTester();

	static void GenerateTrallisS();
	static void GenerateTrallisSTester();

	static void GenerateCorrect();
	static void GenerateHammingDistance();
	static void GenerateCorrectTester();

	static void GenerateMinimumIndex();
	static void GenerateMatrixMultiply();

	static string folder;
	static string quadMinimum;
	static string viterbiDecoderTester;
	static string viterbiDecoder;
	static string trallisP;
	static string trallisPTester;
	static string hammingDistance;
	static string correctTester;
	static string correct;
	static string trallisS;
	static string trallisSTester;
	static string minimumIndex;
	static string matrixMultiply;
	static bool isParallel;
	static int inputLength; //input length in bits.
	static int seqLength; //sequence length in bits.


private:

	static void Println(int num, ...);
	static void Println(string line);
	static void NewLine();
	static void Print(int num, ...);
	static void Print(string line);

	static void MultiplyMatrix(int m1_from, int m1_to, int m2);
	static void MultiplyProduct(int m1_from, int m1_to, int m2_from, int m2_to);
	static void CreateMultiplyOneByOne(int from, int to);
	static void CreateMultiplyByRange(int from, int to);
};




