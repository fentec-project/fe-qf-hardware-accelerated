`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module sdp_ram_block
	(
	clka,
	addra,
	dina,
	wea,

	rstb,
	clkb,
	addrb,
	doutb
	);
	
	// Ports //////////////////////////////////////////////////////////////
	input 			clka;
	input	[8:0]	addra;
	input	[55:0]	dina;
	input			wea;

	input			rstb;
	input 			clkb;
	input	[8:0]	addrb;
	output	[55:0]	doutb;

	// Local parameter ////////////////////////////////////////////////////
	localparam ADDR_DEPTH = 1 << 9;

	// RAM definition /////////////////////////////////////////////////////
	reg	[55:0] ram [ADDR_DEPTH-1:0];

	reg	[8:0] addr_reg_b;

	// Assignment /////////////////////////////////////////////////////////
	assign doutb = ram[addr_reg_b];

	// Sequential procedure ///////////////////////////////////////////////
	always @(posedge clka)
	begin
		if (wea == 1'b1)
			begin
				ram[addra]  <= dina;
			end
	end

	///////////////////////////////////////////
	always @(posedge clkb)
	begin 
		addr_reg_b <= addrb;
	end

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
