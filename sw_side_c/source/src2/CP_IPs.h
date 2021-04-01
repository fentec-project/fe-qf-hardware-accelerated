#ifndef SRC_CP_IPS_H_
#define SRC_CP_IPS_H_

//---------------------------------------------------------------------------------------
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xil_io.h"
#include "stdbool.h"
#include "config.h"
#include "dma.h"

//--- Instruction memory length ---------------------------------------------------------
#define PROG_LD_LEN	0x1000	// 512 * 64b = 2^9 * 2^6b = 2^9 * 2^3B = 2^12B = 4KB

//---------------------------------------------------------------------------------------
//--- n: 1024-bit and g: 2048-bit public key pair; and 2048-bit modular operations ------
//#define PUK_n_WR_LEN	(0x080 + 0x008)	// 1024b: 16 * 64b = 2^4 * 2^3B = 2^7B
//#define PUK_n_RD_LEN	(0x080 + 0x000)	// 1024b: 16 * 64b = 2^4 * 2^3B = 2^7B

//#define PUK_g_WR_LEN	(0x100 + 0x008)	// 2048b: 32 * 64b = 2^5 * 2^3B = 2^8B
//#define PUK_g_RD_LEN	(0x100 + 0x000)	// 2048b: 32 * 64b = 2^5 * 2^3B = 2^8B

//#define L3L1_PARAM_WR_LEN	(0x100 + 0x008)	// 2048b: 32 * 64b = 2^5 * 2^3B = 2^8B
//#define L3L1_PARAM_RD_LEN	(0x100 + 0x000)	// 2048b: 32 * 64b = 2^5 * 2^3B = 2^8B

//#define L1L2_PARAM_WR_LEN	(29 * 8)	// 2048b: 29 * 72b
//#define L1L2_PARAM_RD_LEN	(29 * 8)	// 2048b: 29 * 72b

//---------------------------------------------------------------------------------------
//--- n: 2048-bit and g: 4096-bit public key pair; and 4096-bit modular operations ------
#define PUK_n_WR_LEN	(0x100 + 0x008)	// 2048b: 32 * 64b = 2^5 * 2^3B = 2^8B
#define PUK_n_RD_LEN	(0x100 + 0x000)	// 2048b: 32 * 64b = 2^5 * 2^3B = 2^8B

#define PUK_g_WR_LEN	(0x200 + 0x008)	// 4096b: 64 * 64b = 2^6 * 2^3B = 2^9B
#define PUK_g_RD_LEN	(0x200 + 0x000)	// 4096b: 64 * 64b = 2^6 * 2^3B = 2^9B

#define L3L1_PARAM_WR_LEN	(0x200 + 0x008)	// 4096b: 64 * 64b = 2^6 * 2^3B = 2^9B
#define L3L1_PARAM_RD_LEN	(0x200 + 0x000)	// 4096b: 64 * 64b = 2^6 * 2^3B = 2^9B

#define L1L2_PARAM_WR_LEN	(57 * 8)	// 4096b: 57 * 72b
#define L1L2_PARAM_RD_LEN	(57	* 8)	// 4096b: 57 * 72b

//---------------------------------------------------------------------------------------
#define CP_LD_UNIT_LEN		0x20

//---------------------------------------------------------------------------------------
//--- n: 4096-bit and g: 8192-bit public key pair; and 8192-bit modular operations ------
//#define PUK_n_WR_LEN	(0x200 + 0x008)	// 4096b: 64 * 64b = 2^6 * 2^3B = 2^9B
//#define PUK_n_RD_LEN	(0x200 + 0x000)	// 4096b: 64 * 64b = 2^6 * 2^3B = 2^9B

//#define PUK_g_WR_LEN	(0x400 + 0x008)	// 8192b: 128 * 64b = 2^7 * 2^3B = 2^10B
//#define PUK_g_RD_LEN	(0x400 + 0x000)	// 8192b: 128 * 64b = 2^7 * 2^3B = 2^10B

//#define L3L1_PARAM_WR_LEN	(0x400 + 0x008)	// 8192b: 128 * 64b = 2^7 * 2^3B = 2^10B
//#define L3L1_PARAM_RD_LEN	(0x400 + 0x000)	// 8192b: 128 * 64b = 2^7 * 2^3B = 2^10B

//#define L1L2_PARAM_WR_LEN	(114 * 8)	// 8192b: 114 * 72b
//#define L1L2_PARAM_RD_LEN	(114 * 8)	// 8192b: 114 * 72b

//--- CP commands -----------------------------------------------------------------------
#define	CP_IMEM_LD			0x3
#define	CP_L3L1_DMEM_WR		0x5
#define	CP_L3L1_DMEM_RD		0x7
#define	CP_L1L2_DMEM_WR		0x9
#define	CP_L1L2_DMEM_RD		0xB
#define CP_PROG_RUN			0xD

//--- Local DMEM partition (L1) ---------------------------------------------------------
#define X0_L1_PARAM		0
#define X1_L1_PARAM		1
#define X2_L1_PARAM		2
#define X3_L1_PARAM		3
#define X4_L1_PARAM		4
#define X5_L1_PARAM		5
#define X6_L1_PARAM		6
#define X7_L1_PARAM		7

#define R2_L1_PARAM		8
#define n2_L1_PARAM		9
#define n_L1_PARAM		10
#define g_L1_PARAM		11

//--- Global (Shared) DMEM partition (L2) -----------------------------------------------
#define Y0_L2_PARAM		0
#define Y1_L2_PARAM		1
#define Y2_L2_PARAM		2
#define Y3_L2_PARAM		3
#define Y4_L2_PARAM		4
#define Y5_L2_PARAM		5
#define Y6_L2_PARAM		6
#define Y7_L2_PARAM		7
#define Y8_L2_PARAM		8
#define Y9_L2_PARAM		9
#define Y10_L2_PARAM	10
#define Y11_L2_PARAM	11
#define Y12_L2_PARAM	12
#define Y13_L2_PARAM	13
#define Y14_L2_PARAM	14
#define Y15_L2_PARAM	15

#define NO_PARAM		255

//--- FPGA commands and status base address ---------------------------------------------
#define FPGA_CP_IP0_CMD		XPAR_AXI_GPIO_0_BASEADDR
#define FPGA_CP_IP1_CMD		XPAR_AXI_GPIO_1_BASEADDR
#define FPGA_CP_IP2_CMD		XPAR_AXI_GPIO_2_BASEADDR
#define FPGA_CP_IP3_CMD		XPAR_AXI_GPIO_3_BASEADDR
#define FPGA_CP_IP4_CMD		XPAR_AXI_GPIO_4_BASEADDR
#define FPGA_CP_IP5_CMD		XPAR_AXI_GPIO_5_BASEADDR
#define FPGA_CP_IP6_CMD		XPAR_AXI_GPIO_6_BASEADDR
#define FPGA_CP_IP7_CMD		XPAR_AXI_GPIO_7_BASEADDR
#define FPGA_CP_IP8_CMD		XPAR_AXI_GPIO_8_BASEADDR
#define FPGA_CP_IP9_CMD		XPAR_AXI_GPIO_9_BASEADDR
#define FPGA_CP_IP10_CMD	XPAR_AXI_GPIO_10_BASEADDR
#define FPGA_CP_IP11_CMD	XPAR_AXI_GPIO_11_BASEADDR
#define FPGA_CP_IP12_CMD	XPAR_AXI_GPIO_12_BASEADDR
#define FPGA_CP_IP13_CMD	XPAR_AXI_GPIO_13_BASEADDR
#define FPGA_CP_IP14_CMD	XPAR_AXI_GPIO_14_BASEADDR
#define FPGA_CP_IP15_CMD	XPAR_AXI_GPIO_15_BASEADDR

#define FPGA_CP_IPs_STATUS	XPAR_AXI_GPIO_16_BASEADDR

//---------------------------------------------------------------------------------------
extern u8 Micro_Code_Init[PROG_LD_LEN];
extern u8 Micro_Code_Prog_MMM[PROG_LD_LEN];
extern u8 Micro_Code_Prog_ME[PROG_LD_LEN];

//---------------------------------------------------------------------------------------
extern u8 DMEM_IN_N[L3L1_PARAM_WR_LEN];
extern u8 DMEM_IN_g[L3L1_PARAM_WR_LEN];

extern u8 h0_0[L3L1_PARAM_WR_LEN];
extern u8 h0_1[L3L1_PARAM_WR_LEN];
extern u8 h0_2[L3L1_PARAM_WR_LEN];
extern u8 h0_3[L3L1_PARAM_WR_LEN];
extern u8 h0_4[L3L1_PARAM_WR_LEN];
extern u8 h0_5[L3L1_PARAM_WR_LEN];
extern u8 h0_6[L3L1_PARAM_WR_LEN];
extern u8 h0_7[L3L1_PARAM_WR_LEN];
extern u8 h0_8[L3L1_PARAM_WR_LEN];
extern u8 h0_9[L3L1_PARAM_WR_LEN];
extern u8 h0_10[L3L1_PARAM_WR_LEN];
extern u8 h0_11[L3L1_PARAM_WR_LEN];
extern u8 h0_12[L3L1_PARAM_WR_LEN];
extern u8 h0_13[L3L1_PARAM_WR_LEN];
extern u8 h0_14[L3L1_PARAM_WR_LEN];
extern u8 h0_15[L3L1_PARAM_WR_LEN];

extern u32 u0[16];
extern u32 u1[16];
extern u32 u2[16];
extern u32 u3[16];

extern u8 x0[16];
extern u8 x1[16];
extern u8 x2[16];
extern u8 x3[16];

extern u8 y0[16];
extern u8 y1[16];
extern u8 y2[16];
extern u8 y3[16];

extern u8 w0_0[L3L1_PARAM_WR_LEN];
extern u8 w0_1[L3L1_PARAM_WR_LEN];
extern u8 w0_2[L3L1_PARAM_WR_LEN];
extern u8 w0_3[L3L1_PARAM_WR_LEN];
extern u8 w0_4[L3L1_PARAM_WR_LEN];
extern u8 w0_5[L3L1_PARAM_WR_LEN];
extern u8 w0_6[L3L1_PARAM_WR_LEN];
extern u8 w0_7[L3L1_PARAM_WR_LEN];
extern u8 w0_8[L3L1_PARAM_WR_LEN];
extern u8 w0_9[L3L1_PARAM_WR_LEN];
extern u8 w0_10[L3L1_PARAM_WR_LEN];
extern u8 w0_11[L3L1_PARAM_WR_LEN];
extern u8 w0_12[L3L1_PARAM_WR_LEN];
extern u8 w0_13[L3L1_PARAM_WR_LEN];
extern u8 w0_14[L3L1_PARAM_WR_LEN];
extern u8 w0_15[L3L1_PARAM_WR_LEN];

extern u8 mife_rnd[L3L1_PARAM_WR_LEN];

extern u8 y0_0[L3L1_PARAM_WR_LEN];
extern u8 y0_1[L3L1_PARAM_WR_LEN];
extern u8 y0_2[L3L1_PARAM_WR_LEN];
extern u8 y0_3[L3L1_PARAM_WR_LEN];
extern u8 y0_4[L3L1_PARAM_WR_LEN];
extern u8 y0_5[L3L1_PARAM_WR_LEN];
extern u8 y0_6[L3L1_PARAM_WR_LEN];
extern u8 y0_7[L3L1_PARAM_WR_LEN];
extern u8 y0_8[L3L1_PARAM_WR_LEN];
extern u8 y0_9[L3L1_PARAM_WR_LEN];
extern u8 y0_10[L3L1_PARAM_WR_LEN];
extern u8 y0_11[L3L1_PARAM_WR_LEN];
extern u8 y0_12[L3L1_PARAM_WR_LEN];
extern u8 y0_13[L3L1_PARAM_WR_LEN];
extern u8 y0_14[L3L1_PARAM_WR_LEN];
extern u8 y0_15[L3L1_PARAM_WR_LEN];

extern u8 y1_0[L3L1_PARAM_WR_LEN];
extern u8 y1_1[L3L1_PARAM_WR_LEN];
extern u8 y1_2[L3L1_PARAM_WR_LEN];
extern u8 y1_3[L3L1_PARAM_WR_LEN];
extern u8 y1_4[L3L1_PARAM_WR_LEN];
extern u8 y1_5[L3L1_PARAM_WR_LEN];
extern u8 y1_6[L3L1_PARAM_WR_LEN];
extern u8 y1_7[L3L1_PARAM_WR_LEN];
extern u8 y1_8[L3L1_PARAM_WR_LEN];
extern u8 y1_9[L3L1_PARAM_WR_LEN];
extern u8 y1_10[L3L1_PARAM_WR_LEN];
extern u8 y1_11[L3L1_PARAM_WR_LEN];
extern u8 y1_12[L3L1_PARAM_WR_LEN];
extern u8 y1_13[L3L1_PARAM_WR_LEN];
extern u8 y1_14[L3L1_PARAM_WR_LEN];
extern u8 y1_15[L3L1_PARAM_WR_LEN];

extern u8 y2_0[L3L1_PARAM_WR_LEN];
extern u8 y2_1[L3L1_PARAM_WR_LEN];
extern u8 y2_2[L3L1_PARAM_WR_LEN];
extern u8 y2_3[L3L1_PARAM_WR_LEN];
extern u8 y2_4[L3L1_PARAM_WR_LEN];
extern u8 y2_5[L3L1_PARAM_WR_LEN];
extern u8 y2_6[L3L1_PARAM_WR_LEN];
extern u8 y2_7[L3L1_PARAM_WR_LEN];
extern u8 y2_8[L3L1_PARAM_WR_LEN];
extern u8 y2_9[L3L1_PARAM_WR_LEN];
extern u8 y2_10[L3L1_PARAM_WR_LEN];
extern u8 y2_11[L3L1_PARAM_WR_LEN];
extern u8 y2_12[L3L1_PARAM_WR_LEN];
extern u8 y2_13[L3L1_PARAM_WR_LEN];
extern u8 y2_14[L3L1_PARAM_WR_LEN];
extern u8 y2_15[L3L1_PARAM_WR_LEN];

extern u8 y3_0[L3L1_PARAM_WR_LEN];
extern u8 y3_1[L3L1_PARAM_WR_LEN];
extern u8 y3_2[L3L1_PARAM_WR_LEN];
extern u8 y3_3[L3L1_PARAM_WR_LEN];
extern u8 y3_4[L3L1_PARAM_WR_LEN];
extern u8 y3_5[L3L1_PARAM_WR_LEN];
extern u8 y3_6[L3L1_PARAM_WR_LEN];
extern u8 y3_7[L3L1_PARAM_WR_LEN];
extern u8 y3_8[L3L1_PARAM_WR_LEN];
extern u8 y3_9[L3L1_PARAM_WR_LEN];
extern u8 y3_10[L3L1_PARAM_WR_LEN];
extern u8 y3_11[L3L1_PARAM_WR_LEN];
extern u8 y3_12[L3L1_PARAM_WR_LEN];
extern u8 y3_13[L3L1_PARAM_WR_LEN];
extern u8 y3_14[L3L1_PARAM_WR_LEN];
extern u8 y3_15[L3L1_PARAM_WR_LEN];

extern u8 d0[L3L1_PARAM_WR_LEN];
extern u8 d1[L3L1_PARAM_WR_LEN];
extern u8 d2[L3L1_PARAM_WR_LEN];
extern u8 d3[L3L1_PARAM_WR_LEN];

extern u8 c0_0[L3L1_PARAM_WR_LEN];
extern u8 c0_1[L3L1_PARAM_WR_LEN];
extern u8 c0_2[L3L1_PARAM_WR_LEN];
extern u8 c0_3[L3L1_PARAM_WR_LEN];
extern u8 c0_4[L3L1_PARAM_WR_LEN];
extern u8 c0_5[L3L1_PARAM_WR_LEN];
extern u8 c0_6[L3L1_PARAM_WR_LEN];
extern u8 c0_7[L3L1_PARAM_WR_LEN];
extern u8 c0_8[L3L1_PARAM_WR_LEN];
extern u8 c0_9[L3L1_PARAM_WR_LEN];
extern u8 c0_10[L3L1_PARAM_WR_LEN];
extern u8 c0_11[L3L1_PARAM_WR_LEN];
extern u8 c0_12[L3L1_PARAM_WR_LEN];
extern u8 c0_13[L3L1_PARAM_WR_LEN];
extern u8 c0_14[L3L1_PARAM_WR_LEN];
extern u8 c0_15[L3L1_PARAM_WR_LEN];
extern u8 c0_16[L3L1_PARAM_WR_LEN];

extern u8 c1_0[L3L1_PARAM_WR_LEN];
extern u8 c1_1[L3L1_PARAM_WR_LEN];
extern u8 c1_2[L3L1_PARAM_WR_LEN];
extern u8 c1_3[L3L1_PARAM_WR_LEN];
extern u8 c1_4[L3L1_PARAM_WR_LEN];
extern u8 c1_5[L3L1_PARAM_WR_LEN];
extern u8 c1_6[L3L1_PARAM_WR_LEN];
extern u8 c1_7[L3L1_PARAM_WR_LEN];
extern u8 c1_8[L3L1_PARAM_WR_LEN];
extern u8 c1_9[L3L1_PARAM_WR_LEN];
extern u8 c1_10[L3L1_PARAM_WR_LEN];
extern u8 c1_11[L3L1_PARAM_WR_LEN];
extern u8 c1_12[L3L1_PARAM_WR_LEN];
extern u8 c1_13[L3L1_PARAM_WR_LEN];
extern u8 c1_14[L3L1_PARAM_WR_LEN];
extern u8 c1_15[L3L1_PARAM_WR_LEN];
extern u8 c1_16[L3L1_PARAM_WR_LEN];

extern u8 c2_0[L3L1_PARAM_WR_LEN];
extern u8 c2_1[L3L1_PARAM_WR_LEN];
extern u8 c2_2[L3L1_PARAM_WR_LEN];
extern u8 c2_3[L3L1_PARAM_WR_LEN];
extern u8 c2_4[L3L1_PARAM_WR_LEN];
extern u8 c2_5[L3L1_PARAM_WR_LEN];
extern u8 c2_6[L3L1_PARAM_WR_LEN];
extern u8 c2_7[L3L1_PARAM_WR_LEN];
extern u8 c2_8[L3L1_PARAM_WR_LEN];
extern u8 c2_9[L3L1_PARAM_WR_LEN];
extern u8 c2_10[L3L1_PARAM_WR_LEN];
extern u8 c2_11[L3L1_PARAM_WR_LEN];
extern u8 c2_12[L3L1_PARAM_WR_LEN];
extern u8 c2_13[L3L1_PARAM_WR_LEN];
extern u8 c2_14[L3L1_PARAM_WR_LEN];
extern u8 c2_15[L3L1_PARAM_WR_LEN];
extern u8 c2_16[L3L1_PARAM_WR_LEN];

extern u8 c3_0[L3L1_PARAM_WR_LEN];
extern u8 c3_1[L3L1_PARAM_WR_LEN];
extern u8 c3_2[L3L1_PARAM_WR_LEN];
extern u8 c3_3[L3L1_PARAM_WR_LEN];
extern u8 c3_4[L3L1_PARAM_WR_LEN];
extern u8 c3_5[L3L1_PARAM_WR_LEN];
extern u8 c3_6[L3L1_PARAM_WR_LEN];
extern u8 c3_7[L3L1_PARAM_WR_LEN];
extern u8 c3_8[L3L1_PARAM_WR_LEN];
extern u8 c3_9[L3L1_PARAM_WR_LEN];
extern u8 c3_10[L3L1_PARAM_WR_LEN];
extern u8 c3_11[L3L1_PARAM_WR_LEN];
extern u8 c3_12[L3L1_PARAM_WR_LEN];
extern u8 c3_13[L3L1_PARAM_WR_LEN];
extern u8 c3_14[L3L1_PARAM_WR_LEN];
extern u8 c3_15[L3L1_PARAM_WR_LEN];
extern u8 c3_16[L3L1_PARAM_WR_LEN];

extern u8 invmod_mife_dec_vec[L3L1_PARAM_RD_LEN];

extern u8 mife_dec_r0_vec[L3L1_PARAM_WR_LEN];
extern u8 mife_dec_r1_vec[L3L1_PARAM_WR_LEN];
extern u8 mife_dec_r2_vec[L3L1_PARAM_WR_LEN];
extern u8 mife_dec_r3_vec[L3L1_PARAM_WR_LEN];

//---------------------------------------------------------------------------------------
void cp_cores_data_init();
void cp_cores_dmem_init();

//--- FPGA core functions ---------------------------------------------------------------
int fpga_cp_ip00 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip01 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip02 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip03 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip04 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip05 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip06 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip07 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip08 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip09 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip10 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip11 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip12 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip13 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip14 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);
int fpga_cp_ip15 (u8 cmd_cp, u8 L1_param, u8 L2_param, u8 *input, u32 lenght, u8 *output);

//--- ARM task functions ----------------------------------------------------------------
// Squared Euclidean Distances (SED) functions
void test_cp_ip00 ();
void test_cp_ip01 ();
void test_cp_ip02 ();
void test_cp_ip03 ();
void test_cp_ip04 ();
void test_cp_ip05 ();
void test_cp_ip06 ();
void test_cp_ip07 ();
void test_cp_ip08 ();
void test_cp_ip09 ();
void test_cp_ip10 ();
void test_cp_ip11 ();
void test_cp_ip12 ();
void test_cp_ip13 ();
void test_cp_ip14 ();
void test_cp_ip15 ();

//----------------------------------------
void cp_ip00_paillier ();

// Multi-Input Functional Encryption (MIFE) functions (inner-product computation)
void cp_ip00_mife_enc ();
void cp_ip01_mife_enc ();
void cp_ip02_mife_enc ();
void cp_ip03_mife_enc ();
void cp_ip04_mife_enc ();
void cp_ip05_mife_enc ();
void cp_ip06_mife_enc ();
void cp_ip07_mife_enc ();
void cp_ip08_mife_enc ();
void cp_ip09_mife_enc ();
void cp_ip10_mife_enc ();
void cp_ip11_mife_enc ();
void cp_ip12_mife_enc ();
void cp_ip13_mife_enc ();
void cp_ip14_mife_enc ();
void cp_ip15_mife_enc ();

//----------------------------------------
void init_invmod_mife_dec ();
void cp_ip00_mife_dec ();
void cp_ip01_mife_dec ();
void cp_ip02_mife_dec ();
void cp_ip03_mife_dec ();
void cp_ip04_mife_dec ();
void cp_ip05_mife_dec ();
void cp_ip06_mife_dec ();
void cp_ip07_mife_dec ();
void cp_ip08_mife_dec ();
void cp_ip09_mife_dec ();
void cp_ip10_mife_dec ();
void cp_ip11_mife_dec ();
void cp_ip12_mife_dec ();
void cp_ip13_mife_dec ();
void cp_ip14_mife_dec ();
void cp_ip15_mife_dec ();
void final_ip_calc_mife_dec ();

//----------------------------------------
#endif /* SRC_CP_IPS_H_ */
