#include "utils.h"

#include "constants.h"

#include <time.h>

void utils_get_current_time(char* stime)
{
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(stime, sizeof(TIME_FMT), TIME_FMT, timeinfo);
}

