`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module cp_inst_mem
    #(
		parameter CP_I_WIDTH = 56, 		// CP Instruction width
		parameter IMEM_ADDR_WIDTH = 9	// (2 ^ IMEM_ADDR_WIDTH) * CP_I_WIDTH = IMEM Size
	  )
	(
	nreset,
	clock,

	imem_addr_a,
	imem_out_a,

	imem_addr_b,
	imem_in_b,
	imem_we_b
    );
    
	// Ports //////////////////////////////////////////////////////////////
	input								nreset;
	input 								clock;

	input	[IMEM_ADDR_WIDTH-1:0]		imem_addr_a;
	output	[CP_I_WIDTH-1:0]			imem_out_a;

	input	[IMEM_ADDR_WIDTH-1:0]		imem_addr_b;
	input	[CP_I_WIDTH-1:0]			imem_in_b;
	input								imem_we_b;

	// Instantiations /////////////////////////////////////////////////////
	sdp_ram_block
//	#(
//        .DATA_WIDTH(CP_I_WIDTH),
//        .ADDR_WIDTH(IMEM_ADDR_WIDTH)
//     )
	sdp_ram_block_u		// Simple Dual Port (SDP) RAM
	(
		// Write Port
//		.rsta(!nreset) // TODO
		.clka(clock),
		.addra(imem_addr_b),
		.dina(imem_in_b),
		.wea(imem_we_b),

		// Read Port
//		.rstb(!nreset) // TODO
		.clkb(clock),
		.addrb(imem_addr_a),
		.doutb(imem_out_a)
	);
/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
