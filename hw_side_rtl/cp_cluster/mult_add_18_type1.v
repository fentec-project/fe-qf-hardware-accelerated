`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module mult_add_18_type1
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
	
	reg		[Mult_Add_WIDTH-1:0] a_1, b_1;
	
	///////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				a_1 <= 0;
				b_1 <= 0;
				
				p <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						a_1 <= a;
						b_1 <= b;
						
						p <= a_1 * b_1 + c;
					end
				else
					begin
						a_1 <= a_1;
						b_1 <= b_1;
						
						p <= p;
					end
			end
	end

	///////////////////////////////////////////
endmodule
