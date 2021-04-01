`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module sdp_bram_lpm
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
	input	[9:0]	addra;
	input	[74:0]	dina;
	input			wea;

	input			rstb;
	input 			clkb;
	input	[9:0]	addrb;
	output	[74:0]	doutb;

	// Local parameter ////////////////////////////////////////////////////
	localparam ADDR_DEPTH = 1 << 10;

	// RAM definition /////////////////////////////////////////////////////
	reg	[74:0] ram [ADDR_DEPTH-1:0];

	reg	[9:0] addr_reg_b;

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
