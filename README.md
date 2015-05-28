# Program description

 Set of programs to build biterm-graphs representing documents and summarizing document collections. 

#### **colgraph_build.main**
Builds a biterm-net of a document collection and stores its vertex degrees into a CSV file. 
This program receives 4 parameters, namely: A CSV file representing the collection, a stopword file (one word per line), a window-size value and the output file path. 

Each line of the CSV output file will contain a token (vertex), its in and out degrees. This program assumes that the csv-input file contains a comma-separated token list representing one document per line.

*Example of usage:* 

*colgraph_build.main term_sequence.csv stopword-list.txt 3 biterm.degrees*

### **docgraph_build.main**
Builds and stores a biterm-net for each document of a collection into a csv file. This output file will contain in each line a
sequence of tokens, each one together with its in and out degrees. This program assumes that the csv-input file contains
a comma-separated token list representing one file per line.

*Example of usage:* 

*docgraph_build.main term_sequence.csv stopword-list.txt 3 biterm_docnet.csv*

#### **docgraph_degrees.main**
Loads  a biterm net already stored in Graphml format and outputs the in and out degrees of each vertex (token) to a CSV file.
This program receives 2 parameters, namely: The location a the graphml input file and the path of the output CSV file.

*Example of usage:* 

*docgraph_degrees.main biterm.graphml biterm_in_out_degrees.csv*

# Compilation instructions

For each *'.main'* program described above there is a target in the Makefile. For instance the program *docgraph_degrees.main* is built by executing the command:

*make docgraph_degrees.main*
