/*
* Function: print_final_matrix
* ----------------------------
*	Prints the last matrix that is the result of a series of operations
*
*   matrix: the 1D array containing the matrix
*   k: the k index where the last matrix resides
*
*/
void print_final_matrix(int *matrix, int k);

/*
* Function: init_matrix
* ----------------------------
*	Initializes each index of an array to a given value
*
*   matrix: the 1D array containing the matrixes
*   matrixSize: the size of one matrix (contained in the array)
*	starting_value: the value that will be used to set the indexes
*
*/
void init_matrix(int *matrix, int matrixSize, int starting_value);

/*
* Function: get_offset
* ----------------------------
*   Transforms a 3D set of params into a 1D param (index) in order to get equivalent index in a 1D array.
*
*   k: the k index (iteration #)
*   i: the i param of a 2D array (x)
*   j: the j param of a 2D array (y)
*
*   returns: the 1D index equivalent (offset)
*/
int get_offset(int k, int i, int j);

/*
* Function: first_operation
* ----------------------------
*	Performs the first operation of the lab
*
*   matrix: the 1D array containing the matrixes
*   k: the number of iterations
*
*/
void first_operation(int *matrix, int k);

/*
* Function: second_operation
* ----------------------------
*	Performs the second operation of the lab
*
*   matrix: the 1D array containing the matrixes
*   k: the number of iterations
*
*/
void second_operation(int *matrix, int k);

/*
* Function: set_console_size
* ----------------------------
*	Resizes the console
*
*/
void set_console_size();