//-----------------------------------------------------------------------------------
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "platform.h"
#include "xil_printf.h"
#include "config.h"
#include "dma.h"
#include "PCP_IPs.h"
#include "FES_QP.h"
#include "xtime_l.h"
#include "math.h"

//-----------------------------------------------------------------------------------
int main() {
	init_platform(); // HW platform initialization
	xil_printf("Starting main...\n\r");

	axi_dma_init(); // Initializations of AXI DMAs

	fe_qf_dec(); // Functional encryption for quadratic functions: decrytpion algorithm

	xil_printf("Exiting main!\n\r");
	cleanup_platform();

	return 0;
}

//-----------------------------------------------------------------------------------
