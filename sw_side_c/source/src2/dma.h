#ifndef SRC_DMA_H_
#define SRC_DMA_H_

//----------------------------------------------------------------------------
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xil_io.h"
#include "config.h"
#include "stdbool.h"
#include "PCP_IPs.h"
#include "CP_IPs.h"

//----------------------------------------------------------------------------
#define DMA_ALIGNED __attribute__ ((aligned (XAXIDMA_BD_MINIMUM_ALIGNMENT)))

#define DMA_TX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00400000)
#define DMA_RX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00800000)

//----------------------------------------------------------------------------
XAxiDma AXI_DMA0_CP01;
XAxiDma AXI_DMA1_CP23;
XAxiDma AXI_DMA2_CP45;
XAxiDma AXI_DMA3_CP67;
XAxiDma AXI_DMA4_CP89;
XAxiDma AXI_DMA5_CP1011;
XAxiDma AXI_DMA6_CP1213;
XAxiDma AXI_DMA7_CP1415;
XAxiDma AXI_DMA8_PCP;

#define AXI_DMA0_CP01_DEV_ID	XPAR_AXIDMA_0_DEVICE_ID
#define AXI_DMA1_CP23_DEV_ID	XPAR_AXIDMA_1_DEVICE_ID
#define AXI_DMA2_CP45_DEV_ID	XPAR_AXIDMA_2_DEVICE_ID
#define AXI_DMA3_CP67_DEV_ID	XPAR_AXIDMA_3_DEVICE_ID
#define AXI_DMA4_CP89_DEV_ID	XPAR_AXIDMA_4_DEVICE_ID
#define AXI_DMA5_CP1011_DEV_ID	XPAR_AXIDMA_5_DEVICE_ID
#define AXI_DMA6_CP1213_DEV_ID	XPAR_AXIDMA_6_DEVICE_ID
#define AXI_DMA7_CP1415_DEV_ID	XPAR_AXIDMA_7_DEVICE_ID
#define AXI_DMA8_PCP_DEV_ID		XPAR_AXIDMA_8_DEVICE_ID

//----------------------------------------------------------------------------
int axi_dma_init();
int axi_dma_send_packet(bool aligned_tx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *input);
int axi_dma_receive_packet(bool aligned_rx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *output);

void dma_simple_tx_rx_tests();

//----------------------------------------------------------------------------
#endif /* SRC_DMA_H_ */
