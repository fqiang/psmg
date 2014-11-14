/*
 * autodiff_types.h
 *
 *  Created on: 18 Apr 2013
 *      Author: s0965328
 */

#include "autodiff_marco.h"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

#ifndef AUTO_DIFF_TYPES_H_
#define AUTO_DIFF_TYPES_H_


namespace AutoDiff{

//column compressed matrix types
typedef boost::numeric::ublas::compressed_matrix<double,boost::numeric::ublas::column_major,0,std::vector<std::size_t>,std::vector<double> >  col_compress_matrix;
typedef boost::numeric::ublas::matrix_row<col_compress_matrix> col_compress_matrix_row;
typedef boost::numeric::ublas::matrix_column<col_compress_matrix> col_compress_matrix_col;
typedef boost::numeric::ublas::matrix_range<col_compress_matrix > col_compress_matrix_range;

//indicator matrix types
typedef boost::numeric::ublas::compressed_matrix<uint,boost::numeric::ublas::column_major,0,std::vector<std::size_t>,std::vector<uint> >  col_compress_imatrix;
typedef boost::numeric::ublas::matrix_row<col_compress_imatrix> col_compress_imatrix_row;
typedef boost::numeric::ublas::matrix_column<col_compress_imatrix> col_compress_imatrix_col;
typedef boost::numeric::ublas::matrix_range<col_compress_imatrix > col_compress_imatrix_range;

typedef unsigned int uint;
typedef unsigned long ulong;

#define NaN_Double std::numeric_limits<double>::quiet_NaN()

typedef enum { OPNode_Type=0, VNode_Type, PNode_Type} TYPE;


typedef enum {OP_PLUS=0, OP_MINUS, OP_TIMES, OP_DIVID, OP_SIN, OP_COS, OP_SQRT, OP_POW, OP_NEG} OPCODE;

#if FORWARD_ENABLED
extern uint num_var;
#endif

}
#endif /* AUTO_DIFF_TYPES_H_ */
