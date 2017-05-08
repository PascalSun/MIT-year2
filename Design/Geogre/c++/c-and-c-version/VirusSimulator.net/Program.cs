using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Config;
using System.Diagnostics;

namespace VirusSimulator.net
{
	class Program
	{
		/// <summary>
		/// Susceptible
		/// </summary>
		public const int S = 0;
		/// <summary>
		/// Exposed / Latent
		/// </summary>
		public const int E = 1;
		/// <summary>
		/// Infectious
		/// </summary>
		public const int I = 2;
		/// <summary>
		/// Recovery
		/// </summary>
		public const int R = 3;

		static void Main(string[] args)
		{
			if (args.Length != 1)
			{
				Console.WriteLine("need config file path for params");
				Console.ReadKey();
				return;
			}

			var @params = Config.Params.Load(args[0]);


			var watch = new Stopwatch();
			watch.Start();
			for (var cycle = 0; cycle < @params.duration; cycle++)
			{
				infectionInVectors(@params);
				infectionInHosts(@params);
				regularHumanMovement(@params, cycle);
				vectorMovement(@params, cycle);
				vectorInfection(@params);
				hostInfection(@params, cycle);
				vectorPopulation(@params);
			}
			watch.Stop();
			Console.WriteLine(watch.Elapsed);
			Console.ReadKey();
		}

		/// <summary>
		/// regular human movement
		/// hub  : location ID of daytime cell (-1 for none)
		/// home : location ID of home cell
		/// </summary>
		/// <param name="cycle"></param>
		/// <param name="config"></param>
		private static void regularHumanMovement(Params @params, int cycle)
		{
			var hosts = @params.Host;
			var locations = @params.Location;
			if (isDaytime(cycle))
			{
				foreach (var h in hosts.Values)
				{
					if (h.hub >= 0)
						moveHost(locations, h, h.hub);
				}
			}
			else
			{
				foreach (var h in hosts.Values)
				{
					moveHost(locations, h, h.home);
				}
			}
		}

		private static void moveHost(Dictionary<int, LocationItem> locations, HostItem h, int dst)
		{
			var iid = h.iid;
			var oldLoc = h.location;
			h.location = dst;
			locations[oldLoc].hosts.Remove(iid);
			locations[dst].hosts.Add(iid);
		}

		private static void vectorMovement(Params @params, int cycle)
		{
			if (!isDaytime(cycle))
				return;
			var locations = @params.Location;
			var diffP = @params.vector_diffusion_probability;

			var incoming = new Dictionary<int, double[]>();
			foreach(var i in locations)
			{
				var cid = i.Key;
				var c = i.Value;
				var movers = vectorMovementFrom(c, diffP);
				vSub(locations[cid].v_state, movers);

				var dMovers = distributeVectors(c, movers);
				foreach(var m in dMovers)
				{
					var nb = m.Key;
					if (!incoming.ContainsKey(nb))
						incoming[nb] = emptyVState();
					vAdd(incoming[nb], dMovers[nb]);
				}
			}

			foreach (var i in incoming)
			{
				vAdd(locations[i.Key].v_state, i.Value);
			}
		}

		private static double[] emptyVState()
		{
			return new double[] { 0, 0, 0, 0 };
		}
		private static List<double> vectorMovementFrom(LocationItem c, double diffP)
		{
			var vs = c.v_state;
			return (from n in vs select binSamp(n, diffP)).ToList();
		}

		private static Dictionary<int, double[]> distributeVectors(LocationItem c, List<double> movers)
		{
			var nbs = c.neighbours;
			var nnbs = nbs.Length;
			var dMovers = new Dictionary<int, double[]>();
			for (var i = 0; i < nnbs; i++)
				dMovers[nbs[i]] = emptyVState();
			for (var vs = S; vs <= I; vs++)
			{
				for (var n = 0; n < movers[vs]; n++)
				{
					var nbt = randint(0, nnbs);
					dMovers[nbs[nbt]][vs] += 1;
				}
			}
			return dMovers;
		}

		private static void vectorInfection(Params @params)
		{
			var hosts = @params.Host;
			var locations = @params.Location;
			var br = @params.biting_rate;
			var pbip = @params.vector_infection_probability;
			foreach (var i in locations)
			{
				var cid = i.Key;
				var c = i.Value;
				var ns = c.v_state[S];
				if (ns == 0)
					continue;
				var npop = 0;
				var ipop = 0;
				foreach (var iid in c.hosts)
				{
					npop++;
					if (hosts[iid].i_state == "I")
						ipop++;
				}
				if (ipop == 0)
					continue;
				var beta = (double)ipop / (double)npop * br * pbip;
				var p = 1.0 - Math.Exp(-beta);
				var newInfections = binSamp(ns, p);
				c.v_state[S] -= newInfections;
				c.v_state[E] += newInfections;
			}
		}

		private static void hostInfection(Params @params, int cycle)
		{
			var hosts = @params.Host;
			var locations = @params.Location;
			var br = @params.biting_rate;
			var pbip = @params.host_infection_probability;
			foreach (var i in locations)
			{
				var cid = i.Key;
				var c = i.Value;
				var nvi = c.v_state[I];
				if (nvi == 0)
					continue;
				var hpop = 0;
				var hs = 0;
				foreach (var iid in c.hosts)
				{
					if (hosts[iid].i_state == "S")
						hs ++;
					hpop ++;
				}
				if (hs == 0)
					continue;
				var nv = c.v_state.Sum();
				var beta = br * pbip * (double)nvi / (double)hpop;
				var p = 1.0 - Math.Exp(-beta);
				foreach (var iid in c.hosts)
				{
					var h = hosts[iid];
					if (h.i_state != "S")
						continue;
					if (binSamp(1, p) != 0)
					{
						h.i_state = "E";
						Console.WriteLine(csvLine(infectionEvent(cycle, h, c)));
					}
				}
			}
		}

		private static object[] infectionEvent(int cycle, HostItem h, LocationItem c)
		{
			return new object[] {cycle, 'i' , h.iid, c.cid};
		}

		private static void infectionInVectors(Params @params)
		{
			var locations = @params.Location;
			var eToIRate = @params.vector_e_to_i_rate;
			var pTrans = 1.0 - Math.Exp(-eToIRate);
			foreach (var i in locations)
			{
				var cid = i.Key;
				var c = i.Value;
				var ne = c.v_state[E];
				if (ne == 0)
					continue;
				var nTrans = binSamp(ne, pTrans);
				c.v_state[E] -= nTrans;
				c.v_state[I] += nTrans;
			}
		}

		private static void infectionInHosts(Params @params)
		{
			var hosts = @params.Host;
			var eToIRate = @params.host_e_to_i_rate;
			var iToRRate = @params.host_i_to_r_rate;
			var pITrans = 1.0 - Math.Exp(-eToIRate);
			var pRTrans = 1.0 - Math.Exp(-iToRRate);

			foreach(var i in hosts)
			{
				var iid = i.Key;
				var h = i.Value;
				var s = h.i_state;
				if (s == "E")
				{
					if (binSamp(1, pITrans) != 0)
						h.i_state = "I";
				}
				else if (s == "I")
				{
					if (binSamp(1, pRTrans) != 0)
						h.i_state = "R";
				}
			}
		}

		private static void vectorPopulation(Params @params)
		{
			var locations = @params.Location;
			var br0 = @params.vector_birth_rate;
			var mr = @params.vector_maturation_rate;
			var dr = @params.vector_death_rate;
			var pDeath = 1.0 - Math.Exp(-dr);
			var pMat = 1.0 - Math.Exp(-mr);

			foreach (var i in locations)
			{
				var cid = i.Key;
				var c = i.Value;
				var vs = c.v_state;
				var cc = c.v_capacity;
				var nDeaths = (from n in vs select binSamp(n, pDeath)).ToList();

				var vPop = vs.Sum();
				var br = (double)vPop * br0 * Math.Max(0, 1 - vPop / cc);
				var nBirths = br >= 0 ? poisson(br) : 0;

				var nMats = binSamp(c.v_immature, pMat);

				vSub(vs, nDeaths);
				c.v_immature += nBirths;
				c.v_immature -= nMats;
				vs[S] += nMats;
			}
		}


		static bool isDaytime(int cycle)
		{
			return cycle % 2 == 0; // even cycles are day
		}

		/// <summary>
		/// modifies first arg ie "to u add v"
		/// </summary>
		/// <param name="u"></param>
		/// <param name="v"></param>
		static void vAdd(IList<double> u, IList<double> v)
		{
			for (var i = 0; i < u.Count; i++)
				u[i] += v[i];
		}
		static void vAdd(IList<int> u, IList<int> v)
		{
			for (var i = 0; i < u.Count; i++)
				u[i] += v[i];
		}
		/// <summary>
		/// modifies first arg ie "from u subtract v"
		/// </summary>
		/// <param name="u"></param>
		/// <param name="v"></param>
		static void vSub(IList<double> u, IList<double> v)
		{
			for (var i = 0; i < u.Count; i++)
				u[i] -= v[i];
		}
		static void vSub(IList<int> u, IList<int> v)
		{
			for (var i = 0; i < u.Count; i++)
				u[i] -= v[i];
		}
		static string csvLine(object[] parts)
		{
			return string.Join(",", parts);
		}

		#region random
		private static Random random = new Random(0);
		static int randint(int minValue, int maxValue)
		{
			return random.Next(minValue, maxValue);
		}
		static int poisson(double lambda)
		{
			if (lambda <= 0)
				return 0;
			return MathNet.Numerics.Distributions.Poisson.Sample(random, lambda);
		}
		static int binomial(int n, double p)
		{
			return MathNet.Numerics.Distributions.Binomial.Sample(random, p, n);
		}
		static double binSamp(double n, double p)
		{
			return binSamp((int)n, p);
		}
		static double binSamp(int n, double p)
		{
			if (n == 0)
				return 0;
			return binomial(n, p);
		}
		#endregion
	}
}
