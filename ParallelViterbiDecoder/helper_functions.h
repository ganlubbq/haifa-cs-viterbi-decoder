/* 
*	Authors:
*	Shiran Stan-Meleh  ID: 039067608
*	Gal Keret          ID: 066547969
*/
#pragma once
#include <stdlib.h>		  // srand, rand
#include <iostream>       // cout
#include <thread>         // uint32_t
#include <bitset>         // bitset
#include <vector>		  // vector
#include <map>			  // map
#include <string>		  // string
#include <iostream>		  // file
#include <fstream>
#include <mutex>

using namespace std;

struct AutomataValue {
	uint32_t state;
	uint32_t output;
}typedef state;

struct TrellisValue {
	uint32_t state;
	uint32_t hammingDist;
}typedef trellis;

void PrintBitSet(bitset<32> data, int count);
void PrintBitSetToFile(bitset<32> data, int count, ofstream *file);
int CalcHammingDist(uint32_t num1, uint32_t num2);
uint32_t VectorMin(vector<uint32_t> vect);
vector<vector<uint32_t>> MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB);