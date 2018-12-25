#pragma once



class CPerlin
{
public:
	double persistence;
	double Number_Of_Octaves;
	double Noise(int x, int y);
	double SmoothNoise(double x, double y);
	double InterpolatedNoise(double x, double y);
	double Noise2D(double x, double y);
	CPerlin();
	~CPerlin();
};

