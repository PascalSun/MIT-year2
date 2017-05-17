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
	rows = []
	lat = [] 
	col = []
	long = []
	with open('LocationCellInfo.csv') as csvfile:
		reader = csv.DictReader(csvfile)
		for row in reader:
			locations.append([row['Cell ID'],row['Long'],row['Lat'],row['Column'],row['Row']])
			rows.append(row['Row'])
			lat.append(row['Lat'])
			col.append(row['Column'])
			long.append(row['Long'])

	return locations,rows,lat,col,long

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
	locations,rows,lat,col,long = getfromcsv()
	
	lats = []
	longs = []

	for k in range(1,310):
		try: 
			rowindex = rows.index(str(k))
			lats.append(lat[rowindex])
		except:
			print(k)


	for k in range(1,227):
		try: 
			colindex = col.index(str(k))
			longs.append(long[colindex])
		except:
			print(k)
		

	loca = []
	for rowno in range(1,310):
		for colno in range(1,227):
			cellID = (1+rowno)*226+colno
			try:
				idindex = ids.index(cellID)
				caps = mos[idindex]
			except:
				caps = '3'
		# if caps  = 1 -> capbility = 20, this can be adjusted
			if caps == '1':
				capss = 20
			elif caps == '2':
				capss = 40	
			else:
				capss = 10 # to adjust

			par = [cellID,[float(longs[colno-1]),float(lats[rowno-1])],generateneighbour(rowno,colno),[],capss,0,[capss,0,0,0]]		
			loca.append(par)
			print(rowno,colno)
	generatelocation(loca)
	print('locations done')




if __name__ == "__main__":
	main()
