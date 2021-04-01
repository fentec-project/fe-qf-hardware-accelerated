`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module pcp_core
	(
	nreset,
	clock,

	pcp_active,
	ext_pcp_imem_offset,
	pcp_nbusy,
	pcp_done,

	imem_addr,
	imem_out,

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

	// cp-core ports //////////////////////////////////////////////////////////////////
	// reset and clock signals
	input			nreset;
	input			clock;
	
	// Control interface I/O with the external unit
	input			pcp_active;
	input   [3:0]   ext_pcp_imem_offset;
	output	reg		pcp_nbusy;
	output	reg		pcp_done;

	// Local instruction fetching ports of the pcp_core form the instruction memory
	output	[9:0]	imem_addr;
	input	[74:0]	imem_out;

	// Local data ports of the pcp_core from/to the data memory
	output	[9:0]	dmem_addr_1;
	output	[255:0]	dmem_in_1;
	output			dmem_we_1;
	input	[255:0]	dmem_out_1;

	output	[9:0]	dmem_addr_2;
	output	[255:0]	dmem_in_2;
	output			dmem_we_2;
	input	[255:0]	dmem_out_2;

	output	[9:0]	dmem_addr_3;
	output	[255:0]	dmem_in_3;
	output			dmem_we_3;
	input	[255:0]	dmem_out_3;

	output	[9:0]	dmem_addr_4;
	output	[255:0]	dmem_in_4;
	output			dmem_we_4;
	input	[255:0]	dmem_out_4;

	// Instruction set signals: Datapath and Control unit (75-bit)
	wire	[9:0]	IMEMaddr;	// IMEM addressing
	wire			MMMB1str;	// start of MMMB1
	wire			MMMB2str;	// start of MMMB2
	wire			MMMB3str;	// start of MMMB3
	wire			MASB1as;	// A/S of MASB1
	wire			MASB2as;	// A/S of MASB2
	wire			InREG1xyEn;	// Input REG enable
	wire			InREG2xyEn;	// Input REG enable
	wire			InREG3xyEn;	// Input REG enable
	wire			In4xSel;	// Input select
	wire			In4ySel;	// Input select
	wire			In5xSel;	// Input select
	wire			In5ySel;	// Input select
	wire	[1:0]	Out1Sel;	// Output select
	wire	[1:0]	Out2Sel;	// Output select
	wire			ADDR13wen;	// DMEM write enable
	wire			ADDR24wen;	// DMEM write enable
	wire	[9:0]	ADDR1val;	// Read/Write address_1
	wire	[9:0]	ADDR2val;	// Read/Write address_2
	wire	[9:0]	ADDR3val;	// Read/Write address_3
	wire	[9:0]	ADDR4val;	// Read/Write address_4
	wire	[1:0]	IndexOp;	// Index registers operation
	wire	[2:0]	BranchOp;	// Branch operation
	wire			nBusy;		// Active_low busy signal of pcp_core
	wire			Done;		// Done signal of pcp_core

	// Internal signals ////////////////////////////////////////////////////////////////
	wire	[255:0]	mmmb1_out;
	wire	[255:0]	mmmb2_out;
	wire	[255:0]	mmmb3_out;

	wire	[255:0]	masb1_out;
	wire	[255:0]	masb2_out;

	wire			mmmb1_done_1;
	wire			mmmb1_done_2;
	wire			mmmb2_done_1;
	wire			mmmb2_done_2;
	wire			mmmb3_done_1;
	wire			mmmb3_done_2;

	wire	[255:0]	out_13;
	wire	[255:0]	out_24;

	wire	[9:0]	imem_addr;

	wire	[255:0]	IN_4x;
	wire	[255:0]	IN_4y;
	wire	[255:0]	IN_5x;
	wire	[255:0]	IN_5y;

	// Output and internal registers
	reg		[5:0]	ILC;

	reg				first_cycle;
	reg		[9:0]	ret_imem_addr;

	reg		[255:0]	IN_REG1x;
	reg		[255:0]	IN_REG1y;
	reg		[255:0]	IN_REG2x;
	reg		[255:0]	IN_REG2y;
	reg		[255:0]	IN_REG3x;
	reg		[255:0]	IN_REG3y;

	reg		[255:0]	MREG;
	reg		[63:0]	mpREG;

	// Instantiations ////////////////////////////////////////////////////////////////
	mmmb mmmb1_u1
	(
    .nrst(nreset), 
    .clk(clock), 
    .start(MMMB1str), 
    .mp(mpREG), 
    .m(MREG), 
    .x(IN_REG1x), 
    .y(IN_REG1y), 
    .z(mmmb1_out), 
    .done_1(mmmb1_done_1),
	.done_2(mmmb1_done_2)
    );

	/////////////////////////////////////////////////////////////
	mmmb mmmb2_u2
	(
    .nrst(nreset), 
    .clk(clock), 
    .start(MMMB2str), 
    .mp(mpREG), 
    .m(MREG), 
    .x(IN_REG2x), 
    .y(IN_REG2y), 
    .z(mmmb2_out), 
    .done_1(mmmb2_done_1),
	.done_2(mmmb2_done_2)
    );

	/////////////////////////////////////////////////////////////
	mmmb mmmb3_u3
	(
    .nrst(nreset), 
    .clk(clock), 
    .start(MMMB3str), 
    .mp(mpREG), 
    .m(MREG), 
    .x(IN_REG3x), 
    .y(IN_REG3y), 
    .z(mmmb3_out), 
    .done_1(mmmb3_done_1),
    .done_2(mmmb3_done_2)
    );

	/////////////////////////////////////////////////////////////
	masb masb1_u4
	(
    .nrst(nreset), 
    .clk(clock), 
    .as_op(MASB1as), 
    .m(MREG), 
    .x(IN_4x), 
    .y(IN_4y), 
    .z(masb1_out)
    );

	/////////////////////////////////////////////////////////////
	masb masb2_u5
	(
    .nrst(nreset), 
    .clk(clock), 
    .as_op(MASB2as), 
    .m(MREG), 
    .x(IN_5x), 
    .y(IN_5y), 
    .z(masb2_out)
    );

	// Assignments /////////////////////////////////////////////////////////////

	// Instruction format (75-bit) /////////////////////////////
	assign IMEMaddr	  = imem_out[74:65];
	assign MMMB1str   = imem_out[64];
	assign MMMB2str   = imem_out[63];
	assign MMMB3str   = imem_out[62];
	assign MASB1as 	  = imem_out[61];
	assign MASB2as 	  = imem_out[60];
	assign InREG1xyEn = imem_out[59];
	assign InREG2xyEn = imem_out[58];
	assign InREG3xyEn = imem_out[57];
	assign In4xSel	  = imem_out[56];
	assign In4ySel    = imem_out[55];
	assign In5xSel    = imem_out[54];
	assign In5ySel	  = imem_out[53];
	assign Out1Sel 	  = imem_out[52:51];
	assign Out2Sel	  = imem_out[50:49];
	assign ADDR13wen  = imem_out[48];
	assign ADDR24wen  = imem_out[47];
	assign ADDR1val   = imem_out[46:37];
	assign ADDR2val	  = imem_out[36:27];
	assign ADDR3val   = imem_out[26:17];
	assign ADDR4val   = imem_out[16:7];
	assign IndexOp 	  = imem_out[6:5];
	assign BranchOp   = imem_out[4:2];
	assign nBusy 	  = imem_out[1];
	assign Done 	  = imem_out[0];

	// Combinational assignments //////////////////////////////////////
	assign dmem_addr_1 = ADDR1val;
	assign dmem_we_1 = ADDR13wen;
	assign dmem_in_1 = out_13;

	assign dmem_addr_2 = ADDR2val;
	assign dmem_we_2 = ADDR24wen;
	assign dmem_in_2 = out_24;

	assign dmem_addr_3 = ADDR3val;
	assign dmem_we_3 = ADDR13wen;
	assign dmem_in_3 = out_13;

	assign dmem_addr_4 = ADDR4val;
	assign dmem_we_4 = ADDR24wen;
	assign dmem_in_4 = out_24;

	assign out_13 = (Out1Sel == 2'd0) ? mmmb1_out : (Out1Sel == 2'd1) ? mmmb2_out : (Out1Sel == 2'd2) ? masb1_out : mmmb3_out;
	assign out_24 = (Out2Sel == 2'd0) ? mmmb1_out : (Out2Sel == 2'd1) ? mmmb2_out : (Out2Sel == 2'd2) ? masb1_out : masb2_out;

	assign IN_4x = (In4xSel == 1'b1) ? out_13 : dmem_out_1;
	assign IN_4y = (In4ySel == 1'b1) ? out_24 : dmem_out_3;

	assign IN_5x = (In5xSel == 1'b1) ? out_24 : dmem_out_2;
	assign IN_5y = (In5ySel == 1'b1) ? out_24 : dmem_out_4;

	assign imem_addr = ((first_cycle == 1'b1) ||
					   ((BranchOp == 3'b001) && (mmmb1_done_1 == 1'b1)) ||
					   ((BranchOp == 3'b010) && (mmmb1_done_2 == 1'b1)) ||
					   ((BranchOp == 3'b011) && (mmmb2_done_1 == 1'b1)) ||
					   ((BranchOp == 3'b100) && (mmmb2_done_2 == 1'b1)) ||
					   ((BranchOp == 3'b101) && (mmmb3_done_1 == 1'b1)) ||
					   ((BranchOp == 3'b110) && (mmmb3_done_2 == 1'b1)) ||
					   ((BranchOp == 3'b111) && ((ILC == ADDR1val[5:0]) || (ILC == ADDR2val[5:0]) ||
					   (ILC == ADDR3val[5:0]) || (ILC == ADDR4val[5:0])))) ? ret_imem_addr : IMEMaddr;

	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				pcp_nbusy <= 1'b1;
				pcp_done <= 1'b0;
			end
		else
			begin
				pcp_nbusy <= !pcp_active || nBusy;
				pcp_done <= pcp_active && Done;
			end
	end

	//////////////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				IN_REG1x <= 0;
				IN_REG1y <= 0;
				IN_REG2x <= 0;
				IN_REG2y <= 0;
				IN_REG3x <= 0;
				IN_REG3y <= 0;

				MREG <= 0;
				mpREG <= 0;

				ILC <= 0;
			end
		else
			begin
				//////////////////////////////////////////////////
				if (InREG1xyEn == 1'b1)
					begin
						IN_REG1x <= dmem_out_1;
						IN_REG1y <= dmem_out_2;
					end

				if (InREG2xyEn == 1'b1)
					begin
						IN_REG2x <= dmem_out_3;
						IN_REG2y <= dmem_out_4;
					end

				if (InREG3xyEn == 1'b1)
					begin
						IN_REG3x <= dmem_out_1;
						IN_REG3y <= dmem_out_3;
					end

				// Index registers operations ////////////////////////////////////
				case (IndexOp)
					2'd1:
						begin
							MREG <= dmem_out_1;
							mpREG <= dmem_out_2[63:0];
						end

					2'd2:
						begin
							ILC <= ADDR4val[5:0];
						end

					2'd3:
						begin
							ILC <= ILC - 6'd1;
						end
				endcase
			end
	end

	/////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if(!pcp_active || !nreset)
			begin
				first_cycle <= 1'b1;
				case(ext_pcp_imem_offset)
                    4'h0:
                        ret_imem_addr = 10'h000;
                    4'h1:
                        ret_imem_addr = 10'h010;
                    4'h2:
                        ret_imem_addr = 10'h020;
                    4'h3:
                        ret_imem_addr = 10'h030;
                    4'h4:
                        ret_imem_addr = 10'h040;
                    4'h5:
                        ret_imem_addr = 10'h050;
                    4'h6:
                        ret_imem_addr = 10'h060;
                    4'h7:
                        ret_imem_addr = 10'h070;
                    4'h8:
                        ret_imem_addr = 10'h080;
                    4'h9:
                        ret_imem_addr = 10'h090;
                    default:
                        ret_imem_addr = 10'h000;
                endcase
			end
		else
			begin
				first_cycle <= 1'b0;
				ret_imem_addr <= IMEMaddr + 10'd1;
			end
	end

	///////////////////////////////////////////////////////////////////////////////////
endmodule

/*
///////////////////////////////////////////////////////////////////////////////
module pcp_core
	(
	nreset,
	clock,

	pcp_active,
	pcp_nbusy,
	pcp_done,

	imem_addr,
	imem_out,

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

	// cp-core ports //////////////////////////////////////////////////////////////////
	// reset and clock signals
	input			nreset;
	input			clock;
	
	// Control interface I/O with the external unit
	input			pcp_active;
	output	reg		pcp_nbusy;
	output	reg		pcp_done;

	// Local instruction fetching ports of the pcp_core form the instruction memory
	output	[9:0]	imem_addr;
	input	[79:0]	imem_out;

	// Local data ports of the pcp_core from/to the data memory
	output	[9:0]	dmem_addr_1;
	output	[255:0]	dmem_in_1;
	output			dmem_we_1;
	input	[255:0]	dmem_out_1;

	output	[9:0]	dmem_addr_2;
	output	[255:0]	dmem_in_2;
	output			dmem_we_2;
	input	[255:0]	dmem_out_2;

	output	[9:0]	dmem_addr_3;
	output	[255:0]	dmem_in_3;
	output			dmem_we_3;
	input	[255:0]	dmem_out_3;

	output	[9:0]	dmem_addr_4;
	output	[255:0]	dmem_in_4;
	output			dmem_we_4;
	input	[255:0]	dmem_out_4;

	// Instruction set signals: Datapath and Control unit (80-bit)
	wire	[9:0]	IMEMaddr;	// IMEM addressing
	wire			MMMB1str;	// start of MMMB1
	wire			MMMB2str;	// start of MMMB2
	wire			MMMB3str;	// start of MMMB3
	wire			MASB1as;	// A/S of MASB1
	wire			MASB2as;	// A/S of MASB2
	wire			InREG1xyEn;	// Input REG enable
	wire			InREG2xyEn;	// Input REG enable
	wire			InREG3xyEn;	// Input REG enable
	wire			In4xSel;	// Input select
	wire			In4ySel;	// Input select
	wire			In5xSel;	// Input select
	wire			In5ySel;	// Input select
	wire	[1:0]	Out1Sel;	// Output select
	wire	[1:0]	Out2Sel;	// Output select
	wire			ADDR13wen;	// DMEM write enable
	wire			ADDR24wen;	// DMEM write enable
	wire	[9:0]	ADDR1val;	// Read/Write address_1
	wire	[9:0]	ADDR2val;	// Read/Write address_2
	wire	[9:0]	ADDR3val;	// Read/Write address_3
	wire	[9:0]	ADDR4val;	// Read/Write address_4
	wire	[1:0]	ILCsel;		// Loop counter index registers
	wire	[2:0]	ICVsel;		// Constant value index registers
	wire	[1:0]	IndexOp;	// Index registers operation
	wire	[2:0]	BranchOp;	// Branch operation
	wire			nBusy;		// Active_low busy signal of pcp_core
	wire			Done;		// Done signal of pcp_core

	// Internal signals ////////////////////////////////////////////////////////////////
	wire	[255:0]	mmmb1_out;
	wire	[255:0]	mmmb2_out;
	wire	[255:0]	mmmb3_out;

	wire	[255:0]	masb1_out;
	wire	[255:0]	masb2_out;

	wire			mmmb1_done;
	wire			mmmb2_done;
	wire			mmmb3_done;

	wire	[255:0]	out_13;
	wire	[255:0]	out_24;

	wire	[6:0]	ILC;
	wire	[6:0]	ICV;

	wire	[9:0]	imem_addr;

	wire	[255:0]	IN_4x;
	wire	[255:0]	IN_4y;
	wire	[255:0]	IN_5x;
	wire	[255:0]	IN_5y;

	// Output and internal registers
	reg		[6:0]	ILC_1,
					ILC_2,
					ILC_3;

	reg		[6:0]	ICV_1,
					ICV_2,
					ICV_3,
					ICV_4,
					ICV_5,
					ICV_6,
					ICV_7;

	reg				first_cycle;
	reg		[9:0]	ret_imem_addr;

	reg		[255:0]	IN_REG1x;
	reg		[255:0]	IN_REG1y;
	reg		[255:0]	IN_REG2x;
	reg		[255:0]	IN_REG2y;
	reg		[255:0]	IN_REG3x;
	reg		[255:0]	IN_REG3y;

	reg		[255:0]	MREG;
	reg		[63:0]	mpREG;

	// Instantiations ////////////////////////////////////////////////////////////////
	mmmb mmmb1_u1
	(
    .nrst(nreset), 
    .clk(clock), 
    .start(MMMB1str), 
    .mp(mpREG), 
    .m(MREG), 
    .x(IN_REG1x), 
    .y(IN_REG1y), 
    .z(mmmb1_out), 
    .done(mmmb1_done)
    );

	/////////////////////////////////////////////////////////////
	mmmb mmmb2_u2
	(
    .nrst(nreset), 
    .clk(clock), 
    .start(MMMB2str), 
    .mp(mpREG), 
    .m(MREG), 
    .x(IN_REG2x), 
    .y(IN_REG2y), 
    .z(mmmb2_out), 
    .done(mmmb2_done)
    );

	/////////////////////////////////////////////////////////////
	mmmb mmmb3_u3
	(
    .nrst(nreset), 
    .clk(clock), 
    .start(MMMB3str), 
    .mp(mpREG), 
    .m(MREG), 
    .x(IN_REG3x), 
    .y(IN_REG3y), 
    .z(mmmb3_out), 
    .done(mmmb3_done)
    );

	/////////////////////////////////////////////////////////////
	masb masb1_u4
	(
    .nrst(nreset), 
    .clk(clock), 
    .as_op(MASB1as), 
    .m(MREG), 
    .x(IN_4x), 
    .y(IN_4y), 
    .z(masb1_out)
    );

	/////////////////////////////////////////////////////////////
	masb masb2_u5
	(
    .nrst(nreset), 
    .clk(clock), 
    .as_op(MASB2as), 
    .m(MREG), 
    .x(IN_5x), 
    .y(IN_5y), 
    .z(masb2_out)
    );

	// Assignments /////////////////////////////////////////////////////////////

	// Instruction format (80-bit) /////////////////////////////
	assign IMEMaddr	  = imem_out[79:70];
	assign MMMB1str   = imem_out[69];
	assign MMMB2str   = imem_out[68];
	assign MMMB3str   = imem_out[67];
	assign MASB1as 	  = imem_out[66];
	assign MASB2as 	  = imem_out[65];
	assign InREG1xyEn = imem_out[64];
	assign InREG2xyEn = imem_out[63];
	assign InREG3xyEn = imem_out[62];
	assign In4xSel	  = imem_out[61];
	assign In4ySel    = imem_out[60];
	assign In5xSel    = imem_out[59];
	assign In5ySel	  = imem_out[58];
	assign Out1Sel 	  = imem_out[57:56];
	assign Out2Sel	  = imem_out[55:54];
	assign ADDR13wen  = imem_out[53];
	assign ADDR24wen  = imem_out[52];
	assign ADDR1val   = imem_out[51:42];
	assign ADDR2val	  = imem_out[41:32];
	assign ADDR3val   = imem_out[31:22];
	assign ADDR4val   = imem_out[21:12];
	assign ILCsel 	  = imem_out[11:10];
	assign ICVsel 	  = imem_out[9:7];
	assign IndexOp 	  = imem_out[6:5];
	assign BranchOp   = imem_out[4:2];
	assign nBusy 	  = imem_out[1];
	assign Done 	  = imem_out[0];

	// Combinational assignments //////////////////////////////////////
	assign dmem_addr_1 = ADDR1val;
	assign dmem_we_1 = ADDR13wen;
	assign dmem_in_1 = out_13;

	assign dmem_addr_2 = ADDR2val;
	assign dmem_we_2 = ADDR24wen;
	assign dmem_in_2 = out_24;

	assign dmem_addr_3 = ADDR3val;
	assign dmem_we_3 = ADDR13wen;
	assign dmem_in_3 = out_13;

	assign dmem_addr_4 = ADDR4val;
	assign dmem_we_4 = ADDR24wen;
	assign dmem_in_4 = out_24;

	assign out_13 = (Out1Sel == 2'd0) ? mmmb1_out : (Out1Sel == 2'd1) ? mmmb2_out : (Out1Sel == 2'd2) ? masb1_out : mmmb3_out;
	assign out_24 = (Out2Sel == 2'd0) ? mmmb1_out : (Out2Sel == 2'd1) ? mmmb2_out : (Out2Sel == 2'd2) ? masb1_out : masb2_out;

	assign	ILC = (ILCsel == 2'd1) ? ILC_1 : (ILCsel == 2'd2) ? ILC_2 : (ILCsel == 2'd3) ? ILC_3 : 7'd0;
	assign	ICV = (ICVsel == 3'd1) ? ICV_1 : (ICVsel == 3'd2) ? ICV_2 : (ICVsel == 3'd3) ? ICV_3 : (ICVsel == 3'd4) ? ICV_4 :
				  (ICVsel == 3'd5) ? ICV_5 : (ICVsel == 3'd6) ? ICV_6 : (ICVsel == 3'd7) ? ICV_7 : 7'd0;

	assign IN_4x = (In4xSel == 1'b1) ? out_13 : dmem_out_1;
	assign IN_4y = (In4ySel == 1'b1) ? out_24 : dmem_out_3;

	assign IN_5x = (In5xSel == 1'b1) ? out_24 : dmem_out_2;
	assign IN_5y = (In5ySel == 1'b1) ? out_24 : dmem_out_4;
	
	assign imem_addr = ((first_cycle == 1'b1) || ((BranchOp == 3'b001) && (mmmb1_done == 1'b1)) || 
		((BranchOp == 3'b010) && (mmmb2_done == 1'b1)) || ((BranchOp == 3'b011) && (mmmb3_done == 1'b1)) ||
		((BranchOp == 3'b100) && (ILC == ICV))) ? ret_imem_addr : IMEMaddr;

	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				pcp_nbusy <= 1'b1;
				pcp_done <= 1'b0;
			end
		else
			begin
				pcp_nbusy <= !pcp_active || nBusy;
				pcp_done <= pcp_active && Done;
			end
	end

	//////////////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				IN_REG1x <= 0;
				IN_REG1y <= 0;
				IN_REG2x <= 0;
				IN_REG2y <= 0;
				IN_REG3x <= 0;
				IN_REG3y <= 0;

				MREG <= 0;
				mpREG <= 0;

				ILC_1 <= 0;
				ILC_2 <= 0;
				ILC_3 <= 0;

				ICV_1 <= 0;
				ICV_2 <= 0;
				ICV_3 <= 0;
				ICV_4 <= 0;
				ICV_5 <= 0;
				ICV_6 <= 0;
				ICV_7 <= 0;
			end
		else
			begin
				//////////////////////////////////////////////////
				if (InREG1xyEn == 1'b1)
					begin
						IN_REG1x <= dmem_out_1;
						IN_REG1y <= dmem_out_2;
					end

				if (InREG2xyEn == 1'b1)
					begin
						IN_REG2x <= dmem_out_3;
						IN_REG2y <= dmem_out_4;
					end

				if (InREG3xyEn == 1'b1)
					begin
						IN_REG3x <= dmem_out_1;
						IN_REG3y <= dmem_out_3;
					end

				// Index registers operations ////////////////////////////////////
				case (IndexOp)
					2'd1:
						begin
							MREG <= dmem_out_1;
							mpREG <= dmem_out_2[63:0];
						end

					2'd2:
						begin
							case (ILCsel)
								2'd1:
									ILC_1 <= ADDR1val[6:0];
								2'd2:
									ILC_2 <= ADDR1val[6:0];
								2'd3:
									ILC_3 <= ADDR1val[6:0];
							endcase

							case (ICVsel)
								3'd1:
									ICV_1 <= ADDR2val[6:0];
								3'd2:
									ICV_2 <= ADDR2val[6:0];
								3'd3:
									ICV_3 <= ADDR2val[6:0];
								3'd4:
									ICV_4 <= ADDR2val[6:0];
								3'd5:
									ICV_5 <= ADDR2val[6:0];
								3'd6:
									ICV_6 <= ADDR2val[6:0];
								3'd7:
									ICV_7 <= ADDR2val[6:0];	
							endcase
						end

					2'd3:
						case (ILCsel)
							3'd1:
								ILC_1 <= ILC - 7'd1;
							3'd2:
								ILC_2 <= ILC - 7'd1;
							3'd3:
								ILC_3 <= ILC - 7'd1;
						endcase
				endcase
			end
	end

	/////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if(!pcp_active || !nreset)
			begin
				first_cycle <= 1'b1;
				ret_imem_addr <= 0;
			end
		else
			begin
				first_cycle <= 1'b0;
				ret_imem_addr <= IMEMaddr + 10'd1;
			end
	end

	///////////////////////////////////////////////////////////////////////////////////
endmodule*/