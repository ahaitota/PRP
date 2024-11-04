#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "graph.h"

#define INIT_SIZE 10

/* This is a testing file just to be able to compile the 
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

// - function -----------------------------------------------------------------
graph_t* allocate_graph(void) 
{  
   graph_t *g = (graph_t*) malloc(sizeof(graph_t));
   edge_t *edges = (edge_t*) malloc(INIT_SIZE * sizeof(edge_t));
   if (g == NULL) {
      fprintf(stderr, "Malloc fail: %s line %d\n", __FILE__, __LINE__);
      exit(-1);
   }
   g->edges = edges;
   g->num_edges = 0;
   g->capacity = INIT_SIZE;
   return g;
}

// - function -----------------------------------------------------------------
void free_graph(graph_t **graph)
{
   if (graph != NULL && *graph != NULL) {
      if ((*graph)->capacity > 0) {
         free((*graph)->edges);
      }
      free(*graph);
      *graph = NULL;
   }
   
}


int combine_chars(int c1, int c2, int c3, int c4) {
   return (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
}

// - function -----------------------------------------------------------------
void load_txt(const char *fname, graph_t *graph) {
   FILE *file = fopen(fname, "r");

   if (!file) {
      fprintf(stderr, "Error opening file\n");
      return;
   }

   int numbers[3];
   char string_int[11];
   int curr_int = 0;
   int curr_num = 0;

   while (1) {
      int c = getc_unlocked(file);

      if (c == EOF) {
         break;
      } else if (c == ' ') {
         string_int[curr_int] = '\0';
         numbers[curr_num++] = atoi(string_int);
         curr_int = 0;

      } else if (c == '\n') {
         string_int[curr_int] = '\0';
         numbers[curr_num++] = atoi(string_int);
         curr_int = 0;
         curr_num = 0;
         int from = numbers[0];
         int to = numbers[1];
         int cost = numbers[2];

         if (graph->num_edges >= graph->capacity) {
            edge_t *edges = (edge_t*) realloc(graph->edges, 2 * graph->capacity * sizeof(edge_t));
            graph->capacity = 2 * graph->capacity;
            
            if (!edges) {
                  free_graph(&graph);
                  fclose(file);
                  fprintf(stderr, "Error reallocating memory\n");
                  return;
            }
            graph->edges = edges;
         }
         graph->edges[graph->num_edges].from = from;
         graph->edges[graph->num_edges].to = to;
         graph->edges[graph->num_edges].cost = cost;
         graph->num_edges++;
      } else {
         string_int[curr_int++] = (char) c;
      }
 
   }
   
   fclose(file);
}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph_t *graph)
{
   FILE *file = fopen(fname, "rb");
   if (file == NULL) {
      fprintf(stderr, "Error opening file\n");
      exit(-1);
   }

   while (1) {
      edge_t edge;
      int result = fread(&edge, sizeof(edge_t), 1, file);
      if (result != 1) {
         break;
      }
      if (graph->num_edges >= graph->capacity) {
         edge_t *edges = (edge_t*) realloc(graph->edges, 2 * graph->capacity * sizeof(edge_t));
         graph->capacity = 2 * graph->capacity;
        
         if (!edges) {
               free_graph(&graph);
               fclose(file);
               fprintf(stderr, "Error reallocating memory\n");
               return;
         }
         graph->edges = edges;
      }
      graph->edges[graph->num_edges++] = edge;
   }

   fclose(file);

}

// - function -----------------------------------------------------------------
void save_txt(const graph_t * const graph, const char *fname)
{
   FILE *file = fopen(fname, "w");
 
   if (!file) {
      return;
   }
 
   for (int i = 0; i < graph->num_edges; ++i) {
      fprintf(file, "%d %d %d\n", graph->edges[i].from, graph->edges[i].to, graph->edges[i].cost);
   }
 
   fclose(file);   
   return;
}

// - function -----------------------------------------------------------------
void save_bin(const graph_t * const graph, const char *fname) {
    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(-1);
    }

    // Write the entire array of edges in one call
    size_t written = fwrite(graph->edges, sizeof(edge_t), graph->num_edges, file);
    if (written != graph->num_edges) {
        fprintf(stderr, "Error writing to file\n");
        fclose(file);
        exit(-1);
    }

    fclose(file);
}
