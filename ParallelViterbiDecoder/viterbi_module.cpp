#include "viterbi_module.h"

viterbi_module::viterbi_module(int outputBits, int inputBits, int registersCount, int parallelism)
{
	_outputBits = outputBits;
	_inputBits = inputBits;
	_registersCount = registersCount;
	_parallelism = parallelism;

	// calculate constrain length
	_constrainLength = _inputBits * _registersCount;

	if (_constrainLength > 32) 
	{
		cout << "ERROR: Constrain Length is limited to 32!\n";
		exit(1);
	}

	// initilize polynomials with random sets of bits to xor
	_xors.resize(_outputBits);
	srand(time(NULL));
	for (int i = 0; i < _outputBits; i++)
	{
		uint32_t numOfBits = pow(2, _constrainLength);
		_xors[i] = rand() % numOfBits;

		if (_xors[i] == 0) 
		{
			cout << "ERROR: Random XOR is 0!\n";
		}
	}

	_encoder = encoder(_inputBits, _outputBits, _constrainLength, _xors);
	_automata = automata(_inputBits, _outputBits, _constrainLength, _xors);

	// Create the State Machine to be used later in decode
	_automata.GenerateAutomata();

	_decoder = decoder(_inputBits, _constrainLength, _automata.getAutomata());
}


viterbi_module::~viterbi_module(void)
{
}

void viterbi_module::Send(char* data)
{
	// First encode the data
	vector<uint32_t> encodedData;
	encodedData = _encoder.Encode(data);

	// Print out the encoded data in blocks (each for one input symbol)
	cout << "Encoded Data:\n";
	for (uint16_t i = 0; i < encodedData.size(); i++)
	{
		PrintBitSet(encodedData[i], _constrainLength);
	}
	cout << "\n\n";

	// Filp some bits to simulate bus interference
	Scramble(encodedData);
}

void viterbi_module::Scramble(vector<uint32_t> encodedData)
{
	_bus = encodedData;
	srand(time(NULL));
	for (int i = 0; i < _outputBits; i = i + rand() % _bus.size())
	{
		bitset<32> block = bitset<32>(_bus[i]);
		block[rand() % _outputBits].flip();
		_bus[i] = block.to_ullong();
	}
}

void viterbi_module::Decode()
{
	if (_parallelism == 0)
		_decoder.DecodeSequential(_bus);
	else if (_parallelism > 0)
		_decoder.DecodeParallel();
}

void viterbi_module::PrintProperties()
{
	cout << "Viterbi Module Properties:\n";
	cout << "Input Bits: " << _inputBits << "\n";
	cout << "Output Bits: " << _outputBits << "\n";
	cout << "Registers Count: " << _registersCount << "\n";
	cout << "Constrain Length: " << _constrainLength << "\n";
	PrintXORS();
	cout << "\n";
}

void viterbi_module::PrintBus()
{
	cout << "Bus Data:\n";
	for (uint16_t i = 0; i < _bus.size(); i++)
	{
		PrintBitSet(_bus[i], _constrainLength);
	}
	cout << "\n";
}

void viterbi_module::PrintXORS()
{
	cout << "XORS:\n";
	for (int i = 0; i < _xors.size(); i++)
	{
		PrintBitSet(_xors[i], _constrainLength);
		cout << "\n";
	}
}

void viterbi_module::PrintAutomata()
{
	_automata.PrintAutomata();
}
