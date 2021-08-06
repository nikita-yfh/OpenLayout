#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>

using namespace std;

float to_mil(float v);
float to_mm(float v);

template<typename T>
static string to_str(T value) {
    stringstream s;
    s<<value;
    return s.str();
}


#endif
