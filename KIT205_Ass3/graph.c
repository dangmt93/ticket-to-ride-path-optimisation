#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph.h"


/* Add a new directed edge to a graph -----------------------------
 * Parameters:
 *		self			Graph instance
 *		from_vertex		vertex where the edge begins
 *		to_vertext		vertex where the edge ends
 *		weight			edge weight
 * Return: void
 */
void add_edge(Graph* self, int from_vertex, int to_vertex, int weight) {
	// Check if from- and to-vertex are valid
	if (from_vertex < 0 || from_vertex >= self->V) {
		fprintf(stderr, "From-vertex %d is invalid\n", from_vertex);
	}

	if (to_vertex < 0 || to_vertex >= self->V) {
		fprintf(stderr, "To-vertex %d is invalid\n", to_vertex);
	}

	// Initialise edge node
	EdgeNodePtr node = malloc(sizeof * node);
	node->edge.to_vertex = to_vertex;
	node->edge.weight = weight;

	// Insert edge node at front of existing list
	node->next = self->edges[from_vertex].head;
	self->edges[from_vertex].head = node;
}

/* Create a graph (Read inputs) -------------------------------------
 * No parameters
 * Return: Graph
 */
Graph create_graph() {
	Graph G;

	scanf_s("%d", &G.V);
	G.edges = malloc(sizeof(G.edges) * G.V);
	//printf("%d\n", G.V);

	for (int v = 0; v < G.V; v++) {
		G.edges[v].head = NULL;

		int n_vertices;
		scanf_s("%d", &n_vertices);
		for (int e = 0; e < n_vertices; e++) {
			EdgeNodePtr node = malloc(sizeof(*node)); // *node ~ edgeNode
			scanf_s("%d,%d", &node->edge.to_vertex, &node->edge.weight);

			// Insert at front
			node->next = G.edges[v].head;
			G.edges[v].head = node;
		}
	}

	/* Add edges in other direction - To form undirected graph 
	*		(input only include edges from lower from-vertices to higher to-vertices)
	*/
	for (int v = 0; v < G.V; v++) {
		EdgeNodePtr current = G.edges[v].head;
		while (current != NULL) {
			if (v < current->edge.to_vertex) {
				add_edge(&G, current->edge.to_vertex, v, current->edge.weight);
			}
			current = current->next;
		}
	}
	
	return G;
}

/* Destroy a graph (free all memory) ------------------------------
 * Parameters:
 *		self    Graph instance
 * Return: void
 */
void destroy_graph(Graph* self) {
	for (int v = 0; v < self->V; v++) {
		EdgeNodePtr current = self->edges[v].head;

		while (current != NULL) {
			EdgeNodePtr to_free = current;
			current = current->next;
			free(to_free);
		}
	}

	free(self->edges);
}


/* Print to-vertices of each vertex in graph ----------------------------
 * Parameter:
 *		self		Graph instance
 * Return: void
 */
void print_graph(Graph* self) {
	for (int v = 0; v < self->V; v++) {
		printf("Vertex [%d] -> ", v);
		EdgeNodePtr current = self->edges[v].head;
		while (current != NULL) {
			printf("%d(%d)", current->edge.to_vertex, current->edge.weight);
			if (current->next != NULL) {
				printf(", ");
			}
			current = current->next;
		}
		printf("\n");
	}
}

/* Print in-degrees of all vertices in a graph - FOR TESTING
 * Parameter:
 *		self		Graph instance
 * Return: void
 */
void print_in_degrees(Graph* self) {
	int* in_degrees = malloc(sizeof(int) * self->V);
	for (int i = 0; i < self->V; i++) {
		in_degrees[i] = 0;
	}

	for (int v = 0; v < self->V; v++) {
		EdgeNodePtr current_edge = self->edges[v].head;
		while (current_edge != NULL) {
			in_degrees[current_edge->edge.to_vertex]++;
			current_edge = current_edge->next;
		}
	}

	for (int i = 0; i < self->V; i++) {
		printf("vertex %d has %d in-degrees\n", i, in_degrees[i]);
	}

	/* Free Memory of dynamic array */
	free(in_degrees);
}


/* Run Prim's minimal spanning tree algorithm on graph ---------------
 * Parameters
 *		self		Graph instance
 * Return: MST (Graph)
 */
Graph prims_mst(Graph* self) {
	// Initialise graph with all vertices (but no edges)
	Graph MST_G;
	MST_G.V = self->V;
	MST_G.edges = malloc(sizeof(MST_G.edges) * MST_G.V);
	for (int v = 0; v < MST_G.V; v++) {
		MST_G.edges[v].head = NULL;
	}

	// Array to keep track of cheapest cost for each vertex
	//		Initially, all are +infinity distance (none are in MST, so infinite distance away)
	int* cheapest_cost = malloc(sizeof(int) * MST_G.V);
	
	// Array to keep track of closest edge (to-vertices) with cheapest cost
	//		Initially, all are +infinity (none are in MST)
	int* closest_edge = malloc(sizeof(int) * MST_G.V);
	
	// Array to keep track of vertices that are in MST 
	//		Initially, all are false (none are in MST)
	bool* inMST = malloc(sizeof(bool) * MST_G.V);

	// Initialise values of closest and inMST
	for (int i = 0; i < MST_G.V; i++) {
		cheapest_cost[i] = INT_MAX;
		closest_edge[i] = INT_MAX;
		inMST[i] = false;
	}

	// Since graph has V vertices, need to iterate V times to add all vertices to MST 
	for (int count = 0; count < MST_G.V ; count++) {
		// Find min C[v] in vertices outside MST --> vertex v (min_index)
		int min = INT_MAX;
		int min_index = 0;
		for (int v = 0; v < MST_G.V; v++) {
			if (cheapest_cost[v] <= min && inMST[v] == false) {
				min = cheapest_cost[v];
				min_index = v;
			}
		}

		// Add v to MST
		inMST[min_index] = true;

		// Add edge to MST if closest edge is defined
		if (closest_edge[min_index] != INT_MAX) {
			add_edge(&MST_G, min_index, closest_edge[min_index], cheapest_cost[min_index]);
		}

		// Loop through edges from chosen v (with min_index)
		// If not in MST and edge has smaller weight, change values in arrays
		EdgeNodePtr current = self->edges[min_index].head;
		while (current != NULL) {
			int considered_v = current->edge.to_vertex;
			if (inMST[considered_v] == false && current->edge.weight < cheapest_cost[considered_v]) {
			// if considered_v is not in MST && weight < cheapest cost
				cheapest_cost[considered_v] = current->edge.weight;
				closest_edge[considered_v] = min_index;

			}
			current = current->next;
		}

		// FOR TESTING -----------------------------
		/*
		for (int i = 0; i < MST_G.V; i++) {
			printf("%d, ", cheapest_cost[i]);
		}
		printf("\n");
		for (int i = 0; i < MST_G.V; i++) {
			printf("%d, ", closest_edge[i]);
		}
		printf("\n");
		for (int i = 0; i < MST_G.V; i++) {
			printf("%d, ", inMST[i]);
		}
		printf("\n\n");
		*/
	}

	/* Free Memory of dynamic arrays */
	free(cheapest_cost);
	free(closest_edge);
	free(inMST);

	return MST_G;
}

/* Calculate total weight of a graph
 * Parameters:
 *		self		Graph instance
 * Return: total weight of graph
 */
int graph_weight(Graph* self) {
	int total_weight = 0;
	for (int v = 0; v < self->V - 1; v++) {
		EdgeNodePtr current = self->edges[v].head;
		while (current != NULL) {
			total_weight += current->edge.weight;
			current = current->next;
		}
	}

	return total_weight;
}


/* Dijkstra's algorithm 
 *		Return shortest path from source to destination from a graph
 *		Also change value of pre-created distance and dist_array (passed by reference)
 * Parameters:
 *		self			Graph instance
 *		source			source vertex
 *		destination		destination vertex
 *		distance		pointer to variable storing distance of the shortest path from source to destination
 *		dist_array		array of shortest distance for other vertices from source
 * Return: linked list of vertices in the path
 */
EdgeList dijkstra(Graph* self, int source, int destination, int* distance) {
	// Array to keep track of shortest distance from source --> vertex u
	//		Initially, all are +infinity (none are visited to determine shortest distance)
	int* dist = malloc(sizeof(int) * self->V);
	
	// Array to keey track of visited vertices
	//		Initially, all false (none are visited)
	bool* visited = malloc(sizeof(bool) * self->V);

	// Array to keep track of predecessor vertex of each vertex (closest previous vertex)
	//		Initially, all are -1 (no predecessors found yet)
	int* pred_v = malloc(sizeof(int) * self->V);

	// Initialise arrays
	for (int v = 0; v < self->V; v++) {
		dist[v] = INT_MAX;
		visited[v] = false;
		pred_v[v] = -1;
	}

	// Distance from source -> source = 0
	dist[source] = 0;

	// Looping until all vertices have been visited
	for (int count = 0; count < self->V; count++) {
		// Find unvisited vertex with shortest distance 
		int min = INT_MAX;
		int min_index = 0;
		for (int v = 0; v < self->V; v++) {
			if (visited[v] == false && dist[v] < min) {
				min = dist[v];
				min_index = v;
			}
		}

		// Mark this vertex as visited
		visited[min_index] = true;

		// Loop through edges from chosen v (with min_index)
		// If not visited and d[u] + d[uv] < current d[u], change values in arrays
		EdgeNodePtr current = self->edges[min_index].head;
		while (current != NULL) {
			int considered_v = current->edge.to_vertex;
			if (visited[considered_v] == false && dist[min_index] + current->edge.weight < dist[considered_v]) {
				dist[considered_v] = dist[min_index] + current->edge.weight;
				pred_v[considered_v] = min_index;
			}
			current = current->next;
		}
	}

	// Pass shortest distance from source to target to a pointer (to manipulate value outside the function)
	//		distance must be initialised before executing function to be changed
	if (distance != NULL) {
		*distance = dist[destination];
	}

	// Trace path from destination back to source --> form a linked list of vertices (EdgeNode) with weight = 0
	EdgeList path;
	path.head = NULL;
	int v = destination;

	// Since the source vertex has -1 as predecessor 
	while (v != -1) {
		// Create new node 
		EdgeNodePtr node = malloc(sizeof * node);
		node->edge.to_vertex = v;
		node->edge.weight = 0;

		// Insert at front of path
		node->next = path.head;
		path.head = node;

		// Set v to predecessor 
		v = pred_v[v];
	}

	/* Printing output - FOR TESTING */
	/*
	for (int v = 0; v < self->V; v++) {
		printf("%d(%d)[%d], ", v, dist[v], pred_v[v]);
	}
	*/

	/* Free memory for dynamic arrays */
	free(dist);
	free(visited);
	free(pred_v);

	return path;
}


/* Return edge weight of vertex v --> vertex u (edge uv) 
 * Parameters:
 *		self		Graph instance
 *		v			first vertex
 *		u			second vertex
 * Return: edge weight (int)
 */
int edge_weight(Graph* self, int v, int u) {
	// Check if vertex v and u are valid
	if (v < 0 || v >= self->V) {
		fprintf(stderr, "From-vertex %d is invalid\n", v);
	}

	if (u < 0 || u >= self->V) {
		fprintf(stderr, "To-vertex %d is invalid\n", u);
	}

	// Retrieve weight of edge uv
	EdgeNodePtr current = self->edges[v].head;
	while (current != NULL) {
		if (current->edge.to_vertex == u) {
			return current->edge.weight;
		}
		current = current->next;
	}
}

/* Check if edge uv exists in a graph
 * Parameters:
 *		self		Graph instance
 *		v			first vertex
 *		u			second vertex
 * Return: true / false
 */
bool edge_exists(Graph* self, int v, int u) {
	// Check if vertex v and u are valid
	if (v < 0 || v >= self->V) {
		fprintf(stderr, "From-vertex %d is invalid\n", v);
	}

	if (u < 0 || u >= self->V) {
		fprintf(stderr, "To-vertex %d is invalid\n", u);
	}
	// Retrieve weight of edge uv
	EdgeNodePtr current = self->edges[v].head;
	while (current != NULL) {
		if (current->edge.to_vertex == u) {
			return true;
		}
		current = current->next;
	}
	return false;
}

/* Create a graph that combines edges from tickets 
 * Parameters:
 *		self			Graph instance
 *		from_cities[]	array of source vertices in tickets
 *		to_citites[]	array of destination vertices in tickets
 *		n_tickets		number of tickets
 * Return: Graph containing shortest path edges
 */
Graph shortest_paths(Graph* self, int from_cities[], int to_cities[], int n_tickets) {
	// Initialise graph with all vertices (but no edges)
	Graph shortest_G;
	shortest_G.V = self->V;
	shortest_G.edges = malloc(sizeof(shortest_G.edges) * shortest_G.V);
	for (int v = 0; v < shortest_G.V; v++) {
		shortest_G.edges[v].head = NULL;
	}

	// Use Dijkstra to find shortest path for each ticket, then edges to shortest_G
	for (int n = 0; n < n_tickets; n++) {
		// Apply Dijkstra's
		EdgeList path = dijkstra(self, from_cities[n], to_cities[n], NULL);
		
		// Add edges to shortest_G
		EdgeNodePtr prev = path.head;
		EdgeNodePtr current = path.head->next;
		while (current != NULL) {
			int start_v, end_v;
			// Make sure start_v < end_v
			if (prev->edge.to_vertex < current->edge.to_vertex) {
				start_v = prev->edge.to_vertex;
				end_v = current->edge.to_vertex;
			}
			else {
				start_v = current->edge.to_vertex;
				end_v = prev->edge.to_vertex;
			}

			// If edge not yet added to shortest_G, add it
			if (!edge_exists(&shortest_G, start_v, end_v)) {
				add_edge(&shortest_G, start_v, end_v, edge_weight(self, start_v, end_v));
			}

			prev = prev->next;
			current = current->next;
		}
	}
	return shortest_G;
}


/* Steiner tree with set of terminal vertices in a graph 
 * Logic: https://medium.com/@rkarthik3cse/steiner-tree-in-graph-explained-8eb363786599
 * Parameters:
 *		self			Graph instance
 *		terminals[]		array containing all terminal vertices
 *		n_terminals		number of terminals (= 2 * n_tickets)
 * Return: steiner tree (Graph)
 */
Graph steiner_tree(Graph* self, int terminals[], int n_terminals) {
	// Initialise graph with all vertices (but no edges)
	Graph steiner_t;
	steiner_t.V = self->V;
	steiner_t.edges = malloc(sizeof(steiner_t.edges) * steiner_t.V);
	for (int v = 0; v < steiner_t.V; v++) {
		steiner_t.edges[v].head = NULL;
	}

	// Array to keep check of which vertices are in steiner tree
	//		Initally, all false except the starting vertex (can be any)
	bool* inTree = malloc(sizeof(bool) * self->V);


	// Array to check which vertices are terminals
	bool* isTerm = malloc(sizeof(bool) * self->V);


	for (int v = 0; v < self->V; v++) {
		inTree[v] = false;
		isTerm[v] = false;
	}

	for (int i = 0; i < n_terminals; i++) {
		isTerm[terminals[i]] = true;
	}
	
	// Add first terminal vertex to tree
	inTree[terminals[0]] = true;

	// Loop until all terminal vertices are added to tree 
	//		Only until n_terminals - 1 (first terminal already added)
	for (int count = 0; count < n_terminals - 1; count++) {
		// Find closest terminal not in tree to a vertex in tree
		int closest_distance = INT_MAX;
		int closest_v;
		EdgeList shortest_path;
		for (int n = 0; n < self->V; n++) {
			int distance = 0;
			if (inTree[n]) { // every vertex in tree
				for (int i = 0; i < self->V; i++) {
					if (!inTree[i] && isTerm[i]) { // terminal vertices that are not yet in tree
						EdgeList path = dijkstra(self, n, i, &distance);
						//printf("       Distance = %d\n", distance);
						if (distance < closest_distance) {
							closest_distance = distance;
							closest_v = i;
							shortest_path = dijkstra(self, n, i, &distance);

							//printf("        Closest distance = %d\n", closest_distance);

						}
					}
				}
			}
		}

		//printf("==> Closest distance = %d\n", closest_distance);
		//printf("==> Closest v = %d\n", closest_v);

		// Add all vertices on shortest paths to tree
		EdgeNodePtr prev = shortest_path.head;
		EdgeNodePtr current = shortest_path.head->next;
		while (current != NULL) {
			int start_v, end_v;
			// Make sure start_v < end_v
			if (prev->edge.to_vertex < current->edge.to_vertex) {
				start_v = prev->edge.to_vertex;
				end_v = current->edge.to_vertex;
			}
			else {
				start_v = current->edge.to_vertex;
				end_v = prev->edge.to_vertex;
			}

			// If edge not yet added to steiner_t, add it
			if (!edge_exists(&steiner_t, start_v, end_v)) {
				add_edge(&steiner_t, start_v, end_v, edge_weight(self, start_v, end_v));
			}

			// Set inTree value to true
			inTree[current->edge.to_vertex] = true;

			prev = prev->next;
			current = current->next;
		}
	}

	return steiner_t;
}