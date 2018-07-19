#include "../src/include/matrix/matrix.h"

#include <iostream>

void matrix_test()
{
	math::matrix<double>matrix(3, 3);  // Create 3x3 zeros matrix 
	math::matrix<double>matrix2(matrix); // Create copy 

	std::cout << "Is matrix square :" << std::boolalpha << matrix.is_square() << std::endl;

	/* Fill matrix by some values */

	for (int i = 0; i < matrix2.size1(); i++) {
		for (int j = 0; j < matrix2.size2(); j++){
			matrix2(i, j) = 1 + j + i * matrix2.size2();
		}
	}

	/* Print:
	 * 1 2 3 
	 * 4 5 6 
	 * 7 8 9
	 */
	std::cout << matrix2 << std::endl;

	matrix = matrix2;				//Assigment operator

	std::cout << "Is matrixes equal: " << std::boolalpha << (matrix == matrix2) << std::endl;
	std::cout << "Is matrix sizes equal: " << std::boolalpha << matrix.size_equal(matrix2) << std::endl;

	matrix = std::move(matrix2);	// matrix2 is unavailable 

	math::matrix<double>matrix3(3, 3, 10);	// Create matrix consisting of 10

	auto plus = matrix + matrix3;

	/* Print:
	 * 11 12 13
	 * 14 15 16
	 * 17 18 19
	 */
	std::cout << "After addition:" << std::endl << plus << std::endl;

	auto minus = matrix - matrix3;

	/* Print:
	 * -9 -8 -7
	 * -6 -5 -4
     * -3 -2  -1
	 */
	std::cout << "After subtraction:" << std::endl << minus << std::endl;

	auto mult = matrix * matrix3;

	/* Print:
 	 * 60 60 60
	 * 150 150 150
	 * 240 240 240
	 */

	std::cout << "After multiplication:" << std::endl << mult << std::endl;

	matrix += matrix3;	// matrix values is changed

	/* Print:
	 * 11 12 13
	 * 14 15 16
 	 * 17 18 19
	 */

	std::cout << "After adding matrix3 to matrix: " << std::endl << matrix << std::endl;

	matrix -= matrix3; // matrix values is changed

	/* Print:
	 * 1 2 3
	 * 4 5 6
	 * 7 8 9
	 */

	std::cout << "After subtracting matrix3 from matrix: "  << std::endl << matrix << std::endl;

	matrix *= matrix3; // matrix values is changed


	/* Print:
	 * 60 60 60
	 * 150 150 150
	 * 240 240 240
	 */

	std::cout << "After multiplying matrix3 on matrix: " << std::endl << matrix << std::endl;

	matrix = matrix * 0.1; // multiplication by a number

	 /* Print:
	  * 6 6 6
	  * 15 15 15
	  * 24 24 24
	  */

	std::cout << "After multiplying matrix on 0.1: " << std::endl << matrix << std::endl;

	for (int i = 0; i < matrix.size1(); i++) {
		for (int j = 0; j < matrix.size2(); j++) {
			matrix(i, j) = 1 + j + i * matrix.size2();
		}
	}

	/* Extracting rows / column from matrix */

	auto column = matrix(math::_1, 1);		// Exctract second column

	/* Print:
	 *  2 
	 *  5 
	 *  8 
	 */
	std::cout << "Second column: " << std::endl << column << std::endl;

	auto row = matrix(2, math::_2);		// Exctract third row

	/* Print:
	 * 7 8 9
	 */
											
	std::cout << "Third row: " << row << std::endl;

	auto random = math::matrix<double>::generate(4, 4); // Generate random 4x4 matrix

	std::cout << "Random matrix: " << std::endl << random << std::endl;

	auto eye = math::matrix<double>::eye(4); // Generate eye 4x4 matrix


   /* Print:
    * 1 0 0 0
    * 0 1 0 0 
    * 0 0 1 0
	* 0 0 0 1
    */

	std::cout << "Eye matrix: " << std::endl << eye << std::endl;


	// Utility methods

	auto transpose = math::transpose(matrix);

	/* Print:
	 * 1 4 7
	 * 2 5 8
	 * 3 6 9
	 */

	std::cout << "Transposing matrix: " << std::endl << transpose << std::endl;

	auto determinant = math::determinant(matrix);

	// Print : 0

	std::cout << "Determinant of matrix: " << determinant << std::endl;

	matrix(2, 2) = 20;	// For inverse we need not null determinant value

	auto inverse = math::inverse(matrix);

	std::cout << "Inverse matrix: " << std::endl << inverse << std::endl;
}



int main() {

	matrix_test();

	return 0;
}