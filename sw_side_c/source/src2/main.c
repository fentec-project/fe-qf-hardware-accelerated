//-----------------------------------------------------------------------------------
#include "stdio.h"
#include "stdbool.h"
#include "platform.h"
#include "xil_printf.h"
#include "config.h"
#include "dma.h"
#include "PCP_IPs.h"
#include "CP_IPs.h"

//-----------------------------------------------------------------------------------
void cp_cores_init();
void cp_cores_test();
void cp_cores_paillier();
void cp_cores_mife_enc();
void cp_cores_mife_dec();

void pcp_core_test();
void pcp_core_optimal_ate_pairing();

//-----------------------------------------------------------------------------------
void cp_cores_init()
{
	cp_cores_data_init();
	cp_cores_dmem_init();
	xil_printf("cp_cores_init: cp_cores_data_init() and cp_cores_dmem_init() done.\n\r");
}

//-----------------------------------------------------------------------------------
void cp_cores_test()
{
	test_cp_ip00();
	test_cp_ip01();
	test_cp_ip02();
	test_cp_ip03();
	test_cp_ip04();
	test_cp_ip05();
	test_cp_ip06();
	test_cp_ip07();
	test_cp_ip08();
	test_cp_ip09();
	test_cp_ip10();
	test_cp_ip11();
	test_cp_ip12();
	test_cp_ip13();
	test_cp_ip14();
	test_cp_ip15();
	xil_printf("cp_cores_test: test_cp_ip00() ... test_cp_ip15() done.\n\r");
}

//-----------------------------------------------------------------------------------
void cp_cores_paillier()
{
	cp_ip00_paillier();
	xil_printf("cp_paillier: cp_ip00_paillier() done.\n\r");
}

//-----------------------------------------------------------------------------------
void cp_cores_mife_enc()
{
	cp_ip00_mife_enc();
	cp_ip01_mife_enc();
	cp_ip02_mife_enc();
	cp_ip03_mife_enc();
	cp_ip04_mife_enc();
	cp_ip05_mife_enc();
	cp_ip06_mife_enc();
	cp_ip07_mife_enc();
	cp_ip08_mife_enc();
	cp_ip09_mife_enc();
	cp_ip10_mife_enc();
	cp_ip11_mife_enc();
	cp_ip12_mife_enc();
	cp_ip13_mife_enc();
	cp_ip14_mife_enc();
	cp_ip15_mife_enc();
	xil_printf("cp_mife_enc: cp_ip00_mife_enc ... cp_ip15_mife_enc() done.\n\r");
}

//-----------------------------------------------------------------------------------
void cp_cores_mife_dec()
{
	init_invmod_mife_dec();
	cp_ip00_mife_dec();
	cp_ip01_mife_dec();
	cp_ip02_mife_dec();
	cp_ip03_mife_dec();
	cp_ip04_mife_dec();
	cp_ip05_mife_dec();
	cp_ip06_mife_dec();
	cp_ip07_mife_dec();
	cp_ip08_mife_dec();
	cp_ip09_mife_dec();
	cp_ip10_mife_dec();
	cp_ip11_mife_dec();
	cp_ip12_mife_dec();
	cp_ip13_mife_dec();
	cp_ip14_mife_dec();
	cp_ip15_mife_dec();
	final_ip_calc_mife_dec();
	xil_printf("cp_mife_dec: cp_ip00_mife_dec ... cp_ip15_mife_dec() done.\n\r");
}

//-----------------------------------------------------------------------------------
void pcp_core_test()
{
	dma_simple_tx_rx_tests(); // pcp core simple dma_tx and dma_rx tests
	xil_printf("pcp_core_test: dma_simple_tx_rx_tests() done.\n\r");
}

//-----------------------------------------------------------------------------------
void pcp_core_optimal_ate_pairing()
{
	pcp_optimalAte_pairing_calc();
	xil_printf("pcp_optimal_ate_pairing: pcp_optimalAte_pairing_calc() done.\n\r");
}

//-----------------------------------------------------------------------------------
int main()
{
	//--- Hardware Platform Initialization ---------------------------
	init_platform();
	xil_printf("\n\r*********** Starting main() ***********\n\r");

	//--- Initializations of AXI DMAs --------------------------------
	axi_dma_init();

	//--- Multi-CP Core Functions and Tsks ---------------------------
	cp_cores_init();
	cp_cores_test();
	cp_cores_paillier();
	cp_cores_mife_enc();
	cp_cores_mife_dec();

	//--- PCP Core Functions and Tasks -------------------------------
	pcp_core_test();
	pcp_core_optimal_ate_pairing();

	//----------------------------------------------------------------
	xil_printf("\n\r*********** Exiting main() ***********\n\r");
	cleanup_platform();

	//----------------------------------------------------------------
	return 0;
}

//-----------------------------------------------------------------------------------
