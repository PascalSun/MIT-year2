import random
import numpy as np

def generate(hosts):
	# Generate the file
	t = open('test1.hosts', 'w')
	content ='  "{}" : {{\
	\n    "location" : {},\
	\n    "i-state" : "{}",\
	\n    "home" : {},\
	\n    "hub" : {} \n }},\n'
	t.write("{ \n")
	lens = len(hosts)
	count = 0
	for k in hosts:
		if count > lens - 2:
			break
		else:
			count = count+1
		contentr = content.format(k[0],k[1],k[2],k[3],k[4])
		t.write(contentr)
	content ='  "{}" : {{\
	\n    "location" : {},\
	\n    "i-state" : "{}",\
	\n    "home" : {},\
	\n    "hub" : {} \n }}\n'
	k = hosts[lens-1]
	contentr = content.format(k[0],k[1],k[2],k[3],k[4])
	t.write(contentr)
	t.write("} ")
	t.close()

def generatehosts(num,hubnum,rate,hubhome):
	# Generate the hosts []

	# Choose the initation state of the infectious situation with different rate
	hosts = []
	elems = ['S','E','I','R']

	# A list of hub with different rates
	hubs = [-1]
	hubsrate = [hubhome]
	for k in range(hubnum):
		hubs.append(random.randint(0,num))
		hubsrate.append((1-hubhome)/hubnum)

	for i in range(num):
		hosts.append([i,i,np.random.choice(elems, p=rate),i,np.random.choice(hubs,p=hubsrate)])
	return hosts

def generatelocation(locations):
	t = open('test1.locations','w')
	content = ' "{}" :{{\
	\n   "coordinates": {},\
	\n   "neighbours": {},\
	\n   "hosts":{},\
	\n   "v-capacity":{},\
	\n   "v-immature":{},\
	\n   "v-state":{} \n }},\n'
	t.write("{ \n")

	lens = len(locations)
	count = 0
	for k in locations:
		if count > lens - 2:
			break
		else:
			count = count+1
		contentr = content.format(k[0],k[1],k[2],k[3],k[4],k[5],k[6])
		t.write(contentr)
	content = ' "{}" :{{\
	\n   "coordinates": {},\
	\n   "neighbours": {},\
	\n   "hosts":{},\
	\n   "v-capacity":{},\
	\n   "v-immature":{},\
	\n   "v-state":{} \n }}\n'
	k = locations[lens-1]
	contentr = content.format(k[0],k[1],k[2],k[3],k[4],k[5],k[6])
	t.write(contentr)

	t.write("}")
	t.close()


def generatelocations(line,cap,immature,state):
	locations = []
	for k in range(line):
		for j in range(line):
			location=[]
			location.append(k*line+j)
			location.append([float(100*(j+1)),float(100*(k+1))])
			location.append(generateneighbour(k*line+j,line))
			location.append([])
			location.append(cap)
			location.append(immature)
			location.append(state)
			locations.append(location)
	return locations

def generateneighbour(x,line):
	neighbours = []
	if x%line != 0:

		neighbours.append(x-1)
	if x%line != line-1:

		neighbours.append(x+1)
	if x + line < line*line:
		neighbours.append(x+line)
	if x-line > -1:
		neighbours.append(x-line)
	return neighbours


def main():
	# par = [num of hosts, num of hubs, rate of different situation at initation stage, rate of people stay home with no hub]
	# num of hosts should be line of area * line of area
<<<<<<< HEAD
	par =[90000,10,[0.1,0.3,0.5,0.1],0.2]
=======
	par =[10000,10,[0.1,0.3,0.5,0.1],0.2]
>>>>>>> 026f32f61d6b1849f8b74014e2b1dc2503de76e5

	hosts = generatehosts(par[0],par[1],par[2],par[3])
	generate(hosts)


	# par = [line of the area, capabilty, immature, v-state]
<<<<<<< HEAD
	parlocation = [300,20,0,[20,0,0,0]]
=======
	parlocation = [100,20,0,[20,0,0,0]]
>>>>>>> 026f32f61d6b1849f8b74014e2b1dc2503de76e5
	locations = generatelocations(parlocation[0],parlocation[1],parlocation[2],parlocation[3])
	generatelocation(locations)

main()
