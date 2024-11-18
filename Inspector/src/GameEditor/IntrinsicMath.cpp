// https://github.com/pierreguilmin/3D-engine . Not  written by me.

#include "IntrinsicMath.h"

#include <iostream>
#include <iomanip> // for std::setprecision and std::setw

#ifndef M_PI
#  define M_PI (3.14159265358979323846)
#endif

#define CAMERA_ROTATION_SENSIBILITY    0.25
#define CAMERA_TRANSLATION_SENSIBILITY 4

#define PROJECTION_FACTOR    1024.0
#define PROJECTION_MAX_DEPTH 800

// return the square number of x
double square(const double x) {
	return x * x;
}

// map linearly x from [a, b] to [c, d], if x < a ? c, if x > b ? d
double map(double x, const double a, const double b, const double c, const double d) {
	if (x < a)
		x = a;
	else if (x > b)
		x = b;

	return (x - a) / (b - a) * (d - c) + c;
}

// convert theta in degrees to radian
double as_radians(const double theta) {
	return theta / 180.0 * M_PI;
}

// convert theta in radian to degrees
double as_degrees(const double theta) {
	return theta / M_PI * 180.0;
}

// returns a random integer between [a, b[
int rand(const int a, const int b) {
	return rand() % (b - a) + a;
}

// returns a random double between [a, b[
double rand(const double a, const double b) {
	return rand() / static_cast<double>(RAND_MAX) * (b - a) + a;
}