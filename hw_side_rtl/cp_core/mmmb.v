`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module mmmb
	(
	nrst,
	clk,
	start,
	mp,
	m,
	x,
	y,
	z,
	done_1,
	done_2
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input			nrst;
	input			clk;
	input			start;

	input	[63:0]	mp;
	input	[255:0]	m;

	input	[255:0]	x;
	input	[255:0]	y;

	output	[255:0] z;
	output	reg		done_1;
	output	reg		done_2;

	// Internal signal and register ////////////////////////////////////////////
	wire			op;
	wire			op_1;
	wire			op_2;

	wire	[63:0]	a;
	wire	[63:0]	b;
	wire	[63:0]	c;
	wire	[63:0]	d;
	wire	[63:0]	pl;
	wire	[63:0]	ph;

	wire	[63:0]	a0;
	wire	[63:0]	a1;
	wire	[63:0]	a2;
	wire	[63:0]	a3;

	wire	[63:0]	b0;
	wire	[63:0]	b1;
	wire	[63:0]	b2;
	wire	[63:0]	b3;

	wire	[63:0]	m0;
	wire	[63:0]	m1;
	wire	[63:0]	m2;
	wire	[63:0]	m3;

	reg		[5:0]	counter_s;
	reg		[2:0]	counter_f;

	reg		[63:0]	ai;
	reg		[63:0]	bi;
	reg		[63:0]	ci;
	reg		[63:0]	di;

	reg		[63:0]	x0;
	reg		[63:0]	x1;
	reg		[63:0]	x2;
	reg		[63:0]	x3;
	reg		[63:0]	x4;

	reg		[63:0]	y0;
	reg		[63:0]	y1;
	reg		[63:0]	y2;
	reg		[63:0]	y3;
	reg		[63:0]	y4;

	reg		[63:0]	si1;
	reg		[63:0]	si2;
	reg		[63:0]	si3;
	reg		[63:0]	si4;

	reg		[63:0]	sj1;
	reg		[63:0]	sj2;
	reg		[63:0]	sj3;
	reg		[63:0]	sj4;

	reg		[63:0]	sii1;
	reg		[63:0]	sii2;
	reg		[63:0]	sii3;
	reg		[63:0]	sii4;

	reg		[63:0]	sjj1;
	reg		[63:0]	sjj2;
	reg		[63:0]	sjj3;
	reg		[63:0]	sjj4;

	reg		[63:0]	q;
	reg				br;

	// Instantiation ///////////////////////////////////////////////////////////
	maaab maaab_u0
	(
    .clk(clk), 
    .op(op), 
    .a(ai), 
    .b(bi), 
    .c(ci), 
    .d(di), 
    .pl(pl), 
    .ph(ph)
    );

    // Combinational assignment ////////////////////////////////////////////////
    assign a0 = x[63:0];
	assign a1 = x[127:64];
	assign a2 = x[191:128];
	assign a3 = x[255:192];

	assign b0 = y[63:0];
	assign b1 = y[127:64];
	assign b2 = y[191:128];
	assign b3 = y[255:192];

	assign m0 = m[63:0];
	assign m1 = m[127:64];
	assign m2 = m[191:128];
	assign m3 = m[255:192];

	assign a =
	(counter_s == 6'd0) ? b0 : (counter_s == 6'd1) ? b0 : (counter_s == 6'd2) ? b0 : (counter_s == 6'd3) ? b0 :
	(counter_s == 6'd4) ? pl : (counter_s == 6'd5) ? b1 : (counter_s == 6'd6) ? b1 : (counter_s == 6'd7) ? b1 :
	(counter_s == 6'd8) ? pl : (counter_s == 6'd9) ? q : (counter_s == 6'd10) ? q : (counter_s == 6'd11) ? q :
	(counter_s == 6'd12) ? b1 : (counter_s == 6'd13) ? (pl+y0) : (counter_s == 6'd14) ? b2 : (counter_s == 6'd15) ? b2 :
	(counter_s == 6'd16) ? b2 : (counter_s == 6'd17) ? pl : (counter_s == 6'd18) ? q : (counter_s == 6'd19) ? q :
	(counter_s == 6'd20) ? q : (counter_s == 6'd21) ? b2 : (counter_s == 6'd22) ? (pl+x0) : (counter_s == 6'd23) ? b3 :
	(counter_s == 6'd24) ? b3 : (counter_s == 6'd25) ? b3 : (counter_s == 6'd26) ? pl : (counter_s == 6'd27) ? q :
	(counter_s == 6'd28) ? q : (counter_s == 6'd29) ? q : (counter_s == 6'd30) ? b3 : (counter_s == 6'd31) ? (pl+y0) :
	(counter_s == 6'd35) ? pl : (counter_s == 6'd36) ? q : (counter_s == 6'd37) ? q : (counter_s == 6'd38) ? q : 64'bX;
	
	assign b =
	(counter_s == 6'd0) ? a0 : (counter_s == 6'd1) ? a1 : (counter_s == 6'd2) ? a2 : (counter_s == 6'd3) ? a3 :
	(counter_s == 6'd4) ? mp : (counter_s == 6'd5) ? a0 : (counter_s == 6'd6) ? a1 : (counter_s == 6'd7) ? a2 :
	(counter_s == 6'd8) ? m0 : (counter_s == 6'd9) ? m1 : (counter_s == 6'd10) ? m2 : (counter_s == 6'd11) ? m3 :
	(counter_s == 6'd12) ? a3 : (counter_s == 6'd13) ? mp : (counter_s == 6'd14) ? a0 : (counter_s == 6'd15) ? a1 :
	(counter_s == 6'd16) ? a2 : (counter_s == 6'd17) ? m0 : (counter_s == 6'd18) ? m1 : (counter_s == 6'd19) ? m2 :
	(counter_s == 6'd20) ? m3 : (counter_s == 6'd21) ? a3 : (counter_s == 6'd22) ? mp : (counter_s == 6'd23) ? a0 :
	(counter_s == 6'd24) ? a1 : (counter_s == 6'd25) ? a2 : (counter_s == 6'd26) ? m0 : (counter_s == 6'd27) ? m1 :
	(counter_s == 6'd28) ? m2 : (counter_s == 6'd29) ? m3 : (counter_s == 6'd30) ? a3 : (counter_s == 6'd31) ? mp :
	(counter_s == 6'd35) ? m0 : (counter_s == 6'd36) ? m1 : (counter_s == 6'd37) ? m2 : (counter_s == 6'd38) ? m3 : 64'bX;

	assign c =
	(counter_s == 6'd0) ? 64'b0 : (counter_s == 6'd1) ? 64'b0 : (counter_s == 6'd2) ? 64'b0 : (counter_s == 6'd3) ? 64'b0 :
	(counter_s == 6'd4) ? 64'b0 : (counter_s == 6'd5) ? 64'b0 : (counter_s == 6'd6) ? 64'b0 : (counter_s == 6'd7) ? 64'b0 :
	(counter_s == 6'd8) ? x0 : (counter_s == 6'd9) ? x1 : (counter_s == 6'd10) ? x2 : (counter_s == 6'd11) ? x3 :
	(counter_s == 6'd12) ? ph : (counter_s == 6'd13) ? 64'b0 : (counter_s == 6'd14) ? 64'b0 : (counter_s == 6'd15) ? 64'b0 :
	(counter_s == 6'd16) ? 64'b0 : (counter_s == 6'd17) ? y0 : (counter_s == 6'd18) ? y1 : (counter_s == 6'd19) ? y2 :
	(counter_s == 6'd20) ? y3 : (counter_s == 6'd21) ? ph : (counter_s == 6'd22) ? 64'b0 : (counter_s == 6'd23) ? 64'b0 :
	(counter_s == 6'd24) ? 64'b0 : (counter_s == 6'd25) ? 64'b0 : (counter_s == 6'd26) ? x0 : (counter_s == 6'd27) ? x1 :
	(counter_s == 6'd28) ? x2 : (counter_s == 6'd29) ? x3 : (counter_s == 6'd30) ? ph : (counter_s == 6'd31) ? 64'b0 :
	(counter_s == 6'd35) ? y0 : (counter_s == 6'd36) ? y1 : (counter_s == 6'd37) ? y2 : (counter_s == 6'd38) ? y3 : 64'bX;

	assign d =
	(counter_s == 6'd0) ? 64'b0 : (counter_s == 6'd1) ? 64'b0 : (counter_s == 6'd2) ? 64'b0 : (counter_s == 6'd3) ? 64'b0 :
	(counter_s == 6'd4) ? 64'b0 : (counter_s == 6'd5) ? 64'b0 : (counter_s == 6'd6) ? 64'b0 : (counter_s == 6'd7) ? 64'b0 :
	(counter_s == 6'd8) ? 64'b0 : (counter_s == 6'd9) ? 64'b0 : (counter_s == 6'd10) ? 64'b0 : (counter_s == 6'd11) ? 64'b0 :
	(counter_s == 6'd12) ? 64'b0 : (counter_s == 6'd13) ? 64'b0 : (counter_s == 6'd14) ? 64'b0 : (counter_s == 6'd15) ? 64'b0 :
	(counter_s == 6'd16) ? 64'b0 : (counter_s == 6'd17) ? si1 : (counter_s == 6'd18) ? si2 : (counter_s == 6'd19) ? si3 :
	(counter_s == 6'd20) ? si4 : (counter_s == 6'd21) ? 64'b0 : (counter_s == 6'd22) ? 64'b0 : (counter_s == 6'd23) ? 64'b0 :
	(counter_s == 6'd24) ? 64'b0 : (counter_s == 6'd25) ? 64'b0 : (counter_s == 6'd26) ? sj1 : (counter_s == 6'd27) ? sj2 :
	(counter_s == 6'd28) ? sj3 : (counter_s == 6'd29) ? sj4 : (counter_s == 6'd30) ? 64'b0 : (counter_s == 6'd31) ? 64'b0 :
	(counter_s == 6'd35) ? si1 : (counter_s == 6'd36) ? si2 : (counter_s == 6'd37) ? si3 : (counter_s == 6'd38) ? si4 : 64'bX;

	assign op_1 = ((counter_s == 6'd4) || (counter_s == 6'd8) || (counter_s == 6'd9) || (counter_s == 6'd12) ||
				  (counter_s == 6'd16) || (counter_s == 6'd17) || (counter_s == 6'd18) || (counter_s == 6'd21) ||
				  (counter_s == 6'd25) || (counter_s == 6'd26) || (counter_s == 6'd27) || (counter_s == 6'd30) ||
				  (counter_s == 6'd34) || (counter_s == 6'd35)) ? 1'b0 : 1'b1;
	assign op_2 = (counter_f == 3'd1) ? 1'b0 : 1'b1;
	assign op = op_1 & op_2;

	assign z = (br == 1'b1)? {sii4, sii3, sii2, sii1} : {sjj4, sjj3, sjj2, sjj1};

	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				counter_s <= 6'b0;
				counter_f <= 3'b0;
			end
		else
			begin
				///////////////////////////////////////////
				if (counter_s == 6'd38)
					begin
						counter_s <= 6'b0;
					end
				else if ((counter_s != 0) || (start == 1'b1))
					begin
						counter_s <= counter_s + 6'b1;
					end
				else
					begin
						counter_s <= counter_s;
					end

				///////////////////////////////////////////
				if (counter_f == 3'd5)
					begin
						counter_f <= 3'b0;
					end
				else if ((counter_f != 0) || (counter_s == 6'd38))
					begin
						counter_f <= counter_f + 3'b1;
					end
				else
					begin
						counter_f <= counter_f;
					end
			end	
	end
	
	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				done_1 <= 1'b0;
				done_2 <= 1'b0;
			end
		else
			begin
				///////////////////////////////////////////
				if (counter_s == 6'd36)
					begin
						done_1 <= 1'b1;
					end
				else
					begin
						done_1 <= 1'b0;
					end

				///////////////////////////////////////////
				if (counter_f == 3'd4)
					begin
						done_2 <= 1'b1;
					end
				else
					begin
						done_2 <= 1'b0;
					end
			end
	end

	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		ai <= a;
		bi <= b;
		ci <= c;
		di <= d;
	end

	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				x0 <= 64'b0;
				x1 <= 64'b0;
				x2 <= 64'b0;
				x3 <= 64'b0;
				x4 <= 64'b0;
				
				y0 <= 64'b0;
				y1 <= 64'b0;
				y2 <= 64'b0;
				y3 <= 64'b0;
				y4 <= 64'b0;
				
				si1 <= 64'b0;
				si2 <= 64'b0;
				si3 <= 64'b0;
				si4 <= 64'b0;
				
				sj1 <= 64'b0;
				sj2 <= 64'b0;
				sj3 <= 64'b0;
				sj4 <= 64'b0;

				sii1 <= 64'b0;
				sii2 <= 64'b0;
				sii3 <= 64'b0;
				sii4 <= 64'b0;

				sjj1 <= 64'b0;
				sjj2 <= 64'b0;
				sjj3 <= 64'b0;
				sjj4 <= 64'b0;
				
				q <= 64'b0;
				br <= 1'b0;
			end
		else
			begin
				/////////////////////////////////////////
				case (counter_s)
					6'd4:
						begin
							x0 <= pl;
						end
					
					6'd5:
						begin
							x1 <= pl;
						end
					
					6'd6:
						begin
							x2 <= pl;
						end
					
					6'd7:
						begin
							x3 <= pl;
						end
					
					6'd8:
						begin
							x4 <= ph;
							q <= pl;
						end
					
					6'd9:
						begin
							y0 <= pl;
						end
					
					6'd10:
						begin
							y1 <= pl;
						end
					
					6'd11:
						begin
							y2 <= pl;
						end
					
					6'd13:
						begin
							si1 <= pl;
						end
					
					6'd14:
						begin
							si2 <= pl;
						end
					
					6'd15:
						begin
							si3 <= pl;
						end
					
					6'd16:
						begin
							y3 <= pl;
							si4 <= ph + x4;
						end
					
					6'd17:
						begin
							q <= pl;
							y4 <= ph;
						end
					
					6'd18:
						begin
							x0 <= pl;
						end
					
					6'd19:
						begin
							x1 <= pl;
						end
					
					6'd20:
						begin
							x2 <= pl;
						end
					
					6'd22:
						begin
							sj1 <= pl;
						end
					
					6'd23:
						begin
							sj2 <= pl;
						end
					
					6'd24:
						begin
							sj3 <= pl;
						end
					
					6'd25:
						begin
							x3 <= pl;
							sj4 <= ph + y4;
						end
					
					6'd26:
						begin
							q <= pl;
							x4 <= ph;
						end
					
					6'd27:
						begin
							y0 <= pl;
						end
					
					6'd28:
						begin
							y1 <= pl;
						end
					
					6'd29:
						begin
							y2 <= pl;
						end
					
					6'd31:
						begin
							si1 <= pl;
						end
					
					6'd32:
						begin
							si2 <= pl;
						end
					
					6'd33:
						begin
							si3 <= pl;
						end
					
					6'd34:
						begin
							y3 <= pl;
							si4 <= ph + x4;
						end
					
					6'd35:
						begin
							q <= pl;
							y4 <= ph;
						end

					default:
						begin
							x0 <= x0;
							x1 <= x1;
							x2 <= x2;
							x3 <= x3;
							x4 <= x4;
							
							y0 <= y0;
							y1 <= y1;
							y2 <= y2;
							y3 <= y3;
							y4 <= y4;
							
							si1 <= si1;
							si2 <= si2;
							si3 <= si3;
							si4 <= si4;
							
							sj1 <= sj1;
							sj2 <= sj2;
							sj3 <= sj3;
							sj4 <= sj4;

							q <= q;
						end
				endcase

				/////////////////////////////////////////
				case (counter_f)
					3'd2:
						begin
							sii1 <= pl;
							{br, sjj1} <= pl - m0;
						end
					
					3'd3:
						begin
							sii2 <= pl;
							{br, sjj2} <= pl - m1 - br;
						end
					
					3'd4:
						begin
							sii3 <= pl;
							{br, sjj3} <= pl - m2 - br;
						end
					
					3'd5:
						begin
							sii4 <= ph + y4;
							{br, sjj4} <= ph + y4 - m3 - br;
						end

					default:
						begin
							sii1 <= sii1;
							sii2 <= sii2;
							sii3 <= sii3;
							sii4 <= sii4;

							sjj1 <= sjj1;
							sjj2 <= sjj2;
							sjj3 <= sjj3;
							sjj4 <= sjj4;

							br <= br;
						end
				endcase
			end
	end

	////////////////////////////////////////////////////////////////////////////
endmodule

/*
///////////////////////////////////////////////////////////////////////////////
module mmmb
	(
	nrst,
	clk,
	start,
	mp,
	m,
	x,
	y,
	z,
	done
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input			nrst;
	input			clk;
	input			start;

	input	[63:0]	mp;
	input	[255:0]	m;

	input	[255:0]	x;
	input	[255:0]	y;

	output	[255:0] z;
	output	reg		done;

	// Internal signal and register ////////////////////////////////////////////
	wire			op;
	wire	[63:0]	a;
	wire	[63:0]	b;
	wire	[63:0]	c;
	wire	[63:0]	d;
	wire	[63:0]	pl;
	wire	[63:0]	ph;

	wire	[63:0]	a0;
	wire	[63:0]	a1;
	wire	[63:0]	a2;
	wire	[63:0]	a3;

	wire	[63:0]	b0;
	wire	[63:0]	b1;
	wire	[63:0]	b2;
	wire	[63:0]	b3;

	wire	[63:0]	m0;
	wire	[63:0]	m1;
	wire	[63:0]	m2;
	wire	[63:0]	m3;

	reg		[5:0]	counter;

	reg		[63:0]	ai;
	reg		[63:0]	bi;
	reg		[63:0]	ci;
	reg		[63:0]	di;

	reg		[63:0]	x0;
	reg		[63:0]	x1;
	reg		[63:0]	x2;
	reg		[63:0]	x3;
	reg		[63:0]	x4;

	reg		[63:0]	y0;
	reg		[63:0]	y1;
	reg		[63:0]	y2;
	reg		[63:0]	y3;
	reg		[63:0]	y4;

	reg		[63:0]	si1;
	reg		[63:0]	si2;
	reg		[63:0]	si3;
	reg		[63:0]	si4;

	reg		[63:0]	sj1;
	reg		[63:0]	sj2;
	reg		[63:0]	sj3;
	reg		[63:0]	sj4;

	reg		[63:0]	q;
	reg				br;

	// Instantiation ///////////////////////////////////////////////////////////
	maaab maaab_u0
	(
    .clk(clk), 
    .op(op), 
    .a(ai), 
    .b(bi), 
    .c(ci), 
    .d(di), 
    .pl(pl), 
    .ph(ph)
    );

    // Combinational assignment ////////////////////////////////////////////////
    assign a0 = x[63:0];
	assign a1 = x[127:64];
	assign a2 = x[191:128];
	assign a3 = x[255:192];

	assign b0 = y[63:0];
	assign b1 = y[127:64];
	assign b2 = y[191:128];
	assign b3 = y[255:192];

	assign m0 = m[63:0];
	assign m1 = m[127:64];
	assign m2 = m[191:128];
	assign m3 = m[255:192];

	assign a =
	(counter == 6'd0) ? b0 : (counter == 6'd1) ? b0 : (counter == 6'd2) ? b0 : (counter == 6'd3) ? b0 :
	(counter == 6'd4) ? pl : (counter == 6'd5) ? b1 : (counter == 6'd6) ? b1 : (counter == 6'd7) ? b1 :
	(counter == 6'd8) ? pl : (counter == 6'd9) ? q : (counter == 6'd10) ? q : (counter == 6'd11) ? q :
	(counter == 6'd12) ? b1 : (counter == 6'd13) ? (pl+y0) : (counter == 6'd14) ? b2 : (counter == 6'd15) ? b2 :
	(counter == 6'd16) ? b2 : (counter == 6'd17) ? pl : (counter == 6'd18) ? q : (counter == 6'd19) ? q :
	(counter == 6'd20) ? q : (counter == 6'd21) ? b2 : (counter == 6'd22) ? (pl+x0) : (counter == 6'd23) ? b3 :
	(counter == 6'd24) ? b3 : (counter == 6'd25) ? b3 : (counter == 6'd26) ? pl : (counter == 6'd27) ? q :
	(counter == 6'd28) ? q : (counter == 6'd29) ? q : (counter == 6'd30) ? b3 : (counter == 6'd31) ? (pl+y0) :
	(counter == 6'd35) ? pl : (counter == 6'd36) ? q : (counter == 6'd37) ? q : (counter == 6'd38) ? q : 64'bX;
	
	assign b =
	(counter == 6'd0) ? a0 : (counter == 6'd1) ? a1 : (counter == 6'd2) ? a2 : (counter == 6'd3) ? a3 :
	(counter == 6'd4) ? mp : (counter == 6'd5) ? a0 : (counter == 6'd6) ? a1 : (counter == 6'd7) ? a2 :
	(counter == 6'd8) ? m0 : (counter == 6'd9) ? m1 : (counter == 6'd10) ? m2 : (counter == 6'd11) ? m3 :
	(counter == 6'd12) ? a3 : (counter == 6'd13) ? mp : (counter == 6'd14) ? a0 : (counter == 6'd15) ? a1 :
	(counter == 6'd16) ? a2 : (counter == 6'd17) ? m0 : (counter == 6'd18) ? m1 : (counter == 6'd19) ? m2 :
	(counter == 6'd20) ? m3 : (counter == 6'd21) ? a3 : (counter == 6'd22) ? mp : (counter == 6'd23) ? a0 :
	(counter == 6'd24) ? a1 : (counter == 6'd25) ? a2 : (counter == 6'd26) ? m0 : (counter == 6'd27) ? m1 :
	(counter == 6'd28) ? m2 : (counter == 6'd29) ? m3 : (counter == 6'd30) ? a3 : (counter == 6'd31) ? mp :
	(counter == 6'd35) ? m0 : (counter == 6'd36) ? m1 : (counter == 6'd37) ? m2 : (counter == 6'd38) ? m3 : 64'bX;

	assign c =
	(counter == 6'd0) ? 64'b0 : (counter == 6'd1) ? 64'b0 : (counter == 6'd2) ? 64'b0 : (counter == 6'd3) ? 64'b0 :
	(counter == 6'd4) ? 64'b0 : (counter == 6'd5) ? 64'b0 : (counter == 6'd6) ? 64'b0 : (counter == 6'd7) ? 64'b0 :
	(counter == 6'd8) ? x0 : (counter == 6'd9) ? x1 : (counter == 6'd10) ? x2 : (counter == 6'd11) ? x3 :
	(counter == 6'd12) ? ph : (counter == 6'd13) ? 64'b0 : (counter == 6'd14) ? 64'b0 : (counter == 6'd15) ? 64'b0 :
	(counter == 6'd16) ? 64'b0 : (counter == 6'd17) ? y0 : (counter == 6'd18) ? y1 : (counter == 6'd19) ? y2 :
	(counter == 6'd20) ? y3 : (counter == 6'd21) ? ph : (counter == 6'd22) ? 64'b0 : (counter == 6'd23) ? 64'b0 :
	(counter == 6'd24) ? 64'b0 : (counter == 6'd25) ? 64'b0 : (counter == 6'd26) ? x0 : (counter == 6'd27) ? x1 :
	(counter == 6'd28) ? x2 : (counter == 6'd29) ? x3 : (counter == 6'd30) ? ph : (counter == 6'd31) ? 64'b0 :
	(counter == 6'd35) ? y0 : (counter == 6'd36) ? y1 : (counter == 6'd37) ? y2 : (counter == 6'd38) ? y3 : 64'bX;

	assign d =
	(counter == 6'd0) ? 64'b0 : (counter == 6'd1) ? 64'b0 : (counter == 6'd2) ? 64'b0 : (counter == 6'd3) ? 64'b0 :
	(counter == 6'd4) ? 64'b0 : (counter == 6'd5) ? 64'b0 : (counter == 6'd6) ? 64'b0 : (counter == 6'd7) ? 64'b0 :
	(counter == 6'd8) ? 64'b0 : (counter == 6'd9) ? 64'b0 : (counter == 6'd10) ? 64'b0 : (counter == 6'd11) ? 64'b0 :
	(counter == 6'd12) ? 64'b0 : (counter == 6'd13) ? 64'b0 : (counter == 6'd14) ? 64'b0 : (counter == 6'd15) ? 64'b0 :
	(counter == 6'd16) ? 64'b0 : (counter == 6'd17) ? si1 : (counter == 6'd18) ? si2 : (counter == 6'd19) ? si3 :
	(counter == 6'd20) ? si4 : (counter == 6'd21) ? 64'b0 : (counter == 6'd22) ? 64'b0 : (counter == 6'd23) ? 64'b0 :
	(counter == 6'd24) ? 64'b0 : (counter == 6'd25) ? 64'b0 : (counter == 6'd26) ? sj1 : (counter == 6'd27) ? sj2 :
	(counter == 6'd28) ? sj3 : (counter == 6'd29) ? sj4 : (counter == 6'd30) ? 64'b0 : (counter == 6'd31) ? 64'b0 :
	(counter == 6'd35) ? si1 : (counter == 6'd36) ? si2 : (counter == 6'd37) ? si3 : (counter == 6'd38) ? si4 : 64'bX;

	assign op = ((counter == 6'd4) || (counter == 6'd8) || (counter == 6'd9) || (counter == 6'd12) ||
				(counter == 6'd16) || (counter == 6'd17) || (counter == 6'd18) || (counter == 6'd21) ||
				(counter == 6'd25) || (counter == 6'd26) || (counter == 6'd27) || (counter == 6'd30) ||
				(counter == 6'd34) || (counter == 6'd35) || (counter == 6'd39)) ? 1'b0 : 1'b1;

	assign z = (br == 1'b1)? {sj4, sj3, sj2, sj1} : {si4, si3, si2, si1};

	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				counter <= 6'b0;
			end
		else
			begin
				if (counter == 6'd43)
					begin
						counter <= 6'b0;
					end
				else if ((counter != 0) || (start == 1'b1))
					begin
						counter <= counter + 6'b1;
					end
				else
					begin
						counter <= counter;
					end
			end	
	end
	
	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				done <= 1'b0;
			end
		else
			begin
				if (counter == 6'd42)
					begin
						done <= 1'b1;
					end
				else
					begin
						done <= 1'b0;
					end
			end
	end

	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		ai <= a;
		bi <= b;
		ci <= c;
		di <= d;
	end

	////////////////////////////////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (!nrst)
			begin
				x0 <= 64'b0;
				x1 <= 64'b0;
				x2 <= 64'b0;
				x3 <= 64'b0;
				x4 <= 64'b0;
				
				y0 <= 64'b0;
				y1 <= 64'b0;
				y2 <= 64'b0;
				y3 <= 64'b0;
				y4 <= 64'b0;
				
				si1 <= 64'b0;
				si2 <= 64'b0;
				si3 <= 64'b0;
				si4 <= 64'b0;
				
				sj1 <= 64'b0;
				sj2 <= 64'b0;
				sj3 <= 64'b0;
				sj4 <= 64'b0;
				
				q <= 64'b0;
				br <= 1'b0;
			end
		else
			begin
				case (counter)
					6'd4:
						begin
							x0 <= pl;
						end
					
					6'd5:
						begin
							x1 <= pl;
						end
					
					6'd6:
						begin
							x2 <= pl;
						end
					
					6'd7:
						begin
							x3 <= pl;
						end
					
					6'd8:
						begin
							x4 <= ph;
							q <= pl;
						end
					
					6'd9:
						begin
							y0 <= pl;
						end
					
					6'd10:
						begin
							y1 <= pl;
						end
					
					6'd11:
						begin
							y2 <= pl;
						end
					
					6'd13:
						begin
							si1 <= pl;
						end
					
					6'd14:
						begin
							si2 <= pl;
						end
					
					6'd15:
						begin
							si3 <= pl;
						end
					
					6'd16:
						begin
							y3 <= pl;
							si4 <= ph + x4;
						end
					
					6'd17:
						begin
							q <= pl;
							y4 <= ph;
						end
					
					6'd18:
						begin
							x0 <= pl;
						end
					
					6'd19:
						begin
							x1 <= pl;
						end
					
					6'd20:
						begin
							x2 <= pl;
						end
					
					6'd22:
						begin
							sj1 <= pl;
						end
					
					6'd23:
						begin
							sj2 <= pl;
						end
					
					6'd24:
						begin
							sj3 <= pl;
						end
					
					6'd25:
						begin
							x3 <= pl;
							sj4 <= ph + y4;
						end
					
					6'd26:
						begin
							q <= pl;
							x4 <= ph;
						end
					
					6'd27:
						begin
							y0 <= pl;
						end
					
					6'd28:
						begin
							y1 <= pl;
						end
					
					6'd29:
						begin
							y2 <= pl;
						end
					
					6'd31:
						begin
							si1 <= pl;
						end
					
					6'd32:
						begin
							si2 <= pl;
						end
					
					6'd33:
						begin
							si3 <= pl;
						end
					
					6'd34:
						begin
							y3 <= pl;
							si4 <= ph + x4;
						end
					
					6'd35:
						begin
							q <= pl;
							y4 <= ph;
						end
					
					6'd40:
						begin
							sj1 <= pl;
							{br, si1} <= pl - m0;
						end
					
					6'd41:
						begin
							sj2 <= pl;
							{br, si2} <= pl - m1 - br;
						end
					
					6'd42:
						begin
							sj3 <= pl;
							{br, si3} <= pl - m2 - br;
						end
					
					6'd43:
						begin
							sj4 <= ph + y4;
							{br, si4} <= ph + y4 - m3 - br;
						end

					default:
						begin
							x0 <= x0;
							x1 <= x1;
							x2 <= x2;
							x3 <= x3;
							x4 <= x4;
							
							y0 <= y0;
							y1 <= y1;
							y2 <= y2;
							y3 <= y3;
							y4 <= y4;
							
							si1 <= si1;
							si2 <= si2;
							si3 <= si3;
							si4 <= si4;
							
							sj1 <= sj1;
							sj2 <= sj2;
							sj3 <= sj3;
							sj4 <= sj4;
							
							q <= q;
							br <= br;
						end
				endcase
			end
	end

	////////////////////////////////////////////////////////////////////////////
endmodule
*/
