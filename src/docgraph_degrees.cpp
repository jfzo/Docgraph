/*
 * docgraph_degrees.cpp
 *
 *  Created on: 27-05-2015
 *      Author: jz
 */

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <set>
#include "docgraph.hpp"



using namespace std;
int main(int argc, char* argv[]) {

	cout << "**************************************************\n";
	cout << "Outputs degrees into a CSV file.\n";
	cout << "Fields contain word-vertex, in-degree, out-degree\n";
	cout << "**************************************************\n";

	if (argc < 3) {
		cout << argv[0] << " <graphml-input> <csv-degrees-output>\n";
		return -1;
	}

	DG::docgraph dg;
	cout << "Loading net from file "<< argv[1]<<endl;
	dg.load_graphml(argv[1]);

	cout << "Writing CSV file with in-degrees and out-degrees for each term in file "<< argv[2] << endl;
	ofstream os (argv[2], iostream::out);
	dg.get_degrees(os);
	os.close();
	//dg.traverse_edges();
	return 0;
}


