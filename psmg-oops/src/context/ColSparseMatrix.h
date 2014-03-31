/*
 * SparseMatrix.h
 *
 *  Created on: 17 Sep 2013
 *      Author: s0965328
 */

#ifndef COLSPARSEMATRIX_H_
#define COLSPARSEMATRIX_H_

class ColSparseMatrix {

public:
	double* values;
	int* rownos;
	int* colstarts;
	int* collen;
};

#endif /* COLSPARSEMATRIX_H_ */
