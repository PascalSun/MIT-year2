import json
with open("test2.locations",'r') as f:
    locData = json.load(f)
print(locData)