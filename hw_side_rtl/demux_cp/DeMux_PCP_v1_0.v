`timescale 1 ns / 1 ps

module DeMux_PCP_v1_0
	(
	    input  wire           clk,
	    input  wire           rst,

		output wire           s_axis_tready,
		input  wire   [127:0] s_axis_tdata,
		input  wire   [15:0]  s_axis_tkeep,
		input  wire           s_axis_tlast,
		input  wire           s_axis_tvalid,

		output wire           m00_axis_tvalid,
		output wire   [127:0] m00_axis_tdata,
		output wire   [15:0]  m00_axis_tkeep,
		output wire           m00_axis_tlast,
		input wire            m00_axis_tready,
		
		output wire           m01_axis_tvalid,
		output wire   [127:0] m01_axis_tdata,
		output wire   [15:0]  m01_axis_tkeep,
		output wire           m01_axis_tlast,
		input wire            m01_axis_tready,
		
		output wire           m02_axis_tvalid,
		output wire   [127:0] m02_axis_tdata,
		output wire   [15:0]  m02_axis_tkeep,
		output wire           m02_axis_tlast,
		input wire            m02_axis_tready,
		
		output wire           m03_axis_tvalid,
		output wire   [127:0] m03_axis_tdata,
		output wire   [15:0]  m03_axis_tkeep,
		output wire           m03_axis_tlast,
		input wire            m03_axis_tready,
		
		output wire           m04_axis_tvalid,
		output wire   [127:0] m04_axis_tdata,
		output wire   [15:0]  m04_axis_tkeep,
		output wire           m04_axis_tlast,
		input wire            m04_axis_tready,
		
		output wire           m05_axis_tvalid,
		output wire   [127:0] m05_axis_tdata,
		output wire   [15:0]  m05_axis_tkeep,
		output wire           m05_axis_tlast,
		input wire            m05_axis_tready,
		
		output wire           m06_axis_tvalid,
		output wire   [127:0] m06_axis_tdata,
		output wire   [15:0]  m06_axis_tkeep,
		output wire           m06_axis_tlast,
		input wire            m06_axis_tready,
		
		output wire           m07_axis_tvalid,
		output wire   [127:0] m07_axis_tdata,
		output wire   [15:0]  m07_axis_tkeep,
		output wire           m07_axis_tlast,
		input wire            m07_axis_tready,
		
		output wire           m08_axis_tvalid,
		output wire   [127:0] m08_axis_tdata,
		output wire   [15:0]  m08_axis_tkeep,
		output wire           m08_axis_tlast,
		input wire            m08_axis_tready,
		
		output wire           m09_axis_tvalid,
		output wire   [127:0] m09_axis_tdata,
		output wire   [15:0]  m09_axis_tkeep,
		output wire           m09_axis_tlast,
		input wire            m09_axis_tready,
		
		output wire           m10_axis_tvalid,
		output wire   [127:0] m10_axis_tdata,
		output wire   [15:0]  m10_axis_tkeep,
		output wire           m10_axis_tlast,
		input wire            m10_axis_tready,
		
		output wire           m11_axis_tvalid,
		output wire   [127:0] m11_axis_tdata,
		output wire   [15:0]  m11_axis_tkeep,
		output wire           m11_axis_tlast,
		input wire            m11_axis_tready,
		
		output wire           m12_axis_tvalid,
		output wire   [127:0] m12_axis_tdata,
		output wire   [15:0]  m12_axis_tkeep,
		output wire           m12_axis_tlast,
		input wire            m12_axis_tready,
		
		output wire           m13_axis_tvalid,
		output wire   [127:0] m13_axis_tdata,
		output wire   [15:0]  m13_axis_tkeep,
		output wire           m13_axis_tlast,
		input wire            m13_axis_tready,
		
		output wire           m14_axis_tvalid,
		output wire   [127:0] m14_axis_tdata,
		output wire   [15:0]  m14_axis_tkeep,
		output wire           m14_axis_tlast,
		input wire            m14_axis_tready,
		
		output wire           m15_axis_tvalid,
		output wire   [127:0] m15_axis_tdata,
		output wire   [15:0]  m15_axis_tkeep,
		output wire           m15_axis_tlast,
		input wire            m15_axis_tready,
		
		input wire sel_00,
		input wire sel_01,
		input wire sel_02,
		input wire sel_03,
		input wire sel_04,
		input wire sel_05,
		input wire sel_06,
		input wire sel_07,
		input wire sel_08,
		input wire sel_09,
		input wire sel_10,
		input wire sel_11,
		input wire sel_12,
		input wire sel_13,
		input wire sel_14,
		input wire sel_15
	);
	
	wire [15:0] sel;
	assign sel = {sel_15,sel_14,sel_13,sel_12,sel_11,sel_10,sel_09,sel_08,sel_07,sel_06,sel_05,sel_04,sel_03,sel_02,sel_01,sel_00};

    assign  m00_axis_tvalid = s_axis_tvalid;
    assign  m01_axis_tvalid = s_axis_tvalid;
    assign  m02_axis_tvalid = s_axis_tvalid;
    assign  m03_axis_tvalid = s_axis_tvalid;
    assign  m04_axis_tvalid = s_axis_tvalid;
    assign  m05_axis_tvalid = s_axis_tvalid;
    assign  m06_axis_tvalid = s_axis_tvalid;
    assign  m07_axis_tvalid = s_axis_tvalid;
    assign  m08_axis_tvalid = s_axis_tvalid;
    assign  m09_axis_tvalid = s_axis_tvalid;
    assign  m10_axis_tvalid = s_axis_tvalid;
    assign  m11_axis_tvalid = s_axis_tvalid;
    assign  m12_axis_tvalid = s_axis_tvalid;
    assign  m13_axis_tvalid = s_axis_tvalid;
    assign  m14_axis_tvalid = s_axis_tvalid;
    assign  m15_axis_tvalid = s_axis_tvalid;
    
    assign  m00_axis_tdata = s_axis_tdata;
    assign  m01_axis_tdata = s_axis_tdata;
    assign  m02_axis_tdata = s_axis_tdata;
    assign  m03_axis_tdata = s_axis_tdata;
    assign  m04_axis_tdata = s_axis_tdata;
    assign  m05_axis_tdata = s_axis_tdata;
    assign  m06_axis_tdata = s_axis_tdata;
    assign  m07_axis_tdata = s_axis_tdata;
    assign  m08_axis_tdata = s_axis_tdata;
    assign  m09_axis_tdata = s_axis_tdata;
    assign  m10_axis_tdata = s_axis_tdata;
    assign  m11_axis_tdata = s_axis_tdata;
    assign  m12_axis_tdata = s_axis_tdata;
    assign  m13_axis_tdata = s_axis_tdata;
    assign  m14_axis_tdata = s_axis_tdata;
    assign  m15_axis_tdata = s_axis_tdata;
    
    assign  m00_axis_tkeep = s_axis_tkeep;
    assign  m01_axis_tkeep = s_axis_tkeep;
    assign  m02_axis_tkeep = s_axis_tkeep;
    assign  m03_axis_tkeep = s_axis_tkeep;
    assign  m04_axis_tkeep = s_axis_tkeep;
    assign  m05_axis_tkeep = s_axis_tkeep;
    assign  m06_axis_tkeep = s_axis_tkeep;
    assign  m07_axis_tkeep = s_axis_tkeep;
    assign  m08_axis_tkeep = s_axis_tkeep;
    assign  m09_axis_tkeep = s_axis_tkeep;
    assign  m10_axis_tkeep = s_axis_tkeep;
    assign  m11_axis_tkeep = s_axis_tkeep;
    assign  m12_axis_tkeep = s_axis_tkeep;
    assign  m13_axis_tkeep = s_axis_tkeep;
    assign  m14_axis_tkeep = s_axis_tkeep;
    assign  m15_axis_tkeep = s_axis_tkeep;
    
    assign  m00_axis_tlast = s_axis_tlast;
    assign  m01_axis_tlast = s_axis_tlast;
    assign  m02_axis_tlast = s_axis_tlast;
    assign  m03_axis_tlast = s_axis_tlast;
    assign  m04_axis_tlast = s_axis_tlast;
    assign  m05_axis_tlast = s_axis_tlast;
    assign  m06_axis_tlast = s_axis_tlast;
    assign  m07_axis_tlast = s_axis_tlast;
    assign  m08_axis_tlast = s_axis_tlast;
    assign  m09_axis_tlast = s_axis_tlast;
    assign  m10_axis_tlast = s_axis_tlast;
    assign  m11_axis_tlast = s_axis_tlast;
    assign  m12_axis_tlast = s_axis_tlast;
    assign  m13_axis_tlast = s_axis_tlast;
    assign  m14_axis_tlast = s_axis_tlast;
    assign  m15_axis_tlast = s_axis_tlast;
    
    assign s_axis_tready =
    (sel == 16'h0002)? m01_axis_tready :
    (sel == 16'h0004)? m02_axis_tready :
    (sel == 16'h0008)? m03_axis_tready :
    (sel == 16'h0010)? m04_axis_tready :
    (sel == 16'h0020)? m05_axis_tready :
    (sel == 16'h0040)? m06_axis_tready :
    (sel == 16'h0080)? m07_axis_tready :
    (sel == 16'h0100)? m08_axis_tready :
    (sel == 16'h0200)? m09_axis_tready :
    (sel == 16'h0400)? m10_axis_tready :
    (sel == 16'h0800)? m11_axis_tready :
    (sel == 16'h1000)? m12_axis_tready :
    (sel == 16'h2000)? m13_axis_tready :
    (sel == 16'h4000)? m14_axis_tready :
    (sel == 16'h8000)? m15_axis_tready :
    m00_axis_tready;

	endmodule
