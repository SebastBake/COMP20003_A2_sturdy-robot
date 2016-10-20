#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"

struct heap h;

void initialize_ai(){
	heap_init(&h);
}

/**
 * Updates the scores data structure
 */
void scores_assign(node_t *node,scores_t *scores) {
	// update ancestor score (max)
	if (node->priority>scores->max[node->move]) {
		scores->max[node->move] = node->priority;
	}
	// update ancestor score (avg)
	scores->avg_n[node->move]++;
	scores->avg[node->move] = ((scores->avg[node->move]*(scores->avg_n[node->move]-1))+node->priority)/(scores->avg_n[node->move]);
}

/**
 * A useful function for printing info from my scores data structure
 */
void printscores(scores_t * scores, int depth, propagation_t propagation) {
	int i, tabs;
	for (i=0;i<N_MOVES;i++) {
		if (propagation == max) {
			for (tabs=0;tabs<depth;tabs++) {
				printf("|   ");
			}
			printf("max: (%d,%f)\n",i, scores->max[i]);
		} else {
			for (tabs=0;tabs<depth;tabs++) {
				printf("|   ");
			}
			printf("avg: (%d,%f)\n",i, scores->avg[i]);
		}
	}
}

/**
 * Chooses a move base on the propogation type and the score info of each path
 */
int scores_choose(scores_t scores, propagation_t propagation) {
	printf("score chooser\n");
	uint8_t i;
	uint8_t work_array[] = {0,0,0,0}; // Holds the candidate moves
	uint8_t work_array_n = 0;
	double best_score = 0;
	// Find the best moves
	for (i=0;i<N_MOVES;i++) {
		if (propagation == max) {
			if ((scores.max[i]-best_score) > EPS) {
				// score larger than previous largest
				// ->new largest
				work_array[0] = i;
				work_array[1] = 0;
				work_array[2] = 0;
				work_array[3] = 0;
				work_array_n=1;
				best_score = scores.max[i];
				printf("max: (%d,%f)\n",i, scores.max[i]);
			} else if (
				(scores.max[i]-best_score) < EPS &&
				(scores.max[i]-best_score) > -1*EPS
				) {
				// score equal and best are the same
				work_array[work_array_n++] = i;
				printf("max: (%d,%f)\n",i, scores.max[i]);
			}
		} else {
			if ((scores.avg[i]-best_score) > EPS) {
				// score larger than previous largest
				// ->new largest
				work_array[0] = i;
				work_array[1] = 0;
				work_array[2] = 0;
				work_array[3] = 0;
				work_array_n=1;
				best_score = scores.avg[i];
				printf("avg: (%d,%f)\n",i, scores.avg[i]);
			} else if (
				(scores.avg[i]-best_score) < EPS &&
				(scores.avg[i]-best_score) > -1*EPS
				) {
				// score equal and best are the same
				work_array[work_array_n++] = i;
				printf("avg: (%d,%f)\n",i, scores.avg[i]);
			}
		}
	}

	printf("work_array: ( ");
	for (i=0;i<N_MOVES;i++) {
		printf("%d ",work_array[i]);
	}
	printf(")\n");
	printf("work_array_n: %d\n",work_array_n);

	// Return the best move
	// Choose random from the values in the work array;
	i = rand() % work_array_n;
	printf("chosen - move:%d\n",work_array[i]);
	return work_array[i];
}

/**
 * Creates a new scores data structure to hold my scores
 */
scores_t scores_init() {
	scores_t s;
	uint8_t i;
	for (i=0;i<N_MOVES;i++) {
		s.avg[i] = 0;
		s.max[i] = 0;
		s.avg_n[i] = 0;
	}
	return s;
}

/**
 * A useful function for creating a node representation of an initial board
 */
void start_node(uint8_t newboard[SIZE][SIZE], node_t *start) {
	start->priority=0;
	start->depth=0;
	start->num_childs=0;
	start->parent=NULL;
	int i,j;
	for (i=0;i<SIZE;i++) {
		for (j=0;j<SIZE;j++) {
			start->board[i][j] = newboard[i][j];
		}
	}
};

/**
 * Not really a useful function for me since I didn't use any heuristics
 * Just propogates the number of children in the tree
 */
void propogate_back(node_t *node) {
	if (node->parent!=NULL) {
		node->parent->num_childs++;
		propogate_back(node->parent);
	}
}

/**
 * Handles the file output
 */
void output_head(int depth, propagation_t propagation) {
	FILE *fp;
	fp = fopen(FILENAME_EXP, "a");
	if (propagation == max) {
		fprintf(fp, "propagation: max,depth: %d\n", depth);
	} else {
		fprintf(fp, "propagation: avg,depth: %d\n", depth);
	}
	fprintf(fp, "max_depth\t");
	fprintf(fp, "max_tile\t");
	fprintf(fp, "score\t");
	fprintf(fp, "time\t");
	fprintf(fp, "generated\t");
	fprintf(fp, "expanded\t");
	fprintf(fp, "expanded/second\n");
	fflush(fp);
	fclose(fp);
}
/**
 * Handles the file output
 */
output_t *output_start(int mode) {
	output_t *op;
	op = (output_t*)malloc(sizeof(output_t));
	if(!op) {
		printf("Error allocating memory...\n");
		exit(-1);
	}
	op->fp = fopen(FILENAME_EXP, "a");
	op->mode = mode;
	op->start_time = clock();
	op->generated=0;
	op->expanded=0;
	return op;
}

void output_add_generated(output_t *op) {
	op->generated++;
}

void output_add_expanded(output_t *op) {
	op->expanded++;
}

void output_end(output_t op, uint8_t board[SIZE][SIZE], int score, int depth, int trial, propagation_t propagation) {
	int i,j,max=0;
	double elapsed;
	double expanded_p_millisec;

	elapsed = (clock()-op.start_time)*1000 / CLOCKS_PER_SEC;
	for (i=0;i<SIZE;i++) {
		for (j=0;j<SIZE;j++) {
			if (board[i][j]>max) {
				max = board[i][j];
			}
		}
	}
	expanded_p_millisec = op.expanded/elapsed;

	// Print the things
	if (op.mode == CSV_PRINTMODE) {
		fprintf(op.fp, "%d\t", trial);
		fprintf(op.fp, "%d\t", (int)propagation);
		fprintf(op.fp, "%d\t", depth);
		fprintf(op.fp, "%d\t", (int)pow(2,max));
		fprintf(op.fp, "%d\t", score);
		fprintf(op.fp, "%.4f\t", elapsed/1000);
		fprintf(op.fp, "%llu\t", op.generated);
		fprintf(op.fp, "%llu\t", op.expanded);
		fprintf(op.fp, "%.2f\n", expanded_p_millisec);
		fflush(op.fp);
		fclose(op.fp);
	} else {
		fprintf(op.fp, "max depth: %d\n", depth);
		fprintf(op.fp, "max tile: %d\n", (int)pow(2,max));
		fprintf(op.fp, "score: %d\n", score);
		fprintf(op.fp, "time: %.4f\n", elapsed/1000);
		fprintf(op.fp, "generated: %llu\n", op.generated);
		fprintf(op.fp, "expanded: %llu\n", op.expanded);
		fprintf(op.fp, "expanded/millisecond: %.2f", expanded_p_millisec);
		fflush(op.fp);
		fclose(op.fp);
	}
}



/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */
move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation, output_t *op ){

	node_t *start;
	node_t *tmp;
	node_t *new;
	scores_t scores;
	int i;

	// Get ready for the simulation
	scores = scores_init();
	start = (node_t *) malloc(sizeof(node_t));
	if(!start) {
		printf("Error allocating memory...\n");
		exit(-1);
	}
	start_node(board, start);
	heap_push(&h, start);

	while(h.count>0) {
		// Get highest priority node from the queue
		tmp = heap_delete(&h);
		output_add_expanded(op);
		if (tmp->depth<max_depth) {
			// For each move
			for (i=0;i<N_MOVES;i++) {
				// Create new node representing the new board
				new = NULL;
				new = (node_t *) malloc(sizeof(node_t));
				if(!new) {
					printf("Error allocating memory...\n");
					exit(-1);
				}
				*new = *tmp;
				// Use the new node if move was successful, discard otherwise
				if(execute_move_t(new->board, &new->priority, i)) {
					// Update state for the node and add it to the stack
					addRandom(new->board);
					new->num_childs=0;
					new->depth = tmp->depth+1;
					new->parent = tmp;
					if (new->parent->parent==NULL) {
						new->move = i;
					} else {
						new->move = tmp->move;	
					}
					propogate_back(new);
					scores_assign(new, &scores);
					heap_push(&h,new);
					output_add_generated(op);
				} else {
					free(new);
				}
			}
		} else {
			free(tmp);
		}
	}

	//printscores(&scores, tmp->depth, propagation);

	// Free up remaining memory from the simulation
	emptyPQ(&h);
	//free(tmp);
	if (max_depth!=0) {
		free(start);
	}

	return (move_t)scores_choose(scores, propagation);
}










