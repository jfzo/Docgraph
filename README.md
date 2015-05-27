#(docgraph) Document and Collection Graph
----
 Set of programs to build biterm-graphs representing documents and summarizing document collections. 

#### **docgraph_build.main**
Builds a biterm net and stores it in Graphml format to a specified file. 
This program receives 4 parameters, namely: A CSV file, a stopword file, a window-size value and the graphml output path. 

The CSV file contains one document per line and then a comma separated list of tokens in each one.  The stopword file contains one stopword per line.

*Example of usage:* 

*docgraph_build.main term_sequence.csv stopword-list.txt 3 biterm.graphml*

#### **docgraph_degrees.main**
Loads  a biterm net already stored in Graphml format and outputs the in and out degrees of each vertex (token) to a CSV file.
This program receives 2 parameters, namely: The location a the graphml input file and the path of the output CSV file.

*Example of usage:* 

*docgraph_degrees.main biterm.graphml biterm_in_out_degrees.csv*
