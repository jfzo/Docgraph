/*
 * docgraph.hpp
 *
 *  Created on: 24-05-2015
 *      Author: jz
 */

#ifndef DOCGRAPH_HPP_
#define DOCGRAPH_HPP_

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <tuple>
#include <cmath>
#include <fstream>

typedef uint32_t UINT_T;

namespace DG {

struct edge_t {
public:
	UINT_T id;			// e.g. hash value representing the edge.
	UINT_T hval_ta, hval_tb;
	double weight;
	double count_ta;
	double count_tb;
	double count_tatb;

	edge_t() {
		id = -1;
		hval_ta = -1;
		hval_tb = -1;
		weight = -1;
		count_ta = -1;
		count_tb = -1;
		count_tatb = -1;
	}

	edge_t(const edge_t & e) {
		id = e.id;
		hval_ta = e.hval_ta;
		hval_tb = e.hval_tb;
		weight = e.weight;
		count_ta = e.count_ta;
		count_tb = e.count_tb;
		count_tatb = e.count_tatb;
	}

};

struct vertex_count_t {
	typedef boost::vertex_property_tag kind;
};

struct edge_freq_t {
	typedef boost::edge_property_tag kind;
};

struct edge_tdistsum_t {
	typedef boost::edge_property_tag kind;
};

typedef boost::property<boost::edge_weight_t, int,
		boost::property<boost::edge_name_t, UINT_T,
				boost::property<edge_freq_t, UINT_T,
						boost::property<edge_tdistsum_t, double> > > > EdgeProperties;

typedef boost::property<boost::vertex_index_t, int,
		boost::property<boost::vertex_name_t, std::string,
				boost::property<boost::vertex_degree_t, int,
						boost::property<vertex_count_t, int> > > > VertexProperties;

/** Graph Definition */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
		VertexProperties, EdgeProperties> GRAPH_T;

typedef boost::graph_traits<GRAPH_T>::vertex_descriptor VERTEX;
typedef boost::graph_traits<GRAPH_T>::edge_descriptor EDGE;
typedef GRAPH_T::edge_property_type Weight;

class docgraph {
	edge_t inneredge;
	GRAPH_T G;
	size_t num_vertices;
	std::unordered_map<std::string, VERTEX> term_vertex;
	std::unordered_map<int, EDGE> id_edge;
	boost::graph_traits<GRAPH_T>::edge_iterator eit, eit_end;
	bool traversed;

	//boost::property_map<GRAPH_T, boost::edge_weight_t>::type weight;
	boost::property_map<GRAPH_T, boost::edge_name_t>::type edge_id;
	boost::property_map<GRAPH_T, boost::edge_weight_t>::type edge_freq;
	//boost::property_map<GRAPH_T, edge_freq_t>::type edge_freq;
	boost::property_map<GRAPH_T, edge_tdistsum_t>::type edge_tdist;
	boost::property_map<GRAPH_T, boost::vertex_index_t>::type index;
	boost::property_map<GRAPH_T, boost::vertex_name_t>::type name;
	boost::property_map<GRAPH_T, boost::vertex_degree_t>::type degree;
	boost::property_map<GRAPH_T, vertex_count_t>::type count;

	/**
	 * Displays some information of the given vertex.
	 */
	void print_vertex_info(VERTEX);

public:
	size_t num_edges;

	/**
	 * Constructor
	 */
	docgraph();

	/**
	 Creates a new term vertex into the graph if it does not exist.
	 Otherwise, it does nothing (NO COUNT INCREASING).
	 */
	void add_term(std::string);

	/**
	 Creates a new edge into the graph if it does not exist.
	 Otherwise, increases its edge frequency and the corresponding term degrees.
	 Note: It calls the add_term function.
	 */
	void add_edge(std::string, std::string);

	/**
	 * Displays vertices and then each edge, i.e. source and target vertices together
	 * with edge information (only frequency at the moment).
	 */
	void traverse_edges();

	/**
	 * Saves or stores the graph in graphml format.
	 */
	void graphml_dump(const char *);

	/**
	 * Loads a graph from a previously saved file.
	 */
	void load_graphml(const char*);

	/**
	 * Get degrees from the graph and put each term label as a key and its degree as the value.
	 */
	void get_degrees(std::unordered_map<std::string, std::pair<int, int> >&);

	void get_degrees(std::ostream&);
};

}

#endif /* DOCGRAPH_HPP_ */
