#include "utils.h"

#include "constants.h"

#include <time.h>

void utils_format_time(time_t t, char* stime)
{
	strftime(stime, TIME_SIZE, TIME_FMT, localtime(&t));
}

