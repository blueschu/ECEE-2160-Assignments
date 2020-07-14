/*
 * ECEE 2160 Homework assignment 1 main file.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-13
 * Due:     2020-07-17
 *
 */

#include "matrix.h"

#include <iostream>

int main()
{
    IntMatrix mat(3, 3);

    // Prompt the user for a matrix.
    std::cout << "Please enter a whitespace-delimited "
              << mat.rows() << " by " << mat.cols() << " matrix:\n";
    std::cin >> mat;

    // Print the user-provided matrix.
    std::cout << "Original matrix:\n" << mat << '\n';

    // Compute the transpose of the matrix in-place using array index syntax.
    mat.index_transpose_square();
    std::cout << "Applying transpose using array index syntax:\n" << mat << '\n';

    // Compute the transpose of the matrix in-place using pointer arithmetic.
    mat.pointer_transpose_square();
    std::cout << "Applying transpose using manual pointer arithmetic:\n" << mat << '\n';
}
