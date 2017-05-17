# Transform to Json, python version
- location.py
  - to use, we need to add  **[index,cellID,mos,col,row]** on the first row of the mosBreedingSiteClassification.csv
  - generatelocation(locations): use the clean data to generate the json file
  - command to use: python location.py
  - Need to generate all the cellID
- hosts.py
  - id,{location,state,home,hub}
  - steps:
    1. read data from 3 csv files
    2. Check the data: 
      - anyone has work and school at the same time
      - anyone do have work and school, but do not have home
      - All the individual should be assigned a home
    3. Check all people, if without home, generate one randomly(* here can be changed later)
    4. Check the same people, whether has a school or work or -1
    5. output the data in the lists 

# Problems
- [ ] how to deal with people without home
- [ ] is the number of people the same as cells
