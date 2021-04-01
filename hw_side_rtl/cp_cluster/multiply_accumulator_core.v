`timescale	1ns/1ps
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
module multiply_accumulator_core
    #(
		parameter CP_D_WIDTH = 72 // CP Datapath width
	 )
	(
	clk,
	ce,
	sclr,
	a,
	b,
	c,
	p
	);

	// Ports ///////////////////////////////////////////////////////////////////
	input							clk;
	input							ce;
	input							sclr;
	
	input	[CP_D_WIDTH-1:0]		a;
	input	[CP_D_WIDTH-1:0]		b;
	input	[CP_D_WIDTH-1:0]		c;
	
	output	[2*CP_D_WIDTH-1:0]		p;

	// Local parameter ////////////////////////////////////////////
	localparam Mult_Add_WIDTH = 18;

	// Internal signal and register ////////////////////////////////////////////
	wire	[Mult_Add_WIDTH-1:0] 	d0, d1, d2, e0, e1, e2, e3, e4;
	wire	[Mult_Add_WIDTH-1:0] 	f0, f1, f2, g0, g1, g2, g3, g4;
	wire	[Mult_Add_WIDTH-1:0] 	h0, h1, h2, i0, i1, i2, i3, i4;
	wire	[Mult_Add_WIDTH-1:0] 	j0, j1, j2, k0, k1, k2, k3, k4;
	
	reg		[2*CP_D_WIDTH-1:0]		p;
	
	reg		[Mult_Add_WIDTH-1:0]	e0_2, e0_3, e0_4, e0_5;
	reg		[Mult_Add_WIDTH-1:0]	g0_2, i0_2, k0_2;
	
	reg		[4*Mult_Add_WIDTH:0]	add_3;
	reg		[Mult_Add_WIDTH-1:0]	add3_4, add3_5;
	
	reg		[4*Mult_Add_WIDTH:0]	add_4;
	reg		[Mult_Add_WIDTH-1:0]	add4_5;
	
	reg		[4*Mult_Add_WIDTH:0]	add_5;
	
	reg		[Mult_Add_WIDTH-1:0]	k4_5, i4_5, g4_5, e4_5;	

	// Instantiation ///////////////////////////////////////////////////////////
	// Row_0///////////////////////////////////////////////////
	mult_add_18_type0	mult_add_00
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[Mult_Add_WIDTH-1:0]),
		.b(b[Mult_Add_WIDTH-1:0]),
		.c(c[Mult_Add_WIDTH-1:0]),
		.p({d0, e0})
	);

	mult_add_18_type1	mult_add_01
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.b(b[Mult_Add_WIDTH-1:0]),
		.c(d0),
		.p({d1, e1})
	);

	mult_add_18_type2	mult_add_02
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.b(b[Mult_Add_WIDTH-1:0]),
		.c(d1),
		.p({d2, e2})
	);

	mult_add_18_type3	mult_add_03
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.b(b[Mult_Add_WIDTH-1:0]),
		.c(d2),
		.p({e4, e3})
	);
	
	// Row_1///////////////////////////////////////////////////
	mult_add_18_type0	mult_add_10
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[Mult_Add_WIDTH-1:0]),
		.b(b[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.c(c[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.p({f0, g0})
	);

	mult_add_18_type1	mult_add_11
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.b(b[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.c(f0),
		.p({f1, g1})
	);

	mult_add_18_type2	mult_add_12
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.b(b[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.c(f1),
		.p({f2, g2})
	);

	mult_add_18_type3	mult_add_13
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.b(b[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.c(f2),
		.p({g4, g3})
	);

	// Row_2///////////////////////////////////////////////////
	mult_add_18_type0	mult_add_20
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[Mult_Add_WIDTH-1:0]),
		.b(b[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.c(c[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.p({h0, i0})
	);

	mult_add_18_type1	mult_add_21
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.b(b[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.c(h0),
		.p({h1, i1})
	);

	mult_add_18_type2	mult_add_22
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.b(b[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.c(h1),
		.p({h2, i2})
	);

	mult_add_18_type3	mult_add_23
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.b(b[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.c(h2),
		.p({i4, i3})
	);
	
	// Row_3///////////////////////////////////////////////////
	mult_add_18_type0	mult_add_30
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[Mult_Add_WIDTH-1:0]),
		.b(b[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.c(c[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.p({j0, k0})
	);

	mult_add_18_type1	mult_add_31
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH]),
		.b(b[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.c(j0),
		.p({j1, k1})
	);

	mult_add_18_type2	mult_add_32
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH]),
		.b(b[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.c(j1),
		.p({j2, k2})
	);

	mult_add_18_type3	mult_add_33
	(
		.clk(clk),
		.ce(ce),
		.sclr(sclr),
		.a(a[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.b(b[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH]),
		.c(j2),
		.p({k4, k3})
	);

	// Sequential procedures ///////////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				e0_2 <= 0;
				e0_3 <= 0;
				e0_4 <= 0;
				e0_5 <= 0;
				p[Mult_Add_WIDTH-1:0] <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						e0_2 <= e0;
						e0_3 <= e0_2;
						e0_4 <= e0_3;
						e0_5 <= e0_4;
						p[Mult_Add_WIDTH-1:0] <= e0_5;
					end
				else
					begin
						e0_2 <= e0_2;
						e0_3 <= e0_3;
						e0_4 <= e0_4;
						e0_5 <= e0_5;
						p[Mult_Add_WIDTH-1:0] <= p[Mult_Add_WIDTH-1:0];
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				g0_2 <= 0;
				i0_2 <= 0;
				k0_2 <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						g0_2 <= g0;
						i0_2 <= i0;
						k0_2 <= k0;
					end
				else
					begin
						g0_2 <= g0_2;
						i0_2 <= i0_2;
						k0_2 <= k0_2;
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				add_3 <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						add_3 <= {k0_2, i0_2, g0_2} + {k1, i1, g1, e1};
					end
				else
					begin
						add_3 <= add_3;
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				add3_4 <= 0;
				add3_5 <= 0;
				p[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH] <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						add3_4 <= add_3[Mult_Add_WIDTH-1:0];
						add3_5 <= add3_4;
						p[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH] <= add3_5;
					end
				else
					begin
						add3_4 <= add3_4;
						add3_5 <= add3_5;
						p[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH] <= p[2*Mult_Add_WIDTH-1:Mult_Add_WIDTH];
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				add_4 <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						add_4 <= add_3[4*Mult_Add_WIDTH:Mult_Add_WIDTH] + {k2, i2, g2, e2};
					end
				else
					begin
						add_4 <= add_4;
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				add4_5 <= 0;
				p[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH] <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						add4_5 <= add_4[Mult_Add_WIDTH-1:0];
						p[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH] <= add4_5;
					end
				else
					begin
						add4_5 <= add4_5;
						p[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH] <= p[3*Mult_Add_WIDTH-1:2*Mult_Add_WIDTH];
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				add_5 <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						add_5 <= add_4[4*Mult_Add_WIDTH:Mult_Add_WIDTH] + {k3, i3, g3, e3};
					end
				else
					begin
						add_5 <= add_5;
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				p[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH] <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						p[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH] <= add_5[Mult_Add_WIDTH-1:0];
					end
				else
					begin
						p[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH] <= p[4*Mult_Add_WIDTH-1:3*Mult_Add_WIDTH];
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				p[8*Mult_Add_WIDTH-1:4*Mult_Add_WIDTH] <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						p[8*Mult_Add_WIDTH-1:4*Mult_Add_WIDTH] <= add_5[4*Mult_Add_WIDTH:Mult_Add_WIDTH] + {k4_5, i4_5, g4_5, e4_5};
					end
				else
					begin
						p[8*Mult_Add_WIDTH-1:4*Mult_Add_WIDTH] <= p[8*Mult_Add_WIDTH-1:4*Mult_Add_WIDTH];
					end
			end
	end

	//////////////////////////////////////////////
	always @(posedge clk)
	begin
		if (sclr == 1'b1)
			begin
				k4_5 <= 0;
				i4_5 <= 0;
				g4_5 <= 0;
				e4_5 <= 0;
			end
		else
			begin
				if (ce == 1'b1)
					begin
						k4_5 <= k4;
						i4_5 <= i4;
						g4_5 <= g4;
						e4_5 <= e4;
					end
				else
					begin
						k4_5 <= k4_5;
						i4_5 <= i4_5;
						g4_5 <= g4_5;
						e4_5 <= e4_5;
					end
			end
	end

	////////////////////////////////////////////////////////////////////////////
endmodule
