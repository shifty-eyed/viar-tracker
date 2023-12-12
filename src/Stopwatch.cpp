#include "Stopwatch.h"

void Stopwatch::start(string name)
{
    metrics[name] = chrono::system_clock::now();
}

long Stopwatch::stop(string name)
{
    long duration = measure(name);
    if (duration >= 0) {
        result += name + ":" + to_string(duration) + " ";
    }
    return duration;
}

long Stopwatch::measure(string name)
{
    if (metrics.find(name) == metrics.end()) {
        return -1;
    }
    long duration = (long)chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - metrics[name]).count();
    metrics.erase(name);
    return duration;
}

string Stopwatch::toString()
{
    auto r = result;
    result = "";
    return r;
}
