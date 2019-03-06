#include "Logger.h"
#include <stdio.h>

void Logger::Print(const char * message)
{
	printf(message);
	printf("\n");
}