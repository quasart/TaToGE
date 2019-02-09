#pragma once

#if QT_VERSION >= 0x050a00
	#include <QRandomGenerator>
	inline int getRandomSeed()
	{
		return QRandomGenerator::global()->generate();
	}
#else
	inline int getRandomSeed()
	{
		return std::random_device{}();
	}
#endif

