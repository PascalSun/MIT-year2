import csv
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

def getfromcsv():
    locations = []
    with open('LocationCellInfo.csv') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            par = [row['Cell ID'],row['Long'],row['Lat'],row['Column'],row['Row']]
            locations.append(par)
    return locations

def main():
    locations = getfromcsv()
    location = []
    for k in locations:
        location.append([k[0],[k[1],k[2]],generateneighbour(int(k[4]),int(k[3])),[],20,0,[20,0,0,0]])
    generatelocation(location)
main()
