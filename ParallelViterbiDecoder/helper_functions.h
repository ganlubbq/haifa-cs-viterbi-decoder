#pragma once
#include <stdlib.h>		  // srand, rand
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <bitset>         // std::bitset
#include <vector>		  // std::vector
#include <map>			  // std:map
#include <string>		  // std:string

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
int CalcHammingDist(uint32_t num1, uint32_t num2);
