`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
module cp_top
    #(
		parameter CP_D_WIDTH = 72,	// CP Datapath width
		parameter CP_I_WIDTH = 56,	// CP Instruction width
		parameter DMEM_ADDR_WIDTH = 10,	// (2 ^ DMEM_ADDR_WIDTH) * CP_D_WIDTH = DMEM Size
		parameter IMEM_ADDR_WIDTH = 9	// (2 ^ IMEM_ADDR_WIDTH) * CP_I_WIDTH = IMEM Size
	  )
	(
	nreset,
	clock,

	ext_cp_active,
	cp_ext_nbusy,
	cp_ext_done,
	
	ext_imem_addr,
	ext_imem_in,
	ext_imem_we,

	ext_dmem_addr,
	ext_dmem_in,
	ext_dmem_we,
	dmem_ext_out
    );
	
	// Ports ////////////////////////////////////////////////////////
	input 								nreset;
	input 								clock;

	input 								ext_cp_active;
	output								cp_ext_nbusy;
	output								cp_ext_done;

	input	[IMEM_ADDR_WIDTH-1:0]		ext_imem_addr;
	input	[CP_I_WIDTH-1:0]			ext_imem_in;
	input								ext_imem_we;

	input	[DMEM_ADDR_WIDTH-1:0]		ext_dmem_addr;
	input	[CP_D_WIDTH-1:0]			ext_dmem_in;
	input								ext_dmem_we;
	output	[CP_D_WIDTH-1:0]			dmem_ext_out;

	// Wires /////////////////////////////////////////////////////////	
	wire	[IMEM_ADDR_WIDTH-1:0]		cp_imem_addr;
	wire	[CP_I_WIDTH-1:0]			imem_cp_out;

	wire	[DMEM_ADDR_WIDTH-1:0]		cp_dmem_addr_a;
	wire	[CP_D_WIDTH-1:0]			dmem_cp_out_a;
	
	wire	[DMEM_ADDR_WIDTH-1:0]		cp_dmem_addr_b;
	wire	[CP_D_WIDTH-1:0]			dmem_cp_out_b;
	
	wire	[DMEM_ADDR_WIDTH-1:0]		selected_dmem_addr_c;
	wire	[CP_D_WIDTH-1:0]			selected_dmem_in_c;
	wire								selected_dmem_we_c;

	wire	[DMEM_ADDR_WIDTH-1:0]		cp_dmem_addr_c;
	wire	[CP_D_WIDTH-1:0]			cp_dmem_in_c;
	wire								cp_dmem_we_c;

	// Assignments /////////////////////////////////////////////////////
	assign selected_dmem_addr_c = (ext_cp_active) ? cp_dmem_addr_c : ext_dmem_addr;
	assign selected_dmem_in_c = (ext_cp_active) ? cp_dmem_in_c : ext_dmem_in;
    assign selected_dmem_we_c = (ext_cp_active) ? cp_dmem_we_c : ext_dmem_we;

	// Instantiations //////////////////////////////////////////////////
	cp_core
	#(
		.CP_D_WIDTH(CP_D_WIDTH),
		.CP_I_WIDTH(CP_I_WIDTH),
		.DMEM_ADDR_WIDTH(DMEM_ADDR_WIDTH),
		.IMEM_ADDR_WIDTH(IMEM_ADDR_WIDTH)
	 )
	cp_core_unit
	(
		.nreset(nreset),
		.clock(clock),
		
		.cp_active(ext_cp_active),
		.cp_nbusy(cp_ext_nbusy),
		.cp_done(cp_ext_done),

		.imem_addr(cp_imem_addr),
		.imem_out(imem_cp_out),
		
		.dmem_addr_a(cp_dmem_addr_a),
		.dmem_out_a(dmem_cp_out_a),

		.dmem_addr_b(cp_dmem_addr_b),
		.dmem_out_b(dmem_cp_out_b),
		
		.dmem_addr_c(cp_dmem_addr_c),
		.dmem_in_c(cp_dmem_in_c),
		.dmem_we_c(cp_dmem_we_c)
	);

	///////////////////////////////////////////////
	cp_inst_mem
	#(
		.CP_I_WIDTH(CP_I_WIDTH),
		.IMEM_ADDR_WIDTH(IMEM_ADDR_WIDTH)
	 )
	 cp_inst_mem_unit
	(
		.nreset(nreset),
		.clock(clock),
		
		// Read Port of IMEM
		.imem_addr_a(cp_imem_addr),
		.imem_out_a(imem_cp_out),
		
		// Write Port of IMEM
		.imem_addr_b(ext_imem_addr),
		.imem_in_b(ext_imem_in),
        .imem_we_b(ext_imem_we)
	);

	///////////////////////////////////////////////
	cp_data_mem
	#(
		.CP_D_WIDTH(CP_D_WIDTH),
		.DMEM_ADDR_WIDTH(DMEM_ADDR_WIDTH)
	 )
	cp_data_mem_unit
	(
		.nreset(nreset),
		.clock(clock),

		// Read_Port_A of DMEM
		.dmem_addr_a(cp_dmem_addr_a),
		.dmem_out_a(dmem_cp_out_a),
		
		// Read_Port_B of DMEM
		.dmem_addr_b(cp_dmem_addr_b),
		.dmem_out_b(dmem_cp_out_b),
		
		// Read and Write Port_C of DMEM
		.dmem_addr_c(selected_dmem_addr_c),
		.dmem_in_c(selected_dmem_in_c),
		.dmem_we_c(selected_dmem_we_c),
		.dmem_out_c(dmem_ext_out)
	);

////////////////////////////////////////////////////////////////////////
endmodule
