`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module tdp_bram
	(
	rsta,
	clka,
	addra,
	dina,
	wea,
	douta,

	rstb,
	clkb,
	addrb,
	dinb,
	web,
	doutb
	);
	
	// Ports //////////////////////////////////////////////////////////////
	input			rsta;
	input 			clka;
	input	[9:0]	addra;
	input	[255:0]	dina;
	input			wea;
	output	[255:0]	douta;

	input			rstb;
	input 			clkb;
	input	[9:0]	addrb;
	input	[255:0]	dinb;
	input			web;
	output	[255:0]	doutb;

	// Local parameter ////////////////////////////////////////////////////
	localparam ADDR_DEPTH = 1 << 10;

	// RAM definition /////////////////////////////////////////////////////
	reg	[255:0]	ram [ADDR_DEPTH-1:0];
	
	reg	[9: 0]	addr_reg_a;
	reg	[9: 0]	addr_reg_b;

	// Assignment /////////////////////////////////////////////////////////
	assign douta = ram[addr_reg_a];
	assign doutb = ram[addr_reg_b];

	// Sequential procedures //////////////////////////////////////////////
	always @(posedge clka)
	begin
		if (wea == 1'b1)
			begin
				ram[addra]  <= dina;
			end

		addr_reg_a <= addra;
	end

	///////////////////////////////////////////
	always @(posedge clkb)
	begin 
		if (web == 1'b1)
			begin
				ram[addrb]  <= dinb;
			end

		addr_reg_b <= addrb;
	end

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
