#ifndef SRC_PCP_IPS_H_
#define SRC_PCP_IPS_H_

//---------------------------------------------------------------------------------------
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xil_io.h"
#include "config.h"
#include "dma.h"
#include "stdbool.h"
//#include "FES_QP.h"

//--- Instruction memory length ---------------------------------------------------------
//#define PCP_IMEM_LD_LEN		0x100	// Each IMEM segment space
#define PCP_IMEM_LD_LEN			0x4000	// Whole IMEM space
#define PCP_IMEM_LD_UNIT_LEN	0x20	// Unit IMEM space
#define PCP_DMEM_LD_LEN			0x400	// Address space (2^10)
#define PCP_DMEM_RD_WR_LEN		0x20

//--- PCP commands ----------------------------------------------------------------------
#define	PCP_IMEM_LD		0x3
#define	PCP_DMEM_WR		0x5
#define	PCP_DMEM_RD		0x7
#define PCP_PROG_RUN	0x9

//--- PCP IMEM Segment ------------------------------------------------------------------
#define	PCP_INIT		0x0
#define	ADD_FP			0x1
#define	SUB_FP			0x2
#define	NEG_FP			0x3
#define	MUL_FP			0x4
#define	ADD_FP2			0x5
#define	SUB_FP2			0x6
#define	NEG_FP2			0x7
#define	MUL_FP2			0x8
// Reserved: 0x9 ... 0xE
#define	NO_SEG			0xF

//--- FPGA commands and status base address ---------------------------------------------
#define FPGA_PCP_IP0_CMD		XPAR_AXI_GPIO_0_BASEADDR
#define FPGA_PCP_IP1_CMD		XPAR_AXI_GPIO_1_BASEADDR
#define FPGA_PCP_IP2_CMD		XPAR_AXI_GPIO_2_BASEADDR
#define FPGA_PCP_IP3_CMD		XPAR_AXI_GPIO_3_BASEADDR
#define FPGA_PCP_IP4_CMD		XPAR_AXI_GPIO_4_BASEADDR
#define FPGA_PCP_IP5_CMD		XPAR_AXI_GPIO_5_BASEADDR
#define FPGA_PCP_IP6_CMD		XPAR_AXI_GPIO_6_BASEADDR
#define FPGA_PCP_IP7_CMD		XPAR_AXI_GPIO_7_BASEADDR
#define FPGA_PCP_IP8_CMD		XPAR_AXI_GPIO_8_BASEADDR
#define FPGA_PCP_IP9_CMD		XPAR_AXI_GPIO_9_BASEADDR
#define FPGA_PCP_IP10_CMD		XPAR_AXI_GPIO_10_BASEADDR
#define FPGA_PCP_IP11_CMD		XPAR_AXI_GPIO_11_BASEADDR
#define FPGA_PCP_IP12_CMD		XPAR_AXI_GPIO_12_BASEADDR
#define FPGA_PCP_IP13_CMD		XPAR_AXI_GPIO_13_BASEADDR
#define FPGA_PCP_IP14_CMD		XPAR_AXI_GPIO_14_BASEADDR
#define FPGA_PCP_IP15_CMD		XPAR_AXI_GPIO_15_BASEADDR

#define FPGA_PCP_IPs_STATUS		XPAR_AXI_GPIO_16_BASEADDR

#define HW_PCP_CORE_NO	16

#define FPGA_PCP_IP0	0
#define FPGA_PCP_IP1	1
#define FPGA_PCP_IP2	2
#define FPGA_PCP_IP3	3
#define FPGA_PCP_IP4	4
#define FPGA_PCP_IP5	5
#define FPGA_PCP_IP6	6
#define FPGA_PCP_IP7	7
#define FPGA_PCP_IP8	8
#define FPGA_PCP_IP9	9
#define FPGA_PCP_IP10	10
#define FPGA_PCP_IP11	11
#define FPGA_PCP_IP12	12
#define FPGA_PCP_IP13	13
#define FPGA_PCP_IP14	14
#define FPGA_PCP_IP15	15

//---------------------------------------------------------------------------------------
typedef struct ElementFp2 {
	u8 d0[PCP_DMEM_RD_WR_LEN];
	u8 d1[PCP_DMEM_RD_WR_LEN];
} ElementFp2;

typedef struct ElementFp4 {
	ElementFp2 d0;
	ElementFp2 d1;
} ElementFp4;

typedef struct ElementFp6 {
	ElementFp2 d0;
	ElementFp2 d1;
	ElementFp2 d2;
} ElementFp6;

typedef struct ElementFp12 {
	ElementFp6 d0;
	ElementFp6 d1;
} ElementFp12;

//--- FPGA core functions ---------------------------------------------------------------
int fpga_pcp_ip0 (u8 pcp_imem_seg, u8 cmd_pcp, u32 pcp_dmem_addr, u8 *input, u8 *output);
int fpga_pcp_ips (u8 pcp_imem_seg, u8 cmd_pcp, u32 pcp_dmem_addr, u8 *input, u8 *output, u8 pcp_core_id);

//--- ARM task functions ----------------------------------------------------------------
void pcp_optimalAte_pairing_calc ();

void task_pcp_calc_ip0 ();
void task_pcp_ip0 ();

//--- Functions -------------------------------------------------------------------------
void pcp_calc_init ();

//--- HW Functions of Multi-PCP core ----------------------------------------------------
void pcp_ips_init ();
ElementFp12 hw_optimalAte_pairing_calc (ElementFp4 G1, ElementFp2 G2, u8 pcp_core_id);

void pcp_map_fp (u8 *a, u8 *c, u8 pcp_core_id);
void pcp_unmap_fp12 (ElementFp12 fi, ElementFp12 *fo, u8 pcp_core_id);

void pcp_add_fp (u8 *a, u8 *b, u8 *c, u8 pcp_core_id);
void pcp_sub_fp (u8 *a, u8 *b, u8 *c, u8 pcp_core_id);
void pcp_neg_fp (u8 *a, u8 *c, u8 pcp_core_id);
void pcp_eql_fp (u8 *a, u8 *c, u8 pcp_core_id);
void eql_fp (u8 *a, u8 *c);
void pcp_mul_fp (u8 *a, u8 *b, u8 *c, u8 pcp_core_id);
void pcp_sqr_fp (u8 *a, u8 *c, u8 pcp_core_id);
void pcp_inv_l2r_fp (u8 *a, u8 *c, u8 pcp_core_id);
void pcp_inv_r2l_fp (u8 *a, u8 *c, u8 pcp_core_id);

void pcp_add_fp2 (ElementFp2 a, ElementFp2 b, ElementFp2 *c, u8 pcp_core_id);
void pcp_sub_fp2 (ElementFp2 a, ElementFp2 b, ElementFp2 *c, u8 pcp_core_id);
void pcp_neg_fp2 (ElementFp2 a, ElementFp2 *c, u8 pcp_core_id);
void pcp_eql_fp2 (ElementFp2 a, ElementFp2 *c, u8 pcp_core_id);
void pcp_mul_fp2 (ElementFp2 a, ElementFp2 b, ElementFp2 *c, u8 pcp_core_id);
void pcp_sqr_fp2 (ElementFp2 a, ElementFp2 *c, u8 pcp_core_id);
void pcp_inv_fp2 (ElementFp2 a, ElementFp2 *c, u8 pcp_core_id);
void pcp_cnj_fp2 (ElementFp2 a, ElementFp2 *c, u8 pcp_core_id);
void pcp_mul_keyci_fp2 (ElementFp2 a, ElementFp2 *c, u8 pcp_core_id);
void pcp_mul_gamma_fp2 (ElementFp2 a, ElementFp2 gamma, ElementFp2 *c, u8 pcp_core_id);

void pcp_add_fp4 (ElementFp4 a, ElementFp4 b, ElementFp4 *c, u8 pcp_core_id);
void pcp_sub_fp4 (ElementFp4 a, ElementFp4 b, ElementFp4 *c, u8 pcp_core_id);
void pcp_neg_fp4 (ElementFp4 a, ElementFp4 *c, u8 pcp_core_id);
void pcp_eql_fp4 (ElementFp4 a, ElementFp4 *c, u8 pcp_core_id);
void pcp_sqr_fp4 (ElementFp4 a, ElementFp4 *c, u8 pcp_core_id);

void pcp_add_fp6 (ElementFp6 a, ElementFp6 b, ElementFp6 *c, u8 pcp_core_id);
void pcp_sub_fp6 (ElementFp6 a, ElementFp6 b, ElementFp6 *c, u8 pcp_core_id);
void pcp_neg_fp6 (ElementFp6 a, ElementFp6 *c, u8 pcp_core_id);
void pcp_eql_fp6 (ElementFp6 a, ElementFp6 *c, u8 pcp_core_id);
void pcp_mul_fp6 (ElementFp6 a, ElementFp6 b, ElementFp6 *c, u8 pcp_core_id);
void pcp_sqr_fp6 (ElementFp6 a, ElementFp6 *c, u8 pcp_core_id);
void pcp_mul_gamma_fp6 (ElementFp6 a, ElementFp6 *c, u8 pcp_core_id);
void pcp_inv_fp6 (ElementFp6 a, ElementFp6 *c, u8 pcp_core_id);

void pcp_add_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c, u8 pcp_core_id);
void pcp_sub_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c, u8 pcp_core_id);
void pcp_eql_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id);
void pcp_cnj_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id);
void pcp_mul_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *, u8 pcp_core_id);
void pcp_sqr_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id);
void pcp_csqr_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id);
void pcp_inv_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id);
void pcp_exp_fp12 (ElementFp12 a, int *exp, int len_exp, ElementFp12 *c, u8 pcp_core_id);
void pcp_cexp_fp12 (ElementFp12 a, int *exp, int len_exp, ElementFp12 *c, u8 pcp_core_id);

void pcp_point_doubling_line_evaluation (ElementFp2 XQ, ElementFp2 YQ, ElementFp2 ZQ, u8 *xP, u8 *yP,
										 ElementFp2 *XT, ElementFp2 *YT, ElementFp2 *ZT, ElementFp12 *ell, u8 pcp_core_id);
void pcp_point_addition_line_evaluation (ElementFp2 XQ, ElementFp2 YQ, ElementFp2 ZQ, ElementFp2 XR, ElementFp2 YR, ElementFp2 ZR,
										 u8 *xP, u8 *yP, ElementFp2 *XT, ElementFp2 *YT, ElementFp2 *ZT, ElementFp12 *ell, u8 pcp_core_id);

void pcp_Q1_mQ2_point_computation (ElementFp2 XQ, ElementFp2 YQ, ElementFp2 ZQ, ElementFp2 *XQ1, ElementFp2 *YQ1, ElementFp2 *ZQ1,
								   ElementFp2 *XmQ2, ElementFp2 *YmQ2, ElementFp2 *ZmQ2, u8 pcp_core_id);

void pcp_frob_p  (ElementFp12 f, ElementFp12 *c, u8 pcp_core_id);
void pcp_frob_p2 (ElementFp12 f, ElementFp12 *c, u8 pcp_core_id);
void pcp_frob_p3 (ElementFp12 f, ElementFp12 *c, u8 pcp_core_id);

void pcp_final_exponentiation (ElementFp12 fi, ElementFp12 *fo, u8 pcp_core_id);

void pcp_print_fp12 (const char *name, ElementFp12 f);

//----------------------------------------------------------------------------
#endif /* SRC_PCP_IPS_H_ */
