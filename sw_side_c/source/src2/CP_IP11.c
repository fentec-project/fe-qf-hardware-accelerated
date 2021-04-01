#include "CP_IPs.h"

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
int fpga_cp_ip11(u8 cmd_cp, u8 L1_param,  u8 L2_param, u8 *input, u32 length, u8 *output)
{
	int Status;

	u32 command = (cmd_cp & 0x0000000F) | (((length / 8) << 4) & 0x00000FF0) |
					((L1_param << 12) & 0x0000F000) | ((L2_param << 16) & 0x000F0000);
	Xil_Out32(FPGA_CP_IP11_CMD, command);

	//-------------------------------------------------------------------------------------------------
	if ((cmd_cp == CP_IMEM_LD) || (cmd_cp == CP_L3L1_DMEM_WR))
	{
		Status = axi_dma_send_packet(TRUE, AXI_DMA5_CP1011, AXI_DMA5_CP1011_DEV_ID, length, input);
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi_dma_send_packet() failed!\r\n");
			return XST_FAILURE;
		}
	}

	//-------------------------------------------------------------------------------------------------
	if (cmd_cp == CP_L3L1_DMEM_RD)
	{
		Status = axi_dma_receive_packet(TRUE, AXI_DMA5_CP1011, AXI_DMA5_CP1011_DEV_ID, length, output);
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi_dma_receive_packet() failed!\r\n");
			return XST_FAILURE;
		}
	}

	//-------------------------------------------------------------------------------------------------
	while((Xil_In32(FPGA_CP_IPs_STATUS) & 0x00C00000) != 0x00C00000); // FPGA_CP_IP0 busy checking!
	Xil_Out32(FPGA_CP_IP11_CMD, 0x00000000);

	return XST_SUCCESS;
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void test_cp_ip11()
{
	//--------------------------------------------------------------------------------------------------------------------------------
	// DMEM input parameter  (X0: 4096 bit)
	u8 DMEM_IN_X0[] =
	"\x00\x00\x00\x00\x00\x00\x00\x00" // 8-Bytes padded with zeros
	"\x72\x3B\x6A\xC6\x80\x54\xED\xE5\xE1\xDD\x8F\xDC\xB5\x83\x09\xE9\x3B\xCE\xFF\x25\xD3\xFD\xC1\x39\x2B\x39\x0E\x9E\x29\xF6\x6B\x4A"
	"\x55\xD4\xE7\x08\x1E\xC3\xD7\x1D\x69\xA2\xCD\x43\x9B\x5A\x8D\x6D\x33\xE0\x2F\x17\xF1\xFD\x27\x22\x12\x79\x72\x53\xBB\x2A\x16\xFB"
	"\x71\x98\xD8\xBF\x77\x6D\x2A\x9A\xF2\x8B\x80\x98\xE9\x97\x6F\xB3\x38\x7E\x5E\xBC\x1E\xEE\x2F\xCB\xDB\x79\x27\x0A\x06\x8C\x11\x1C"
	"\xFA\xF7\xDA\xDA\x02\x23\xD8\x2C\xB5\xF3\x81\x9C\x3E\xC9\x90\x83\x7C\xBB\x8A\x20\xD6\x61\x12\xB0\x3E\x22\x01\xC4\xF3\x1A\x1D\x0A"
	"\x9B\x0F\xF6\xC4\x0E\x7F\x89\x7C\x1F\xF4\x2D\x43\xB6\x47\x6B\x09\xF5\xE4\x17\x89\xB5\x7C\xD9\xE2\xF0\xBA\x95\x90\xE5\x8F\xB9\x82"
	"\x91\xA1\x26\xF9\x2C\x34\x06\x54\x84\xEC\x1B\x24\xBE\x84\x41\xEE\x02\x14\xC1\x10\xF7\x28\x52\x9F\x48\x4F\x90\xA5\x6A\x14\x53\x33"
	"\x2F\x98\x45\x21\xE6\x1D\x59\xC7\xEF\x61\xDE\xC5\xB8\x52\xE1\x75\x23\x13\x13\x8F\xAC\x63\x91\x60\x95\xB3\xD4\xA2\xC2\x57\x5F\x3A"
	"\x85\xA6\x04\x2C\xC1\x41\x8D\xBB\xC7\xDA\xE1\x71\x48\x88\x98\x04\xFF\xDE\x6B\xFA\x0D\x86\xE3\xEE\x17\xD9\xAE\xA5\x79\xD0\xA2\x77"
	"\xDE\x35\x89\x4B\x9D\xE3\xEE\x6A\x24\x94\x15\xB7\x75\xFF\xA7\xE7\xAF\x7E\xFF\x44\xA4\x4C\x64\x58\xBE\x84\x57\x3A\x2A\x4F\xDE\x52"
	"\x9D\x73\xB1\x0B\x47\xC2\xAD\xCA\x1D\x45\x59\x2E\xEE\x07\xDC\x52\xF3\x27\x49\x78\x78\x20\xEF\x30\xD9\x1D\x53\x0A\xD6\x00\xC5\xB6"
	"\x94\x6B\x19\x92\x0B\xAE\xE1\x27\x99\x9D\xC2\x10\x22\xFB\x18\x20\x03\xC0\x5D\x1F\x25\x2E\x30\x9A\xEF\xC0\x14\xF6\x3A\x67\x38\x45"
	"\x35\x5D\x91\xE6\x15\x3D\x1D\xF7\xB5\x63\x96\x78\xC9\xEE\x56\x30\xFB\x74\x8C\x2A\x2E\x22\xC9\x88\x82\x6E\x54\x7C\xE2\x59\x8B\x1A"
	"\xF0\x8D\x5B\x7C\x74\x70\x4A\xD6\x50\xB3\xAB\x61\xAF\xDF\x3D\xB9\x12\xE0\x4F\x28\xFB\x28\x60\xBB\x20\x95\x3D\x81\x9B\xFC\x3C\xC1"
	"\x99\x51\xB3\xEA\x48\x86\x62\x49\x5F\x58\xE7\xE7\x18\xCA\xC9\x74\xEA\x4C\xE2\x34\x47\x8F\xC1\xB8\x4B\x19\x19\x93\x36\x6F\x9E\x0F"
	"\x1C\x8F\xC1\x15\xE7\x32\xE2\x85\xE6\xA1\x53\xCA\x6D\x5D\x5A\x2B\x7E\x4F\x16\x3C\xCA\xF5\x53\xF8\xE7\xF7\x5E\xFA\x11\x51\x48\x00"
	"\xF2\xD3\x23\x7C\x28\x8A\x94\xF2\x0F\xB4\xD8\xAD\x1A\x3C\xA9\x98\xE9\xAB\x41\xBE\xF8\x78\x8F\x80\x0E\x1F\xA1\x98\x4C\xEE\xFF\x37";

	// DMEM input parameter  (X1: 4096 bit)
	u8 DMEM_IN_X1[] =
	"\x00\x00\x00\x00\x00\x00\x00\x00" // 8-Bytes padded with zeros
	"\x52\x3B\x6A\xC6\x80\x54\xED\xE5\xE1\xDD\x8F\xDC\xB5\x83\x09\xE9\x3B\xCE\xFF\x25\xD3\xFD\xC1\x39\x2B\x39\x0E\x9E\x29\xF6\x6B\x4A"
	"\x55\xD4\xE7\x08\x1E\xC3\xD7\x1D\x69\xA2\xCD\x43\x9B\x5A\x8D\x6D\x33\xE0\x2F\x17\xF1\xFD\x27\x22\x12\x79\x72\x53\xBB\x2A\x16\xFB"
	"\x71\x98\xD8\xBF\x77\x6D\x2A\x9A\xF2\x8B\x80\x98\xE9\x97\x6F\xB3\x38\x7E\x5E\xBC\x1E\xEE\x2F\xCB\xDB\x79\x27\x0A\x06\x8C\x11\x1C"
	"\xFA\xF7\xDA\xDA\x02\x23\xD8\x2C\xB5\xF3\x81\x9C\x3E\xC9\x90\x83\x7C\xBB\x8A\x20\xD6\x61\x12\xB0\x3E\x22\x01\xC4\xF3\x1A\x1D\x0A"
	"\x9B\x0F\xF6\xC4\x0E\x7F\x89\x7C\x1F\xF4\x2D\x43\xB6\x47\x6B\x09\xF5\xE4\x17\x89\xB5\x7C\xD9\xE2\xF0\xBA\x95\x90\xE5\x8F\xB9\x82"
	"\x91\xA1\x26\xF9\x2C\x34\x06\x54\x84\xEC\x1B\x24\xBE\x84\x41\xEE\x02\x14\xC1\x10\xF7\x28\x52\x9F\x48\x4F\x90\xA5\x6A\x14\x53\x33"
	"\x2F\x98\x45\x21\xE6\x1D\x59\xC7\xEF\x61\xDE\xC5\xB8\x52\xE1\x75\x23\x13\x13\x8F\xAC\x63\x91\x60\x95\xB3\xD4\xA2\xC2\x57\x5F\x3A"
	"\x85\xA6\x04\x2C\xC1\x41\x8D\xBB\xC7\xDA\xE1\x71\x48\x88\x98\x04\xFF\xDE\x6B\xFA\x0D\x86\xE3\xEE\x17\xD9\xAE\xA5\x79\xD0\xA2\x77"
	"\xDE\x35\x89\x4B\x9D\xE3\xEE\x6A\x24\x94\x15\xB7\x75\xFF\xA7\xE7\xAF\x7E\xFF\x44\xA4\x4C\x64\x58\xBE\x84\x57\x3A\x2A\x4F\xDE\x52"
	"\x9D\x73\xB1\x0B\x47\xC2\xAD\xCA\x1D\x45\x59\x2E\xEE\x07\xDC\x52\xF3\x27\x49\x78\x78\x20\xEF\x30\xD9\x1D\x53\x0A\xD6\x00\xC5\xB6"
	"\x94\x6B\x19\x92\x0B\xAE\xE1\x27\x99\x9D\xC2\x10\x22\xFB\x18\x20\x03\xC0\x5D\x1F\x25\x2E\x30\x9A\xEF\xC0\x14\xF6\x3A\x67\x38\x45"
	"\x35\x5D\x91\xE6\x15\x3D\x1D\xF7\xB5\x63\x96\x78\xC9\xEE\x56\x30\xFB\x74\x8C\x2A\x2E\x22\xC9\x88\x82\x6E\x54\x7C\xE2\x59\x8B\x1A"
	"\xF0\x8D\x5B\x7C\x74\x70\x4A\xD6\x50\xB3\xAB\x61\xAF\xDF\x3D\xB9\x12\xE0\x4F\x28\xFB\x28\x60\xBB\x20\x95\x3D\x81\x9B\xFC\x3C\xC1"
	"\x99\x51\xB3\xEA\x48\x86\x62\x49\x5F\x58\xE7\xE7\x18\xCA\xC9\x74\xEA\x4C\xE2\x34\x47\x8F\xC1\xB8\x4B\x19\x19\x93\x36\x6F\x9E\x0F"
	"\x1C\x8F\xC1\x15\xE7\x32\xE2\x85\xE6\xA1\x53\xCA\x6D\x5D\x5A\x2B\x7E\x4F\x16\x3C\xCA\xF5\x53\xF8\xE7\xF7\x5E\xFA\x11\x51\x48\x00"
	"\xF2\xD3\x23\x7C\x28\x8A\x94\xF2\x0F\xB4\xD8\xAD\x1A\x3C\xA9\x98\xE9\xAB\x41\xBE\xF8\x78\x8F\x80\x0E\x1F\xA1\x98\x4C\xEE\xFF\x35";

	// DMEM input parameter  (X2: 4096 bit)
	u8 DMEM_IN_X2[] =
	"\x00\x00\x00\x00\x00\x00\x00\x00" // 8-Bytes padded with zeros
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x11\x00\x00\x00\x00\x00\x00\x00\x01\x10\x00\x00\x00\x00\x00\x00\x00\x11";

	//--------------------------------------------------------------------------------------------------------------------------------
	// Endianness_setting of the DMEM inputs (from the "Least Significant Byte" to the "Most Significant Byte")
	u8 temp;

	//------------------------------------------------------------
	for(int i = 0; i < (L3L1_PARAM_WR_LEN / 2); i++)
	{
		temp = DMEM_IN_X0[i];
		DMEM_IN_X0[i] = DMEM_IN_X0[L3L1_PARAM_WR_LEN - 1 - i];
		DMEM_IN_X0[L3L1_PARAM_WR_LEN - 1 - i] = temp;
	}

	//------------------------------------------------------------
	for(int i = 0; i < (L3L1_PARAM_WR_LEN / 2); i++)
	{
		temp = DMEM_IN_X1[i];
		DMEM_IN_X1[i] = DMEM_IN_X1[L3L1_PARAM_WR_LEN - 1 - i];
		DMEM_IN_X1[L3L1_PARAM_WR_LEN - 1 - i] = temp;
	}

	//------------------------------------------------------------
	for(int i = 0; i < (L3L1_PARAM_WR_LEN / 2); i++)
	{
		temp = DMEM_IN_X2[i];
		DMEM_IN_X2[i] = DMEM_IN_X2[L3L1_PARAM_WR_LEN - 1 - i];
		DMEM_IN_X2[L3L1_PARAM_WR_LEN - 1 - i] = temp;
	}

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- Start of fpga_cp_ip11() -----\r\n");

	u8 output_packet[L3L1_PARAM_RD_LEN]; // Declaration of the output receiving packet from the FPGA

	//------------------------------------------------------------------------------------------
	//--- Start of MM --------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, DMEM_IN_X0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, DMEM_IN_X1, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("Result of Modular Multiplication (X2 Parameter):\r\n");
	for(int i = (L3L1_PARAM_RD_LEN - 1); i >= 0; i--)
	{
		xil_printf("%02x ", output_packet[i]);

		if(i % 32 == 0)
			xil_printf("\r\n");
	}
	//------------------------------------------------------------------------------------------
	//--- End of MM ----------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	//--- Start of ME --------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, DMEM_IN_X0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, DMEM_IN_X2, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_RD, X1_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("Result of Modular Exponentiation (X1 Parameter):\r\n");
	for(int i = (L3L1_PARAM_RD_LEN - 1); i >= 0; i--)
	{
		xil_printf("%02x ", output_packet[i]);

		if(i % 32 == 0)
			xil_printf("\r\n");
	}
	//------------------------------------------------------------------------------------------
	//--- End of ME ----------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	//--- Start of L1 <==> L3 Data WR/RD test scenario -----------------------------------------
	//------------------------------------------------------------------------------------------
	fpga_cp_ip11(CP_L1L2_DMEM_WR, X1_L1_PARAM, Y3_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L1L2_DMEM_RD, X5_L1_PARAM, Y3_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_RD, X5_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("X1_L1 ==> Y3_L2 and Y3_L2 ==> X5_L1; and X5:\r\n");
	for(int i = (L3L1_PARAM_RD_LEN - 1); i >= 0; i--)
	{
		xil_printf("%02x ", output_packet[i]);

		if(i % 32 == 0)
			xil_printf("\r\n");
	}
	//------------------------------------------------------------------------------------------
	//--- End of L1 <==> L3 Data WR/RD test scenario -------------------------------------------
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("-----  End  of fpga_cp_ip11() -----\r\n");
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void cp_ip11_mife_enc()
{
	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- Start of fpga_cp_ip11_mife_enc() -----\r\n");

	//--------------------------------------------------------------------------------------------------------------------------------
	u8 output_packet[L3L1_PARAM_WR_LEN];

	//------------------------------------------------------------------------------------------
	//--- Start of computing c0_16 --------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, h0_15, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, mife_rnd, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	//--- Start of L1 ==> L2 Data WR -----------------------------------------
	fpga_cp_ip11(CP_L1L2_DMEM_WR, X1_L1_PARAM, Y0_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, w0_15, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, DMEM_IN_N, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip11(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	u16 tmp = 0x0001;
	for(int i = 0; i < 0x200; i++)
	{
		tmp = output_packet[i] + tmp;
		output_packet[i] = tmp % 0x100;
		tmp = tmp >> 0x08;
	}

	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, output_packet, L3L1_PARAM_WR_LEN, NULL);

	//--- Start of L1 <== L2 Data RD -----------------------------------------
	fpga_cp_ip11(CP_L1L2_DMEM_RD, X1_L1_PARAM, Y0_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip11(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("Ciphertext c0_16:\r\n");
	for(int i = (L3L1_PARAM_RD_LEN - 1); i >= 0; i--)
	{
		xil_printf("%02x ", output_packet[i]);

		if(i % 32 == 0)
			xil_printf("\r\n");
	}

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("-----  End  of fpga_cp_ip11_mife_enc() ----- Iteration %d ----- \r\n");
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void cp_ip11_mife_dec()
{

}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void init_invmod_mife_dec()
{
	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- Start of init_invmod_mife_dec() ----- \r\n");

	//------------------------------------------------------------------------------------------
	u8 output_packet[L3L1_PARAM_RD_LEN];

	//------------------------------------------------------------------------------------------
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c0_0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, c1_0, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip11(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y0_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	fpga_cp_ip11(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip11(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, c3_0, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip11(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y1_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	fpga_cp_ip11(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y0_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);
	fpga_cp_ip11(CP_L1L2_DMEM_RD, X1_L1_PARAM, Y1_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip11(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip11(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip11(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	//-----------------------------------------------------------------------------------
//	for(int i = 0; i < 0x200; i++)
//	{
//		invmod_mife_dec_vec[i] = output_packet[0x200 - 1 - i];
//	}

	//-----------------------------------------------------------------------------------
	u8 inv_tmp_buffer[] = // Constant inversion result for the fixed random r
	"\x45\x47\xE5\x63\xCD\x3C\x26\x7B\x47\x87\x73\x37\xEE\xAE\x2D\x5F\x3E\x86\x12\x97\x8A\xF4\x54\x08\x1D\xCE\x36\x43\x94\x1B\x85\xEA"
	"\xF0\x23\x03\x0E\x7C\x18\x8B\x55\x79\xD7\xC4\xB4\x1C\x75\xAA\x6B\x5C\x2F\x3E\xC8\x9F\x31\xF6\xB5\x99\x36\x99\x65\x60\x3F\x95\xDC"
	"\x87\x94\x4D\x1C\x0C\xAE\x96\xBF\xBA\x6E\xEA\xC9\x9B\x84\x50\x38\x53\x50\x18\x91\x6C\xF2\x25\xBD\x30\x2E\x5E\x47\xA3\x62\x20\x16"
	"\x15\x3E\x45\xAF\xA6\x19\xE1\x4C\xA1\x8A\xBE\x9D\x3D\x35\xE6\x1D\x22\x3E\xDB\x6B\x39\xA1\x61\x2B\xBD\x5B\x60\x7D\x8B\x5E\x22\xC5"
	"\xD3\xD4\x4B\xBE\xC6\x15\xE5\x2A\x49\x23\x67\x55\x6C\xA9\x1D\x79\x07\xF6\x27\x84\x16\x28\xDE\xEA\xE6\x50\x3F\x77\x16\x38\xAD\xE5"
	"\xA2\x45\xC6\x40\x25\xB6\x39\x33\xB4\x4E\x19\xEE\xB4\x5F\x0B\xA0\xCC\x55\xC3\xDD\x44\x9C\xBD\xAD\x5E\xBE\x44\xE5\x79\x6E\xF5\xE8"
	"\xF0\xE6\x4D\x0F\x39\x60\x3C\xAD\xCD\x1B\xD2\x82\xAC\x29\x25\x75\x38\xA9\x83\x9F\xB9\xA4\x05\x02\x3D\xE7\x98\x61\xEB\xC1\xF3\x9C"
	"\x6A\xDF\x4D\x76\xD4\xF9\x9D\x4E\x36\x3F\x02\x86\x72\x0A\xE6\x45\xFF\x71\x90\x87\x49\x73\x35\x61\x47\xD4\xE5\xC0\x0B\x62\x15\x09"
	"\xCB\x29\x40\xE3\x71\x61\x48\xC1\xC4\x97\x82\xCF\x6B\x36\xE3\xF3\x7B\x7E\x22\xED\x1F\x64\x27\xCC\xE8\xD4\xEE\xCC\x17\xE7\x2F\xE1"
	"\x57\x02\x35\x5C\xC7\xDE\x36\x74\x00\x11\x9D\x49\x6F\x90\xFC\x23\x73\x5D\x5A\xE6\x0C\x60\xF5\xD3\xF7\xE7\x07\x9D\x7A\xAB\x8E\x33"
	"\x29\xC3\x37\xBD\xB5\x30\x53\x51\xBD\xDE\xE9\x7E\xCF\x28\xB7\x87\x4B\xD9\x59\x0F\x40\x87\x6E\x2A\x4A\xC0\xE5\x1B\xC0\x96\x77\x55"
	"\xAC\x2C\x84\xD1\x46\x64\x7C\x66\xEE\x2E\x76\xA6\x4A\x52\x07\xA7\xBC\x8F\xDB\xCD\x49\xC9\xAE\x79\xAE\x12\xC9\x9D\xA2\x13\x95\x54"
	"\xFA\x8F\xAD\xC3\x44\xD0\xF7\x75\x00\x21\xCD\x4D\xE0\x77\x43\xD7\x10\x8C\x13\x32\xD1\x15\x34\xBF\x52\xAB\x6B\x96\xCD\xFB\xA4\xE0"
	"\x5E\xDC\xA6\xF2\xE4\xE2\x8E\xEE\xA3\x49\xF7\xD7\x48\x43\xC8\x18\xB6\x4C\x19\x52\xD7\x7B\x11\x80\xE5\xB7\xE6\xC7\xCD\x9D\xB9\xCB"
	"\x0E\x8A\xC7\x1C\xC1\x68\x13\xA0\x38\xEB\x03\xA2\x01\xD0\xEF\x9C\xDB\x2A\xE9\xF4\xE4\x01\x3A\x89\x9D\x9C\x5A\x00\x61\xFF\xC6\x83"
	"\x83\xEE\xF3\x28\xB9\x69\x44\x94\x84\x71\xCE\x7F\x05\x18\x98\xA7\xC6\xF5\x29\x0A\xA7\x8A\x36\xF9\x76\xB2\x9B\x15\xAF\xAE\x35\xD7";

	for(int i = 0; i < 0x200; i++)
	{
		invmod_mife_dec_vec[i] = inv_tmp_buffer[i];
	}

	// Result printing ////////////////////////////////////////////////////////////////////
//	xil_printf("\r\n");
//	xil_printf("invmod_mife_dec_vec:\r\n");
//	for(i = 0; i < 0x200; i++)
//	{
//		xil_printf("%02x ", invmod_mife_dec_vec[i]);
//
//		if(i % 32 == 31)
//			xil_printf("\r\n");
//	}

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- End of init_invmod_mife_dec() -----\r\n");
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void final_ip_calc_mife_dec ()
{
	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- Start of final_ip_calc_mife_dec() -----\r\n");

	//------------------------------------------------------------------------------------------
	u32 N_Inv_L = 0x5EFD18F1; // Pre-computed value once for each key (i.e., N): N_Inv_L = (N ^ -1) mod L, and L = 2^32
	u32 z = 0xDC5E2217; // A parameter of MIFE-IP decryption key set

	//------------------------------------------------------------------------------------------
	u32 expected_ip = 0;
	for (int i = 0; i < 16; i++)
	{
		expected_ip = expected_ip + (x0[i] * y0[i]) + (x1[i] * y1[i]) + (x2[i] * y2[i]) + (x3[i] * y3[i]);
	}

	//------------------------------------------------------------------------------------------
	u32 computed_ip =
	(((((((mife_dec_r0_vec[0x1fc] << 24) + (mife_dec_r0_vec[0x1fd] << 16) + (mife_dec_r0_vec[0x1fe] << 8) + mife_dec_r0_vec[0x1ff]) +
	((mife_dec_r1_vec[0x1fc] << 24) + (mife_dec_r1_vec[0x1fd] << 16) + (mife_dec_r1_vec[0x1fe] << 8) + mife_dec_r1_vec[0x1ff]) +
	((mife_dec_r2_vec[0x1fc] << 24) + (mife_dec_r2_vec[0x1fd] << 16) + (mife_dec_r2_vec[0x1fe] << 8) + mife_dec_r2_vec[0x1ff]) +
	((mife_dec_r3_vec[0x1fc] << 24) + (mife_dec_r3_vec[0x1fd] << 16) + (mife_dec_r3_vec[0x1fe] << 8) + mife_dec_r3_vec[0x1ff]))
	% 0x100000000) * N_Inv_L) % 0x100000000) - z) % 0x100000000;

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("---------------------------------------------------------------\r\n");
	xil_printf("Expected inner-product (IP) = %08x\r\n", expected_ip);
	xil_printf("---------------------------------------------------------------\r\n");

	xil_printf("\r\n");
	xil_printf("---------------------------------------------------------------\r\n");
	xil_printf("MIFE-IP decryption scheme result:\r\n");
	xil_printf("Computed inner-product (IP) = %08x\r\n", computed_ip);
	xil_printf("---------------------------------------------------------------\r\n");

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("***********************************************************\r\n");
	if(computed_ip == expected_ip)
		xil_printf(" MIFE-IP decryption process performed successfully.\r\n");
	else
		xil_printf(" MIFE-IP decryption process performed with error!!!\r\n");
	xil_printf("***********************************************************\r\n");

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- End of final_ip_calc_mife_dec() -----\r\n");
}

//------------------------------------------------------------------------------------------------------------
