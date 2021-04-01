`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module mult_add_18_type3
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
	
	reg		[Mult_Add_WIDTH-1:0] a_1, b_1, a_2, b_2, a_3, b_3;
	
	///////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				a_1 <= 0;
				b_1 <= 0;
				
				a_2 <= 0;
				b_2 <= 0;
				
				a_3 <= 0;
				b_3 <= 0;
				
				p <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						a_1 <= a;
						b_1 <= b;
						
						a_2 <= a_1;
						b_2 <= b_1;
						
						a_3 <= a_2;
						b_3 <= b_2;
						
						p <= a_3 * b_3 + c;
					end
				else
					begin
						a_1 <= a_1;
						b_1 <= b_1;
						
						a_2 <= a_2;
						b_2 <= b_2;
						
						a_3 <= a_3;
						b_3 <= b_3;
						
						p <= p;
					end
			end
	end

	///////////////////////////////////////////
endmodule
