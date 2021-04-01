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
#define FPGA_PCP_IP0_CMD		XPAR_AXI_GPIO_17_BASEADDR
#define FPGA_PCP_IPs_STATUS		XPAR_AXI_GPIO_18_BASEADDR

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

//---------------------------------------------------------------------------------------
extern u8 unmapped_xP[];
extern u8 unmapped_yP[];

extern u8 unmapped_XQ_0[];
extern u8 unmapped_XQ_1[];

extern u8 unmapped_YQ_0[];
extern u8 unmapped_YQ_1[];

extern u8 unmapped_zero[];
extern u8 unmapped_unit[];

extern u8 unmapped_gamma11_0[];
extern u8 unmapped_gamma12_0[];
extern u8 unmapped_gamma13_0[];
extern u8 unmapped_gamma14_0[];
extern u8 unmapped_gamma15_0[];

extern u8 unmapped_gamma21_0[];
extern u8 unmapped_gamma22_0[];
extern u8 unmapped_gamma23_0[];
extern u8 unmapped_gamma24_0[];
extern u8 unmapped_gamma25_0[];

extern u8 unmapped_gamma31_0[];
extern u8 unmapped_gamma32_0[];
extern u8 unmapped_gamma33_0[];
extern u8 unmapped_gamma34_0[];
extern u8 unmapped_gamma35_0[];

//---------------------------------------------------------------------------------------
extern u8 xP[];
extern u8 yP[];

extern u8 XQ_0[];
extern u8 XQ_1[];

extern u8 YQ_0[];
extern u8 YQ_1[];

extern u8 zero[];
extern u8 unit[];

extern u8 gamma11_0[];
extern u8 gamma12_0[];
extern u8 gamma13_0[];
extern u8 gamma14_0[];
extern u8 gamma15_0[];

extern u8 gamma21_0[];
extern u8 gamma22_0[];
extern u8 gamma23_0[];
extern u8 gamma24_0[];
extern u8 gamma25_0[];

extern u8 gamma31_0[];
extern u8 gamma32_0[];
extern u8 gamma33_0[];
extern u8 gamma34_0[];
extern u8 gamma35_0[];

//---------------------------------------------------------------------------------------
extern u8 M[];
extern u8 Mp[];
extern u8 R[];
extern u8 R2[];
extern u8 Rm1[];

//---------------------------------------------------------------------------------------
extern u8 pcp_optimalAte_microcode[];

extern u8 pcp_init_microcode[];
extern u8 pcp_add_fp_microcode[];
extern u8 pcp_sub_fp_microcode[];
extern u8 pcp_neg_fp_microcode[];
extern u8 pcp_mul_fp_microcode[];
extern u8 pcp_add_fp2_microcode[];
extern u8 pcp_sub_fp2_microcode[];
extern u8 pcp_neg_fp2_microcode[];
extern u8 pcp_mul_fp2_microcode[];

//---------------------------------------------------------------------------------------
extern int s[];
extern int len_s;

extern int t_rglr[];
extern int len_t_rglr;

extern int t_sign[];
extern int len_t_sign;

extern int exp_pm2[];
extern int len_exp_pm2;

extern u8 tmp1[];
extern u8 tmp2[];

extern ElementFp2 gamma11, gamma12, gamma13, gamma14, gamma15;
extern ElementFp2 gamma21, gamma22, gamma23, gamma24, gamma25;
extern ElementFp2 gamma31, gamma32, gamma33, gamma34, gamma35;

extern ElementFp2 t_zero, keyci;

//---------------------------------------------------------------------------------------
extern int pcp_add_fp_num;
extern int pcp_sub_fp_num;
extern int pcp_neg_fp_num;
extern int pcp_mul_fp_num;
extern int pcp_sqr_fp_num;
extern int pcp_inv_fp_num;
extern int pcp_add_fp2_num;
extern int pcp_sub_fp2_num;
extern int pcp_neg_fp2_num;
extern int pcp_mul_fp2_num;
extern int pcp_sqr_fp2_num;
extern int pcp_inv_fp2_num;
extern int pcp_cnj_fp2_num;
extern int pcp_mul_keyci_fp2_num;
extern int pcp_mul_gamma_fp2_num;

//--- FPGA core functions ---------------------------------------------------------------
int fpga_pcp_ip0 (u8 pcp_imem_seg, u8 cmd_pcp, u32 pcp_dmem_addr, u8 *input, u8 *output);

//--- ARM task functions ----------------------------------------------------------------
void pcp_optimalAte_pairing_calc ();

void task_pcp_calc_ip0 ();
void task_pcp_ip0 ();

//--- Functions -------------------------------------------------------------------------
void pcp_calc_init ();

void pcp_map_fp (u8 *a, u8 *c);
void pcp_unmap_fp12 (ElementFp12 fi, ElementFp12 *fo);

void pcp_add_fp (u8 *a, u8 *b, u8 *c);
void pcp_sub_fp (u8 *a, u8 *b, u8 *c);
void pcp_neg_fp (u8 *a, u8 *c);
void pcp_eql_fp (u8 *a, u8 *c);
void pcp_mul_fp (u8 *a, u8 *b, u8 *c);
void pcp_sqr_fp (u8 *a, u8 *c);
void pcp_inv_l2r_fp (u8 *a, u8 *c);
void pcp_inv_r2l_fp (u8 *a, u8 *c);

void pcp_add_fp2 (ElementFp2 a, ElementFp2 b, ElementFp2 *c);
void pcp_sub_fp2 (ElementFp2 a, ElementFp2 b, ElementFp2 *c);
void pcp_neg_fp2 (ElementFp2 a, ElementFp2 *c);
void pcp_eql_fp2 (ElementFp2 a, ElementFp2 *c);
void pcp_mul_fp2 (ElementFp2 a, ElementFp2 b, ElementFp2 *c);
void pcp_sqr_fp2 (ElementFp2 a, ElementFp2 *c);
void pcp_inv_fp2 (ElementFp2 a, ElementFp2 *c);
void pcp_cnj_fp2 (ElementFp2 a, ElementFp2 *c);
void pcp_mul_keyci_fp2 (ElementFp2 a, ElementFp2 *c);
void pcp_mul_gamma_fp2 (ElementFp2 a, ElementFp2 gamma, ElementFp2 *c);

void pcp_add_fp4 (ElementFp4 a, ElementFp4 b, ElementFp4 *c);
void pcp_sub_fp4 (ElementFp4 a, ElementFp4 b, ElementFp4 *c);
void pcp_neg_fp4 (ElementFp4 a, ElementFp4 *c);
void pcp_eql_fp4 (ElementFp4 a, ElementFp4 *c);
void pcp_sqr_fp4 (ElementFp4 a, ElementFp4 *c);

void pcp_add_fp6 (ElementFp6 a, ElementFp6 b, ElementFp6 *c);
void pcp_sub_fp6 (ElementFp6 a, ElementFp6 b, ElementFp6 *c);
void pcp_neg_fp6 (ElementFp6 a, ElementFp6 *c);
void pcp_eql_fp6 (ElementFp6 a, ElementFp6 *c);
void pcp_mul_fp6 (ElementFp6 a, ElementFp6 b, ElementFp6 *c);
void pcp_sqr_fp6 (ElementFp6 a, ElementFp6 *c);
void pcp_mul_gamma_fp6 (ElementFp6 a, ElementFp6 *c);
void pcp_inv_fp6 (ElementFp6 a, ElementFp6 *c);

void pcp_add_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c);
void pcp_sub_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c);
void pcp_eql_fp12 (ElementFp12 a, ElementFp12 *c);
void pcp_cnj_fp12 (ElementFp12 a, ElementFp12 *c);
void pcp_mul_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c);
void pcp_sqr_fp12 (ElementFp12 a, ElementFp12 *c);
void pcp_csqr_fp12 (ElementFp12 a, ElementFp12 *c);
void pcp_inv_fp12 (ElementFp12 a, ElementFp12 *c);
void pcp_exp_fp12 (ElementFp12 a, int *exp, int len_exp, ElementFp12 *c);
void pcp_cexp_fp12 (ElementFp12 a, int *exp, int len_exp, ElementFp12 *c);

void pcp_point_doubling_line_evaluation (ElementFp2 XQ, ElementFp2 YQ, ElementFp2 ZQ, u8 *xP, u8 *yP,
										 ElementFp2 *XT, ElementFp2 *YT, ElementFp2 *ZT, ElementFp12 *ell);
void pcp_point_addition_line_evaluation (ElementFp2 XQ, ElementFp2 YQ, ElementFp2 ZQ, ElementFp2 XR, ElementFp2 YR, ElementFp2 ZR,
										 u8 *xP, u8 *yP, ElementFp2 *XT, ElementFp2 *YT, ElementFp2 *ZT, ElementFp12 *ell);

void pcp_Q1_mQ2_point_computation (ElementFp2 XQ, ElementFp2 YQ, ElementFp2 ZQ, ElementFp2 *XQ1, ElementFp2 *YQ1, ElementFp2 *ZQ1,
								   ElementFp2 *XmQ2, ElementFp2 *YmQ2, ElementFp2 *ZmQ2);

void pcp_frob_p  (ElementFp12 f, ElementFp12 *c);
void pcp_frob_p2 (ElementFp12 f, ElementFp12 *c);
void pcp_frob_p3 (ElementFp12 f, ElementFp12 *c);

void pcp_final_exponentiation (ElementFp12 fi, ElementFp12 *fo);

void pcp_print1_fp12 (const char *name, ElementFp12 f);
void pcp_print2_fp12 (const char *name, ElementFp12 f);

//----------------------------------------------------------------------------
#endif /* SRC_PCP_IPS_H_ */
