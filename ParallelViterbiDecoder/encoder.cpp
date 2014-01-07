/* 
 *	Authors:
 *	Shiran Stan-Meleh  ID: 039067608
 *	Gal Keret          ID: 066547969
 */
#include "encoder.h"

encoder::encoder(void)
{
}

encoder::~encoder(void)
{
}

encoder::encoder(int inputBits, int outputBits, int constrainLength, vector<uint32_t> xors)
{
	_inputBits = inputBits;
	_outputBits = outputBits;
	_constrainLength = constrainLength;
	_xors = xors;
	_memoryRegisters = 0;
}

vector<uint32_t> encoder::Encode(char *data)
{
	vector<uint32_t> encodedData;

	// Init pointer to iterate on input data
	char *dataPtr = data;

	// Move over char input with jumps of size inputbits
	for (uint16_t i = 0; i < strlen(data); i = i + _inputBits)
	{
		// Take the next Block to encode with size of InputBits
		char* block = new char[_inputBits]();
		strncpy_s(block, sizeof(block), dataPtr, _inputBits);
		// Shift Right the bits within the block
		for (int j = _inputBits - 1; j >= 0; j--) 
		{
			ShiftRight((block[j] == '1')? 1 : 0);
		}
		// Calculate the output using XORS * outputBits and Push the encoded result to encoded output
		encodedData.push_back(CalcOutput(_memoryRegisters));
		// Move to start of next Block
		dataPtr += _inputBits;
	}

	return encodedData;
}

void encoder::ShiftRight(bool input)
{
	// Each time shift right by 1 bit and put the next input bit on left (reading wise)
	// We always use bitset of 32 (assumed to be max of constrain length) but we only refer to constrain length
	_memoryRegisters = (_memoryRegisters >> 1) | (input << (_constrainLength - 1));
}

uint32_t encoder::CalcOutput(uint32_t input)
{
	// Init temp values for calculation
	bool currXOR;
	bool first = true;
	uint32_t output = 0;
	bitset<32> buffer = bitset<32>(input);

	// Perform XORS as many as outputBits
	for (int i = 0; i < _outputBits; i++)
	{
		// Get the current xor
		bitset<32> xor = bitset<32>(_xors[i]);
		currXOR = false;

		// Perform XOR between all bits in the buffer which has 1 in relative position on the xors vector
		for (int bit = 0; bit < _constrainLength; bit++)
		{
			if (xor[bit] == 1) 
				currXOR ^= buffer[bit];
		}

		// Put the result in output in consistent order (left to right)
		output = (output << 1) | ((currXOR)? 1 : 0);
	}
	return output;
}