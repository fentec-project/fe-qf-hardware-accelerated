`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module maaab
	(
	clk,
	op,
	a,
	b,
	c,
	d,
	pl,
	ph
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input			clk;

	input			op;

	input	[63:0]	a;
	input	[63:0]	b;
	input	[63:0]	c;
	input	[63:0]	d;

	output	[63:0]	pl;
	output	[63:0]	ph;

	// Internal signal and register ////////////////////////////////////////////
	wire	[127:0]	maaab_core_out;
	wire	[63:0]	carry_w_in;	
	wire			carry_1_in;
	wire	[64:0]	final_add_res;	

	reg		[63:0]	maaab_carry_w;
	reg				maaab_carry_1;

    // Combinational assignment ////////////////////////////////////////////////
    assign final_add_res = maaab_core_out[63:0] + carry_w_in + carry_1_in;
	
	assign carry_w_in = (op == 1'b1) ? maaab_carry_w : 64'b0;
	assign carry_1_in = (op == 1'b1) ? maaab_carry_1 : 1'b0;

	assign pl = final_add_res[63:0];
	assign ph = maaab_carry_w + maaab_carry_1;

	// Instantiation ///////////////////////////////////////////////////////////
	maab maab_u
	(
		.clk(clk),
		.a(a),
		.b(b),
		.c(c),
		.d(d),
		.p(maaab_core_out)
	);
	
	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clk)
	begin
		maaab_carry_w <= maaab_core_out[127:64];
		maaab_carry_1 <= final_add_res[64];
	end

	////////////////////////////////////////////////////////////////////////////
endmodule
