// TestInterpolation.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <math.h>     

using namespace std;

float LinearInterpolation(unsigned int step, unsigned int steps)
{
	float val = step / (float)steps;
	return val;
}

double MorePreciseLinearInterpolation(unsigned int step, unsigned int steps)
{
	float val = step / (double)steps;
	return val;
}

float QuadraticInterpolation(unsigned int step, unsigned int steps)
{
	float val = step / (float)steps;
	return val * val;
}

float CubicInterpolation(unsigned int step, unsigned int steps)
{
	float val = step / (float)steps;
	return val * val * val;
}

float SinusoidalInterpolation(unsigned int step, unsigned int steps)
{
	float multiplyer = step / (float)steps;
	const float piO2 = 3.14159265 /2;	
	float res = sin(piO2*multiplyer);
	return res;
}


template <typename T, typename InterpolationFuncType>
class TemplatedInterpolator
{
private:
	T _start;
	T _end;
	unsigned int _steps;
	unsigned int _step{ 1 };
	InterpolationFuncType* _interpolationMethod;
public:
	explicit TemplatedInterpolator(T start, T end, unsigned int steps, InterpolationFuncType* func):
		_start(start),
		_end(end),
		_steps(steps),
		_interpolationMethod(func)
	{
	}

	unsigned int steps() const
	{
		return _steps;
	}

	T operator ()()
	{
		return interpolate(_start, _end, _interpolationMethod(_step, _steps));
	}

	template <typename T>
	T interpolate(T start, T end, double multiplyer)
	{
		++_step;
		return start + (end - start) * multiplyer;
	}

};

template<typename T, typename F>
TemplatedInterpolator<T, F> CreateInterpolator(const T& start, const T& end, unsigned int steps, F* func)
{
	return TemplatedInterpolator<T, F> { start, end, steps, func};
}

//TODO 1  Add iterator to intepolator
//TODO 2 Implement intepolator as statefull lambda (if possible)
//TODO 3 Implement interpolator with no floating point types

int main()
{
	uint64_t start = 1000;
	uint64_t end = 1234567890123;

	auto interpolator = CreateInterpolator(start, end, 10, &LinearInterpolation);

	cout << "Interpolator from " << start << " to " << end << "\n ";

	for (unsigned int step = 1; step <= interpolator.steps(); ++step)
	{
		cout << "step " << interpolator() << " \n";
	}

	char z;
	cin >> z;

	return 0;
}

