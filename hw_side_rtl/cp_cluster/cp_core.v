`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module cp_core
    #(
		parameter CP_D_WIDTH = 72,	// CP Datapath width
		parameter CP_I_WIDTH = 56,	// CP Instruction width
		parameter DMEM_ADDR_WIDTH = 10,	// (2 ^ DMEM_ADDR_WIDTH) * CP_D_WIDTH = DMEM Size
		parameter IMEM_ADDR_WIDTH = 9	// (2 ^ IMEM_ADDR_WIDTH) * CP_I_WIDTH = IMEM Size
	  )
	(
	nreset,
	clock,

	cp_active,
	cp_nbusy,
	cp_done,

	imem_addr,
	imem_out,

	dmem_addr_a,
	dmem_out_a,

	dmem_addr_b,
	dmem_out_b,
	
	dmem_addr_c,
	dmem_in_c,
	dmem_we_c
	);

	// cp-core ports //////////////////////////////////////////////////////////////////
	// reset and clock signals
	input							nreset;
	input							clock;
	
	// Control interface I/O with the external unit
	input							cp_active;
	output							cp_nbusy;
	output							cp_done;

	// Local instruction fetching ports of the cp_core form the instruction memory
	output	[IMEM_ADDR_WIDTH-1:0]	imem_addr;
	input	[CP_I_WIDTH-1:0]		imem_out;

	// Local data ports (Read_Port_A) of the cp_core from the data memory
	output	[DMEM_ADDR_WIDTH-1:0]	dmem_addr_a;
	input	[CP_D_WIDTH-1:0]		dmem_out_a;

	// Local data ports (Read_Port_B) of the cp_core from the data memory
	output	[DMEM_ADDR_WIDTH-1:0]	dmem_addr_b;
	input	[CP_D_WIDTH-1:0]		dmem_out_b;
	
	// Local data ports (Write_Port_C) of the cp_core from the data memory
	output	[DMEM_ADDR_WIDTH-1:0]	dmem_addr_c;
	output	[CP_D_WIDTH-1:0]		dmem_in_c;
	output							dmem_we_c;

	// Local parameter /////////////////////////////////////////////////////////////////
	localparam	Index_LCCV_WIDTH = 8; // 1 + Log2((Maximum_Length_Operation=8192)/(CP_D_WIDTH=72))

	// Instruction set signals: Datapath and Control unit
	wire	[8:0]					IMEMaddr;	// IMEM addressing
	wire							MulAccEn;   // Mul_Acc unit enable
	wire	[2:0]					ArithOp;	// Arithmetic units operation
	wire	[1:0]					ArithRegOp;	// Arithmetic registers operation
	wire	[1:0]					AuxREGOp;	// Auxiliary REG (AUX_REG) operation
	wire	[1:0]					InREG0Ld;	// Input REG0 (In_REG0) loading type
	wire	[1:0]					InREG1Ld;	// Input REG1 (In_REG1) loading type
	wire	[1:0]					InREG2Ld;	// Input REG2 (In_REG2) loading type

	wire	[9:0]					IntValue;	// Integer value
	wire	[1:0]					IRD1sel;	// Read address_1 (port_A) index registers of DMEM (IRD1)
	wire	[1:0]					IRD2sel;	// Read address_2 (port_B) index registers of DMEM (IRD2)
	wire	[1:0]					IWRsel;		// Write address (port_C) index registers of DMEM (IWR)
	wire							DMEMwe;		// DMEM write enable (Port_C) signal
	wire	[1:0]					ICVsel;		// Constant value index registers (ICV)
	wire	[2:0]					ILCsel;		// Loop counter index registers (ILC)
	wire	[2:0]					ILCrst;		// ILC registers reset
	wire	[2:0]					IndexOp;	// Index registers operation
	wire	[2:0]					BranchOp;	// Branch operation
	wire							nBusy;		// Active_low busy signal of cp_core
	wire							Done;		// Done signal of cp_core

	// Internal signals ////////////////////////////////////////////////////////////////
	wire	[CP_D_WIDTH-1:0]		mul_acc_out;
	wire	[CP_D_WIDTH-1:0]		mul_acc_carry;
	
	wire	[CP_D_WIDTH-1:0]		add_sub_out;
	wire							add_sub_carry;

	wire	[CP_D_WIDTH-1:0]		arith_out;

	wire	[DMEM_ADDR_WIDTH-1:0]	IRD1;
	wire	[DMEM_ADDR_WIDTH-1:0]	IRD2;
	wire	[DMEM_ADDR_WIDTH-1:0]	IWR;
	
	wire	[Index_LCCV_WIDTH-1:0]	ILC;
	wire	[Index_LCCV_WIDTH-1:0]	ICV;

	wire 	[1:0]					stack_pointer_sel;
	wire	[IMEM_ADDR_WIDTH-1:0]	ret_imem_addr;

	// Output and internal registers
	reg								cp_done;
	reg								cp_nbusy;

	reg		[DMEM_ADDR_WIDTH-1:0]	dmem_addr_c;
	reg		[CP_D_WIDTH-1:0]		dmem_in_c;
	reg								dmem_we_c;

	reg		[DMEM_ADDR_WIDTH-1:0]	IRD1_1,
									IRD1_2,
									IRD1_3;

	reg		[DMEM_ADDR_WIDTH-1:0]	IRD2_1,
									IRD2_2,
									IRD2_3;
	
	reg		[DMEM_ADDR_WIDTH-1:0]	IWR_1,
									IWR_2,
									IWR_3;

	reg		[Index_LCCV_WIDTH-1:0]	ILC_1,
									ILC_2,
									ILC_3,
									ILC_4,
									ILC_5,
									ILC_6,
									ILC_7;

	reg		[Index_LCCV_WIDTH-1:0]	ICV_1,
									ICV_2,
									ICV_3;

	reg		[IMEM_ADDR_WIDTH-1:0]	ret_imem_addr0,
									ret_imem_addr1,
									ret_imem_addr2,
									ret_imem_addr3;

	reg								first_cycle;
	reg		[1:0]					stack_pointer;
	reg		[IMEM_ADDR_WIDTH-1:0]	imem_addr;

	reg		[CP_D_WIDTH-1:0]		IN_REG0; // Input register 0
	reg		[CP_D_WIDTH-1:0]		IN_REG1; // Input register 1
	reg		[CP_D_WIDTH-1:0]		IN_REG2; // Input register 2

	reg		[CP_D_WIDTH-1:0]		AUX_REG; // Auxiliary register
	reg								extracted_bit_AuxREG;

	// Instantiations ////////////////////////////////////////////////////////////////
	mul_acc
	#(
		.CP_D_WIDTH(CP_D_WIDTH)
	 )
	mul_acc_unit
	(
		.nreset			(nreset),
		.clock 			(clock),
		.MulAccEn		(MulAccEn),
		.ArithOp		(ArithOp[2]),
		.ArithRegOp		(ArithRegOp[1]),
		.IN_REG0		(IN_REG0),
		.IN_REG1		(IN_REG1),
		.IN_REG2		(IN_REG2),
		.mul_acc_out	(mul_acc_out),
		.mul_acc_carry	(mul_acc_carry)
	);

	////////////////////////////////////////////////////////////
	add_sub
	#(
		.CP_D_WIDTH(CP_D_WIDTH)
	 )
	add_sub_unit
	(
		.nreset			(nreset),
		.clock 			(clock),
		.ArithOp		({ArithOp[2], ArithOp[0]}),
		.ArithRegOp		(ArithRegOp[0]),
		.IN_REG0		(IN_REG0),
		.IN_REG1		(IN_REG1),
		.add_sub_out	(add_sub_out),
		.add_sub_carry	(add_sub_carry)
	);

	// Assignments /////////////////////////////////////////////////////////////

	// Instruction format (CP_I_WIDTH = 56 bit) ////////////////////
	assign IMEMaddr		= imem_out[55:47];
	assign MulAccEn		= imem_out[46];
	assign ArithOp		= imem_out[45:43];
	assign ArithRegOp	= imem_out[42:41];
	assign AuxREGOp		= imem_out[40:39];
	assign InREG0Ld		= imem_out[38:37];
	assign InREG1Ld		= imem_out[36:35];
	assign InREG2Ld		= imem_out[34:33];

	assign IntValue		= imem_out[32:23];
	assign IRD1sel		= imem_out[22:21];
	assign IRD2sel		= imem_out[20:19];
	assign IWRsel		= imem_out[18:17];
	assign DMEMwe		= imem_out[16];
	assign ICVsel		= imem_out[15:14];
	assign ILCsel		= imem_out[13:11];
	assign ILCrst		= imem_out[10:8];
	assign IndexOp		= imem_out[7:5];
	assign BranchOp		= imem_out[4:2];
	assign nBusy		= imem_out[1];
	assign Done			= imem_out[0];

	// Combinational assignments //////////////////////////////////////
	assign dmem_addr_a = IRD1 + {2'b00, ILC};
	assign dmem_addr_b = IRD2 + {2'b00, ILC};

	assign arith_out = ({ArithOp[1], ArithOp[0]} == 2'b10) ? mul_acc_out :
					   ({ArithOp[1], ArithOp[0]} == 2'b11) ? mul_acc_carry :
					    add_sub_out;

	assign	IRD1 =	(IRD1sel == 2'b01) ? IRD1_1 :
					(IRD1sel == 2'b10) ? IRD1_2 :
					(IRD1sel == 2'b11) ? IRD1_3 :
					10'd0;

	assign	IRD2 =	(IRD2sel == 2'b01) ? IRD2_1 :
					(IRD2sel == 2'b10) ? IRD2_2 :
					(IRD2sel == 2'b11) ? IRD2_3 :
					10'd0;

	assign	IWR =	(IWRsel == 2'b01) ? IWR_1 :
					(IWRsel == 2'b10) ? IWR_2 :
					(IWRsel == 2'b11) ? IWR_3 :
					10'd0;

	assign	ILC =	(ILCsel == 3'b001) ? ILC_1 :
					(ILCsel == 3'b010) ? ILC_2 :
					(ILCsel == 3'b011) ? ILC_3 :
					(ILCsel == 3'b100) ? ILC_4 :
					(ILCsel == 3'b101) ? ILC_5 :
					(ILCsel == 3'b110) ? ILC_6 :
					(ILCsel == 3'b111) ? ILC_7 :
					8'd0;

	assign	ICV =	(ICVsel == 2'b01) ? ICV_1 :
					(ICVsel == 2'b10) ? ICV_2 :
					(ICVsel == 2'b11) ? ICV_3 :
					8'd0;

	assign	stack_pointer_sel = (BranchOp == 3'b101) ? (stack_pointer - 2'b01) : stack_pointer;

	assign	ret_imem_addr =	(stack_pointer_sel == 2'b00) ? ret_imem_addr0 :
							(stack_pointer_sel == 2'b01) ? ret_imem_addr1 :
							(stack_pointer_sel == 2'b10) ? ret_imem_addr2 :
							ret_imem_addr3;

	// IMEM address combinational procedures ////////////////////////////////////
	always @(*)
	begin
		if (first_cycle == 1'b1)
			begin
				imem_addr = ret_imem_addr;
			end
		else
			begin
				case (BranchOp)
					3'b000:	// No Branch
						imem_addr = IMEMaddr;

					3'b001:	// Loop_end conditional Jump
						if (ILC == ICV)
							begin
								imem_addr = ret_imem_addr;
							end
						else
							begin
								imem_addr = IMEMaddr;
							end

					3'b010:	// add_sub_carry conditional Jump
						if (add_sub_carry == 1'b1)
							begin
								imem_addr = ret_imem_addr;
							end
						else
							begin
								imem_addr = IMEMaddr;
							end

					3'b011:	// extracted_bit_AuxREG conditional Jump
						if (extracted_bit_AuxREG == 1'b0)
							begin
								imem_addr = ret_imem_addr;
							end
						else
							begin
								imem_addr = IMEMaddr;
							end

					3'b100: // Unconditional Call
						imem_addr = IMEMaddr;

					3'b101:	// Return
						imem_addr = ret_imem_addr;
					
					default:
						imem_addr = 9'bX;
				endcase
			end
	end

	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				cp_nbusy <= 1'b1;
				cp_done <= 1'b0;
				
				dmem_in_c <= 0;
				dmem_we_c <= 1'b0;
				dmem_addr_c <= 0;
			end
		else
			begin
				cp_nbusy <= !cp_active || nBusy;
				cp_done <= cp_active && Done;
				
				dmem_in_c <= arith_out;
				dmem_we_c <= DMEMwe;
				dmem_addr_c <= IWR + {2'b00, ILC} + IntValue;
			end
	end

	//////////////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				IN_REG0 <= 0;
				IN_REG1 <= 0;
				IN_REG2 <= 0;

				AUX_REG <= 0;
				extracted_bit_AuxREG <= 1'b0;

				IRD1_1 <= 10'd0;
				IRD1_2 <= 10'd0;
				IRD1_3 <= 10'd0;

				IRD2_1 <= 10'd0;
				IRD2_2 <= 10'd0;
				IRD2_3 <= 10'd0;

				IWR_1 <= 10'd0;
				IWR_2 <= 10'd0;
				IWR_3 <= 10'd0;

				ILC_1 <= 8'd0;
				ILC_2 <= 8'd0;
				ILC_3 <= 8'd0;
				ILC_4 <= 8'd0;
				ILC_5 <= 8'd0;
				ILC_6 <= 8'd0;
				ILC_7 <= 8'd0;

				ICV_1 <= 8'd0;
				ICV_2 <= 8'd0;
				ICV_3 <= 8'd0;
			end
		else
			begin
				// Input registers operations ////////////////////////////////////
				case (InREG0Ld)
					2'b01:
						IN_REG0 <= dmem_out_a;

					2'b10:
						IN_REG0 <= dmem_out_b;

					2'b11:
						IN_REG0 <= dmem_in_c;
				endcase

				case (InREG1Ld)
					2'b01:
						IN_REG1 <= dmem_out_a;

					2'b10:
						IN_REG1 <= dmem_out_b;

					2'b11:
						IN_REG1 <= 0;
				endcase

				case (InREG2Ld)
					2'b01:
						IN_REG2 <= dmem_out_a;

					2'b10:
						IN_REG2 <= dmem_out_b;
			
					2'b11:
						IN_REG2 <= 0;
				endcase

				case (AuxREGOp)
					2'b01:
						AUX_REG <= dmem_out_b;

					2'b10:
						begin
							extracted_bit_AuxREG <= AUX_REG[CP_D_WIDTH - 1];
							AUX_REG <= AUX_REG << 1;
						end
				endcase

				// Index registers operations ////////////////////////////////////
				case (IndexOp)
					3'b001:	// Load IRD1
						case (IRD1sel)
							2'b01:
								IRD1_1 <= IntValue;

							2'b10:
								IRD1_2 <= IntValue;

							2'b11:
								IRD1_3 <= IntValue;
						endcase
					
					3'b010:	// Load IRD2
						case (IRD2sel)
							2'b01:
								IRD2_1 <= IntValue;

							2'b10:
								IRD2_2 <= IntValue;

							2'b11:
								IRD2_3 <= IntValue;
						endcase

					3'b011:	// Load IWR
						case (IWRsel)
							2'b01:
								IWR_1 <= IntValue;

							2'b10:
								IWR_2 <= IntValue;

							2'b11:
								IWR_3 <= IntValue;
						endcase

					3'b100:	// Load ILC
						case (ILCsel)
							3'b001:
									ILC_1 <= IntValue[Index_LCCV_WIDTH-1:0];

							3'b010:
									ILC_2 <= IntValue[Index_LCCV_WIDTH-1:0];

							3'b011:
									ILC_3 <= IntValue[Index_LCCV_WIDTH-1:0];

							3'b100:
									ILC_4 <= IntValue[Index_LCCV_WIDTH-1:0];

							3'b101:
									ILC_5 <= IntValue[Index_LCCV_WIDTH-1:0];

							3'b110:
									ILC_6 <= IntValue[Index_LCCV_WIDTH-1:0];

							3'b111:
									ILC_7 <= IntValue[Index_LCCV_WIDTH-1:0];
						endcase

					3'b101:	// Load ICV
						case (ICVsel)
							2'b01:
								ICV_1 <= IntValue[Index_LCCV_WIDTH-1:0];

							2'b10:
								ICV_2 <= IntValue[Index_LCCV_WIDTH-1:0];

							2'b11:
								ICV_3 <= IntValue[Index_LCCV_WIDTH-1:0];
						endcase

					3'b110:	// Increment ILC
						case (ILCsel)
							3'b001:
								ILC_1 <= ILC + 8'd1;

							3'b010:
								ILC_2 <= ILC + 8'd1;

							3'b011:
								ILC_3 <= ILC + 8'd1;

							3'b100:
								ILC_4 <= ILC + 8'd1;

							3'b101:
								ILC_5 <= ILC + 8'd1;

							3'b110:
								ILC_6 <= ILC + 8'd1;

							3'b111:
								ILC_7 <= ILC + 8'd1;
						endcase

					3'b111:	// 	Decrement ILC
						case (ILCsel)
							3'b001:
								ILC_1 <= ILC - 8'd1;

							3'b010:
								ILC_2 <= ILC - 8'd1;

							3'b011:
								ILC_3 <= ILC - 8'd1;

							3'b100:
								ILC_4 <= ILC - 8'd1;

							3'b101:
								ILC_5 <= ILC - 8'd1;

							3'b110:
								ILC_6 <= ILC - 8'd1;

							3'b111:
								ILC_7 <= ILC - 8'd1;
						endcase
				endcase

				// ILC registers reset //////////////////////////////////
				case (ILCrst)
					3'b001:
						ILC_1 <= 8'd0;

					3'b010:
						ILC_2 <= 8'd0;

					3'b011:
						ILC_3 <= 8'd0;

					3'b100:
						ILC_4 <= 8'd0;

					3'b101:
						ILC_5 <= 8'd0;

					3'b110:
						ILC_6 <= 8'd0;

					3'b111:
						ILC_7 <= 8'd0;
				endcase
			end
	end

	/////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if(!cp_active || !nreset)
			begin
				first_cycle <= 1'b1;
				stack_pointer <= 2'b00;

				ret_imem_addr0 <= 0;
				ret_imem_addr1 <= 0;
				ret_imem_addr2 <= 0;
				ret_imem_addr3 <= 0;
			end
		else
			begin
				// imem_addr and stack logic //////////////////////
				first_cycle <= 1'b0;
				
				if (BranchOp == 3'b100)	// Call
					begin
						stack_pointer <= stack_pointer + 2'b01;
					end

				if (BranchOp == 3'b101)	// Return
					begin
						stack_pointer <= stack_pointer - 2'b01;
					end

				if (BranchOp != 3'b100)	// Call
				begin
					case (stack_pointer)
						2'b00:
							ret_imem_addr0 <= IMEMaddr + 9'd1;

						2'b01:
							ret_imem_addr1 <= IMEMaddr + 9'd1;

						2'b10:
							ret_imem_addr2 <= IMEMaddr + 9'd1;

						2'b11:
							ret_imem_addr3 <= IMEMaddr + 9'd1;
					endcase
				end
			end
	end

	///////////////////////////////////////////////////////////////////////////////////
endmodule
