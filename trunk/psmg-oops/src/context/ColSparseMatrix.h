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
