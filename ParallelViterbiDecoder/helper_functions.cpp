#include "helper_functions.h"

void PrintBitSet(bitset<32> data, int count)
{
	// Print bitset from left to right (for clearer reading)- only the actual part used
	for (int i = count - 1; i >= 0; i--)
	{
		char bit = (data[i])? '1' : '0';
		cout << bit;
	}
}

int CalcHammingDist(uint32_t num1, uint32_t num2)
{
	bitset<32> set1 = bitset<32>(num1);
	bitset<32> set2 = bitset<32>(num2);
	int result = 0;
	for (int i = 0; i < 32; i++)
	{
		if (set1[i] != set2[i]) result += 1;
	}
	return result;
}
