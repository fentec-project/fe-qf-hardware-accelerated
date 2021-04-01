`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
  module masb
	(
	nrst,
	clk,
	as_op,
	m,
	x,
	y,
	z
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input			nrst;
	input			clk;

	input			as_op;

	input	[255:0]	m;

	input	[255:0]	x;
	input	[255:0]	y;

	output	[255:0] z;

	// Internal signal and register ////////////////////////////////////////////
	wire	[256:0]	w0;
	wire			c0;
	wire	[255:0] w1;
	wire			c1;

	wire	[256:0]	w2;
	wire			c2;
	wire	[255:0]	w3;
	wire			c3;
	wire	[255:0]	w4;

	reg				r_as_op;
	reg		[255:0] rw1;
	reg				rc1;
	reg		[255:0] rw4;

    // Combinational assignment ////////////////////////////////////////////////
	assign w0 = (as_op == 1'b1) ? {1'b1, ~y} : {1'b0, y};
	assign c0 = (as_op == 1'b1) ? 1'b1 : 1'b0;
	assign {c1,w1} = {1'b0, x} + w0 + c0;
	
	assign w2 = (r_as_op == 1'b1) ? {1'b0, m} : {1'b1, ~m};
	assign c2 = (r_as_op == 1'b1) ? 1'b0 : 1'b1;
	assign {c3,w3} = {rc1, rw1} + w2 + c2;

	assign w4 = (((r_as_op == 1'b1) && (rc1 == 1'b1)) || ((r_as_op == 1'b0) && (c3 == 1'b0))) ? w3 : rw1;
	assign z = rw4;

	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				r_as_op <= 1'b0;
				rw1 <= 256'b0;
				rc1 <= 1'b0;
				rw4 <= 256'b0;
			end
		else
			begin
				r_as_op <= as_op;
				rw1 <= w1;
				rc1 <= c1;
				rw4 <= w4;
			end
	end

	////////////////////////////////////////////////////////////////////////////
endmodule
