// Implicit casts from system headers should not be counted

#include <cmath>
#include <cstdio>

double compute(int x) {
    return sqrt(x);
}