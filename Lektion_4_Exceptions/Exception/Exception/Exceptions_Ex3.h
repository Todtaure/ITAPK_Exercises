#include <cinttypes>

/*
it appears that the method pollPressureValve expects the data not to be ready at some point,
thus throwing an exception seems to be overkill and thus an approach with reference parameter and
boolean return value seems better. We are assuming that the method belongs to a sensor whose value
is not guaranteed to be ready immediately.
*/

bool pollPressureValve(int16_t &port_data)
{
	if (PORT6IO_STATUS & DATA_RDY)
	{
		port_data = PORT6IO_DATA;
		return true;
	}
	else
		return false;
}

/* Using */
void f()
{
	int16_t value;
	bool result = false;
	while (!result)
	{
		result = pollPressureValve(value);
	}
	ctrlSpeed = process(value);
	writeToLog(ctrlSpeed, value);
}

/*
Snippet 2:
Assuming a sensor error is not something that happens very often, using exceptions seems okay, since you do not have
to check error codes every time the method is called.
*/