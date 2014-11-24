/* 
 *	Authors:
 *	Shiran Stan-Meleh  ID: 039067608
 *	Gal Keret          ID: 066547969
 */
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

void PrintBitSetToFile(bitset<32> data, int count, ofstream *file)
{
	// Print bitset from left to right (for clearer reading)- only the actual part used
	for (int i = count - 1; i >= 0; i--)
	{
		char bit = (data[i])? '1' : '0';
		*file << bit;
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

uint32_t VectorMin(vector<uint32_t> vect)
{
	uint32_t min = vect[0];
	uint32_t state = 0;
	for (uint32_t i = 1; i < vect.size(); i++)
	{
		if (vect[i] < min)
		{
			min = vect[i];
			state = i;
		}
	}
	return state;
}

uint32_t min(uint32_t num1, uint32_t num2)
{
	if (num1 < num2)
		return num1;
	return num2;
}

vector<vector<uint32_t>> MultiplyMetrics(vector<vector<uint32_t>> metA, vector<vector<uint32_t>> metB)
{
	// Init the result metric and the temp minium used un calculations
	vector<vector<uint32_t>> metC;
	uint32_t minimum;

	// Make sure matric size comply
	if (metA.size() != metB.size())
	{
		cout << "Metrics Size not match!\n";
		exit(1);
	}

	for (uint32_t i = 0; i < metA.size(); i++)
	{
		vector<uint32_t> cols;
		for (uint32_t j = 0; j < metB.size(); j++)
		{
			minimum = 0xFFFFFFFF;
			for (uint32_t k = 0; k < metA.size(); k++)
			{
				// avoid overflow
				if (metA[i][k] == 0xFFFFFFFF || metB[k][j] == 0xFFFFFFFF)
					minimum = min(minimum, 0xFFFFFFFF);
				else
					minimum = min(minimum, metA[i][k] + metB[k][j]);
			}
			cols.push_back(minimum);
		}
		metC.push_back(cols);
	}
	return metC;
}



void printAutoamta();