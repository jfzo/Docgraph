/*
 * docgraph_impl.cpp
 *
 *  Created on: 25-05-2015
 *      Author: jz
 */
#include "docgraph.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace DG;

docgraph::docgraph() {
	num_edges = 0;
	//weight = boost::get(boost::edge_weight, G);
	edge_id = boost::get(boost::edge_name, G);
	//edge_freq = boost::get(edge_freq_t(), G);
	edge_freq = boost::get(boost::edge_weight, G);
	edge_tdist = boost::get(edge_tdistsum_t(), G);
	index = boost::get(boost::vertex_index, G);
	name = boost::get(boost::vertex_name, G);
	degree = boost::get(boost::vertex_degree, G);
	count = boost::get(vertex_count_t(), G);
	traversed = false;
	num_vertices = 0;
}

void docgraph::add_term(std::string t) {
	if (term_vertex.find(t) == term_vertex.end()) { // a new vertex.
		VERTEX v = boost::add_vertex(G);
		term_vertex[t] = v; // added to the map of vertices
		boost::put(name, v, t); // stores the name (string)
		boost::put(count, v, 1); // inits. the frequency
		degree[v] = 0; // initially disconnected
		num_vertices++; // vertex counter increased
	}
	// NOTE: Not always term count should be increased. (term sequence problem) Better to increase manually.
	/*else{
	 VERTEX v = term_vertex[t];
	 count[v] += 1;
	 }*/
}

void docgraph::add_edge(std::string t1, std::string t2) {
	add_term(t1);
	add_term(t2);

	VERTEX v_t1, v_t2;
	v_t1 = term_vertex[t1];
	v_t2 = term_vertex[t2];

	EDGE e;
	bool exists;
	boost::tie(e, exists) = boost::edge(v_t1, v_t2, G);
	// If the edge already exists, then only increment its weight.
	if (exists) {
		//weight[e]++;
		edge_freq[e]++;

		//edge_tdist[e] += dist; // sum of distances
		//edge_tdist[e] = (dist > edge_tdist[e]) ? dist : edge_tdist[e]; // max distance.

		//degree[v_t1]++;
		//degree[v_t2]++;
	} else {
		boost::add_edge(v_t1, v_t2, G);
		boost::tie(e, exists) = boost::edge(v_t1, v_t2, G);
		//			UINT_T id; // It is important to generate the edges in this fashion in order to enable all docs.
		//			//to use features from the same space.
		//			if (t1 < t2)
		//				id = strhash(t1.c_str(), t1.length(), t2.c_str(), t2.length());
		//			else
		//				id = strhash(t2.c_str(), t2.length(), t1.c_str(), t1.length());
		//
		//			edge_id[e] = id;
		//weight[e] = 1;
		edge_freq[e] = 1;
		//edge_tdist[e] = dist;
		degree[v_t1]++;
		degree[v_t2]++;
		num_edges++;
	}

	//std::cout << "Added edge: "<< t1 << " -- " << t2 << "("<< dist<<")" <<" now sum_dist:" << edge_tdist[e] << "\n";
}

/**
 * Prints the vertices and then the edges (source and target vertices).
 */
void docgraph::traverse_edges() {
	int counter = 0;
	boost::graph_traits<GRAPH_T>::vertex_iterator v, v_end;
	std::cout << "Vertices:" << std::endl;
	for (boost::tie(v, v_end) = boost::vertices(G); v != v_end; ++v) {
		std::cout << name[*v] << std::endl;
	}

	boost::graph_traits<GRAPH_T>::edge_iterator e, e_end;
	for (boost::tie(e, e_end) = edges(G); e != e_end; ++e) {
		counter++;
		VERTEX v1 = source(*e, G);
		VERTEX v2 = target(*e, G);
		//assert(weight[*e] == edge_freq[*e]);
		std::cout << name[v1] << " -- " << name[v2] << " /edge-freq:"
				<< edge_freq[*e] << std::endl;
		//print_vertex_info(v1);
		//print_vertex_info(v2);
	}
}

void docgraph::print_vertex_info(VERTEX v) {
	// INDEX, NAME, DEGREE
	std::cout << "id:" << index[v] << " name:" << name[v];
	std::cout << " count:" << count[v] << " degree:" << degree[v] << "\n";
}

void docgraph::graphml_dump(const char * out) {
	boost::dynamic_properties dp;

	dp.property("label", boost::get(boost::vertex_name, G) );
	dp.property("degree",boost::get(boost::vertex_degree, G) );
	//dp.property("freq", boost::get(edge_freq_t(), G) );
	dp.property("freq", boost::get(boost::edge_weight, G) );

	ofstream os(out, iostream::out);
	boost::write_graphml(os, G, dp, true);
	os.close();
}

void docgraph::load_graphml(const char* in) {
	boost::dynamic_properties dp;

	dp.property("label", boost::get(boost::vertex_name, G) );
	dp.property("degree",boost::get(boost::vertex_degree, G) );
	//dp.property("freq", boost::get(edge_freq_t(), G) );
	dp.property("freq", boost::get(boost::edge_weight, G) );

	ifstream is(in, iostream::in);

	boost::read_graphml(is, G, dp, 0);

	is.close();
}








