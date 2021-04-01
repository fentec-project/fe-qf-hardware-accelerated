`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module sp_ram_block
	(
	rsta,
	clka,
	addra,
	dina,
	wea,
	douta
	);
	
	// Ports //////////////////////////////////////////////////////////////
	input 			rsta;
	input 			clka;
	input	[9:0]	addra;
	input	[71:0]	dina;
	input			wea;
	output	[71:0]	douta;

	// Local parameter ////////////////////////////////////////////////////
	localparam ADDR_DEPTH = 1 << 10;

	// RAM definition /////////////////////////////////////////////////////
	reg	[71:0] ram [ADDR_DEPTH-1:0];

	reg	[9:0] addr_reg_a;

	// Assignment /////////////////////////////////////////////////////////
	assign douta = ram[addr_reg_a];

	// Sequential procedure ///////////////////////////////////////////////
	always @(posedge clka)
	begin
		if (wea == 1'b1)
			begin
				ram[addra]  <= dina;
			end
	end

	///////////////////////////////////////////
	always @(posedge clka)
	begin 
		addr_reg_a <= addra;
	end

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
