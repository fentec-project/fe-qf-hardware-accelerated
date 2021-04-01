`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module pcp_imem
	(
	nreset,
	clock,

	imem_addr_1,
	imem_out_1,

	imem_addr_2,
	imem_in_2,
	imem_we_2
    );

	// Ports //////////////////////////////////////////////////////////////
	input			nreset;
	input 			clock;

	input	[9:0]	imem_addr_1;
	output	[74:0]	imem_out_1;

	input	[9:0]	imem_addr_2;
	input	[74:0]	imem_in_2;
	input			imem_we_2;

	// Instantiations /////////////////////////////////////////////////////
	sdp_bram sdp_bram_u
	(
		// Write Port
		.clka(clock),
		.addra(imem_addr_2),
		.dina(imem_in_2),
		.wea(imem_we_2),

		// Read Port
		.rstb(!nreset),
		.clkb(clock),
		.addrb(imem_addr_1),
		.doutb(imem_out_1)
	);

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
