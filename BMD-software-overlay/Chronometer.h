#include <chrono>
using namespace std;
using namespace std::chrono;

class Chronometer {
private:
	static high_resolution_clock::time_point t1;

public:
	static void start();
	static void stopAndPrint();
};