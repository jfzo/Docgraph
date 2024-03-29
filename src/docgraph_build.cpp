/*
 * docgraph_build.cpp
 *
 *  Created on: 27-05-2015
 *      Author: jz
 */

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
 * Extract all pairs between the pivot word (avoiding stopwords) and every word in the list Q.
 * Then inserts each pair into the graph G.
 */
void extract_edges(const string &p, const list<string>& Q, DG::docgraph& G,
		const set<string>& stopwords) {

	if (stopwords.find(p) != stopwords.cend())
		return;

	for (auto s = Q.begin(); s != Q.end(); ++s) {
		//cout << "edge: " << p << " -- " << *s << endl;
		if (stopwords.find(*s) == stopwords.cend())
			G.add_edge(p, *s);

	}
}


/**
 * Extract all pairs between the pivot word and every word in the list Q.
 * Then inserts each pair into the graph G.
 */
void extract_edges(const string &p, const list<string>& Q, DG::docgraph& G) {
	for (auto s = Q.begin(); s != Q.end(); ++s) {
		//cout << "edge: " << p << " -- " << *s << endl;
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

void init_stopwords(const char * infile, set<string>& stopwords) {
	cout << "Reading stopwords from " << infile << endl;
	char name[256];
	ifstream inf(infile, iostream::in);

	while (inf.good()) {
		inf.getline(name, 256);
		if (inf.gcount() > 0)
			stopwords.insert(name);
	}
	inf.close();
}

int main(int argc, char* argv[]) {

	cout << "*************************************************************\n";
	cout
			<< "Builds and stores a biterm-net for each document of a collection\n";
	cout << "into a csv file. This output file will contain in each line a\n";
	cout
			<< "sequence of tokens, each one together with its in and out degrees.\n";
	cout << "This program assumes that the csv-input file contains\n";
	cout << "a comma-separated token list representing one file per line.\n";
	cout << "*************************************************************\n";

	if (argc < 5) {
		cout << argv[0]
				<< " <csv-doc-collection> <stopwords-file> <windows-size> <csv-output>\n";
		return -1;
	}

	/* Open input file */
	ifstream inf(argv[1], iostream::in);

	ofstream out(argv[4], iostream::out);

	/* Set window size and create empty document graph */
	int wsize = stoi(argv[3]);
	DG::docgraph dgraph;

	/* Load stopwords */
	set<string> stopwords;
	init_stopwords(argv[2], stopwords);

	cout << "Loading input file " << argv[1] << ". Using stopwords file "
			<< argv[2] << " and window-size " << wsize << endl;

	/* Extract tokens and build biterm net */
	list<string> queue;
	string pivot, t;
	string fileid;
	FILESTATUS val = next_token(inf, fileid); //FILEID

	val = next_token(inf, pivot);
	while (val != EOF_FOUND) {

		val = next_token(inf, t);

		if (val == EOL_FOUND) {
			//extract_edges(pivot, queue, dgraph, stopwords);
			extract_edges(pivot, queue, dgraph);
			while (queue.size() > 1) {
				pivot = queue.front();
				queue.pop_front();
				//extract_edges(pivot, queue, dgraph, stopwords);
				extract_edges(pivot, queue, dgraph);
			}
			// end of current document
			queue.clear();

			//cout << "Writting info of "<< fileid << endl;

			/* clearing the net */
			//cout << "OUTPUT OF NET FOR FILE "<< fileid <<"\n";
			//dgraph.traverse_edges();
			// store the current graph.
			out << ">" << fileid << "\n";
			//dgraph.get_degrees(out);
			dgraph.get_degrees(out, stopwords);
			out << "\n";
			dgraph.clear();

			/** get the next document Id **/
			val = next_token(inf, fileid); // FILEID
			val = next_token(inf, pivot);

		} else if (queue.size() < (wsize - 1)) {
			queue.push_back(t);
		} else { // |queue| == (w - 1)
			//extract_edges(pivot, queue, dgraph, stopwords);
			extract_edges(pivot, queue, dgraph);
			pivot = queue.front();
			queue.pop_front();
			queue.push_back(t);
		}
	}

	inf.close();

	//cout << "Writing docnet to file "<< argv[4] <<endl;
	//dgraph.graphml_dump(argv[4]);
	out.close();
	return 0;
}

