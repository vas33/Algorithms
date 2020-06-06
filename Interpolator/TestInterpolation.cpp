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
	auto val = step / (double)steps;
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

auto SinusoidalInterpolation(unsigned int step, unsigned int steps)
{
	float multiplyer = step / (float)steps;
	const double piO2 = 3.14159265 /2;	
	auto res = sin(piO2*multiplyer);
	return res;
}


template <typename T, typename InterpolationFuncType>
class TemplatedInterpolator
{
	template <typename T, typename InterpolationFuncType>
	class TemplatedInterpolatorIterator
	{
	public:
		using self_type = TemplatedInterpolatorIterator;
		using value_type = T;
		using reference = T&;
		using pointer = T*;

	private:
		T _start;
		T _end;
		T _value;
		unsigned int _steps;
		unsigned int _step{ 1 };
		InterpolationFuncType* _interpolationMethod;

		void CalculateValue()
		{
			double multiplyer = _interpolationMethod(_step, _steps);

			_value = interpolate(_start, _end, multiplyer);
		}

	public:
		explicit TemplatedInterpolatorIterator(T start, T end, unsigned int steps, unsigned int step, InterpolationFuncType* func) :
			_start(start),
			_end(end),
			_steps(steps),
			_step(step),
			_interpolationMethod(func)
		{
			CalculateValue();
		}

		bool operator==(self_type& other)
		{
			bool result = this->_start == other._start &&
				this->_end == other._end &&
				this->_step == other._step &&
				this->_steps == other._steps;
			return result;
		}

		bool operator !=(self_type& other)
		{
			return !operator==(other);
		}

		self_type& operator++()
		{
			if (_step < _steps)
			{
				//throw std out of range
			}
			++_step;

			CalculateValue();

			return *this;
		}

		const value_type& operator*() const
		{
			return _value;
		}
		
		const value_type& operator->() const
		{
			return _value;
		}
	};

private:
	T _start;
	T _end;
	unsigned int _steps;
	unsigned int _step{ 1 };
	InterpolationFuncType* _interpolationMethod;
	
	template<typename T>
	static T interpolate(T start, T end, double multiplyer)
	{
		return 	start + (end - start) * multiplyer;
	}
public:
	explicit TemplatedInterpolator(T start, T end, unsigned int steps, InterpolationFuncType* func):
		_start(start),
		_end(end),
		_steps(steps),
		_interpolationMethod(func)
	{
	}

	TemplatedInterpolatorIterator<T, InterpolationFuncType> begin()
	{
		return  TemplatedInterpolatorIterator<T, InterpolationFuncType>{_start, _end, _steps, 1, _interpolationMethod};
	}

	TemplatedInterpolatorIterator<T, InterpolationFuncType> end()
	{
		return  TemplatedInterpolatorIterator<T, InterpolationFuncType>{_start, _end, _steps, _steps+1, _interpolationMethod};
	}
};

template<typename T, typename F>
TemplatedInterpolator<T, F> CreateInterpolator(const T& start, const T& end, unsigned int steps, F* func)
{
	return TemplatedInterpolator<T, F> { start, end, steps, func};
}




//TODO 1  Add iterator to intepolator - DONE
//TODO 2 Implement intepolator as statefull lambda (if possible) - DONE
//TODO 3 Implement interpolator with no floating point types

int main()
{
	uint64_t start = 1000;
	uint64_t end = 1234567890123;

	//interpolator object
	{
		auto interpolator = CreateInterpolator(start, end, 10, &LinearInterpolation);

		cout << "\n\nInterpolator with iterator" << start << " to " << end << "\n ";

		for (const auto& val : interpolator)
		{
			cout << "step " << val << " \n";
		}
	}


	
	//interpolator lambda
	{
		auto interpolatorLambda = [start, end, _step = 1, _steps = 10, intepolationMethod = &LinearInterpolation]() mutable
			->decltype(start)
		{
			double multiplyer = intepolationMethod(_step, _steps);
			++_step;
			return start + (end - start) * multiplyer;
		};

		cout << "\n\nInterpolator lambda" << start << " to " << end << "\n ";

		for (unsigned int step = 1; step <= 10; ++step)
		{
			cout << "step " << interpolatorLambda() << " \n";
		}
	}
		
	char z;
	cin >> z;

	return 0;
}

