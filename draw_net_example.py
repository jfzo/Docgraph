import networkx as nx
import matplotlib.pyplot as plt

g = nx.read_graphml("/Users/jz/git/Docgraph/test.graphml")

labels = {}
for n in g.nodes():
    labels[n] = g.node[n]['label']

pos = nx.circular_layout(g)

nx.draw_networkx_nodes(g, pos,alpha=0.4, node_color='w', node_size=500)
nx.draw_networkx_edges(g, pos, alpha=0.4, width=0.4, edge_color='b',style='dashed')
nx.draw_networkx_labels(g,pos,labels,font_size=6)
#plt.figure(figsize=(30.5, 30.0))
plt.axis('off')
plt.savefig('/Users/jz/Desktop/edge_labels.png',dpi=1500)
plt.show()
