#pragma once

#include "helper_functions.h"

class decoder
{
public:
	decoder(void);
	~decoder(void);

	void DecodeSequential();
	void DecodeParallel();
};

