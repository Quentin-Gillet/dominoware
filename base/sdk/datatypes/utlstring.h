#pragma once

#include "utlmap.h";

struct CUtlString
{
	UtlMemory<char> pBuffer;
	int iLength;

	const char* Data() const noexcept { return pBuffer.memory; }
};