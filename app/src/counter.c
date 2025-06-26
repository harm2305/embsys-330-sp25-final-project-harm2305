/**
 * Simple module for incrementing a global counter.
 * Primarily to demonstrate Ztest
 */

#include "counter.h"

static volatile uint32_t counter;

void counter_reset(void)
{
	counter = 0;
}

void counter_increment(void)
{
	counter++;
}

uint32_t counter_value(void)
{
	return counter;
}
