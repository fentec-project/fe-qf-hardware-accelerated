#include "dma.h"

//----------------------------------------------------------------------------
int axi_dma_init() {
	int Status;
	XAxiDma_Config *CfgPtr;

	//---------- AXI_DMA0_PCPs ------------------------------------------------------
	CfgPtr = XAxiDma_LookupConfig(AXI_DMA0_PCPs_DEV_ID);
	if (!CfgPtr) {
		xil_printf("No configuration found for AXI_DMA_%d\r\n", AXI_DMA0_PCPs_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AXI_DMA0_PCPs, CfgPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization of AXI_DMA_%d failed of %d\r\n", AXI_DMA0_PCPs_DEV_ID, Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AXI_DMA0_PCPs)) {
		xil_printf("AXI_DMA_%d configured as SG mode!\r\n", AXI_DMA0_PCPs_DEV_ID);
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AXI_DMA0_PCPs, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AXI_DMA0_PCPs, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	//----------------------------------------------------------------------------
	// Initialize other DMAs here ...

	xil_printf("AXI_DMAs initialization done.\n\n\r");

	return XST_SUCCESS;
}

//----------------------------------------------------------------------------
int axi_dma_send_packet(bool aligned_tx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *input) {
	int Status;

	if (aligned_tx == TRUE) {
		Xil_DCacheFlushRange((UINTPTR) input, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) input, length, XAXIDMA_DMA_TO_DEVICE);
		if (Status != XST_SUCCESS)
			return XST_FAILURE;
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DMA_TO_DEVICE));
	}
	else {
		u8 *TxBufferPtr;
		TxBufferPtr = (u8 *) DMA_TX_BUFFER_BASE;
		memcpy(TxBufferPtr, input, length);

		Xil_DCacheFlushRange((UINTPTR) TxBufferPtr, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) TxBufferPtr, length, XAXIDMA_DMA_TO_DEVICE);
		if (Status != XST_SUCCESS)
			return XST_FAILURE;
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DMA_TO_DEVICE));
	}

	return XST_SUCCESS;
}

//----------------------------------------------------------------------------
int axi_dma_receive_packet(bool aligned_rx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *output) {
	int Status;

	if (aligned_rx == TRUE) {
		Xil_DCacheFlushRange((UINTPTR) output, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) output, length, XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS)
			return XST_FAILURE;
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DEVICE_TO_DMA));
	}
	else {
		u8 *RxBufferPtr;
		RxBufferPtr = (u8 *) DMA_RX_BUFFER_BASE;

		Xil_DCacheFlushRange((UINTPTR) RxBufferPtr, length);
		Status = XAxiDma_SimpleTransfer(&axi_dma, (UINTPTR) RxBufferPtr, length, XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS)
			return XST_FAILURE;
		while (XAxiDma_Busy(&axi_dma, XAXIDMA_DEVICE_TO_DMA));
		memcpy(output, RxBufferPtr, length);
	}

	return XST_SUCCESS;
}

//----------------------------------------------------------------------------
