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

//----------------------------------------------------------------------------
#define DMA_ALIGNED __attribute__ ((aligned (XAXIDMA_BD_MINIMUM_ALIGNMENT)))

#define DMA_TX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00400000)
#define DMA_RX_BUFFER_BASE	(MEM_BASE_ADDR + 0x00800000)

//----------------------------------------------------------------------------
XAxiDma AXI_DMA0_PCPs;
#define AXI_DMA0_PCPs_DEV_ID	XPAR_AXIDMA_0_DEVICE_ID

//----------------------------------------------------------------------------
int axi_dma_init();
int axi_dma_send_packet(bool aligned_tx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *input);
int axi_dma_receive_packet(bool aligned_rx, XAxiDma axi_dma, u32 axi_dma_dev_id, u32 length, u8 *output);

//----------------------------------------------------------------------------
#endif /* SRC_DMA_H_ */
