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

#include "docgraph.hpp"

using namespace std;

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

/**
 * Reads data in cluto's format (sparse or non sparse autodetection).
 * Stores the data and their labels into a Realdata object.
 * */
void read_csv_string_data(const char * fname) {
	cout << "openning file " << fname << endl;
	ifstream inf(fname, iostream::in);
	char c;
	int nlines = 0;
	//while (!inf.eof()) {
	while (inf.good()) {
		inf.get(c);
		if (c == '\n') {
			cout << "EOL\n";
			nlines++;
		} else
			cout << c << "|";
	}

	cout << "NLINES:" << nlines << endl;
	inf.close();
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


int main(int argc, char* argv[]) {

	if (argc < 3) {
		cout << "Enter the filename and windows-size.\n";
		return -1;
	}

	ifstream inf(argv[1], iostream::in);

	DG::docgraph dgraph;
	int wsize = stoi(argv[2]);
	cout << "Loading file "<<argv[1] << ". Using window-size "<<wsize << endl;

	list<string> queue;
	string pivot, t;
	FILESTATUS val = next_token(inf, t); //FILEID

	val = next_token(inf, pivot);
	while (val != EOF_FOUND) {

		val = next_token(inf, t);

		if (val == EOL_FOUND) {
			extract_edges(pivot, queue, dgraph);
			while (queue.size() > 1) {
				pivot = queue.front();
				queue.pop_front();
				extract_edges(pivot, queue, dgraph);
			}
			// end of current document
			queue.clear();
			val = next_token(inf, t); // FILEID
			val = next_token(inf, pivot);
		} else if (queue.size() < (wsize - 1)) {
			queue.push_back(t);
		} else { // |queue| == (w - 1)
			extract_edges(pivot, queue, dgraph);
			pivot = queue.front();
			queue.pop_front();
			queue.push_back(t);
		}
	}

	inf.close();
	//dgraph.traverse_edges();

	dgraph.graphml_dump("test.graphml");
}

int main2(int argc, char* argv[]) {
	vector<string> ts;
	ts.push_back("19828283");
	ts.push_back("este");
	ts.push_back("mensaje");
	ts.push_back("de");
	ts.push_back("este");
	ts.push_back("tema");
	ts.push_back("no");
	ts.push_back("habla");
	ts.push_back("de");
	ts.push_back("ningun");
	ts.push_back("mensaje");

	DG::docgraph dgraph;

	list<string> queue;
	string pivot, t;

	int wsize = 3;
	if (argc > 1) {
		wsize = stoi(argv[1]);
		cout << "wsize set to " << wsize << endl;
	}

	/* the algorithm begins
	 * It assumes that the first field represents the FILEID, so it's not considered.
	 * */

	auto it = ts.begin();
	cout << "FILEID: " << *it << endl;

	pivot = *(++it); // |queue| = 0
	while (1) {
		t = *(++it);
		if (it == ts.end()) {
			extract_edges(pivot, queue, dgraph);
			while (queue.size() > 1) {
				pivot = queue.front();
				queue.pop_front();
				extract_edges(pivot, queue, dgraph);
			}
			break; // end of iteration
		} else if (queue.size() < (wsize - 1)) {
			queue.push_back(t);
		} else { // |queue| == (w - 1)
			extract_edges(pivot, queue, dgraph);
			pivot = queue.front();
			queue.pop_front();
			queue.push_back(t);
		}
	}

//cout << "Final queue size:" << queue.size() << endl;

	dgraph.traverse_edges();

	return 0;
}

int main3(int argc, char* argv[]) {

	if (argc == 1) {
		cout << "Enter the filename\n";
		return -1;
	}

	ifstream inf(argv[1], iostream::in);
	string t;
	FILESTATUS val = next_token(inf, t);
	while (val != EOF_FOUND) {
		if (val == EOL_FOUND) {
			cout << "\n";
		} else {
			cout << t << ":";
		}
		val = next_token(inf, t);
	}
	inf.close();
	return 0;
}
