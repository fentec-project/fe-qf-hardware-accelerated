`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
module pcp_top
	(
	nreset,
	clock,

	ext_pcp_active,
	ext_pcp_imem_offset,
	pcp_ext_nbusy,
	pcp_ext_done,
	
	ext_imem_addr,
	ext_imem_in,
	ext_imem_we,

	ext_dmem_addr,
	ext_dmem_in,
	ext_dmem_we,
	dmem_ext_out
    );
	
	// Ports ////////////////////////////////////////////////////////
	input 			nreset;
	input 			clock;

	input 			ext_pcp_active;
	input   [3:0]   ext_pcp_imem_offset;
	output			pcp_ext_nbusy;
	output			pcp_ext_done;

	input	[9:0]	ext_imem_addr;
	input	[74:0]	ext_imem_in;
	input			ext_imem_we;

	input	[9:0]	ext_dmem_addr;
	input	[255:0]	ext_dmem_in;
	input			ext_dmem_we;
	output	[255:0]	dmem_ext_out;

	// Wires /////////////////////////////////////////////////////////	
	wire	[9:0]	pcp_imem_addr;
	wire	[74:0]	imem_pcp_out;

	wire	[9:0]	w_addr_1;
	wire	[255:0]	w_in_1;
	wire			w_we_1;
	wire	[255:0]	w_out_1;

	wire	[9:0]	w_addr_2;
	wire	[255:0]	w_in_2;
	wire			w_we_2;
	wire	[255:0]	w_out_2;

	wire	[9:0]	w_addr_3;
	wire	[255:0]	w_in_3;
	wire			w_we_3;
	wire	[255:0]	w_out_3;

	wire	[9:0]	w_addr_4;
	wire	[255:0]	w_in_4;
	wire			w_we_4;
	wire	[255:0]	w_out_4;

	wire	[9:0]	w_sel_addr_1;
	wire	[255:0]	w_sel_in_1;
	wire			w_sel_we_1;

	wire	[9:0]	w_sel_addr_3;
	wire	[255:0]	w_sel_in_3;
	wire			w_sel_we_3;

	// Assignments /////////////////////////////////////////////////////
	assign w_sel_addr_1 = (ext_pcp_active) ? w_addr_1 : ext_dmem_addr;
	assign w_sel_in_1 = (ext_pcp_active) ? w_in_1 : ext_dmem_in;
    assign w_sel_we_1 = (ext_pcp_active) ? w_we_1 : ext_dmem_we;

	assign w_sel_addr_3 = (ext_pcp_active) ? w_addr_3 : ext_dmem_addr;
	assign w_sel_in_3 = (ext_pcp_active) ? w_in_3 : ext_dmem_in;
    assign w_sel_we_3 = (ext_pcp_active) ? w_we_3 : ext_dmem_we;

	assign dmem_ext_out = w_out_1;

	// Instantiations //////////////////////////////////////////////////
	pcp_core pcp_core_u
	(
		.nreset(nreset),
		.clock(clock),

		.pcp_active(ext_pcp_active),
		.ext_pcp_imem_offset(ext_pcp_imem_offset),
		.pcp_nbusy(pcp_ext_nbusy),
		.pcp_done(pcp_ext_done),

		.imem_addr(pcp_imem_addr),
		.imem_out(imem_pcp_out),

		.dmem_addr_1(w_addr_1),
		.dmem_in_1(w_in_1),
		.dmem_we_1(w_we_1),
		.dmem_out_1(w_out_1),

		.dmem_addr_2(w_addr_2),
		.dmem_in_2(w_in_2),
		.dmem_we_2(w_we_2),
		.dmem_out_2(w_out_2),

		.dmem_addr_3(w_addr_3),
		.dmem_in_3(w_in_3),
		.dmem_we_3(w_we_3),
		.dmem_out_3(w_out_3),

		.dmem_addr_4(w_addr_4),
		.dmem_in_4(w_in_4),
		.dmem_we_4(w_we_4),
		.dmem_out_4(w_out_4)
	);

	///////////////////////////////////////////////
	pcp_imem pcp_imem_u
	(
		.nreset(nreset),
		.clock(clock),
		
		// Read Port of IMEM
		.imem_addr_1(pcp_imem_addr),
		.imem_out_1(imem_pcp_out),
		
		// Write Port of IMEM
		.imem_addr_2(ext_imem_addr),
		.imem_in_2(ext_imem_in),
        .imem_we_2(ext_imem_we)
	);

	///////////////////////////////////////////////
	pcp_dmem pcp_dmem_u
	(
		.nreset(nreset),
		.clock(clock),

		.dmem_addr_1(w_sel_addr_1),
		.dmem_in_1(w_sel_in_1),
		.dmem_we_1(w_sel_we_1),
		.dmem_out_1(w_out_1),

		.dmem_addr_2(w_addr_2),
		.dmem_in_2(w_in_2),
		.dmem_we_2(w_we_2),
		.dmem_out_2(w_out_2),

		.dmem_addr_3(w_sel_addr_3),
		.dmem_in_3(w_sel_in_3),
		.dmem_we_3(w_sel_we_3),
		.dmem_out_3(w_out_3),

		.dmem_addr_4(w_addr_4),
		.dmem_in_4(w_in_4),
		.dmem_we_4(w_we_4),
		.dmem_out_4(w_out_4)
	);

////////////////////////////////////////////////////////////////////////
endmodule
