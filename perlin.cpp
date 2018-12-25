#include "perlin.h"
#include <math.h>
double Interpolate(double a, double b, double x) {
	double ft = x * 3.1415927;
	double f = (1 - cos(ft)) * .5;
	return a * (1 - f) + b * f;
}


double CPerlin::Noise(int x, int y)
{
	long n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double CPerlin::SmoothNoise(double x, double y)
{
	double corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	double center = Noise(x, y) / 4;
	return corners + sides + center;
}

double CPerlin::InterpolatedNoise(double x, double y)
{
	long integer_X = int(x);
	double fractional_X = x - integer_X;
	long integer_Y = int(y);
	double fractional_Y = y - integer_Y;
	double v1 = SmoothNoise(integer_X, integer_Y);
	double v2 = SmoothNoise(integer_X + 1, integer_Y);
	double v3 = SmoothNoise(integer_X, integer_Y + 1);
	double v4 = SmoothNoise(integer_X + 1, integer_Y + 1);
	double i1 = Interpolate(v1, v2, fractional_X);
	double i2 = Interpolate(v3, v4, fractional_X);
	return Interpolate(i1, i2, fractional_Y);
}



double CPerlin::Noise2D(double x, double y)
{
	double total = 0;
	double p = persistence;
	int n = Number_Of_Octaves - 1;
	//loop i from 0 to n
	for (int i = 0; i <= n; i++) {
		double frequency = pow(2, i);
		double amplitude = pow(p, i);
		total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
	}//end of i loop
	return total;
}

CPerlin::CPerlin()
{
}


CPerlin::~CPerlin()
{
}
