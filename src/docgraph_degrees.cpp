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

	if (argc < 2) {
		cout << "Enter the input filename.\n";
		return -1;
	}

	DG::docgraph dg;
	dg.load_graphml(argv[1]);

	ofstream os ("bitermnet_in-out.degrees", iostream::out);
	dg.get_degrees(os);
	os.close();
	//dg.traverse_edges();
	return 0;
}


