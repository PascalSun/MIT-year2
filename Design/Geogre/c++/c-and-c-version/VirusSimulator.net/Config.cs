using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Config
{
	public class HostItem
	{
		[JsonIgnore]
		public int iid;
		[JsonProperty("location")]
		public int location;
		[JsonProperty("i-state")]
		public string i_state;
		[JsonProperty("home")]
		public int home;
		[JsonProperty("hub")]
		public int hub;
	}

	public class LocationItem
	{
		[JsonIgnore]
		public int cid;
		[JsonProperty("coordinates")]
		public double[] coordinates;
		[JsonProperty("neighbours")]
		public int[] neighbours;
		[JsonProperty("hosts")]
		public List<int> hosts;
		[JsonProperty("v-capacity")]
		public double v_capacity;
		[JsonProperty("v-immature")]
		public double v_immature;
		[JsonProperty("v-state")]
		public double[] v_state;
	}

	public class Params
	{
		[JsonProperty("hosts-file")]
		public string hosts_file;
		[JsonIgnore]
		public Dictionary<int, HostItem> Host { get; private set; }
		[JsonProperty("locations-file")]
		public string locations_file;
		[JsonIgnore]
		public Dictionary<int, LocationItem> Location { get; private set; }
		[JsonProperty("datalog-file")]
		public string datalog_file;
		[JsonProperty("duration")]
		public int duration;
		[JsonProperty("cycle")]
		public int cycle;
		[JsonProperty("cpu-core")]
		public int cpu_core;
		[JsonProperty("vector-diffusion-probability")]
		public double vector_diffusion_probability;
		[JsonProperty("biting-rate")]
		public double biting_rate;
		[JsonProperty("vector-infection-probability")]
		public double vector_infection_probability;
		[JsonProperty("host-infection-probability")]
		public double host_infection_probability;
		[JsonProperty("vector-e-to-i-rate")]
		public double vector_e_to_i_rate;
		[JsonProperty("host-e-to-i-rate")]
		public double host_e_to_i_rate;
		[JsonProperty("host-i-to-r-rate")]
		public double host_i_to_r_rate;
		[JsonProperty("vector-birth-rate")]
		public double vector_birth_rate;
		[JsonProperty("vector-maturation-rate")]
		public double vector_maturation_rate;
		[JsonProperty("vector-death-rate")]
		public double vector_death_rate;

		public static Params Load(string path)
		{
			path = Path.GetFullPath(path);
			var ret = JsonConvert.DeserializeObject<Params>(File.ReadAllText(path));
			{
				var json = File.ReadAllText(Path.Combine(Path.GetDirectoryName(path), ret.locations_file));
				var obj = JsonConvert.DeserializeObject<Dictionary<int, LocationItem>>(json);
				ret.Location = new Dictionary<int, LocationItem>();
				foreach (var i in obj)
				{
					var cid = i.Key;
					var c = i.Value;
					c.cid = cid;
					c.hosts = new List<int>();
					ret.Location[cid] = c;
				}
			}
			{
				var json = File.ReadAllText(Path.Combine(Path.GetDirectoryName(path), ret.hosts_file));
				var obj = JsonConvert.DeserializeObject<Dictionary<int, HostItem>>(json);
				ret.Host = new Dictionary<int, HostItem>();
				foreach (var i in obj)
				{
					var iid = i.Key;
					var h = i.Value;
					h.iid = iid;
					ret.Host[iid] = h;
					ret.Location[h.location].hosts.Add(iid);
				}
			}
			return ret;
		}
	}
}