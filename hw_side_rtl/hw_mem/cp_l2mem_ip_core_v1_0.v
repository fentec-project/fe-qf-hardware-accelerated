`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
module cp_l2mem_ip_core
    #(
		parameter CP_D_WIDTH = 72, 		// CP Datapath width
		parameter DMEM_ADDR_WIDTH = 10	// (2 ^ DMEM_ADDR_WIDTH) * CP_D_WIDTH = DMEM Size
	  )
	(
	nreset,
	clock,

	trq_0,
	trq_1,
	trq_2,
	trq_3,
	trq_4,
	trq_5,
	trq_6,
	trq_7,
	trq_8,
	trq_9,
	trq_10,
	trq_11,
	
    tack_0,
    tack_1,
    tack_2,
    tack_3,
    tack_4,
    tack_5,
    tack_6,
    tack_7,
    tack_8,
    tack_9,
    tack_10,
    tack_11,

	addr_0,
	in_0,
	we_0,

	addr_1,
	in_1,
	we_1,

	addr_2,
	in_2,
	we_2,

	addr_3,
	in_3,
	we_3,

	addr_4,
	in_4,
	we_4,

	addr_5,
	in_5,
	we_5,

	addr_6,
	in_6,
	we_6,

	addr_7,
	in_7,
	we_7,

	addr_8,
	in_8,
	we_8,

	addr_9,
	in_9,
	we_9,

	addr_10,
	in_10,
	we_10,

	addr_11,
	in_11,
	we_11,

	dmem_l2_out
    );

	// Ports //////////////////////////////////////////////////////////
	input								nreset;
	input 								clock;

	input								trq_0;
	input								trq_1;
	input								trq_2;
	input								trq_3;
	input								trq_4;
	input								trq_5;
	input								trq_6;
	input								trq_7;
	input								trq_8;
	input								trq_9;
	input								trq_10;
	input								trq_11;

    output                              tack_0;
    output                              tack_1;
    output                              tack_2;
    output                              tack_3;
    output                              tack_4;
    output                              tack_5;
    output                              tack_6;
    output                              tack_7;
    output                              tack_8;
    output                              tack_9;
    output                              tack_10;
    output                              tack_11;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_0;
	input	[CP_D_WIDTH-1:0]			in_0;
	input								we_0;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_1;
	input	[CP_D_WIDTH-1:0]			in_1;
	input								we_1;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_2;
	input	[CP_D_WIDTH-1:0]			in_2;
	input								we_2;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_3;
	input	[CP_D_WIDTH-1:0]			in_3;
	input								we_3;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_4;
	input	[CP_D_WIDTH-1:0]			in_4;
	input								we_4;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_5;
	input	[CP_D_WIDTH-1:0]			in_5;
	input								we_5;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_6;
	input	[CP_D_WIDTH-1:0]			in_6;
	input								we_6;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_7;
	input	[CP_D_WIDTH-1:0]			in_7;
	input								we_7;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_8;
	input	[CP_D_WIDTH-1:0]			in_8;
	input								we_8;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_9;
	input	[CP_D_WIDTH-1:0]			in_9;
	input								we_9;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_10;
	input	[CP_D_WIDTH-1:0]			in_10;
	input								we_10;

	input	[DMEM_ADDR_WIDTH-1:0]		addr_11;
	input	[CP_D_WIDTH-1:0]			in_11;
	input								we_11;

	output	[CP_D_WIDTH-1:0]			dmem_l2_out;

	// Local parameters /////////////////////////////////////////////
	localparam	IDLE		= 4'b0000;
	localparam	TRQ_CP_0	= 4'b0001;
	localparam	TRQ_CP_1	= 4'b0010;
	localparam	TRQ_CP_2	= 4'b0011;
	localparam	TRQ_CP_3	= 4'b0100;
	localparam	TRQ_CP_4	= 4'b0101;
	localparam	TRQ_CP_5	= 4'b0110;
	localparam	TRQ_CP_6	= 4'b0111;
	localparam	TRQ_CP_7	= 4'b1000;
	localparam	TRQ_CP_8	= 4'b1001;
	localparam	TRQ_CP_9	= 4'b1010;
	localparam	TRQ_CP_10	= 4'b1011;
	localparam	TRQ_CP_11	= 4'b1100;

	// Wires ///////////////////////////////////////////////////////////
	wire	[DMEM_ADDR_WIDTH-1:0]	dmem_addr;
	wire	[CP_D_WIDTH-1:0]		dmem_in;
	wire							dmem_we;

	// Registers ///////////////////////////////////////////////////////
	reg		[3:0]					state;
	reg		[3:0]					next_state;

	// Assignments /////////////////////////////////////////////////////
	assign dmem_addr =	(state == TRQ_CP_0) ? addr_0 :
						(state == TRQ_CP_1) ? addr_1 :
						(state == TRQ_CP_2) ? addr_2 :
						(state == TRQ_CP_3) ? addr_3 :
						(state == TRQ_CP_4) ? addr_4 :
						(state == TRQ_CP_5) ? addr_5 :
						(state == TRQ_CP_6) ? addr_6 :
						(state == TRQ_CP_7) ? addr_7 :
						(state == TRQ_CP_8) ? addr_8 :
						(state == TRQ_CP_9) ? addr_9 :
						(state == TRQ_CP_10) ? addr_10 :
						(state == TRQ_CP_11) ? addr_11 :
						addr_0;

	assign dmem_in =	(state == TRQ_CP_0) ? in_0 :
						(state == TRQ_CP_1) ? in_1 :
						(state == TRQ_CP_2) ? in_2 :
						(state == TRQ_CP_3) ? in_3 :
						(state == TRQ_CP_4) ? in_4 :
						(state == TRQ_CP_5) ? in_5 :
						(state == TRQ_CP_6) ? in_6 :
						(state == TRQ_CP_7) ? in_7 :
						(state == TRQ_CP_8) ? in_8 :
						(state == TRQ_CP_9) ? in_9 :
						(state == TRQ_CP_10) ? in_10 :
						(state == TRQ_CP_11) ? in_11 :
						in_0;

	assign dmem_we =	(state == TRQ_CP_0) ? we_0 :
						(state == TRQ_CP_1) ? we_1 :
						(state == TRQ_CP_2) ? we_2 :
						(state == TRQ_CP_3) ? we_3 :
						(state == TRQ_CP_4) ? we_4 :
						(state == TRQ_CP_5) ? we_5 :
						(state == TRQ_CP_6) ? we_6 :
						(state == TRQ_CP_7) ? we_7 :
						(state == TRQ_CP_8) ? we_8 :
						(state == TRQ_CP_9) ? we_9 :
						(state == TRQ_CP_10) ? we_10 :
						(state == TRQ_CP_11) ? we_11 :
						we_0;

    assign tack_0 = ((state == TRQ_CP_0) && (trq_0 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_1 = ((state == TRQ_CP_1) && (trq_1 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_2 = ((state == TRQ_CP_2) && (trq_2 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_3 = ((state == TRQ_CP_3) && (trq_3 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_4 = ((state == TRQ_CP_4) && (trq_4 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_5 = ((state == TRQ_CP_5) && (trq_5 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_6 = ((state == TRQ_CP_6) && (trq_6 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_7 = ((state == TRQ_CP_7) && (trq_7 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_8 = ((state == TRQ_CP_8) && (trq_8 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_9 = ((state == TRQ_CP_9) && (trq_9 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_10 = ((state == TRQ_CP_10) && (trq_10 == 1'b1)) ? 1'b1 : 1'b0;
    assign tack_11 = ((state == TRQ_CP_11) && (trq_11 == 1'b1)) ? 1'b1 : 1'b0;

	/////////////////////////////////////////////////////////////////
	// Sequential procedure /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge clock)
	begin
		if (!nreset)
			begin
				state <= IDLE;
			end
		else
			begin
				state <= next_state;
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(*)
	begin
		next_state <= IDLE;

		case(state)
			//-----------------------------------------------------------
			IDLE:
				begin
					if (trq_0 == 1'b1)
						begin
							next_state <= TRQ_CP_0;
						end
					else if (trq_1 == 1'b1)
						begin
							next_state <= TRQ_CP_1;
						end
					else if (trq_2 == 1'b1)
						begin
							next_state <= TRQ_CP_2;
						end
					else if (trq_3 == 1'b1)
						begin
							next_state <= TRQ_CP_3;
						end
					else if (trq_4 == 1'b1)
						begin
							next_state <= TRQ_CP_4;
						end
					else if (trq_5 == 1'b1)
						begin
							next_state <= TRQ_CP_5;
						end
					else if (trq_6 == 1'b1)
						begin
							next_state <= TRQ_CP_6;
						end
					else if (trq_7 == 1'b1)
						begin
							next_state <= TRQ_CP_7;
						end
					else if (trq_8 == 1'b1)
						begin
							next_state <= TRQ_CP_8;
						end
					else if (trq_9 == 1'b1)
						begin
							next_state <= TRQ_CP_9;
						end
					else if (trq_10 == 1'b1)
						begin
							next_state <= TRQ_CP_10;
						end
					else if (trq_11 == 1'b1)
						begin
							next_state <= TRQ_CP_11;
						end
					else
						begin
							next_state <= IDLE;
						end
				end
			
			//-----------------------------------------------------------
			TRQ_CP_0:
				begin
					if (trq_0 == 1'b0)
						begin
							next_state <= TRQ_CP_1;
						end
					else
						begin
							next_state <= TRQ_CP_0;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_1:
				begin
					if (trq_1 == 1'b0)
						begin
							next_state <= TRQ_CP_2;
						end
					else
						begin
							next_state <= TRQ_CP_1;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_2:
				begin
					if (trq_2 == 1'b0)
						begin
							next_state <= TRQ_CP_3;
						end
					else
						begin
							next_state <= TRQ_CP_2;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_3:
				begin
					if (trq_3 == 1'b0)
						begin
							next_state <= TRQ_CP_4;
						end
					else
						begin
							next_state <= TRQ_CP_3;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_4:
				begin
					if (trq_4 == 1'b0)
						begin
							next_state <= TRQ_CP_5;
						end
					else
						begin
							next_state <= TRQ_CP_4;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_5:
				begin
					if (trq_5 == 1'b0)
						begin
							next_state <= TRQ_CP_6;
						end
					else
						begin
							next_state <= TRQ_CP_5;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_6:
				begin
					if (trq_6 == 1'b0)
						begin
							next_state <= TRQ_CP_7;
						end
					else
						begin
							next_state <= TRQ_CP_6;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_7:
				begin
					if (trq_7 == 1'b0)
						begin
							next_state <= TRQ_CP_8;
						end
					else
						begin
							next_state <= TRQ_CP_7;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_8:
				begin
					if (trq_8 == 1'b0)
						begin
							next_state <= TRQ_CP_9;
						end
					else
						begin
							next_state <= TRQ_CP_8;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_9:
				begin
					if (trq_9 == 1'b0)
						begin
							next_state <= TRQ_CP_10;
						end
					else
						begin
							next_state <= TRQ_CP_9;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_10:
				begin
					if (trq_10 == 1'b0)
						begin
							next_state <= TRQ_CP_11;
						end
					else
						begin
							next_state <= TRQ_CP_10;
						end
				end

			//-----------------------------------------------------------
			TRQ_CP_11:
				begin
					if (trq_11 == 1'b0)
						begin
							next_state <= IDLE;
						end
					else
						begin
							next_state <= TRQ_CP_11;
						end
				end

			//-----------------------------------------------------------
			default:
				begin
					next_state <= IDLE;
				end
		endcase
	end

	// Instantiations //////////////////////////////////////////////////
	sp_ram_block
//	#(
//        .DATA_WIDTH(CP_D_WIDTH),
//        .ADDR_WIDTH(DMEM_ADDR_WIDTH)
//     )
	sp_ram_block_u1		// Simple Port (SP) RAM
	(
		// Read and Write Port
		.rsta(!nreset),
		.clka(clock),
		.addra(dmem_addr),
		.dina(dmem_in),
		.wea(dmem_we),
		.douta(dmem_l2_out)
	);

/////////////////////////////////////////////////////////////////////////////////////////////
endmodule
