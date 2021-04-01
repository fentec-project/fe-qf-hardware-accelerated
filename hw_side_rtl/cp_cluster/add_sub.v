`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module add_sub
    #(
		parameter CP_D_WIDTH = 72 // CP Datapath width
	 )
	(
	nreset,
	clock,
	ArithOp,
	ArithRegOp,
	IN_REG0,
	IN_REG1,
	add_sub_out,
	add_sub_carry
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input							nreset;
	input							clock;

	input	[1:0]					ArithOp;
	input							ArithRegOp;

	input	[CP_D_WIDTH-1:0]		IN_REG0;
	input	[CP_D_WIDTH-1:0]		IN_REG1;

	output	[CP_D_WIDTH-1:0]		add_sub_out;
	output							add_sub_carry;

	// Internal Signals ////////////////////////////////////////////////////////
	wire	[CP_D_WIDTH:0]			add_sub_result;
	wire							carry_in;

	reg								add_sub_carry;

	// Combinational Assignments ///////////////////////////////////////////////
	assign add_sub_result = (ArithOp[0]) ? (IN_REG0 + IN_REG1 + carry_in) :
										   (IN_REG0 - IN_REG1 - carry_in);
	
	assign carry_in = (ArithOp[1]) ? add_sub_carry : 1'b0;
	
	assign add_sub_out = add_sub_result[CP_D_WIDTH-1:0];

	// Sequential Procedures ///////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				add_sub_carry <= 1'b0;
			end
		else
			begin
				if (ArithRegOp == 1'b1)
					begin
						add_sub_carry <= 1'b1;
					end
				else
					begin
						add_sub_carry <= add_sub_result[CP_D_WIDTH];
					end
			end
	end

	////////////////////////////////////////////////////////////////////////////
endmodule
