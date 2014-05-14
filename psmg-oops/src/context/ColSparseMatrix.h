/*
 * SparseMatrix.h
 *
 *  Created on: 17 Sep 2013
 *      Author: s0965328
 */

#ifndef COLSPARSEMATRIX_H_
#define COLSPARSEMATRIX_H_

/*
 * The compressed column stroage for a sparse matrix
 */

typedef boost::numeric::ublas::compressed_matrix<double,boost::numeric::ublas::column_major,0,std::vector<std::size_t>,std::vector<double> >  col_compress_matrix;
typedef boost::numeric::ublas::matrix_row<col_compress_matrix> col_compress_matrix_row;
typedef boost::numeric::ublas::matrix_column<col_compress_matrix> col_compress_matrix_col;
typedef boost::numeric::ublas::matrix_range<col_compress_matrix > col_compress_matrix_range;

class ColSparseMatrix {


public:
	ColSparseMatrix(double* val, int* row, int* col, int* len):
		values(val),rownos(row),colstarts(col),collen(len) {};
	double* values;
	int* rownos;
	int* colstarts;
	int* collen;
};

#endif /* COLSPARSEMATRIX_H_ */
