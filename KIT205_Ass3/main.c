#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main() {
	/* Getting input */
	Graph G = create_graph();

	// Getting tickets input
	int n_tickets;
	scanf_s("%d", &n_tickets);
	int* from_cities = malloc(sizeof(int) * n_tickets);
	int* to_cities = malloc(sizeof(int) * n_tickets);
	for (int n = 0; n < n_tickets; n++) {
		scanf_s("%d,%d", &from_cities[n], &to_cities[n]);
	}

	//printf("%d\n", n_tickets);

	/* Print Graph G - FOR TESTING */
	//print_graph(&G);
	//print_in_degrees(&G);
	//printf("\n");
	

	// PART A: Minimum spanning tree ---------------------------------
	printf("Part A: Prim's Minimum spanning tree -----------------------------\n");
	Graph MST = prims_mst(&G);
	//print_graph(&MST);

	/* Print all edges of MST and total cost */
	//		Each vertex in this version of MST will only have 1 edge to it (excluding the last vertex)
	printf("Edges of MST:\n");
	for (int v = 0; v < MST.V - 1; v++) {
		printf("%d-%d(%d)", v, MST.edges[v].head->edge.to_vertex, MST.edges[v].head->edge.weight);
		if (v != MST.V - 2) {
			printf(", ");
		}
	}
	printf("\nTotal cost of MST = %d\n\n", graph_weight(&MST));


	// PART B: Steiner tree problem -----------------------------------
	printf("\nPart B: Steiner tree problem -------------------------------------\n");
	/* Solution 1 */
	printf("Solution 1: Shortest paths combination\n");
	Graph shortest_G = shortest_paths(&G, from_cities, to_cities, n_tickets);
	//print_graph(&shortest_G);

	printf("Edges of shortest paths:\n");
	for (int v = 0; v < shortest_G.V; v++) {
		EdgeNodePtr current = shortest_G.edges[v].head;
		while (current != NULL) {
			printf("%d-%d(%d), ", v, current->edge.to_vertex, current->edge.weight);
			current = current->next;
		}
	}
	printf("\b\b "); // Remove last comma
	printf("\nTotal cost of MST = %d\n\n", graph_weight(&shortest_G));



	/* Solution 2 */
	printf("Solution 2: Steiner tree\n");
	// Merge from and to_cities into 1 array
	int* terminals = malloc(sizeof(int) * n_tickets * 2);
	for (int n = 0; n < n_tickets; n++) {
		terminals[n] = from_cities[n];
		terminals[n + n_tickets] = to_cities[n];
	}

	// Print array of terminal vertices - FOR TESTING
	/*
	for (int n = 0; n < n_tickets * 2; n++) {
		printf("%d, ", terminals[n]);
	}
	*/

	Graph steiner_t = steiner_tree(&G, terminals, n_tickets * 2);
	//print_graph(&steiner_t);
	printf("Edges of Steiner Tree:\n");
	for (int v = 0; v < steiner_t.V; v++) {
		EdgeNodePtr current = steiner_t.edges[v].head;
		while (current != NULL) {
			printf("%d-%d(%d), ", v, current->edge.to_vertex, current->edge.weight);
			current = current->next;
		}
	}
	printf("\b\b "); // Remove last comma
	printf("\nTotal cost of MST = %d\n\n", graph_weight(&steiner_t));


	printf("\n\n----End----\n");
	destroy_graph(&G);

	return 0;
}