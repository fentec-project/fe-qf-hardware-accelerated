`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module cp_data_mem
    #(
		parameter CP_D_WIDTH = 72, 		// CP Datapath width
		parameter DMEM_ADDR_WIDTH = 10	// (2 ^ DMEM_ADDR_WIDTH) * CP_D_WIDTH = DMEM Size
	  )
	(
	nreset,
	clock,
	
	dmem_addr_a,
	dmem_out_a,
	
	dmem_addr_b,
	dmem_out_b,
	
	dmem_addr_c,
	dmem_in_c,
	dmem_we_c,
	dmem_out_c
    );
	
	// Ports //////////////////////////////////////////////////////////
	input								nreset;
	input 								clock;

	input	[DMEM_ADDR_WIDTH-1:0]		dmem_addr_a;
	output	[CP_D_WIDTH-1:0]			dmem_out_a;

	input	[DMEM_ADDR_WIDTH-1:0]		dmem_addr_b;
	output	[CP_D_WIDTH-1:0]			dmem_out_b;

	input	[DMEM_ADDR_WIDTH-1:0]		dmem_addr_c;
	input	[CP_D_WIDTH-1:0]			dmem_in_c;
	input								dmem_we_c;
	output	[CP_D_WIDTH-1:0]			dmem_out_c;

	// Instantiations //////////////////////////////////////////////////
	tdp_ram_block
//	#(
//        .DATA_WIDTH(CP_D_WIDTH),
//        .ADDR_WIDTH(DMEM_ADDR_WIDTH)
//     )
	tdp_ram_block_u1		// True Dual Port (TDP) RAM
	(
		// Read and Write Port_A
		.rsta(!nreset),
		.clka(clock),
		.addra(dmem_addr_c),
		.dina(dmem_in_c),
		.wea(dmem_we_c),
		.douta(dmem_out_c),
		
		// Read and Write Port_B
		.rstb(!nreset),
		.clkb(clock),
		.addrb(dmem_addr_a),
		.dinb(72'b0),
		.web(1'b0),
		.doutb(dmem_out_a)
	);
	
	////////////////////////////////////////////////////////////////////
	tdp_ram_block
//    #(
//        .DATA_WIDTH(CP_D_WIDTH),
//        .ADDR_WIDTH(DMEM_ADDR_WIDTH)
//     )
    tdp_ram_block_u2		// True Dual Port (TDP) RAM
	(
		// Read and Write Port_A
		.rsta(!nreset),
		.clka(clock),
		.addra(dmem_addr_c),
		.dina(dmem_in_c),
		.wea(dmem_we_c),
		.douta(),

		// Read and Write Port_B
		.rstb(!nreset),
		.clkb(clock),
		.addrb(dmem_addr_b),
		.dinb(72'b0),
		.web(1'b0),
		.doutb(dmem_out_b)
	);

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
