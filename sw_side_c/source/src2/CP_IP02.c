#include "CP_IPs.h"

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
int fpga_cp_ip02(u8 cmd_cp, u8 L1_param,  u8 L2_param, u8 *input, u32 length, u8 *output)
{
	int Status;

	u32 command = (cmd_cp & 0x0000000F) | (((length / 8) << 4) & 0x00000FF0) |
					((L1_param << 12) & 0x0000F000) | ((L2_param << 16) & 0x000F0000);
	Xil_Out32(FPGA_CP_IP2_CMD, command);

	//-------------------------------------------------------------------------------------------------
	if ((cmd_cp == CP_IMEM_LD) || (cmd_cp == CP_L3L1_DMEM_WR))
	{
		Status = axi_dma_send_packet(TRUE, AXI_DMA1_CP23, AXI_DMA1_CP23_DEV_ID, length, input);
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi_dma_send_packet() failed!\r\n");
			return XST_FAILURE;
		}
	}

	//-------------------------------------------------------------------------------------------------
	if (cmd_cp == CP_L3L1_DMEM_RD)
	{
		Status = axi_dma_receive_packet(TRUE, AXI_DMA1_CP23, AXI_DMA1_CP23_DEV_ID, length, output);
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi_dma_receive_packet() failed!\r\n");
			return XST_FAILURE;
		}
	}

	//-------------------------------------------------------------------------------------------------
	while((Xil_In32(FPGA_CP_IPs_STATUS) & 0x00000030) != 0x00000030); // FPGA_CP_IP2 busy checking!
	Xil_Out32(FPGA_CP_IP2_CMD, 0x00000000);

	return XST_SUCCESS;
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void test_cp_ip02()
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
	xil_printf("----- Start of fpga_cp_ip2() -----\r\n");

	u8 output_packet[L3L1_PARAM_RD_LEN]; // Declaration of the output receiving packet_1 from the FPGA

	//------------------------------------------------------------------------------------------
	//--- Start of MM --------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, DMEM_IN_X0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, DMEM_IN_X1, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

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
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, DMEM_IN_X0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, DMEM_IN_X2, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_RD, X1_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

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
	fpga_cp_ip02(CP_L1L2_DMEM_WR, X1_L1_PARAM, Y3_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L1L2_DMEM_RD, X5_L1_PARAM, Y3_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_RD, X5_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

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
	xil_printf("-----  End  of fpga_cp_ip2() -----\r\n");
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void cp_ip02_mife_enc()
{
	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- Start of fpga_cp_ip02_mife_enc() -----\r\n");

	//--------------------------------------------------------------------------------------------------------------------------------
	u8 output_packet[L3L1_PARAM_WR_LEN];

	//------------------------------------------------------------------------------------------
	//--- Start of computing c0_4 --------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, h0_3, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, mife_rnd, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	//--- Start of L1 ==> L2 Data WR -----------------------------------------
	fpga_cp_ip02(CP_L1L2_DMEM_WR, X1_L1_PARAM, Y4_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, w0_3, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, DMEM_IN_N, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	u16 tmp = 0x0001;
	for(int i = 0; i < 0x200; i++)
	{
		tmp = output_packet[i] + tmp;
		output_packet[i] = tmp % 0x100;
		tmp = tmp >> 0x08;
	}
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, output_packet, L3L1_PARAM_WR_LEN, NULL);

	//--- Start of L1 <== L2 Data RD -----------------------------------------
	fpga_cp_ip02(CP_L1L2_DMEM_RD, X1_L1_PARAM, Y4_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("Ciphertext c0_4:\r\n");
	for(int i = (L3L1_PARAM_RD_LEN - 1); i >= 0; i--)
	{
		xil_printf("%02x ", output_packet[i]);

		if(i % 32 == 0)
			xil_printf("\r\n");
	}

	//------------------------------------------------------------------------------------------
	//--- Start of computing c0_5 --------------------------------------------------------------
	//------------------------------------------------------------------------------------------
	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, h0_4, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, mife_rnd, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	//--- Start of L1 ==> L2 Data WR -----------------------------------------
	fpga_cp_ip02(CP_L1L2_DMEM_WR, X1_L1_PARAM, Y5_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, w0_4, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, DMEM_IN_N, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	tmp = 0x0001;
	for(int i = 0; i < 0x200; i++)
	{
		tmp = output_packet[i] + tmp;
		output_packet[i] = tmp % 0x100;
		tmp = tmp >> 0x08;
	}

	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, output_packet, L3L1_PARAM_WR_LEN, NULL);

	//--- Start of L1 <== L2 Data RD -----------------------------------------
	fpga_cp_ip02(CP_L1L2_DMEM_RD, X1_L1_PARAM, Y5_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	// Result printing ////////////////////////////////////////////////////////////////////
	xil_printf("\r\n");
	xil_printf("Ciphertext c0_5:\r\n");
	for(int i = (L3L1_PARAM_RD_LEN - 1); i >= 0; i--)
	{
		xil_printf("%02x ", output_packet[i]);

		if(i % 32 == 0)
			xil_printf("\r\n");
	}

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("-----  End  of fpga_cp_ip02_mife_enc() -----\r\n");
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/
void cp_ip02_mife_dec()
{
	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- Start of fpga_cp_ip02_mife_dec() -----\r\n");

	//------------------------------------------------------------------------------------------
	u8 output_packet[L3L1_PARAM_RD_LEN]; // Declaration of the output receiving packet_1 from the FPGA

	//------------------------------------------------------------------------------------------
	for(int i = 0; i < 0x200; i++)
	{
		mife_dec_r2_vec[i] = invmod_mife_dec_vec[0x200 - 1 - i];
	}

	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, mife_dec_r2_vec, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, c0_0, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c1_0, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X1_L1_PARAM, NO_PARAM, c3_0, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L1L2_DMEM_RD, X1_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_RD_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, d2, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X1_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_1, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_0, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_2, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_1, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_3, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_2, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_4, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_3, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_5, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_4, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_6, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_5, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_7, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_6, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_8, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_7, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_9, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_8, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_10, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_9, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_11, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_10, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_12, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_11, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_13, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_12, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_14, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_13, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_15, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_14, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_WR, X2_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP input parameters writing on DMEM //////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X0_L1_PARAM, NO_PARAM, c2_16, L3L1_PARAM_WR_LEN, NULL);
	fpga_cp_ip02(CP_L3L1_DMEM_WR, X2_L1_PARAM, NO_PARAM, y2_15, L3L1_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME loading on IMEM (X1 = (X0 ^ X2) mod n2) ////////////////
	// Sub-operations: Mapping X0; X1R = (X0R ^ X2) mod n2, Un-mapping X1R to X1
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_ME, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_ME running ////////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	fpga_cp_ip02(CP_L1L2_DMEM_RD, X0_L1_PARAM, Y2_L2_PARAM, NULL, L1L2_PARAM_WR_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM loading on IMEM (X2 = (X0 * X1) mod n2) ///////////////
	// Sub-operations: Mapping X0 and X1; X2R = (X0R * X1R) mod n2, Un-mapping X2R to X2
	fpga_cp_ip02(CP_IMEM_LD, NO_PARAM, NO_PARAM, Micro_Code_Prog_MMM, PROG_LD_LEN, NULL);

	// CP_IP Micro_Code_Prog_MMM running ///////////////////////////////////////////////
	fpga_cp_ip02(CP_PROG_RUN, NO_PARAM, NO_PARAM, NULL, 0, NULL);

	// CP_IP output parameter reading from DMEM ////////////////////////////////////////
	fpga_cp_ip02(CP_L3L1_DMEM_RD, X2_L1_PARAM, NO_PARAM, NULL, L3L1_PARAM_RD_LEN, output_packet);

	u8 sub_flag = 0;
	for(int i = 0; i < 0x200; i++)
	{
		if(sub_flag == 0)
		{
			if(output_packet[i] == 0x00)
			{
				mife_dec_r2_vec[0x200 - 1 - i] = 0xFF;
			}
			else
			{
				mife_dec_r2_vec[0x200 - 1 - i] = output_packet[i] - 0x01;
				sub_flag = 1;
			}
		}
		else
		{
			mife_dec_r2_vec[0x200 - 1 - i] = output_packet[i];
		}
	}

	//------------------------------------------------------------------------------------------
	xil_printf("\r\n");
	xil_printf("----- End of fpga_cp_ip02_mife_dec() -----\r\n");
}

//------------------------------------------------------------------------------------------------------------
