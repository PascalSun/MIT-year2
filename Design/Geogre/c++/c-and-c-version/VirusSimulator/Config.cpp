#include "stdafx.h"
#include "Config.h"
#include "ArbovirusSimulator.h"


std::string LocationItem::ToString()
{
	std::ostringstream oss;
	oss << "{ \"coordinates\" : [" << ArbovirusSimulator::csvLine(this->coordinates) << "]," << std::endl
		<< "  \"neighbours\"  : [" << ArbovirusSimulator::csvLine(this->neighbours) << "]," << std::endl
		<< "  \"hosts\"       : [" << ArbovirusSimulator::csvLine(this->hosts) << "]," << std::endl
		<< "  \"v-capacity\"  : " << this->capacity << "," << std::endl
		<< "  \"v-immature\"  : " << this->immature << "," << std::endl
		<< "  \"v-state\"     : [" << ArbovirusSimulator::csvLine(this->v_state) << "] }";
	return oss.str();
}

std::string HostItem::ToString()
{
	std::ostringstream oss;
	oss << "{ \"location\" : " << this->location 
		<< ", \"i-state\" : \"" << this->i_state 
		<< "\", \"home\" : " << this->home 
		<< ", \"hub\" : " << this->hub << " }";
	return oss.str();
}

////////////////////////////////////////////////////////////////////////////
map_locations g_locations;
map_hosts g_hosts;
/// <summary>
/// Susceptible
/// </summary>
const int S = 0;
/// <summary>
/// Exposed / Latent
/// </summary>
const int E = 1;
/// <summary>
/// Infectious
/// </summary>
const int I = 2;
/// <summary>
/// Recovery
/// </summary>
const int R = 3;

void LoadConfig()
{
	/*
	g_locations.clear();
	{
		auto item = std::shared_ptr<LocationItem>(new LocationItem());
		item->coordinates.push_back(100); item->coordinates.push_back(100);
		item->neighbours.push_back(1); item->neighbours.push_back(2);
		item->capacity = 20;
		item->state.push_back(20); item->state.push_back(0); item->state.push_back(0); item->state.push_back(0);
		g_locations.push_back(item);
	}
	g_hosts.clear();
	{
		auto item = std::shared_ptr<HostItem>(new HostItem());
		item->location = 0;		item->state = "I";		item->home = 0;		item->hub = -1;
		g_hosts.push_back(item);
	}
	*/
}

void InfectionInVectors(map_locations& locations, double pTrans)
{
	for each (auto i in locations)
	{
		auto c = i.second;
		auto ne = c->v_state[E];
		if (ne == 0)
			continue;
		auto nTrans = binSamp(ne, pTrans);
		c->v_state[E] -= nTrans;
		c->v_state[I] += nTrans;
	}
}

void InfectionInHosts(map_hosts& hosts, double pITrans, double pRTrans)
{
	for each (auto i in hosts)
	{
		auto h = i.second;
		auto s = h->i_state;
		if (s == "E")
		{
			if (binSamp(1, pITrans) != 0)
				h->i_state = "I";
		}
		else if (s == "I")
		{
			if (binSamp(1, pRTrans) != 0)
				h->i_state = "R";
		}
	}
}

void RegularHumanMovement(bool isDaytime, map_hosts& hosts, map_locations& locations)
{
	if (isDaytime)
	{
		for each (auto i in hosts)
		{
			auto hub = i.second->hub;
			if (hub >= 0)
				moveHost(locations, *(i.second), hub);
		}
	}
	else
	{
		for each (auto i in hosts)
			moveHost(locations, *(i.second), i.second->home);
	}
}

void moveHost(map_locations& locations, HostItem& h, int dst)
{
	auto idd = h.iid;
	auto oldLoc = h.location;
	h.location = dst;
	locations[oldLoc]->hosts.remove(idd);
	locations[dst]->hosts.push_back(idd);
}

void VectorMovement(map_locations& locations, double diffProb)
{
	std::map<int, vector> incoming;
	for each(auto i in locations)
	{
		auto c = i.second;
		auto movers = VectorMovementFrom(*c, diffProb);
		ArbovirusSimulator::vSub(i.second->v_state, movers);

		auto dMovers = distributeVectors(*c, movers);
		for each(auto m in dMovers)
		{
			auto nb = m.first;
			if (incoming.find(nb) == incoming.end())
			{
				incoming.insert(std::make_pair(nb, vector(4, 0.0)));
			}
			ArbovirusSimulator::vAdd(incoming[nb], dMovers[nb]);
		}
	}

	for each(auto i in incoming)
	{
		ArbovirusSimulator::vAdd(locations[i.first]->v_state, i.second);
	}
}

vector VectorMovementFrom(LocationItem& c, double diffProb)
{
	vector v;
	for each (auto n in c.v_state)
		v.push_back(binSamp(n, diffProb));
	return v;
}

std::map<int, vector> distributeVectors(LocationItem& c, vector movers)
{
	auto nbs = c.neighbours;
	auto nnbs = nbs.size();
	std::map<int, vector> dMovers;
	for (size_t i = 0; i < nnbs; i++)
	{
		auto n = static_cast<int>(nbs[i]);
		dMovers.insert(std::make_pair(n, vector(4, 0.0)));
	}
	for (size_t vs = S; vs <= I; vs++)
	{
		for (size_t n = 0; n < movers[vs]; n++)
		{
			auto nbt = randint(0, nnbs);
			auto index = static_cast<int>(nbs[nbt]);
			dMovers[index][vs] += 1.0;
		}
	}
	return dMovers;
}
void VectorInfection(map_hosts& hosts, map_locations& locations, double biteRate, double vecInfectProb)
{
	for each (auto i in locations)
	{
		auto c = i.second;
		auto ns = c->v_state[S];
		if (ns == 0)
			continue;
		auto npop = 0;
		auto ipop = 0;
		for each (auto idd in c->hosts)
		{
			npop++;
			if (hosts[idd]->i_state == "I")
				ipop++;
		}
		if (ipop == 0)
			continue;
		auto beta = (double)ipop / (double)npop * biteRate * vecInfectProb;
		auto p = 1.0 - exp(-beta);
		int newInfctions = binSamp(ns, p);
		c->v_state[S] -= newInfctions;
		c->v_state[E] += newInfctions;
	}
}

const vector infectionEvent(HostItem& h, LocationItem& c, unsigned long round)
{

	vector event({ (double)round, 'i', (double)h.iid, (double)c.cid });
	return event;
}

void HostInfection(map_hosts& hosts, map_locations& location, double biteRate, double hostInfectProb, unsigned long round)
{
	for each (auto i in location)
	{
		auto c = i.second;
		auto nvi = c->v_state[I];
		if (nvi == 0)
			continue;
		auto hpop = 0;
		auto hs = 0;
		for each (auto idd in c->hosts)
		{
			if (hosts[idd]->i_state == "S")
				hs++;
			hpop++;
		}
		if (hs == 0)
			continue;
		auto nv = vSum(c->v_state);
		auto beta = biteRate * hostInfectProb * (double)nvi / (double)hpop;
		auto p = 1.0 - exp(-beta);
		for each (auto idd in c->hosts)
		{
			auto h = hosts[idd];
			if (h->i_state != "S")
				continue;
			if (binSamp(1, p) != 0)
			{
				h->i_state = "E";
				std::cout << ArbovirusSimulator::csvLine(infectionEvent(*h, *c, round)) << std::endl;
			}
		}
	}
}

void VectorPopulation(map_locations& location, double probDeath, double vBirthRate, double probMaturation)
{
	double vPop = 0;
	vector nDeaths;
	for each (auto& i in location)
	{
		auto& c = i.second;
		auto vs = c->v_state;
		auto cc = c->capacity;
		nDeaths.clear();
		for each (auto n in vs)
			nDeaths.push_back(binSamp(n, probDeath));

		vPop = vSum(vs);
		auto br = vPop * vBirthRate * std::max(0.0, double(1.0 - vPop / cc));
		auto nBirths = br > 0 ? poisson(br) : 0;
		auto nMats = binSamp(c->immature, probMaturation);

		ArbovirusSimulator::vSub(vs, nDeaths);
		c->immature += nBirths;
		c->immature -= nMats;
		vs[S] += nMats;
	}
}

#pragma region random

static std::default_random_engine g_randomGenerator;
extern int binomial(int n, double p)
{
	std::binomial_distribution<int> distribution(n, p);
	return distribution(g_randomGenerator);
}

extern int poisson(double lambda)
{
	if (lambda <= 0)
		return 0;
	std::poisson_distribution<int> distribution(lambda);
	auto n = distribution(g_randomGenerator);
	return n;
}

extern int binSamp(double n, double p)
{
	return binSamp(int(n), p);
}

extern int binSamp(int n, double p)
{
	if (n == 0)
		return 0;
	return binomial(n, p);
}
#pragma endregion

#pragma region tool function
extern double vSum(const vector& v)
{
	double sum = 0;
	for each (auto n in v)
		sum += n;
	return sum;
}

extern int vSum(const list& v)
{
	int sum = 0;
	for each (auto n in v)
		sum += n;
	return sum;
}

vector emptyVState()
{
	return vector({ 0.0, 0.0, 0.0, 0.0 });
}

// generate pseudo-random numbers in interval [min, max)
int randint(int min, int max)
{
	if (min > max)
		std::swap(min, max);
	return rand() % (max - min) + min;
}
#pragma endregion
