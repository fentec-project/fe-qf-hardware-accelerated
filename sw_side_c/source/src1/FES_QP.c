//----------------------------------------------------------------------------
#include "FES_QP.h"

ElementFp12 fes_qp_pairings (int bound, int ell) {
	//========================================================================
	//===== First phase: Pairings computations etc. fes_qp_pairings() ========
	//========================================================================
	xil_printf("Start of first phase...\n\r");

	pcp_ips_init();	// HW PCP cores initialization

	//========================================================================
	// G1 and G2 inputs setting for pairing computations (generators)
	u8 G1x0[] = "\x19\xB0\xBE\xA4\xAF\xE4\xC3\x30\xDA\x93\xCC\x35\x33\xDA\x38\xA9\xF4\x30\xB4\x71\xC6\xF8\xA5\x36\xE8\x19\x62\xED\x96\x79\x09\xB5";
	u8 G1x1[] = "\x0A\x1C\xF5\x85\x58\x5A\x61\xC6\xE9\x88\x0B\x1F\x2A\x5C\x53\x9F\x7D\x90\x6F\xFF\x23\x8F\xA6\x34\x1E\x1D\xE1\xA2\xE4\x5C\x3F\x72";
	u8 G1y0[] = "\x17\xAB\xD3\x66\xEB\xBD\x65\x33\x3E\x49\xC7\x11\xA8\x0A\x0C\xF6\xD2\x4A\xDF\x1B\x9B\x39\x90\xEE\xDC\xC9\x17\x31\x38\x4D\x26\x27";
	u8 G1y1[] = "\x0E\xE9\x7D\x6D\xE9\x90\x2A\x27\xD0\x0E\x95\x22\x32\xA7\x87\x00\x86\x3B\xC9\xAA\x9B\xE9\x60\xC3\x2F\x5B\xF9\xFD\x0A\x32\xD3\x45";
	u8 G2x[]  = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01";
	u8 G2y[]  = "\x16\x2B\xA2\x69\x87\xB1\x64\xC7\xA3\x67\xCF\xC4\xF9\x4B\x73\xA7\xFA\x1A\x3F\xDC\x81\xB1\x1E\xBB\x7B\x3D\xBA\x6D\x98\x83\x33\x51";

//	u8 gT_0_0_0[] = "\x04\x45\x8B\x6B\xB7\xEF\x0D\xDA\x02\xB9\xAD\x61\x3E\x44\x09\xB2\xD6\xDF\x24\xF0\xC1\x85\xFA\x2D\x33\x00\x00\xAB\xAB\xAB\xAB\xAB";
	u8 gT_0_0_0[] = "\x04\x45\x8B\x6B\xB7\xEF\x0D\xDA\x02\xB9\xAD\x61\x3E\x44\x09\xB2\xD6\xDF\x24\xF0\xC1\x85\xFA\x2D\x78\x12\x3C\xA6\xF7\x7D\x07\xDA";
	u8 gT_0_0_1[] = "\x22\x31\x01\x71\x30\xD2\xFA\xB5\x95\xF7\xE6\x5D\x65\x23\xC9\xA0\x00\x19\x4B\x87\xEC\xAA\x4C\x7E\xA3\x8F\xD6\x52\x1A\xFD\x5A\x71";
	u8 gT_0_1_0[] = "\x0A\xD3\x46\xBD\x68\x8C\xC0\x84\xEA\xFD\x40\x46\xC8\x91\x7E\x0F\xA9\xAB\x4A\x57\xC3\x80\x30\xA1\x38\xD9\x2D\x2C\x01\xE7\xAE\xD8";
	u8 gT_0_1_1[] = "\x17\x15\x85\x47\x5D\x4F\xF2\x1F\x16\xD9\x8A\x1D\x4F\xE6\x02\x60\x02\x91\x39\x5C\x2B\xB9\x04\x10\x11\x0E\x3D\x37\x1D\xEB\xB5\xBE";
	u8 gT_0_2_0[] = "\x22\x58\x1D\xE9\x73\x33\x19\x65\xD6\xD9\x9E\x91\xE0\x99\xF7\x10\x3F\xC1\xAD\xAE\x7F\xF1\x44\xB2\x88\x37\x00\xE8\xA6\x2C\x73\x6D";
	u8 gT_0_2_1[] = "\x1A\x1A\xEA\x16\xEA\x2F\x8A\x1A\x83\xBB\xB9\x4F\x31\x30\x17\xD4\xD2\x19\x93\x42\x99\xF1\x64\xA4\xCF\x81\xD2\x38\xBA\x1A\x28\xF7";
	u8 gT_1_0_0[] = "\x15\x5D\x2C\x04\x09\xC1\x81\x83\x86\xD0\x79\x9D\x3E\xCF\xA5\x40\xA2\xDC\x5F\x45\x14\xB2\x2B\x44\x9E\x9F\x6E\xFB\x3B\x52\x2E\x7B";
	u8 gT_1_0_1[] = "\x02\xF8\x0E\xAA\x1B\x16\xB8\x08\x57\x9C\xC2\x11\xF3\x13\x11\x0D\xB7\x04\xBE\x35\x1E\x11\x27\x0C\xDE\xF1\x31\x11\x35\x8D\x6E\x33";
	u8 gT_1_1_0[] = "\x18\x8D\x51\x5D\x73\x7A\x5B\x74\xB0\x0D\x57\x19\xAC\xC7\xD6\x8C\x6C\xD5\xB2\x4A\x2C\x31\x6B\x42\xDE\xB0\xB3\x73\x61\xF5\x6E\x62";
	u8 gT_1_1_1[] = "\x04\x63\x6F\xBB\x4F\xD4\x11\x56\xB2\x31\xC4\x0A\x4C\x43\x7E\xFD\x51\x3E\x8C\x0F\x3C\x12\x77\x64\x1E\xAE\x21\x8D\xFB\xD4\x6A\x9E";
	u8 gT_1_2_0[] = "\x1B\xFF\xB5\xC4\xE9\xF7\xAF\x88\xA2\x7D\x08\x8B\x0C\xB5\x98\xCB\x70\x3C\x35\xD3\x2D\xAA\x47\x8E\x32\x8E\x7C\xE3\x04\xB8\x44\x44";
	u8 gT_1_2_1[] = "\x1C\x4B\xD4\x8B\xBD\xCA\xFB\x47\x8F\x11\xEB\x0D\xEE\x74\x39\x7D\xF2\x95\xB5\x92\x55\xF8\x71\x8F\x3C\xF7\x68\x48\x8C\xE7\x66\x0E";

	// Required microcodes segment/full packs for Optimal-Ate pairing computation are loaded initially in
	// the shared_IMEM of the HW side for runnig by all 16 PCP cores based on common CMD and STS signals

	//===== Input, intermediate, and output variables setting phase ==================
	int N = HW_PCP_CORE_NO;
	ElementFp4 *G1_in;
	ElementFp2 *G2_in;
	G1_in = (ElementFp4 *) malloc (1 * sizeof(ElementFp4));
	G2_in = (ElementFp2 *) malloc (1 * sizeof(ElementFp2));

	eql_fp(G1x0, G1_in->d0.d0);
	eql_fp(G1x1, G1_in->d0.d1);
	eql_fp(G1y0, G1_in->d1.d0);
	eql_fp(G1y1, G1_in->d1.d1);
	eql_fp(G2x, G2_in->d0);
	eql_fp(G2y, G2_in->d1);

	ell = 1;
//	ell = 10;
//	ell = 20;
//	ell = 30;
//	ell = 40;
//	ell = 50;
//	ell = 60;
//	ell = 70;
//	ell = 80;
//	ell = 90;
//	ell = 100;
//	ell = 110;
//	ell = 120;
//	ell = 130;
//	ell = 140;

	ElementFp12 pairing_res[ell][ell][2];
	ElementFp12 out;

	// In this implementation, we assume the worse case that there is not any zero element
	// in matrix f and also, almost 25% of the elements are minus. In the real use case the
	// distribution of the elements for positive, zero, and minus valuse are more simpler.
	int f[ell][ell];

	int pairing_cnt = 0, mult_cnt = 0, exp_cnt = 0, inv_cnt = 0;

	XTime tStart, tEnd;
	XTime_GetTime(&tStart);

	//=== Pairing computations =====================================================================
	int row = 0, column = 0, depth = 0;

#ifdef GENERAL_F
	int mlic = (2 * ell * ell + 1) / N + 1; //ceil((double) (((2 * ell * ell) + 1) / N)); // maximum loop iteration count (mlic)
#endif
#ifdef DIAGONAL_F
	int mlic = (2 * ell + 1) / N + 1; //ceil((double) (((2 * ell) + 1) / N)); // maximum loop iteration count (mlic)
#endif

	for (int j = 0; j < mlic; j++) {
		// Loading G1_in and G2_in into the local DMEMs of the PCP cores in the HW side
		for (int i = 0; i < N; i++) {
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 500, G1_in->d0.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 501, G1_in->d0.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 502, G1_in->d1.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 503, G1_in->d1.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 506, G2_in->d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 507, G2_in->d1, NULL, i);
		}

		// Parallel running of the 16 PCP core for computing optimal-ate pairing
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // hw_optimalAte_pairing_calc(G1_in, G2_in, pcp_core_id)
		pairing_cnt++;

		// Idling PCP cores
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

		// Reading the pairing resuls from the 16 PCP cores in the HW side
		for (int i = 0; i < N; i++) {
			if ((j == 0) && (i == 0)) {
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 508, NULL, out.d0.d0.d0, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 509, NULL, out.d0.d0.d1, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 510, NULL, out.d0.d1.d0, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 511, NULL, out.d0.d1.d1, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 512, NULL, out.d0.d2.d0, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 513, NULL, out.d0.d2.d1, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 514, NULL, out.d1.d0.d0, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 515, NULL, out.d1.d0.d1, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 516, NULL, out.d1.d1.d0, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 517, NULL, out.d1.d1.d1, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 518, NULL, out.d1.d2.d0, i);
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 519, NULL, out.d1.d2.d1, i);
				continue;
			}

			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 508, NULL, pairing_res[row][column][depth].d0.d0.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 509, NULL, pairing_res[row][column][depth].d0.d0.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 510, NULL, pairing_res[row][column][depth].d0.d1.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 511, NULL, pairing_res[row][column][depth].d0.d1.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 512, NULL, pairing_res[row][column][depth].d0.d2.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 513, NULL, pairing_res[row][column][depth].d0.d2.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 514, NULL, pairing_res[row][column][depth].d1.d0.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 515, NULL, pairing_res[row][column][depth].d1.d0.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 516, NULL, pairing_res[row][column][depth].d1.d1.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 517, NULL, pairing_res[row][column][depth].d1.d1.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 518, NULL, pairing_res[row][column][depth].d1.d2.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 519, NULL, pairing_res[row][column][depth].d1.d2.d1, i);

			depth++;
			if (depth < 2)
				continue;
			depth = 0;
			column++;
			if (column < ell)
				continue;
			column = 0;
			row++;
			if (row < ell)
				continue;
			break;
		}
	}

	//=== Fp12 multiplications, exponentiations, and inversions ====================================================
	row = 0; column = 0;
	int xrow = 0, xcolumn = 0;

#ifdef GENERAL_F
	mlic = ell * ell / N + 1; //ceil((double) ((ell * ell) / N)); // maximum loop iteration count (mlic)
#endif
#ifdef DIAGONAL_F
	mlic = ell / N + 1; //ceil((double) (ell / N)); // maximum loop iteration count (mlic)
#endif

	for (int j = 0; j < mlic; j++) {
		// Loading pairing results in the depth_0/1 and f_i,j into the local DMEMs of the PCP cores in the HW side
		for (int i = 0; i < N; i++) {
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 600, pairing_res[row][column][0].d0.d0.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 601, pairing_res[row][column][0].d0.d0.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 602, pairing_res[row][column][0].d0.d1.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 603, pairing_res[row][column][0].d0.d1.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 604, pairing_res[row][column][0].d0.d2.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 605, pairing_res[row][column][0].d0.d2.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 606, pairing_res[row][column][0].d1.d0.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 607, pairing_res[row][column][0].d1.d0.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 608, pairing_res[row][column][0].d1.d1.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 609, pairing_res[row][column][0].d1.d1.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 610, pairing_res[row][column][0].d1.d2.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 611, pairing_res[row][column][0].d1.d2.d1, NULL, i);

			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 700, pairing_res[row][column][1].d0.d0.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 701, pairing_res[row][column][1].d0.d0.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 702, pairing_res[row][column][1].d0.d1.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 703, pairing_res[row][column][1].d0.d1.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 704, pairing_res[row][column][1].d0.d2.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 705, pairing_res[row][column][1].d0.d2.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 706, pairing_res[row][column][1].d1.d0.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 707, pairing_res[row][column][1].d1.d0.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 708, pairing_res[row][column][1].d1.d1.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 709, pairing_res[row][column][1].d1.d1.d1, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 710, pairing_res[row][column][1].d1.d2.d0, NULL, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 711, pairing_res[row][column][1].d1.d2.d1, NULL, i);

			fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 650, (u8 *) &f[row][column], NULL, i);

			column++;
			if (column < ell)
				continue;
			column = 0;
			row++;
			if (row < ell)
				continue;
			break;
		}

		// Parallel running of the 16 PCP core for computing Fp12 multiplications and Fp12 exponentiations
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // fp12_mult_calc(fp12_&600, fp12_&700, pcp_core_id) --> fp12_&700
		// fp12_exp_calc(fp12_&700, fp_&650, pcp_core_id) --> fp12_&700
		mult_cnt++; // pcp_mul_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c, u8 pcp_core_id)
		exp_cnt++; // pcp_exp_fp12 (ElementFp12 a, int *exp, int len_exp, ElementFp12 *c, u8 pcp_core_id)

		// Idling PCP cores
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

		// Parallel running of the 16 PCP core for computing Fp12 inversions
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // fp12_inv_calc(fp12_&700, pcp_core_id) --> fp12_&700

#ifdef GENERAL_F
		inv_cnt = (ell * ell) / (2 * N) + 1; // ceil((double) ((ell * ell) / (4 * N))); // pcp_inv_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id)
#endif
#ifdef DIAGONAL_F
		inv_cnt = ell / (2 * N) + 1; // ceil((double) ((ell * ell) / (4 * N))); // pcp_inv_fp12 (ElementFp12 a, ElementFp12 *c, u8 pcp_core_id)
#endif

		// Idling PCP cores
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

		// Reading the mult., exp., and inv. resuls from the 16 PCP cores in the HW side
		for (int i = 0; i < N; i++) {
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 700, NULL, pairing_res[xrow][xcolumn][0].d0.d0.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 701, NULL, pairing_res[xrow][xcolumn][0].d0.d0.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 702, NULL, pairing_res[xrow][xcolumn][0].d0.d1.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 703, NULL, pairing_res[xrow][xcolumn][0].d0.d1.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 704, NULL, pairing_res[xrow][xcolumn][0].d0.d2.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 705, NULL, pairing_res[xrow][xcolumn][0].d0.d2.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 706, NULL, pairing_res[xrow][xcolumn][0].d1.d0.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 707, NULL, pairing_res[xrow][xcolumn][0].d1.d0.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 708, NULL, pairing_res[xrow][xcolumn][0].d1.d1.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 709, NULL, pairing_res[xrow][xcolumn][0].d1.d1.d1, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 710, NULL, pairing_res[xrow][xcolumn][0].d1.d2.d0, i);
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 711, NULL, pairing_res[xrow][xcolumn][0].d1.d2.d1, i);

			xcolumn++;
			if (xcolumn < ell)
				continue;
			xcolumn = 0;
			xrow++;
			if (xrow < ell)
				continue;
			break;
		}
	}

	//=== Final Fp12 multiplications and reduction to produce the final out ==========================================
#ifdef GENERAL_F
	mlic = ell * ell + 1;
#endif
#ifdef DIAGONAL_F
	mlic = ell + 1;
#endif

	for (int j = 0; j < mlic; j++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 700, gT_0_0_0, NULL, 0);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 701, gT_0_0_1, NULL, 1);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 702, gT_0_1_0, NULL, 2);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 703, gT_0_1_1, NULL, 3);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 704, gT_0_2_0, NULL, 4);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 705, gT_0_2_1, NULL, 5);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 706, gT_1_0_0, NULL, 6);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 707, gT_1_0_1, NULL, 7);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 708, gT_1_1_0, NULL, 8);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 709, gT_1_1_1, NULL, 9);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 710, gT_1_2_0, NULL, 10);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 711, gT_1_2_1, NULL, 11);
	}

#ifdef GENERAL_F
	mlic = 2 * ell;
#endif
#ifdef DIAGONAL_F
	mlic = ell;
#endif

	for (int j = 0; j < mlic; j++) {
		// Parallel running of the 16 PCP core for computing Fp12 multiplications
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // fp12_mult_calc(fp12_&600, fp12_&700, pcp_core_id) --> fp12_&700
		mult_cnt++; // pcp_mul_fp12 (ElementFp12 a, ElementFp12 b, ElementFp12 *c, u8 pcp_core_id)

		// Idling PCP cores
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);
	}

	// Reading the mult. resuls from the 16 PCP cores in the HW side
	mlic = 16;
	for (int j = 0; j < mlic; j++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 700, NULL, out.d0.d0.d0, 0);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 701, NULL, out.d0.d0.d1, 1);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 702, NULL, out.d0.d1.d0, 2);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 703, NULL, out.d0.d1.d1, 3);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 704, NULL, out.d0.d2.d0, 4);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 705, NULL, out.d0.d2.d1, 5);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 706, NULL, out.d1.d0.d0, 6);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 707, NULL, out.d1.d0.d1, 7);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 708, NULL, out.d1.d1.d0, 8);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 709, NULL, out.d1.d1.d1, 9);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 710, NULL, out.d1.d2.d0, 10);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 711, NULL, out.d1.d2.d1, 11);
	}

	//========================================================================
	XTime_GetTime(&tEnd);

	float final_res = pairing_cnt * 982.597 + mult_cnt * 3.348 + exp_cnt * 57.276 + inv_cnt * 64.338;

	printf("First phase of fes_qp_dec pairings computation took %.3f milliseconds.\n\r", 0.001 * (final_res + ((tEnd - tStart) / 99.9900050))); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz

	//========================================================================
	xil_printf("pairing_cnt = %d\tmult_cnt = %d\texp_cnt = %d\tinv_cnt = %d\n\r", pairing_cnt, mult_cnt, exp_cnt, inv_cnt);

	xil_printf("Result of first phase (out):\n\r");
	pcp_print_fp12("out", out);

	free(G1_in);
	free(G2_in);

	return out;
}

/**************************************************************************************************************/
void fes_qp_shared_table_precomputation (int arr_size, int N, t_entry *table, ElementFp12 *alpha, ElementFp12 *delta, ElementFp12 *gamma) {

#ifndef PRE_COMP_MEAS
	(table + 0)->hash = 0x0000000000000001;
	(table + 0)->value = 0x0000000000000000;
#else
	(table + 0)->hash = 0x0000000000000000;
	(table + 0)->value = 0x0000000000000000;
#endif

	// Loading alpha and all u parameters to the local DMEMs of the PCP cores
	for (int i = 0; i < N; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 600, alpha->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 601, alpha->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 602, alpha->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 603, alpha->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 604, alpha->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 605, alpha->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 606, alpha->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 607, alpha->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 608, alpha->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 609, alpha->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 610, alpha->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 611, alpha->d1.d2.d1, NULL, i);

		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 50, alpha->d0.d0.d0, NULL, i);
	}

	Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // Parall computation of the 16 PCP cores: gamma[u] = alpha ^ u
	// 16 parallel Fp12 Exp.
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores

	// Reading the resuls from the 16 PCP cores in the HW side
	__UINT64_TYPE__ *hash_value;
	ElementFp12 *tmp;
	for (int i = 0; i < N; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 610, NULL, tmp->d0.d0.d0, i);
#ifdef PRE_COMP_REAL
		*hash_value = *(__UINT64_TYPE__*) &tmp->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH];
		(table + i + 1)->hash = *hash_value;
		(table + i + 1)->value = (__UINT64_TYPE__)(i + 1);
#endif
#ifdef PRE_COMP_RAND
		srnd_generation(hash_value);
		(table + i + 1)->hash = *hash_value;
		(table + i + 1)->value = *hash_value - 1;
#endif
#ifdef PRE_COMP_MEAS
		(table + i + 1)->hash = (__UINT64_TYPE__)(i + 1);
		(table + i + 1)->value = (__UINT64_TYPE__)(i + 1);
#endif
	}

	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 600, NULL, delta->d0.d0.d0, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 601, NULL, delta->d0.d0.d1, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 602, NULL, delta->d0.d1.d0, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 603, NULL, delta->d0.d1.d1, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 604, NULL, delta->d0.d2.d0, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 605, NULL, delta->d0.d2.d1, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 606, NULL, delta->d1.d0.d0, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 607, NULL, delta->d1.d0.d1, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 608, NULL, delta->d1.d1.d0, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 609, NULL, delta->d1.d1.d1, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 610, NULL, delta->d1.d2.d0, 15);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 611, NULL, delta->d1.d2.d1, 15);
	// Loading delta parameters to the local DMEMs of the PCP cores
	for (int i = 0; i < N; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 800, delta->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 801, delta->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 802, delta->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 803, delta->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 804, delta->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 805, delta->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 806, delta->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 807, delta->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 808, delta->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 809, delta->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 810, delta->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 811, delta->d1.d2.d1, NULL, i);
	}

	for (int j = 1; j < (arr_size / N); j++) {
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // Parall computation of the 16 PCP cores: gamma[u] = gamma[u] * delta
		// 16 parallel Fp12 Mult.
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores

		// Reading the resuls from the 16 PCP cores in the HW side
		for (int i = 0; i < N; i++) {
			fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 610, NULL, tmp->d0.d0.d0, i);
#ifdef PRE_COMP_REAL
			*hash_value = *(__UINT64_TYPE__*) &tmp->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH];
			(table + i + 1 + (j * N))->hash = *hash_value;
			(table + i + 1 + (j * N))->value = (__UINT64_TYPE__)(i + 1 + (j * N));
#endif
#ifdef PRE_COMP_RAND
			srnd_generation(hash_value);
			(table + i + 1 + (j * N))->hash = *hash_value;
			(table + i + 1 + (j * N))->value = *hash_value - 1;
#endif
#ifdef PRE_COMP_MEAS
		(table + i + 1 + (j * N))->hash = (__UINT64_TYPE__)(i + 1 + (j * N));
		(table + i + 1 + (j * N))->value = (__UINT64_TYPE__)(i + 1 + (j * N));
#endif
		}
	}

//	XTime tStart, tEnd;
//	XTime_GetTime(&tStart);

	// Sorting shared table T in ascending order on column hash values
#ifndef PRE_COMP_MEAS
	qsort(table, arr_size, sizeof(t_entry), compare_entries);
//	heapSort(table, arr_size);
#endif

//	XTime_GetTime(&tEnd);
//	printf("Test quick sorting (qsort()) computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
//
//	for(int i = 0; i < 20; i++)
//		xil_printf("hash = 0x%016lx : value = 0x%016lx\n\r", (table + i)->hash, (table + i)->value);

	// Loading alpha an mu parameter to the local DMEMs of the PCP core 0
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 600, alpha->d0.d0.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 601, alpha->d0.d0.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 602, alpha->d0.d1.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 603, alpha->d0.d1.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 604, alpha->d0.d2.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 605, alpha->d0.d2.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 606, alpha->d1.d0.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 607, alpha->d1.d0.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 608, alpha->d1.d1.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 609, alpha->d1.d1.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 610, alpha->d1.d2.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 611, alpha->d1.d2.d1, NULL, 0);

	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 51, alpha->d0.d0.d0, NULL, 0);

	Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // Computation of PCP core_0: alpha_p = alpha ^ mu
	// 1 Fp12 Exp.
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores

	Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // Computation of PCP core_0: alpha_p = alpha_p ^ -1 = alpha ^ -mu
	// 1 Fp12 Inv.
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores

	// Reading the resuls from PCP core_0 in the HW side
	ElementFp12 *alpha_p;
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 600, NULL, alpha_p->d0.d0.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 601, NULL, alpha_p->d0.d0.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 602, NULL, alpha_p->d0.d1.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 603, NULL, alpha_p->d0.d1.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 604, NULL, alpha_p->d0.d2.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 605, NULL, alpha_p->d0.d2.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 606, NULL, alpha_p->d1.d0.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 607, NULL, alpha_p->d1.d0.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 608, NULL, alpha_p->d1.d1.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 609, NULL, alpha_p->d1.d1.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 610, NULL, alpha_p->d1.d2.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 611, NULL, alpha_p->d1.d2.d1, 0);
	// Loading delta parameters to the local DMEMs of the PCP cores
	for (int i = 0; i < N; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 800, alpha_p->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 801, alpha_p->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 802, alpha_p->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 803, alpha_p->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 804, alpha_p->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 805, alpha_p->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 806, alpha_p->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 807, alpha_p->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 808, alpha_p->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 809, alpha_p->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 810, alpha_p->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 811, alpha_p->d1.d2.d1, NULL, i);

		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 50, alpha_p->d0.d0.d0, NULL, i);
	}

	Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // Parall computation of the 16 PCP cores: gamma_p[u] = alpha_p ^ u
	// 16 parallel Fp12 Exp.
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores

	// Reading gamma_p[u] from the local DMEMs of the PCP cores
	for (int i = 0; i < N; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 700, NULL, (gamma + i)->d0.d0.d0, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 701, NULL, (gamma + i)->d0.d0.d1, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 702, NULL, (gamma + i)->d0.d1.d0, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 703, NULL, (gamma + i)->d0.d1.d1, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 704, NULL, (gamma + i)->d0.d2.d0, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 705, NULL, (gamma + i)->d0.d2.d1, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 706, NULL, (gamma + i)->d1.d0.d0, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 707, NULL, (gamma + i)->d1.d0.d1, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 708, NULL, (gamma + i)->d1.d1.d0, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 709, NULL, (gamma + i)->d1.d1.d1, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 710, NULL, (gamma + i)->d1.d2.d0, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 711, NULL, (gamma + i)->d1.d2.d1, i);
	}

	ElementFp12 *delta_p = (gamma + 15);
}

/**************************************************************************************************************/
int compare_entries(const void *a, const void *b) {
	t_entry a_t, b_t;

	a_t = *(t_entry*) a;
	b_t = *(t_entry*) b;

	if (a_t.hash < b_t.hash)
		return -1;
	else if (a_t.hash > b_t.hash)
		return 1;

	return 0;
}

/**************************************************************************************************************/
void srnd_generation(__UINT64_TYPE__ *srnd_result) {
	XTime t_srnd;
	XTime_GetTime(&t_srnd);

	srand(t_srnd);

	int a = rand();
	int b = rand();
	long e = (long)(a * b);
	e = abs(e);

	int c = rand();
	int d = rand();
	long f = (long)(c * d);
	f = abs(f);

	*srnd_result = (__UINT64_TYPE__)(e * f);
}

/**************************************************************************************************************/
void heapSort(t_entry *table, int arr_size) {
	// Build heap (rearrange array)
	for (int i = ((arr_size / 2) - 1); i >= 0; i--)
		heapify(table, arr_size, i);

	// One by one extract an element from heap
	__UINT64_TYPE__ tmp;
	for (int i = arr_size - 1; i > 0; i--) {
		// Move current root to end
		// Swap(arr[0], arr[i]);
		tmp = (table + 0)->hash;
		(table + 0)->hash = (table + i)->hash;
		(table + i)->hash = tmp;

		tmp = (table + 0)->value;
		(table + 0)->value = (table + i)->value;
		(table + i)->value = tmp;

		// Call max heapify on the reduced heap
		heapify(table, i, 0);
	}
}

/**************************************************************************************************************/
// To heapify a subtree rooted with node i which is an index in arr1[]. arr_size is size of heap
void heapify(t_entry *table, int arr_size, int i) {
	int largest = i; // Initialize largest as root
	int l = 2 * i + 1; // left
	int r = 2 * i + 2; // right

	// If left child is larger than root
	if ((l < arr_size) && ((table + l)->hash > (table + largest)->hash))
		largest = l;

	// If right child is larger than largest so far
	if ((r < arr_size) && ((table + r)->hash > (table + largest)->hash))
		largest = r;

	// If largest is not root
	__UINT64_TYPE__ tmp;
	if (largest != i) {
		// Swap(arr[i], arr[largest]);
		tmp = (table + i)->hash;
		(table + i)->hash = (table + largest)->hash;
		(table + largest)->hash = tmp;

		tmp = (table + i)->value;
		(table + i)->value = (table + largest)->value;
		(table + largest)->value = tmp;

		// Recursively heapify the affected sub-tree
		heapify(table, arr_size, largest);
	}
}

/**************************************************************************************************************/
// Binary search from the shared table T
__UINT64_TYPE__ fes_qp_shared_table_search (int arr_size, t_entry *table, __UINT64_TYPE__ fin) {
	int min = 0, max = (arr_size - 1), mid;

	while (min <= max) {
		mid = min + ((max - min) / 2);
		if ((table + mid)->hash == fin)
			return (table + mid)->value;

		if ((table + mid)->hash < fin)
			min = mid + 1;
		else
			max = mid - 1;
	}

	return -1;
}

/**************************************************************************************************************/
__UINT64_TYPE__ fes_qp_dis_log (int bound, int ell, ElementFp12 *out) {
	// Second phase: Discrete logarithm (i.e., dis_log()) computation
	xil_printf("Start of second phase...\n\r");

	// gT = OptAtePairing(g1, g2) ==> hw_optimalAte_pairing_calc(G1, G2, pcp_core_id)
	u8 gT_0_0_0[] = "\x04\x45\x8B\x6B\xB7\xEF\x0D\xDA\x02\xB9\xAD\x61\x3E\x44\x09\xB2\xD6\xDF\x24\xF0\xC1\x85\xFA\x2D\x78\x12\x3C\xA6\xF7\x7D\x07\xDA";
	u8 gT_0_0_1[] = "\x22\x31\x01\x71\x30\xD2\xFA\xB5\x95\xF7\xE6\x5D\x65\x23\xC9\xA0\x00\x19\x4B\x87\xEC\xAA\x4C\x7E\xA3\x8F\xD6\x52\x1A\xFD\x5A\x71";
	u8 gT_0_1_0[] = "\x0A\xD3\x46\xBD\x68\x8C\xC0\x84\xEA\xFD\x40\x46\xC8\x91\x7E\x0F\xA9\xAB\x4A\x57\xC3\x80\x30\xA1\x38\xD9\x2D\x2C\x01\xE7\xAE\xD8";
	u8 gT_0_1_1[] = "\x17\x15\x85\x47\x5D\x4F\xF2\x1F\x16\xD9\x8A\x1D\x4F\xE6\x02\x60\x02\x91\x39\x5C\x2B\xB9\x04\x10\x11\x0E\x3D\x37\x1D\xEB\xB5\xBE";
	u8 gT_0_2_0[] = "\x22\x58\x1D\xE9\x73\x33\x19\x65\xD6\xD9\x9E\x91\xE0\x99\xF7\x10\x3F\xC1\xAD\xAE\x7F\xF1\x44\xB2\x88\x37\x00\xE8\xA6\x2C\x73\x6D";
	u8 gT_0_2_1[] = "\x1A\x1A\xEA\x16\xEA\x2F\x8A\x1A\x83\xBB\xB9\x4F\x31\x30\x17\xD4\xD2\x19\x93\x42\x99\xF1\x64\xA4\xCF\x81\xD2\x38\xBA\x1A\x28\xF7";
	u8 gT_1_0_0[] = "\x15\x5D\x2C\x04\x09\xC1\x81\x83\x86\xD0\x79\x9D\x3E\xCF\xA5\x40\xA2\xDC\x5F\x45\x14\xB2\x2B\x44\x9E\x9F\x6E\xFB\x3B\x52\x2E\x7B";
	u8 gT_1_0_1[] = "\x02\xF8\x0E\xAA\x1B\x16\xB8\x08\x57\x9C\xC2\x11\xF3\x13\x11\x0D\xB7\x04\xBE\x35\x1E\x11\x27\x0C\xDE\xF1\x31\x11\x35\x8D\x6E\x33";
	u8 gT_1_1_0[] = "\x18\x8D\x51\x5D\x73\x7A\x5B\x74\xB0\x0D\x57\x19\xAC\xC7\xD6\x8C\x6C\xD5\xB2\x4A\x2C\x31\x6B\x42\xDE\xB0\xB3\x73\x61\xF5\x6E\x62";
	u8 gT_1_1_1[] = "\x04\x63\x6F\xBB\x4F\xD4\x11\x56\xB2\x31\xC4\x0A\x4C\x43\x7E\xFD\x51\x3E\x8C\x0F\x3C\x12\x77\x64\x1E\xAE\x21\x8D\xFB\xD4\x6A\x9E";
	u8 gT_1_2_0[] = "\x1B\xFF\xB5\xC4\xE9\xF7\xAF\x88\xA2\x7D\x08\x8B\x0C\xB5\x98\xCB\x70\x3C\x35\xD3\x2D\xAA\x47\x8E\x32\x8E\x7C\xE3\x04\xB8\x44\x44";
	u8 gT_1_2_1[] = "\x1C\x4B\xD4\x8B\xBD\xCA\xFB\x47\x8F\x11\xEB\x0D\xEE\x74\x39\x7D\xF2\x95\xB5\x92\x55\xF8\x71\x8F\x3C\xF7\x68\x48\x8C\xE7\x66\x0E";

	// Required microcodes segment/full packs for Fp12 multiplication (i.e., gamma[i] = gamma[i] * x, such that x is beta or delta)
	// are loaded initially in the shared_IMEM of the HW side for runnig by all 16 PCP cores based on common CMD and STS signals

	// (a) Precomputation part ///////////////////////////////////////////
	XTime tStart, tEnd;
	XTime_GetTime(&tStart);

	int N = HW_PCP_CORE_NO; // Number of PCP cores
	int mu_max = 130023424; // Memory bound Bp // maximum (-e_bound, +e_bound) ==> (-B, +B) = (-2^54, +2^54) ==> mu_max = 0x7C00000 or 27-bit

	t_entry *table; // Size of the shared table T: ((mu_max + 1) * (64/8 + 64/8)) Bytes = (mu_max * 16) Bytes
	table = (t_entry *) malloc ((mu_max + 1) * sizeof(t_entry));

	ElementFp12 *alpha, *delta, *gamma;
	alpha = (ElementFp12 *) malloc (1 * sizeof(ElementFp12));
	delta = (ElementFp12 *) malloc (1 * sizeof(ElementFp12));
	gamma = (ElementFp12 *) malloc (N * sizeof(ElementFp12));

	eql_fp(gT_0_0_0, alpha->d0.d0.d0);
	eql_fp(gT_0_0_1, alpha->d0.d0.d1);
	eql_fp(gT_0_1_0, alpha->d0.d1.d0);
	eql_fp(gT_0_1_1, alpha->d0.d1.d1);
	eql_fp(gT_0_2_0, alpha->d0.d2.d0);
	eql_fp(gT_0_2_1, alpha->d0.d2.d1);
	eql_fp(gT_1_0_0, alpha->d1.d0.d0);
	eql_fp(gT_1_0_1, alpha->d1.d0.d1);
	eql_fp(gT_1_1_0, alpha->d1.d1.d0);
	eql_fp(gT_1_1_1, alpha->d1.d1.d1);
	eql_fp(gT_1_2_0, alpha->d1.d2.d0);
	eql_fp(gT_1_2_1, alpha->d1.d2.d1);

	fes_qp_shared_table_precomputation((mu_max + 1), N, table, alpha, delta, gamma);

	XTime_GetTime(&tEnd);
	xil_printf("Precomputation part is done!\n\r");
	printf("Precomputation part of discrete logarithm computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz

	// (b) On-the-fly part ////////////////////////////////////////////////
	__UINT64_TYPE__ output_bound_max = 130023424; // 1 << 27; // B

	/******* Measument test setups *******/
	// int N = 16;
	// int M = N / 2 = 8;
	// int mu = Bp = 1 << 27; // maximum (-e_bound, +e_bound) ==> (-Bp, +Bp) = (-2^54, +2^54) ==> Bp = 0x8000000 or 27-bit
	// __UINT64_TYPE__ B = 130023424; // measurements' var.
	// mu_p = ceil(B / Bp) - 1;
//	int mlic = 0; // mlic = ceil(mu_p / M); 1-bit
//	int mlic = 0; // mlic = ceil(mu_p / M); 5-bit
//	int mlic = 0; // mlic = ceil(mu_p / M); 10-bit
//	int mlic = 0; // mlic = ceil(mu_p / M); 15-bit
//	int mlic = 0; // mlic = ceil(mu_p / M); 20-bit
//	int mlic = 0; // mlic = ceil(mu_p / M); 25-bit
//	int mlic = 0; // mlic = ceil(mu_p / M); 27-bit
//	int mlic = 1; // mlic = ceil(mu_p / M); 30-bit
//	int mlic = 8; // mlic = ceil(mu_p / M); 33-bit
//	int mlic = 32; // mlic = ceil(mu_p / M); 35-bit
//	int mlic = 1024; // mlic = ceil(mu_p / M); 40-bit
	int mlic = 2048; // mlic = ceil(mu_p / M); 41-bit
//	int mlic = 32768; // mlic = ceil(mu_p / M); 45-bit
//	int mlic = 1048576; // mlic = ceil(mu_p / M); 50-bit
//	int mlic = 16777216; // mlic = ceil(mu_p / M); 54-bit
	/*************************************/

	ElementFp12 *beta_p, *beta_m;
	beta_p = (ElementFp12 *) malloc (1 * sizeof(ElementFp12));
	beta_m = (ElementFp12 *) malloc (1 * sizeof(ElementFp12));
	__UINT64_TYPE__ searchResult;

	beta_p = out;
	// beta_m = out ^ -1

	XTime_GetTime(&tStart);

	// loading data to CP core's LDM for computing beta_m = out ^ -1
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 600, out->d0.d0.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 601, out->d0.d0.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 602, out->d0.d1.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 603, out->d0.d1.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 604, out->d0.d2.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 605, out->d0.d2.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 606, out->d1.d0.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 607, out->d1.d0.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 608, out->d1.d1.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 609, out->d1.d1.d1, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 610, out->d1.d2.d0, NULL, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 611, out->d1.d2.d1, NULL, 0);

	Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000); // Computation of PCP core_0: beta_m = out ^ -1
	// CP core_0 busy for doing Fp12 Inv.

	searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &beta_p->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);
	if (searchResult != -1) {
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP core_0
		XTime_GetTime(&tEnd);
		xil_printf("FES_QP out = 0x%016lx\n\r", searchResult);
		printf("Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
		free(table);
		free(alpha);
		free(beta_p);
		free(beta_m);
		free(delta);
		free(gamma);
		return searchResult;
	}

	// Idling PCP core_0
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

	// Reading the beta_m->d0.d0.d0 result
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 800, NULL, beta_m->d0.d0.d0, 0);

	searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &beta_m->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);
	if (searchResult != -1) {
//	if (searchResult == -1) {
		XTime_GetTime(&tEnd);
		xil_printf("FES_QP out = 0x%016lx\n\r", (-1 * searchResult));
		printf("Beginning: Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
		free(table);
		free(alpha);
		free(beta_p);
		free(beta_m);
		free(delta);
		free(gamma);
		return searchResult;
	}

	// Reading the rest of beta_m result from the CP core_0's LDM
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 801, NULL, beta_m->d0.d0.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 802, NULL, beta_m->d0.d1.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 803, NULL, beta_m->d0.d1.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 804, NULL, beta_m->d0.d2.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 805, NULL, beta_m->d0.d2.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 806, NULL, beta_m->d1.d0.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 807, NULL, beta_m->d1.d0.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 808, NULL, beta_m->d1.d1.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 809, NULL, beta_m->d1.d1.d1, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 810, NULL, beta_m->d1.d2.d0, 0);
	fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 811, NULL, beta_m->d1.d2.d1, 0);

	int M = N / 2;

	// Loading beta, gamma[], and delata to the local DMEMs of the PCP cores
	for (int i = 0; i < M; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 600, (gamma + i)->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 601, (gamma + i)->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 602, (gamma + i)->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 603, (gamma + i)->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 604, (gamma + i)->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 605, (gamma + i)->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 606, (gamma + i)->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 607, (gamma + i)->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 608, (gamma + i)->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 609, (gamma + i)->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 610, (gamma + i)->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 611, (gamma + i)->d1.d2.d1, NULL, i);

		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 700, beta_p->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 701, beta_p->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 702, beta_p->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 703, beta_p->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 704, beta_p->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 705, beta_p->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 706, beta_p->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 707, beta_p->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 708, beta_p->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 709, beta_p->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 710, beta_p->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 711, beta_p->d1.d2.d1, NULL, i);
	}

	for (int i = 0; i < M; i++) {
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 600, (gamma + i)->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 601, (gamma + i)->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 602, (gamma + i)->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 603, (gamma + i)->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 604, (gamma + i)->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 605, (gamma + i)->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 606, (gamma + i)->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 607, (gamma + i)->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 608, (gamma + i)->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 609, (gamma + i)->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 610, (gamma + i)->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 611, (gamma + i)->d1.d2.d1, NULL, i);

		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 700, beta_m->d0.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 701, beta_m->d0.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 702, beta_m->d0.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 703, beta_m->d0.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 704, beta_m->d0.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 705, beta_m->d0.d2.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 706, beta_m->d1.d0.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 707, beta_m->d1.d0.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 708, beta_m->d1.d1.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 709, beta_m->d1.d1.d1, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 710, beta_m->d1.d2.d0, NULL, i);
		fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 711, beta_m->d1.d2.d1, NULL, i);
	}

	/******* Measument test setups *******/
	// int N = 16;
	// int M = N / 2 = 8;
	// int mu = Bp = 1 << 27; // maximum (-e_bound, +e_bound) ==> (-Bp, +Bp) = (-2^54, +2^54) ==> Bp = 0x8000000 or 27-bit
	// __UINT64_TYPE__ B = 130023424; // measurements' var.
	// mu_p = ceil(B / Bp) - 1;
//	int mlic = 0; // mlic = ceil(mu_p / M); X-bit
	/*************************************/

	// main for loop for the second phase main computation in the HW/SW codesign implementation
	for (int j = 0; j < mlic; j++) {
		if (j == 0) {
			// Parall computation of the 16 PCP cores: gamma[u] = gamma[u] *(beta_p or beta_m)
			Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000);

			for (int i = 0; i < (M / 4); i++)
				searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &beta_p->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);

			for (int i = 0; i < (M / 4); i++)
				searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &beta_m->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);

			// Idling PCP cores
			Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

			// Reading the resuls from the 16 PCP cores in the HW side
			for (int i = 0; i < N; i++)
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 610, NULL, (gamma + i)->d0.d0.d0, i);

			if (mlic > 1) {
				// Replacing beta_p and beta_n with delta_p for the remain Fp12 multiplication in the HW side
				for (int i = 0; i < N; i++) {
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 700, delta->d0.d0.d0, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 701, delta->d0.d0.d1, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 702, delta->d0.d1.d0, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 703, delta->d0.d1.d1, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 704, delta->d0.d2.d0, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 705, delta->d0.d2.d1, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 706, delta->d1.d0.d0, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 707, delta->d1.d0.d1, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 708, delta->d1.d1.d0, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 709, delta->d1.d1.d1, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 710, delta->d1.d2.d0, NULL, i);
					fpga_pcp_ips(NO_SEG, PCP_DMEM_WR, 711, delta->d1.d2.d1, NULL, i);
				}

				// Parall computation of the 16 PCP cores: gamma[u] = gamma[u] * delta
				Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000);
			}

			// Searching the HW results in the shared table T
			for (int k = 0; k < M; k++) {
				searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &(gamma + k)->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);
				if (searchResult != -1) {
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					searchResult = ((k + 1) + j * N) * mu_max + searchResult;
					XTime_GetTime(&tEnd);
					xil_printf("FES_QP out = 0x%016lx\n\r", searchResult);
					printf("Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					free(table);
					free(alpha);
					free(beta_p);
					free(beta_m);
					free(delta);
					free(gamma);
					return searchResult;
				}
			}

			for (int k = 0; k < M; k++) {
				searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &(gamma + M + k)->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);
				if (searchResult != -1) {
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					searchResult = -1 * (((k + 1) + j * N) * mu_max + searchResult);
					XTime_GetTime(&tEnd);
					xil_printf("FES_QP out = 0x%016lx\n\r", searchResult);
					printf("Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					free(table);
					free(alpha);
					free(beta_p);
					free(beta_m);
					free(delta);
					free(gamma);
					return searchResult;
				}
			}
		}
		else {
			// Idling PCP cores
			Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

			// Reading the resuls from the 16 PCP cores in the HW side
			for (int i = 0; i < N; i++)
				fpga_pcp_ips(NO_SEG, PCP_DMEM_RD, 600, NULL, (gamma + i)->d0.d0.d0, i);

			// Parall computation of the 16 PCP cores: gamma[u] = gamma[u] * delta
			Xil_Out32(FPGA_PCP_IP0_CMD, 0x80000000);

			// Searching the HW results in the shared table T
			for (int k = 0; k < M; k++) {
				searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &(gamma + k)->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);
				if (searchResult != -1) {
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					searchResult = ((k + 1) + j * N) * mu_max + searchResult;
					XTime_GetTime(&tEnd);
					xil_printf("FES_QP out = 0x%016lx\n\r", searchResult);
					printf("Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					free(table);
					free(alpha);
					free(beta_p);
					free(beta_m);
					free(delta);
					free(gamma);
					return searchResult;
				}
			}

			for (int k = 0; k < M; k++) {
				searchResult = fes_qp_shared_table_search((mu_max + 1), table, *(__UINT64_TYPE__*) &(gamma + M + k)->d0.d0.d0[PCP_DMEM_RD_WR_LEN - HASH_LENGTH]);
				if (searchResult != -1) {
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					searchResult = -1 * (((k + 1) + j * N) * mu_max + searchResult);
					XTime_GetTime(&tEnd);
					xil_printf("FES_QP out = 0x%016lx\n\r", searchResult);
					printf("Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz
					Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores
					free(table);
					free(alpha);
					free(beta_p);
					free(beta_m);
					free(delta);
					free(gamma);
					return searchResult;
				}
			}
		}
	}

	if (mlic > 1)
		Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000); // Idling PCP cores

	XTime_GetTime(&tEnd);
	xil_printf("FES_QP out = 0x%016lx\n\r", searchResult);
	printf("Measurement: Second phase of fes_qp_dec main computation took %.6f seconds.\n\r", (tEnd - tStart) / 99990005.0); // XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ = 99990005 Hz

	free(table);
	free(alpha);
	free(beta_p);
	free(beta_m);
	free(delta);
	free(gamma);

	return searchResult;
}

/**************************************************************************************************************/

/**************************************************************************************************************/
void fe_qf_dec() {
	//==============================================================================
	//===== FE scheme for quadratic polynomials (FES_QP): Decryption algorithm =====
	//==============================================================================
	int bound = 100;
	int ell = 5;

	ElementFp12 out = fes_qp_pairings(bound, ell);
	__UINT64_TYPE__ dec_res = fes_qp_dis_log (bound, ell, &out);
	xil_printf("FES_QP decryption result = 0x%016lx\n\r", dec_res);
	//==============================================================================
}

/**************************************************************************************************************/
