#include "dma.h"

//----------------------------------------------------------------------------
int axi_dma_init()
{
	int Status;
	XAxiDma_Config *CfgPtr;

	//---------- AXI_DMA0_CP01 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA0_CP01_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA0_CP01_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA0_CP01, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA0_CP01_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA0_CP01))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA0_CP01_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA0_CP01, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA0_CP01, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA1_CP23 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA1_CP23_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA1_CP23_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA1_CP23, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA1_CP23_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA1_CP23))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA1_CP23_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA1_CP23, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA1_CP23, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA2_CP45 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA2_CP45_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA2_CP45_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA2_CP45, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA2_CP45_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA2_CP45))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA2_CP45_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA2_CP45, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA2_CP45, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA3_CP67 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA3_CP67_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA3_CP67_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA3_CP67, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA3_CP67_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA3_CP67))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA3_CP67_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA3_CP67, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA3_CP67, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA4_CP89 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA4_CP89_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA4_CP89_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA4_CP89, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA4_CP89_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA4_CP89))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA4_CP89_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA4_CP89, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA4_CP89, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA5_CP1011 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA5_CP1011_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA5_CP1011_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA5_CP1011, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA5_CP1011_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA5_CP1011))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA5_CP1011_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA5_CP1011, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA5_CP1011, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA6_CP1213 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA6_CP1213_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA6_CP1213_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA6_CP1213, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA6_CP1213_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA6_CP1213))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA6_CP1213_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA6_CP1213, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA6_CP1213, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA7_CP1415 ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA7_CP1415_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA7_CP1415_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA7_CP1415, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA7_CP1415_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA7_CP1415))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA7_CP1415_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA7_CP1415, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA7_CP1415, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//---------- AXI_DMA8_PCP ----------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA8_PCP_DEV_ID);
	if (!CfgPtr)
	{
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA8_PCP_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA8_PCP, CfgPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA8_PCP_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA8_PCP))
	{
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA8_PCP_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA8_PCP, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA8_PCP, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//----------------------------------------------------------------------------
	// Initialize other DMAs here ...

	xil_printf("AXI_DMAs initialization done.\n\r");

	return XST_SUCCESS;
}

//----------------------------------------------------------------------------
int axi_dma_send_packet(bool aligned_tx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *input)
{
	int Status;

	if (aligned_tx == TRUE)
	{
		Xil_DCacheFlushRange((UINTPTR) input, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) input, length, XAXIDMA_DMA_TO_DEVICE);
		if (Status != XST_SUCCESS)
		{
			return XST_FAILURE;
		}
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DMA_TO_DEVICE));
	}
	else
	{
		u8 *TxBufferPtr;
		TxBufferPtr = (u8 *) DMA_TX_BUFFER_BASE;
		memcpy(TxBufferPtr, input, length);

		Xil_DCacheFlushRange((UINTPTR) TxBufferPtr, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) TxBufferPtr, length, XAXIDMA_DMA_TO_DEVICE);
		if (Status != XST_SUCCESS)
		{
			return XST_FAILURE;
		}
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DMA_TO_DEVICE));
	}

	return XST_SUCCESS;
}

//----------------------------------------------------------------------------
int axi_dma_receive_packet(bool aligned_rx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *output)
{
	int Status;

	if (aligned_rx == TRUE)
	{
		Xil_DCacheFlushRange((UINTPTR) output, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) output, length, XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS)
		{
			return XST_FAILURE;
		}
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DEVICE_TO_DMA));
	}
	else
	{
		u8 *RxBufferPtr;
		RxBufferPtr = (u8 *) DMA_RX_BUFFER_BASE;

		Xil_DCacheFlushRange((UINTPTR) RxBufferPtr, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) RxBufferPtr, length, XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS)
		{
			return XST_FAILURE;
		}
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DEVICE_TO_DMA));
		memcpy(output, RxBufferPtr, length);
	}

	return XST_SUCCESS;
}

//----------------------------------------------------------------------------
void dma_simple_tx_rx_tests()
{
	int Status;

	u8 data1[] /* DMA_ALIGNED */ =
	"\x16\x2B\xA2\x69\x87\xB1\x64\xC7\xA3\x67\xCF\xC4\xF9\x4B\x73\xA7\xFA\x1A\x3F\xDC\x81\xB1\x1E\xBB\x7B\x3D\xBA\x6D\x98\x83\x33\x51";
	u32 length_data1 = 0x20;

	u8 data2[] /* DMA_ALIGNED */ =
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	u32 length_data2 = 0x20;

	//----------------------------------------------------------------------------
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000AA5);

	Status = axi_dma_send_packet(FALSE, AXI_DMA8_PCP, AXI_DMA8_PCP_DEV_ID, length_data1, data1);
	if (Status != XST_SUCCESS)
	{
		xil_printf("axi_dma_send_packet() failed!\r\n");
		while(TRUE);
	}

	while((Xil_In32(FPGA_PCP_IPs_STATUS) & 0x00000001) != 0x00000001); // FPGA_PCP_IP0 busy checking!
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

	//----------------------------------------------------------------------------
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000AA7);

	Status = axi_dma_receive_packet(FALSE, AXI_DMA8_PCP, AXI_DMA8_PCP_DEV_ID, length_data2, data2);
	if (Status != XST_SUCCESS)
	{
		xil_printf("axi_dma_receive_packet() failed!\r\n");
		while(TRUE);
	}

	while((Xil_In32(FPGA_PCP_IPs_STATUS) & 0x00000001) != 0x00000001); // FPGA_PCP_IP0 busy checking!
	Xil_Out32(FPGA_PCP_IP0_CMD, 0x00000000);

	//----------------------------------------------------------------------------
	xil_printf("Rec_Data:\r\n");
	for (int i = 0; i < length_data2; i++)
	{
		xil_printf("%02x ", data2[i]);
		if (i % 16 == 15)
			xil_printf("\r\n");
	}
	xil_printf("\r\n");
}

//----------------------------------------------------------------------------
