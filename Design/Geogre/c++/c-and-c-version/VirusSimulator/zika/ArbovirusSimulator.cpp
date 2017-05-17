#include "stdafx.h"
#include "ArbovirusSimulator.h"

vector& ArbovirusSimulator::vSum(const vector& u, const vector& v, vector& sumOUT)
{
	assert(u.size() == v.size());
	sumOUT.resize(u.size());
	for (size_t i = 0; i < u.size(); i++)
		sumOUT[i] = u[i] + v[i];
	return sumOUT;
}

vector ArbovirusSimulator::vSum(const vector& u, const vector& v)
{
	vector sum;
	return vSum(u, v, sum);
}

void ArbovirusSimulator::vAdd(vector& u, const vector& v)
{
	assert(u.size() == v.size());
	for (size_t i = 0; i < u.size(); i++)
		u[i] += v[i];
}

void ArbovirusSimulator::vSub(vector& u, const vector& v)
{
	assert(u.size() == v.size());
	for (size_t i = 0; i < u.size(); i++)
		u[i] -= v[i];
}

std::string ArbovirusSimulator::csvLine(const vector& parts)
{
	std::ostringstream oss;
	for (size_t i = 0; i < parts.size(); i++)
	{
		oss << parts[i];
		if (i != parts.size() - 1)
			oss << ",";
	}
	return oss.str();
}

std::string ArbovirusSimulator::csvLine(const list& parts)
{
	std::ostringstream oss;
	for each (auto i in parts)
	{
		oss << i;
		if (i != parts.back())
			oss << ",";
	}
	return oss.str();
}