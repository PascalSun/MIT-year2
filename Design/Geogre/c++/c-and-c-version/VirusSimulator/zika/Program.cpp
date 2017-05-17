// ZikaModel.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Config.h"
#include "ArbovirusSimulator.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <random>

// for convenience
using json = nlohmann::json;
using namespace std;

int main()
{
	//LoadConfig();
	ifstream fileIn("test1.params.json");
	if (!fileIn.is_open()) return -1;

	json params;
	fileIn >> params;
	for (json::iterator it = params.begin(); it != params.end(); ++it) {
		cout << it.key() << " : " << it.value() << endl;

	}


	string hostsFile = params["hosts-file"];
	string locationFile = params["locations-file"];
	//unsigned long duration = params["duration"];
	double bitingRate = params["biting-rate"];
	double eToIRate = params["vector-e-to-i-rate"];
	double pTrans = 1.0 - exp(-eToIRate);

	double heToIRate = params["host-e-to-i-rate"];
	double iToRRate = params["host-i-to-r-rate"];
	double pITrans = 1.0 - exp(-heToIRate);
	double pRTrans = 1.0 - exp(-iToRRate);

	double diffProb = params["vector-diffusion-probability"];

	double biteRate = params["biting-rate"];
	double vecInfectProb = params["vector-infection-probability"];
	double hostInfectProb = params["host-infection-probability"];

	double vBirthRate = params["vector-birth-rate"];
	double mr = params["vector-maturation-rate"];
	double dr = params["vector-death-rate"];
	double probDeath = 1.0 - exp(-dr);
	double probMaturation = 1.0 - exp(-mr);
	fileIn.close();
	params.clear();

	ifstream fileIn2(hostsFile);
	if (!fileIn2.is_open()) return -1;
	fileIn2 >> params;
	g_hosts.clear();
	for (json::iterator it = params.begin(); it != params.end(); ++it)
	{
		auto item = std::shared_ptr<HostItem>(new HostItem());
		item->iid = stoi(it.key());
		item->location = it.value()["location"];
		item->i_state = it.value()["i-state"].dump().substr(1, 1);
		item->home = it.value()["home"];
		item->hub = it.value()["hub"];
		g_hosts[item->iid] = item;
		//cout << it.key() << " : " << it.value() << endl;
		//cout << it.value()["location"] << endl;
		//cout << item->i_state << endl;
	}

	fileIn2.close();
	params.clear();
	ifstream fileIn3(locationFile);
	fileIn3 >> params;
	g_locations.clear();
	for (json::iterator it = params.begin(); it != params.end(); ++it)
	{
		auto item = std::shared_ptr<LocationItem>(new LocationItem());
		item->cid = stoi(it.key());
		for each (auto i in it.value()["coordinates"])
		{
			item->coordinates.push_back(i);
		}
		for each (auto i in it.value()["neighbours"])
		{
			item->neighbours.push_back(i);
		}
		item->capacity = it.value()["v-capacity"];
		item->immature = it.value()["v-immature"];
		for each (auto i in it.value()["v-state"])
		{
			item->v_state.push_back(i);
		}
		item->hosts.push_back(item->cid);
		g_locations[item->cid] = item;
	}
	fileIn3.close();
	//for each (auto i in g_locations)
	//	std::cout << i.second->ToString() << std::endl;
	//std::cout << std::endl;
	//for each (auto i in g_hosts)
	//	std::cout << i.second->ToString() << std::endl;
	const clock_t begin_time = clock();
	unsigned long duration = 100;
	bool isDaytime = true;
	for (unsigned long round = 0; round < duration; ++round)
	{
		InfectionInVectors(g_locations, pTrans);
		InfectionInHosts(g_hosts, pITrans, pRTrans);
		RegularHumanMovement(isDaytime, g_hosts, g_locations);
		if (isDaytime)
			VectorMovement(g_locations, diffProb);
		VectorInfection(g_hosts, g_locations, biteRate, vecInfectProb);
		HostInfection(g_hosts, g_locations, biteRate, hostInfectProb, round);
		VectorPopulation(g_locations, vBirthRate, probDeath, probMaturation);
		isDaytime ^= true;
		//for each (auto i in g_locations)
		//	std::cout << i.second->ToString() << std::endl;
		//std::cout << std::endl;
		//for each (auto i in g_hosts)
		//	std::cout << i.second->ToString() << std::endl;
	}
	std::cout << "Total run time is " << float(clock() - begin_time) / CLOCKS_PER_SEC << " seconds" << endl;
	return 0;
}

