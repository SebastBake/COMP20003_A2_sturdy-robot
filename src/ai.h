#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"

#define EPS 1e-6
#define FILENAME "output.txt"
#define CSV_MODE 1


typedef struct scoreholder {
	double max[N_MOVES], avg[N_MOVES];
	int avg_n[N_MOVES];
} scores_t;

typedef struct outputholder {
	FILE *fp;
	unsigned long long generated;
	unsigned long long expanded;
	int mode;
	time_t start_time;
} output_t;

// Handle the global heap
void initialize_ai();

// Handle nodes and moves
move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation, output_t *op);
void propogate_back(node_t *node);
void start_node(uint8_t newboard[SIZE][SIZE], node_t *start);

// Handle scores
void scores_assign(node_t *node,scores_t *scores);
scores_t scores_init();
int scores_choose(scores_t scores, propagation_t propagation);
void printscores(scores_t * scores, int depth, propagation_t propagation);

// Handle output
output_t *output_start(int mode);
void output_add_generated(output_t *op);
void output_add_expanded(output_t *op);
void output_end(output_t op, uint8_t board[SIZE][SIZE], int score, int depth);

#endif
