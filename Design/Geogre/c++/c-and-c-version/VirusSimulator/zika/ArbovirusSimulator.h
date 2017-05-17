#pragma once

class ArbovirusSimulator
{
public:
	static vector& vSum(const vector& u, const vector& v, vector& sumOUT);
	static vector vSum(const vector& u, const vector& v);
	// modifies first arg ie "to u add v"
	static void vAdd(vector& u, const vector& v);
	// modifies first arg ie "from u subtract v"
	static void vSub(vector& u, const vector& v);
	static std::string csvLine(const vector& parts);
	static std::string ArbovirusSimulator::csvLine(const list& parts);
};

