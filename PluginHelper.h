#pragma once

#include "stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

/*
	Checks if directory in path exists.
*/
int directoryExists(const char* path);

/*
	Starts wheel controller executable.
*/
void startWheelController();

