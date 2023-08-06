#ifndef LIST_H
#define LIST_H

typedef struct edge {
	int to_vertex;
	int weight;
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode* next;
} *EdgeNodePtr;

typedef struct edgeList {
	EdgeNodePtr head;
} EdgeList;

typedef struct graph {
	int V;
	EdgeList* edges;
} Graph;


/* Create a graph (Read inputs) -------------------------------------
 * No parameters
 * Return: Graph
 */
Graph create_graph();


/* Destroy a graph (free all memory) ------------------------------
 * Parameters:
 *		self    Graph instance
 * Return: void
 */
void destroy_graph(Graph* self);


/* Print to-vertices of each vertex in graph ----------------------
 * Parameter:
 *		self		Graph instance
 * Return: void
 */
void print_graph(Graph* self);


/* Print in-degrees of all vertices in a graph
 * Parameter:
 *		self		Graph instance
 * Return: void
 */
void print_in_degrees(Graph* self);

/* Run Prim's minimal spanning tree algorithm on graph ---------------
 * Parameters
 *		self		Graph instance
 * Return: MST (Graph)
*/
Graph prims_mst(Graph* self);


/* Calculate total weight of a graph
 * Parameters:
 *		self		Graph instance
 * Return: total weight of graph
 */
int graph_weight(Graph* self);


/* Dijkstra's algorithm
 *		Return shortest path from source to destination from a graph
 *		Also change value of pre-created distance (passed by reference)
 * Parameters:
 *		self			Graph instance
 *		source			source vertex
 *		destination		destination vertex
 *		distance		pointer to variable storing distance of the shortest path from source to destination
 * Return: linked list of vertices in the path
 */
EdgeList dijkstra(Graph* self, int source, int destination, int* distance);


/* Create a graph that combines edges from tickets
 * Parameters:
 *		self			Graph instance
 *		from_cities[]	array of source vertices in tickets
 *		to_citites[]	array of destination vertices in tickets
 *		n_tickets		number of tickets
 * Return: Graph containing shortest path edges
 */
Graph shortest_paths(Graph* self, int from_cities[], int to_cities[], int n_tickets);

/* Steiner tree with set of terminal vertices in a graph
 * Logic: https://medium.com/@rkarthik3cse/steiner-tree-in-graph-explained-8eb363786599
 * Parameters:
 *		self			Graph instance
 *		terminals[]		array containing all terminal vertices
 *		n_terminals		number of terminals (= 2 * n_tickets)
 * Return: steiner tree (Graph)
 */
Graph steiner_tree(Graph* self, int terminals[], int n_terminals);
#endif