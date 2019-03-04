#include "Chronometer.h"
#include "settings.h"
#include <iostream>

using namespace std;

high_resolution_clock::time_point Chronometer::t1;

void Chronometer::start()
{
#if EN_CHRONO
	t1 = high_resolution_clock::now();
#endif // EN_CHRONO
}

void Chronometer::stopAndPrint()
{
#if EN_CHRONO
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();

	cout << duration << endl;
#endif
}
