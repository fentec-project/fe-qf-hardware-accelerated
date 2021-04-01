`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module mul_acc
    #(
		parameter CP_D_WIDTH = 72 // CP Datapath width
	 )
	(
	nreset,
	clock,
	MulAccEn,
	ArithOp,
	ArithRegOp,
	IN_REG0,
	IN_REG1,
	IN_REG2,
	mul_acc_out,
	mul_acc_carry
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input							nreset;
	input							clock;
	
	input							MulAccEn;
	input							ArithOp;
	input							ArithRegOp;
	
	input	[CP_D_WIDTH-1:0]		IN_REG0;
	input	[CP_D_WIDTH-1:0]		IN_REG1;
	input	[CP_D_WIDTH-1:0]		IN_REG2;

	output	[CP_D_WIDTH-1:0]		mul_acc_out;
	output	[CP_D_WIDTH-1:0]		mul_acc_carry;

	// Internal signal and register ////////////////////////////////////////////
	wire	[2*CP_D_WIDTH-1:0]		mul_acc_core_out;
	wire	[CP_D_WIDTH-1:0]		carry_w_in;	
	wire							carry_1_in;
	wire	[CP_D_WIDTH:0]			final_add_res;	

	reg		[CP_D_WIDTH-1:0]		mul_acc_carry_w;
	reg								mul_acc_carry_1;

    // Combinational assignment ////////////////////////////////////////////////
    assign final_add_res = mul_acc_core_out[CP_D_WIDTH-1:0] + carry_w_in + carry_1_in;
	
	assign carry_w_in = (ArithOp) ? mul_acc_carry_w : 0;
	assign carry_1_in = (ArithOp) ? mul_acc_carry_1 : 1'b0;

	assign mul_acc_out = final_add_res[CP_D_WIDTH-1:0];
	assign mul_acc_carry = mul_acc_carry_w + mul_acc_carry_1;

	// Instantiation ///////////////////////////////////////////////////////////
	multiply_accumulator_core
	#(
		.CP_D_WIDTH(CP_D_WIDTH)
	 )
	multiply_accumulator_core_u // Pipelined with latency = 6
	(
		.clk(clock),
		.ce(MulAccEn),
		.sclr(!nreset),
		.a(IN_REG0),
		.b(IN_REG1),
		.c(IN_REG2),
		.p(mul_acc_core_out)
	);
	
	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				mul_acc_carry_w <= 0;
				mul_acc_carry_1 <= 0;
			end
		else
			begin
				if (MulAccEn == 1'b1)
					begin
						if (ArithRegOp == 1'b1)
							begin
								mul_acc_carry_w <= mul_acc_carry_w;
								mul_acc_carry_1 <= mul_acc_carry_1;
							end
						else
							begin
								mul_acc_carry_w <= mul_acc_core_out[2*CP_D_WIDTH-1:CP_D_WIDTH];
								mul_acc_carry_1 <= final_add_res[CP_D_WIDTH];
							end
					end
				else
					begin
						mul_acc_carry_w <= mul_acc_carry_w;
						mul_acc_carry_1 <= mul_acc_carry_1;
					end
			end
	end

	////////////////////////////////////////////////////////////////////////////
endmodule
