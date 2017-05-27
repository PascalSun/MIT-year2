import random
import numpy as np
import csv
# id,{location,state,home,hub}
# 1. state: random
# 2. home/hub
# If the people do not have a home, generate one randomly
# If the people do not have a work or school, use -1
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


def getcsv(par):
	home = []
	location = []
	with open(par) as csvfile:
		reader = csv.DictReader(csvfile)
		for row in reader:
			home.append(row['iid'])
			location.append(row['id'])
	return home,location

def getfromcsv():
    locations = []
    with open('LocationCellInfo.csv') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            locations.append(row['Cell ID'])

	return locations

def main():
	# total number of individuals
	all = 56898

	# rate for different states
	rate = [0.1,0.3,0.5,0.1]
	elems = ['S','E','I','R']


	location = getfromcsv()
	home,homelocation =getcsv('ihg_id.csv')
	school,schoollocation = getcsv('isg_id.csv')
	work,worklocation = getcsv('iwg_id.csv')

	print(len(home))
	print(len(work))
	print(len(school))


	hosts = []
	# For all people, find home first 
	# Then find the hub
	# Generate one state

	for i in range(all):

		# Find home
		try:
			ihome = home.index(str(i))
			hlocation = homelocation[i]
		except:
			hlocation = random.choice(location)
		
		# Find hub
		try:
			ischool = school.index(str(i))
			hub = schoollocation[ischool]
		except:
			try:
				iwork = work.index(str(i))
				hub= worklocation[iwork]
			except:
				hub = -1
		hosts.append([i,hlocation,np.random.choice(elems, p=rate),hlocation,hub])

	generate(hosts)	




#-----
	# something to test use
	# for k in worklocation:
	# 	try:
	# 		ilocation = location.index(k)
	# 	except:
	# 		print(k)
	# nohome = []
	# for i in range(56898):
	# 	try:
	# 		k = home.index(str(i))
	# 	except:
	# 		nohome.append(i)
	# print(nohome)
	# print(len(nohome))
	# nowork=[]

	# for x in nohome:
	# 	try:
	# 		towork = school.index(str(x))
	# 		print(x)
	# 	except:
	# 		nowork.append(x)
	# print(len(nowork))

	# test whether there is people have a work and school at the same time.

	# workandschool = []
	# for k in school:
	# 	try:
	# 		x = work.index(k)
	# 		workandschool.append(k)
	# 	except:
	# 		print('ok')
	# print(len(workandschool))
	# print(len(school))
			

main()