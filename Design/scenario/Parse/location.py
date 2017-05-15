import csv
# to use, we need to add  index,cellID,mos,col,row on the first row of the mosBreedingSiteClassification.csv
# generatelocation(locations): use the clean data to generate the json file

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

# Find the Von Neumann neighbourhood for each cell
def generateneighbour(row,col):
	neighbours = []
	if row > 1 and row < 310:
		neighbours.append(row*226+col)
	if row > 0 and row < 309:
		neighbours.append((row+2)*226+col)

	if col>1 and col < 227:
		neighbours.append((row+1)*226+col-1)
	if col > 0 and col < 226:
		neighbours.append((row+1)*226+col+1)
	return neighbours

# Get cell info from LocationCellInfo.csv
def getfromcsv():
    locations = []
    with open('LocationCellInfo.csv') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            locations.append([row['Cell ID'],row['Long'],row['Lat'],row['Column'],row['Row']])

	return locations

# Get capability info from mosBreeding...csv
def getcap():
	ids  = []
	mos = []
	with open('mosBreedingSiteClassification.csv') as csvfile:
		reader = csv.DictReader(csvfile)
		for row in reader:
			ids.append(row['cellID'])
			mos.append(row['mos'])

	return ids,mos

def main():

 	ids,mos = getcap()
	locations = getfromcsv()

	loca = []
	i = 0
	for k in locations:
		indexs = ids.index(k[0])
		caps = mos[indexs]
		# if caps  = 1 -> capbility = 20, this can be adjusted
		if caps == '1':
			capss = 20
		else:
			capss = 40
		# id, [lat,long],neighbour,
		par = [k[0],[k[1],k[2]],generateneighbour(int(k[4]),int(k[3])),[],capss,0,[20,0,0,0]]
		loca.append(par)
	generatelocation(loca)
	print('locations done')


if __name__ == "__main__":
	main()
