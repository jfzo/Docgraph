//============================================================================
// Name        : Docgraph.cpp
// Author      : J.Z.
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <set>
#include "docgraph.hpp"



using namespace std;

/**
 * Extract all pairs between the pivot word and every word in the list Q.
 * Then inserts each pair into the graph G.
 */
void extract_edges(const string &p, const list<string>& Q, DG::docgraph& G, const set<string>& stopwords) {

	if(stopwords.find(p) != stopwords.cend() )
		return;

	for (auto s = Q.begin(); s != Q.end(); ++s) {
		//cout << "edge: " << p << " -- " << *s << endl;
		if(stopwords.find(*s) == stopwords.cend() )
			G.add_edge(p, *s);

	}
}



enum FILESTATUS {
	KEEP_READING, EOL_FOUND, EOF_FOUND
};

FILESTATUS next_token(ifstream &inf, string& s) {
	stringstream ss;
	//char word[250];
	char c;
	int read_chars = 0;
	FILESTATUS STAT = KEEP_READING;

	while (1) {
		inf.get(c);
		if (inf.good()) {
			//cout << "READ("<< c<<") ";
			if (c != '\n') {
				if (c != ',') {
					/*
					 if (read_chars == 0){
					 strcpy(word, &c);
					 cout << " ..copy\n";
					 }else{
					 strcat(word, &c);
					 cout << " ..concat ("<< word<<")\n";
					 }
					 */
					ss << c;

					read_chars++;
				} else {
					break;
				}
			} else {
				STAT = EOL_FOUND;
				break;
			}
		} else {
			STAT = EOF_FOUND;
			break;
		}
	}

	if (read_chars > 0) {
		//s = string(word);
		//ss = ss.str();
		ss >> s;
	}
	return STAT; // returned true if there are more chars to read and false when an EOL o EOF are found.
}

void init_stopwords(const char * infile, set<string>& stopwords){
	cout << "Reading stopwords from "<< infile << endl;
	char name[256];
	ifstream inf(infile, iostream::in);

	while(inf.good() ){
		inf.getline (name,256);
		if( inf.gcount() > 0 )
			stopwords.insert( name);
	}
	inf.close();
}

int main(int argc, char* argv[]) {

	if (argc < 4) {
		cout << "Enter the input filename, stopword filename and windows-size.\n";
		return -1;
	}

	/* Open input file */
	ifstream inf(argv[1], iostream::in);

	/* Set window size and create empty document graph */
	int wsize = stoi(argv[3]);
	DG::docgraph dgraph;


	/* Load stopwords */
	set<string> stopwords;
	init_stopwords(argv[2], stopwords);



	cout << "Loading input file "<<argv[1] << ". Using stopwords file "<< argv[2]<<" and window-size "<<wsize << endl;

	/* Extract tokens and build biterm net */
	list<string> queue;
	string pivot, t;
	FILESTATUS val = next_token(inf, t); //FILEID

	val = next_token(inf, pivot);
	while (val != EOF_FOUND) {

		val = next_token(inf, t);

		if (val == EOL_FOUND) {
			extract_edges(pivot, queue, dgraph, stopwords);
			while (queue.size() > 1) {
				pivot = queue.front();
				queue.pop_front();
				extract_edges(pivot, queue, dgraph, stopwords);
			}
			// end of current document
			queue.clear();
			val = next_token(inf, t); // FILEID
			val = next_token(inf, pivot);
		} else if (queue.size() < (wsize - 1)) {
			queue.push_back(t);
		} else { // |queue| == (w - 1)
			extract_edges(pivot, queue, dgraph, stopwords);
			pivot = queue.front();
			queue.pop_front();
			queue.push_back(t);
		}
	}

	inf.close();

	/* Dump resulting graph */
	//dgraph.traverse_edges();

	dgraph.graphml_dump( "test.graphml");
	// TODO: extract degrees and store them.
/*
	DG::docgraph dgraph2;
	dgraph2.load_graphml("test.graphml");

	cout << "Original graph:"<< endl;
	dgraph.traverse_edges();
	cout << "\nLoaded graph:"<< endl;
	dgraph2.traverse_edges();
*/
	return 0;
}
