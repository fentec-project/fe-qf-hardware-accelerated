`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module mult_add_18_type0
	(
	clk,
	ce,
	sclr,
	a,
	b,
	c,
	p
    );
	
	///////////////////////////////////////////
	localparam Mult_Add_WIDTH = 18;
	
	///////////////////////////////////////////
	input clk;
	input ce;
	input sclr;
	
	input 	[Mult_Add_WIDTH-1:0] a;
	input 	[Mult_Add_WIDTH-1:0] b;
	input 	[Mult_Add_WIDTH-1:0] c;
	
	output 	[2*Mult_Add_WIDTH-1:0] p;
	
	///////////////////////////////////////////
	reg 	[2*Mult_Add_WIDTH-1:0] p;
	
	///////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				p <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						p <= a * b + c;
					end
				else
					begin
						p <= p;
					end
			end
	end

	///////////////////////////////////////////
endmodule
