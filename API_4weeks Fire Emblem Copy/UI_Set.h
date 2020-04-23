#pragma once
#include "objects.h"
class UI_Set :
	public objects
{
protected:
	bool isCommand;
	bool isTagUnit;
public:
	UI_Set();
	~UI_Set();
};

