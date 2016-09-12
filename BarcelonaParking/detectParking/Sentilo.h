#pragma once

#include "stdafx.h"

#include <curl/curl.h>

#include "Parking.h"

class Sentilo
{
private:
	void UnregisterFromSentilo();
	void RegisterToSentilo();
public:
	Sentilo();
	~Sentilo();
	void Init();
	void SendData(float data);
};