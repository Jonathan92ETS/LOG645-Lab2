#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <omp.h>


#define MATRIX_ROW_LENGTH 10
#define SLEEP_TIME 50000

void print_final_matrix(int *matrix, int k);
void init_matrix(int *matrix, int matrixSize, int starting_value);
int get_offset(int k, int i, int j);
void first_operation_seq(int *matrix, int k);
void first_operation_par(int *matrix, int k);
void second_operation_seq(int *matrix, int k);
void second_operation_par(int *matrix, int k);
void start_timer(double *timeStart);
double stop_timer(double *timeStart);


int main(int argc, char** argv) {
	const int K = atoi(argv[3]); 
	const int MATRIX_SIZE = MATRIX_ROW_LENGTH * MATRIX_ROW_LENGTH * (K+1);
	int starting_value = atoi(argv[2]);
	int matrix[MATRIX_SIZE];
	double timeStart;
	double tSeq, tPar, acc;

	if(atoi(argv[1]) == 1) {
		start_timer(&timeStart);
		init_matrix(matrix, MATRIX_SIZE, starting_value);
		first_operation_seq(matrix, K);
		print_final_matrix(matrix, K);
		printf("Séquentiel\n");
		tSeq = stop_timer(&timeStart);

		start_timer(&timeStart);
		init_matrix(matrix, MATRIX_SIZE, starting_value);
		first_operation_par(matrix, K);
		print_final_matrix(matrix, K);
		printf("Parallèle\n");
		tPar = stop_timer(&timeStart);
	}
	else {
		start_timer(&timeStart);
		init_matrix(matrix, MATRIX_SIZE, starting_value);
		second_operation_seq(matrix, K);
		print_final_matrix(matrix, K);
		printf("Séquentiel\n");
		tSeq = stop_timer(&timeStart);

		start_timer(&timeStart);
		init_matrix(matrix, MATRIX_SIZE, starting_value);
		second_operation_par(matrix, K);
		print_final_matrix(matrix, K);
		printf("Parallèle\n");
		tPar = stop_timer(&timeStart);
	}

	acc = tSeq/tPar;
	printf("Accéleration: %lf\n\n", acc );


}

void start_timer(double *timeStart) {
	struct timeval tp;
	gettimeofday (&tp, NULL); // Debut du chronometre
	*timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
}

double stop_timer(double *timeStart) {
	struct timeval tp;
	double timeEnd, Texec;
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - *timeStart; //Temps d'execution en secondes
	printf("Temps d\'éxecution: %lf\n\n", Texec);
	return Texec;

}

int get_offset(int k, int i, int j) { 
	return (k * MATRIX_ROW_LENGTH * MATRIX_ROW_LENGTH) + (i * MATRIX_ROW_LENGTH) + j; 
}

void first_operation_seq(int *matrix, int k) {
	int current_k, i, j;
	for (current_k = 1; current_k <= k; current_k++) {
		for (i = 0; i < MATRIX_ROW_LENGTH; i++) {
			for (j = 0; j < MATRIX_ROW_LENGTH; j++) {
				usleep(SLEEP_TIME);
				matrix[get_offset(current_k, i, j)] = matrix[get_offset(current_k - 1, i, j)] + ((i + j));
			}
		}
	}
}


void first_operation_par(int *matrix, int k) {
	int current_k, index, i, j;
	#pragma omp parallel private(current_k,index,i,j)
	{
		for (current_k = 1; current_k <= k; current_k++) {
			#pragma omp for
			for (index = 0; index < (MATRIX_ROW_LENGTH * MATRIX_ROW_LENGTH); index++) {
				i = index / MATRIX_ROW_LENGTH;
				j = index % MATRIX_ROW_LENGTH;
		
				usleep(SLEEP_TIME);
				matrix[get_offset(current_k, i, j)] = matrix[get_offset(current_k - 1, i, j)] + ((i + j));
			}
		}
	}
}


void second_operation_seq(int *matrix, int k) {
	int current_k, current_offset, value_at_previous_k, value_at_next_j, i, j;
	for (current_k = 1; current_k <= k; current_k++) {
		for (i = 0; i < MATRIX_ROW_LENGTH; i++) {
			for (j = MATRIX_ROW_LENGTH - 1; j >= 0; j--) {
				current_offset = get_offset(current_k, i, j);
				value_at_previous_k = matrix[get_offset(current_k - 1, i, j)];
				value_at_next_j = matrix[get_offset(current_k, i, j + 1)];

				usleep(SLEEP_TIME);
				if (j == (MATRIX_ROW_LENGTH - 1)) {
					matrix[current_offset] = value_at_previous_k + i;
				} else {
					matrix[current_offset] = value_at_previous_k + value_at_next_j;
				}
				
			}
		}
	}
}

void second_operation_par(int *matrix, int k) {
	int current_k, current_offset, i, j;
	#pragma omp parallel private(current_k, i, j, current_offset)
	{
		for (current_k = 1; current_k <= k; current_k++) {
			#pragma omp for
			for (i = 0; i < MATRIX_ROW_LENGTH; i++) {
				for (j = MATRIX_ROW_LENGTH - 1 + i; j >= 0 + i; j--) {
					
					current_offset = get_offset(current_k, i, j-i);

					usleep(SLEEP_TIME);
					if (j-i == (MATRIX_ROW_LENGTH - 1)) {
						matrix[current_offset] = matrix[get_offset(current_k-1, i, j-i)] + i;
					} else {
						matrix[current_offset] = matrix[get_offset(current_k-1, i, j-i)] + matrix[get_offset(current_k, i, j+1-i)];
					}
						
				}
			}
		}
	}
}

void init_matrix(int *matrix, int matrixSize, int startingValue) {
	int i;
	for (i = 0; i < matrixSize; i++) {
		matrix[i] = startingValue;
	}
}

void print_final_matrix(int *matrix, int k) {
	for (int i = 0; i < MATRIX_ROW_LENGTH; i++){
		for (int j = 0; j < MATRIX_ROW_LENGTH; j++) {
			printf("%7d \t", matrix[get_offset(k, i, j)]);
		}
		printf("\n");
	}
	printf("\n");
}
