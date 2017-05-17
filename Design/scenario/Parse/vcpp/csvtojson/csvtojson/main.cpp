//
//  main.cpp
//  csvtojson
//
//  Created by Pascal_Sun on 17/5/17.
//  Copyright © 2017 Pascal_Sun. All rights reserved.
//

#include <fstream>
#include <iostream>
using namespace std;

int main ()
{
    
    ifstream file ( "LocationCellInfo.csv" );
    string value;
    while (file.good())
    {
        getline ( file, value );
        cout << string( value, 1, value.length()-2 );
        break;
    }
    
    return 0;
}
