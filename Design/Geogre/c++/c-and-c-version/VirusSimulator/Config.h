#pragma once

struct LocationItem
{
	int cid;
	vector coordinates;
	vector neighbours;
	list hosts;
	double capacity;
	double immature;
	vector v_state;

	std::string ToString();
};

struct HostItem
{
	int iid;
	int location;
	std::string i_state;
	int home;
	int hub;

	std::string ToString();
};

typedef std::map<int, std::shared_ptr<LocationItem>> map_locations;
typedef std::map<int, std::shared_ptr<HostItem>> map_hosts;


extern void LoadConfig();
extern void InfectionInVectors(map_locations& locations, double pTrans);
extern void InfectionInHosts(map_hosts& hosts, double pITrans, double pRTrans);
extern void RegularHumanMovement(bool isDaytime, map_hosts& hosts, map_locations& locations);
extern void VectorMovement(map_locations& locations, double diffProb);
extern void VectorInfection(map_hosts& hosts, map_locations& locations, double biteRate, double vecInfectProb);
extern void HostInfection(map_hosts& hosts, map_locations& locations, double biteRate, double hostInfectProb, unsigned long round);
extern void VectorPopulation(map_locations& location, double vBirthRate, double probDeath, double probMaturation);
extern vector VectorMovementFrom(LocationItem& c, double diffProb);
extern void moveHost(map_locations& locations, HostItem& h, int dst);
extern std::map<int, vector> distributeVectors(LocationItem& c, vector movers);
extern const vector infectionEvent(HostItem& h, LocationItem& c, unsigned long round);

extern int binomial(int n, double p);
extern int poisson(double lambda);
extern int binSamp(int n, double p);
extern int binSamp(double n, double p);

extern vector emptyVState();
extern double vSum(const vector& v);
extern int vSum(const list& v);
extern int randint(int min, int max);

extern map_locations g_locations;
extern map_hosts g_hosts;
extern const int S;
extern const int E;
extern const int I;
extern const int R;