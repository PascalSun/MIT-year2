
Python Dengue Simulator Input Data Files
----------------------------------------


group membership files
----------------------

  iX_id.csv

where X in {fhg,hg,sg,wg}

  hg - home group (household)
  sg - school group
  wg - work group
  fhg - not used. Possibly "friends household group".

Columns (no header row):

0 individual ID --> id for hosts
1 group ID (household, school or work group ID)
2 grid ID --> used for location and home
3 grid index ID (?)
4 logitude
5 latitude
6 column number (x cell co-ordinate)
7 row number (y cell co-ordinate)

age file
--------

individual_age.csv

Columns (there is a header row):

0 individual ID
1 age in years

cell geometry data
------------------

LocationCellInfo.csv

Columns (there is a header row):

0 Cell ID
1 longitude
2 latitude
3 column number
4 row number

For mosquito movement purposes the simulator assumes that
cells are connected to the 4 nearest neighbours (Von Neumann
neighbourhood) (except on edges and corners).

mosquito habitat data
---------------------

mosBreedingSiteClassification.csv

Columns (no header row)

0 Cell index (line number in file, not cell ID)
1 Cell ID
2 breeding site index (1 = low, 2 = high)
3 column number
4 row number

The Thailand Community Data Set
-------------------------------

SharedInput/cairnsPopDataFiles : human info
SharedInput/cairnsMosDataFiles : cell geography and mosquito info

Individual ids are 0-56898

School ids are     0-73
Work               0-4812
Households         0-25312

Some individuals don't seem to have households.

Grid is 1-309 rows by 1-226 columns, 69834 cells.
Grid ids range 493-70213 (52604 cells are listed).
Not all cells are listed in the location cell and
breeding site info files.

The relationship between row and column is

cellID = (1 + row)*226 + col


———
Dear Team Culico,

I have attached some input data files and notes
from the old python Dengue simulator.

You should be able to use the household group
data file to set the home location of each host,
and the school or workplace group file to set the
"commuting" location. You can also set the host
age even though that is not used yet.

The mosquito breeding habitat data file assigns a
value of 1 or 2 for "low" and "high". This can be
used to set the v-capacity cell field - perhaps
use 20 for low and 40 for high. We may have to
adjust this as we go to get plausible numbers of
mosquitoes per cell in equilibrium.

One thing to note is that the simulation grid is
a 226 column by 309 row grid with 69384 cells, but
the cell location and mosquito habitat files do
not list every cell (there are only 52604 cells
listed). For the missing cells, you can assume
they are there but they never contain hosts and
they have 0 mosquito carrying capacity. I've
worked out the formula for calculating a cell ID
from row and column, its at the bottom of the
notes file.

I suggest that you make some effort to maintain
any scripts you write to build the new JSON input
files from these CSV format files, since I will
try to send you some additional input data file
sets that we have.


Python Dengue Simulator Input Data Files
----------------------------------------


group membership files
----------------------

  iX_id.csv

where X in {fhg,hg,sg,wg}

  hg - home group (household)
  sg - school group
  wg - work group
  fhg - not used. Possibly "friends household group".

Columns (no header row):

0 individual ID
1 group ID (household, school or work group ID)
2 grid ID
3 grid index ID (?)
4 logitude
5 latitude
6 column number (x cell co-ordinate)
7 row number (y cell co-ordinate)

age file
--------

individual_age.csv

Columns (there is a header row):

0 individual ID
1 age in years

cell geometry data
------------------

LocationCellInfo.csv

Columns (there is a header row):

0 Cell ID
1 longitude
2 latitude
3 column number
4 row number

For mosquito movement purposes the simulator assumes that
cells are connected to the 4 nearest neighbours (Von Neumann
neighbourhood) (except on edges and corners).

mosquito habitat data
---------------------

mosBreedingSiteClassification.csv

Columns (no header row)

0 Cell index (line number in file, not cell ID)
1 Cell ID
2 breeding site index (1 = low, 2 = high)
3 column number
4 row number

The Thailand Community Data Set
-------------------------------

SharedInput/cairnsPopDataFiles : human info
SharedInput/cairnsMosDataFiles : cell geography and mosquito info

Individual ids are 0-56898

School ids are     0-73
Work               0-4812
Households         0-25312

Some individuals don't seem to have households.

Grid is 1-309 rows by 1-226 columns, 69834 cells.
Grid ids range 493-70213 (52604 cells are listed).
Not all cells are listed in the location cell and
breeding site info files.

The relationship between row and column is

cellID = (1 + row)*226 + col


———
Dear Team Culico,

I have attached some input data files and notes
from the old python Dengue simulator.

You should be able to use the household group
data file to set the home location of each host,
and the school or workplace group file to set the
"commuting" location. You can also set the host
age even though that is not used yet.

The mosquito breeding habitat data file assigns a
value of 1 or 2 for "low" and "high". This can be
used to set the v-capacity cell field - perhaps
use 20 for low and 40 for high. We may have to
adjust this as we go to get plausible numbers of
mosquitoes per cell in equilibrium.

One thing to note is that the simulation grid is
a 226 column by 309 row grid with 69384 cells, but
the cell location and mosquito habitat files do
not list every cell (there are only 52604 cells
listed). For the missing cells, you can assume
they are there but they never contain hosts and
they have 0 mosquito carrying capacity. I've
worked out the formula for calculating a cell ID
from row and column, its at the bottom of the
notes file.

I suggest that you make some effort to maintain
any scripts you write to build the new JSON input
files from these CSV format files, since I will
try to send you some additional input data file
sets that we have.
