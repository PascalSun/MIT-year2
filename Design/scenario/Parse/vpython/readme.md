# Transform to Json, python version
- location.py
  - to use, we need to add  **[index,cellID,mos,col,row]** on the first row of the mosBreedingSiteClassification.csv
  - generatelocation(locations): use the clean data to generate the json file
  - command to use: python location.py
  - Need to generate all the cellID
  - The cells which have not specified the mos capibility, we set it as 10

- hosts.py
  - id,{location,state,home,hub}
  - steps:
    0. add **[iid,gid,id,index,log,lat,col,row]** to the ixx_id.csv
    1. read data from 3 csv files
    2. Check the data: 
      - anyone has work and school at the same time --> none
      - anyone do have work and school, but do not have home --> yes
      - All the individual should be assigned a home --> solutions
    3. Check all people, if without home, generate one randomly(* here can be changed later)
    4. Check the same people, whether has a school or work or -1
    5. output the data in the lists 

# Problems
- [x] how to deal with people without home
- [x] is the number of people the same as cells
