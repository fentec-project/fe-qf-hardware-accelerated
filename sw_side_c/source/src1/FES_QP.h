#ifndef SRC_FES_QP_H_
#define SRC_FES_QP_H_

//---------------------------------------------------------------------------------------
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "platform.h"
#include "xil_printf.h"
#include "config.h"
#include "dma.h"
#include "PCP_IPs.h"
#include "math.h"
#include <time.h>

//---------------------------------------------------------------------------------------
typedef struct t_entry {
	__UINT64_TYPE__ hash;
	__UINT64_TYPE__ value;
} t_entry;

//--- Defines ---------------------------------------------------------------------------
#define	HASH_LENGTH		8 // Bytes

//#define PRE_COMP_REAL
//#define PRE_COMP_RAND
#define PRE_COMP_MEAS

#define GENERAL_F
//#define DIAGONAL_F

//--- Functions -------------------------------------------------------------------------
void fe_qf_dec ();

ElementFp12 fes_qp_pairings (int bound, int ell);
__UINT64_TYPE__ fes_qp_dis_log (int bound, int ell, ElementFp12 *out);

void fes_qp_shared_table_precomputation (int arr_size, int N, t_entry *table, ElementFp12 *alpha, ElementFp12 *delta, ElementFp12 *gamma);
__UINT64_TYPE__ fes_qp_shared_table_search (int arr_size, t_entry *table, __UINT64_TYPE__ fin);

int compare_entries(const void *a, const void *b);
void srnd_generation(__UINT64_TYPE__ *srnd_result);

void heapSort(t_entry *table, int arr_size);
void heapify(t_entry *table, int arr_size, int i);

//----------------------------------------------------------------------------
#endif /* SRC_FES_QP_H_ */
