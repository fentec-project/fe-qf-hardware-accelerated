`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module pcp_dmem
	(
	nreset,
	clock,

	dmem_addr_1,
	dmem_in_1,
	dmem_we_1,
	dmem_out_1,

	dmem_addr_2,
	dmem_in_2,
	dmem_we_2,
	dmem_out_2,

	dmem_addr_3,
	dmem_in_3,
	dmem_we_3,
	dmem_out_3,

	dmem_addr_4,
	dmem_in_4,
	dmem_we_4,
	dmem_out_4
    );
	
	// Ports //////////////////////////////////////////////////////////
	input			nreset;
	input 			clock;

	input	[9:0]	dmem_addr_1;
	input	[255:0]	dmem_in_1;
	input			dmem_we_1;
	output	[255:0]	dmem_out_1;

	input	[9:0]	dmem_addr_2;
	input	[255:0]	dmem_in_2;
	input			dmem_we_2;
	output	[255:0]	dmem_out_2;

	input	[9:0]	dmem_addr_3;
	input	[255:0]	dmem_in_3;
	input			dmem_we_3;
	output	[255:0]	dmem_out_3;

	input	[9:0]	dmem_addr_4;
	input	[255:0]	dmem_in_4;
	input			dmem_we_4;
	output	[255:0]	dmem_out_4;

	// Instantiations //////////////////////////////////////////////////
	tdp_bram tdp_bram_u1
	(
		// Read and Write Port_A
		.rsta(!nreset),
		.clka(clock),
		.addra(dmem_addr_1),
		.dina(dmem_in_1),
		.wea(dmem_we_1),
		.douta(dmem_out_1),
		
		// Read and Write Port_B
		.rstb(!nreset),
		.clkb(clock),
		.addrb(dmem_addr_2),
		.dinb(dmem_in_2),
		.web(dmem_we_2),
		.doutb(dmem_out_2)
	);
	
	////////////////////////////////////////////////////////////////////
	tdp_bram tdp_bram_u2
	(
		// Read and Write Port_A
		.rsta(!nreset),
		.clka(clock),
		.addra(dmem_addr_3),
		.dina(dmem_in_3),
		.wea(dmem_we_3),
		.douta(dmem_out_3),
		
		// Read and Write Port_B
		.rstb(!nreset),
		.clkb(clock),
		.addrb(dmem_addr_4),
		.dinb(dmem_in_4),
		.web(dmem_we_4),
		.doutb(dmem_out_4)
	);

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
