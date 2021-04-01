`timescale 1 ns / 1 ps

module Mux_PCP_v1_0
	(
	    input  wire           clk,
	    input  wire           rst,
	    
		output wire           s00_axis_tready,
		input  wire   [127:0] s00_axis_tdata,
		input  wire   [15:0]  s00_axis_tkeep,
		input  wire           s00_axis_tlast,
		input  wire           s00_axis_tvalid,

		output wire           s01_axis_tready,
		input  wire   [127:0] s01_axis_tdata,
		input  wire   [15:0]  s01_axis_tkeep,
		input  wire           s01_axis_tlast,
		input  wire           s01_axis_tvalid,
		
		output wire           s02_axis_tready,
		input  wire   [127:0] s02_axis_tdata,
		input  wire   [15:0]  s02_axis_tkeep,
		input  wire           s02_axis_tlast,
		input  wire           s02_axis_tvalid,
		
		output wire           s03_axis_tready,
		input  wire   [127:0] s03_axis_tdata,
		input  wire   [15:0]  s03_axis_tkeep,
		input  wire           s03_axis_tlast,
		input  wire           s03_axis_tvalid,
		
		output wire           s04_axis_tready,
		input  wire   [127:0] s04_axis_tdata,
		input  wire   [15:0]  s04_axis_tkeep,
		input  wire           s04_axis_tlast,
		input  wire           s04_axis_tvalid,

		output wire           s05_axis_tready,
		input  wire   [127:0] s05_axis_tdata,
		input  wire   [15:0]  s05_axis_tkeep,
		input  wire           s05_axis_tlast,
		input  wire           s05_axis_tvalid,
		
		output wire           s06_axis_tready,
		input  wire   [127:0] s06_axis_tdata,
		input  wire   [15:0]  s06_axis_tkeep,
		input  wire           s06_axis_tlast,
		input  wire           s06_axis_tvalid,
		
		output wire           s07_axis_tready,
		input  wire   [127:0] s07_axis_tdata,
		input  wire   [15:0]  s07_axis_tkeep,
		input  wire           s07_axis_tlast,
		input  wire           s07_axis_tvalid,
		
		output wire           s08_axis_tready,
		input  wire   [127:0] s08_axis_tdata,
		input  wire   [15:0]  s08_axis_tkeep,
		input  wire           s08_axis_tlast,
		input  wire           s08_axis_tvalid,
		
		output wire           s09_axis_tready,
		input  wire   [127:0] s09_axis_tdata,
		input  wire   [15:0]  s09_axis_tkeep,
		input  wire           s09_axis_tlast,
		input  wire           s09_axis_tvalid,
		
		output wire           s10_axis_tready,
		input  wire   [127:0] s10_axis_tdata,
		input  wire   [15:0]  s10_axis_tkeep,
		input  wire           s10_axis_tlast,
		input  wire           s10_axis_tvalid,
		
		output wire           s11_axis_tready,
		input  wire   [127:0] s11_axis_tdata,
		input  wire   [15:0]  s11_axis_tkeep,
		input  wire           s11_axis_tlast,
		input  wire           s11_axis_tvalid,
		
		output wire           s12_axis_tready,
		input  wire   [127:0] s12_axis_tdata,
		input  wire   [15:0]  s12_axis_tkeep,
		input  wire           s12_axis_tlast,
		input  wire           s12_axis_tvalid,
		
		output wire           s13_axis_tready,
		input  wire   [127:0] s13_axis_tdata,
		input  wire   [15:0]  s13_axis_tkeep,
		input  wire           s13_axis_tlast,
		input  wire           s13_axis_tvalid,
		
		output wire           s14_axis_tready,
		input  wire   [127:0] s14_axis_tdata,
		input  wire   [15:0]  s14_axis_tkeep,
		input  wire           s14_axis_tlast,
		input  wire           s14_axis_tvalid,
		
		output wire           s15_axis_tready,
		input  wire   [127:0] s15_axis_tdata,
		input  wire   [15:0]  s15_axis_tkeep,
		input  wire           s15_axis_tlast,
		input  wire           s15_axis_tvalid,
		
		output wire           m_axis_tvalid,
		output wire   [127:0] m_axis_tdata,
		output wire   [15:0]  m_axis_tkeep,
		output wire           m_axis_tlast,
		input wire            m_axis_tready,
		
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

    assign m_axis_tvalid =
    (sel == 16'h0002)? s01_axis_tvalid :
    (sel == 16'h0004)? s02_axis_tvalid :
    (sel == 16'h0008)? s03_axis_tvalid :
    (sel == 16'h0010)? s04_axis_tvalid :
    (sel == 16'h0020)? s05_axis_tvalid :
    (sel == 16'h0040)? s06_axis_tvalid :
    (sel == 16'h0080)? s07_axis_tvalid :
    (sel == 16'h0100)? s08_axis_tvalid :
    (sel == 16'h0200)? s09_axis_tvalid :
    (sel == 16'h0400)? s10_axis_tvalid :
    (sel == 16'h0800)? s11_axis_tvalid :
    (sel == 16'h1000)? s12_axis_tvalid :
    (sel == 16'h2000)? s13_axis_tvalid :
    (sel == 16'h4000)? s14_axis_tvalid :
    (sel == 16'h8000)? s15_axis_tvalid :
    s00_axis_tvalid;

    assign m_axis_tdata =
    (sel == 16'h0002)? s01_axis_tdata :
    (sel == 16'h0004)? s02_axis_tdata :
    (sel == 16'h0008)? s03_axis_tdata :
    (sel == 16'h0010)? s04_axis_tdata :
    (sel == 16'h0020)? s05_axis_tdata :
    (sel == 16'h0040)? s06_axis_tdata :
    (sel == 16'h0080)? s07_axis_tdata :
    (sel == 16'h0100)? s08_axis_tdata :
    (sel == 16'h0200)? s09_axis_tdata :
    (sel == 16'h0400)? s10_axis_tdata :
    (sel == 16'h0800)? s11_axis_tdata :
    (sel == 16'h1000)? s12_axis_tdata :
    (sel == 16'h2000)? s13_axis_tdata :
    (sel == 16'h4000)? s14_axis_tdata :
    (sel == 16'h8000)? s15_axis_tdata :
    s00_axis_tdata;
    
    assign m_axis_tkeep =
    (sel == 16'h0002)? s01_axis_tkeep :
    (sel == 16'h0004)? s02_axis_tkeep :
    (sel == 16'h0008)? s03_axis_tkeep :
    (sel == 16'h0010)? s04_axis_tkeep :
    (sel == 16'h0020)? s05_axis_tkeep :
    (sel == 16'h0040)? s06_axis_tkeep :
    (sel == 16'h0080)? s07_axis_tkeep :
    (sel == 16'h0100)? s08_axis_tkeep :
    (sel == 16'h0200)? s09_axis_tkeep :
    (sel == 16'h0400)? s10_axis_tkeep :
    (sel == 16'h0800)? s11_axis_tkeep :
    (sel == 16'h1000)? s12_axis_tkeep :
    (sel == 16'h2000)? s13_axis_tkeep :
    (sel == 16'h4000)? s14_axis_tkeep :
    (sel == 16'h8000)? s15_axis_tkeep :
    s00_axis_tkeep;
    
    assign m_axis_tlast =
    (sel == 16'h0002)? s01_axis_tlast :
    (sel == 16'h0004)? s02_axis_tlast :
    (sel == 16'h0008)? s03_axis_tlast :
    (sel == 16'h0010)? s04_axis_tlast :
    (sel == 16'h0020)? s05_axis_tlast :
    (sel == 16'h0040)? s06_axis_tlast :
    (sel == 16'h0080)? s07_axis_tlast :
    (sel == 16'h0100)? s08_axis_tlast :
    (sel == 16'h0200)? s09_axis_tlast :
    (sel == 16'h0400)? s10_axis_tlast :
    (sel == 16'h0800)? s11_axis_tlast :
    (sel == 16'h1000)? s12_axis_tlast :
    (sel == 16'h2000)? s13_axis_tlast :
    (sel == 16'h4000)? s14_axis_tlast :
    (sel == 16'h8000)? s15_axis_tlast :
    s00_axis_tlast;
    
    assign s00_axis_tready = m_axis_tready;
    assign s01_axis_tready = m_axis_tready;
    assign s02_axis_tready = m_axis_tready;
    assign s03_axis_tready = m_axis_tready;
    assign s04_axis_tready = m_axis_tready;
    assign s05_axis_tready = m_axis_tready;
    assign s06_axis_tready = m_axis_tready;
    assign s07_axis_tready = m_axis_tready;
    assign s08_axis_tready = m_axis_tready;
    assign s09_axis_tready = m_axis_tready;
    assign s10_axis_tready = m_axis_tready;
    assign s11_axis_tready = m_axis_tready;
    assign s12_axis_tready = m_axis_tready;
    assign s13_axis_tready = m_axis_tready;
    assign s14_axis_tready = m_axis_tready;
    assign s15_axis_tready = m_axis_tready;

	endmodule
