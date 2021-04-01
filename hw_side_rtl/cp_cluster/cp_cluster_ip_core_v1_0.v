`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
module cp_cluster_ip_core_v1_0
    #(
		parameter MS_AXI_WIDTH  = 64, // Maste and Slave AXI interface data width
		parameter CP_CMD_WIDTH = 20, // Command width of CP core

		parameter CP_D_WIDTH = 72,	// CP Datapath width
		parameter CP_I_WIDTH = 56,	// CP Instruction width
		parameter DMEM_ADDR_WIDTH = 10,	// (2 ^ DMEM_ADDR_WIDTH) * CP_D_WIDTH = DMEM Size
		parameter IMEM_ADDR_WIDTH = 9	// (2 ^ IMEM_ADDR_WIDTH) * CP_I_WIDTH = IMEM Size
	 )
	(
		s_axis_aresetn,
		s_axis_aclk,
		s_axis_tvalid,
		s_axis_tready,
		s_axis_tdata,
		s_axis_tkeep,
		s_axis_tlast,

		m_axis_aresetn,
		m_axis_aclk,    
		m_axis_tvalid,
		m_axis_tready,
		m_axis_tdata,
		m_axis_tkeep,
		m_axis_tlast,

		dmem_shared_addr_0,
		dmem_shared_in_0,
		dmem_shared_we_0,
		shared_dmem_out_0,

		dmem_shared_addr_1,
		dmem_shared_in_1,
		dmem_shared_we_1,
		shared_dmem_out_1,

		command_0,
		dmem_shared_trq_0,
		dmem_shared_tack_0,
		status_0,

		command_1,
		dmem_shared_trq_1,
		dmem_shared_tack_1,
		status_1
	);

	// Ports ////////////////////////////////////////////////////////
	// Slave_AXI_Stream port
	input							s_axis_aresetn;
	input							s_axis_aclk;
	input							s_axis_tvalid;
	output							s_axis_tready;
	input	[MS_AXI_WIDTH-1:0]		s_axis_tdata;
	input	[MS_AXI_WIDTH/8-1:0]	s_axis_tkeep;
	input							s_axis_tlast;

	// Master_AXI_Stream port
	input							m_axis_aresetn;
	input							m_axis_aclk;
	output							m_axis_tvalid;
	input							m_axis_tready;
	output	[MS_AXI_WIDTH-1:0]		m_axis_tdata;
	output	[MS_AXI_WIDTH/8-1:0]	m_axis_tkeep;
	output							m_axis_tlast;

	output	[DMEM_ADDR_WIDTH-1:0]	dmem_shared_addr_0;
	output	[CP_D_WIDTH-1:0]		dmem_shared_in_0;
	output							dmem_shared_we_0;
	input	[CP_D_WIDTH-1:0]		shared_dmem_out_0;
	
	output	[DMEM_ADDR_WIDTH-1:0]	dmem_shared_addr_1;
	output	[CP_D_WIDTH-1:0]		dmem_shared_in_1;
	output							dmem_shared_we_1;
	input	[CP_D_WIDTH-1:0]		shared_dmem_out_1;

	input	[CP_CMD_WIDTH-1:0]		command_0;
	output							dmem_shared_trq_0;
	input                           dmem_shared_tack_0;
	output	[1:0]					status_0;

	input	[CP_CMD_WIDTH-1:0]		command_1;
	output							dmem_shared_trq_1;
	input                           dmem_shared_tack_1;
	output	[1:0]					status_1;

	// Local parameters /////////////////////////////////////////////
	// States declaration of FSMs
	localparam	IDLE			= 4'b0000;
	localparam	DMA_SHARING		= 4'b0001;
    localparam	CP_IMEM_LD		= 4'b0010;
    localparam	CP_DMEM_L3L1_WR	= 4'b0011;
	localparam	CP_DMEM_L3L1_RD	= 4'b0100;
	localparam	L2_TRANSFERING	= 4'b0101;
	localparam	CP_DMEM_L1L2_WR	= 4'b0110;
	localparam	CP_DMEM_L1L2_RD	= 4'b0111;
	localparam	CP_PROG_RUN		= 4'b1000;
    localparam	WAIT_CMD_ZERO	= 4'b1001;

	// Maximum address of IMEM
	localparam	IMEM_MAX_ADDR	= 9'h1FF;

	// Level_1 DMEM (local) address offset
	localparam	X0_DMEM_L1_ADDR_OFFSET	= 10'h209;
	localparam	X1_DMEM_L1_ADDR_OFFSET	= 10'h243;
	localparam	X2_DMEM_L1_ADDR_OFFSET	= 10'h27D;
	localparam	X3_DMEM_L1_ADDR_OFFSET	= 10'h2B7;
	localparam	X4_DMEM_L1_ADDR_OFFSET	= 10'h2F1;
	localparam	X5_DMEM_L1_ADDR_OFFSET	= 10'h32B;
	localparam	X6_DMEM_L1_ADDR_OFFSET	= 10'h365;
	localparam	X7_DMEM_L1_ADDR_OFFSET	= 10'h39F;
	localparam	R2_DMEM_L1_ADDR_OFFSET	= 10'h003;
	localparam	n2_DMEM_L1_ADDR_OFFSET	= 10'h0E9;
	localparam	n_DMEM_L1_ADDR_OFFSET	= 10'h15C;
	localparam	g_DMEM_L1_ADDR_OFFSET	= 10'h196;
	localparam	RES_DMEM_L1_ADDR_OFFSET	= 10'h3D9;

	// Level_2 DMEM (global and shared) address offset
	localparam	Y0_DMEM_L2_ADDR_OFFSET	= 10'h000;
	localparam	Y1_DMEM_L2_ADDR_OFFSET	= 10'h03A;
	localparam	Y2_DMEM_L2_ADDR_OFFSET	= 10'h074;
	localparam	Y3_DMEM_L2_ADDR_OFFSET	= 10'h0AE;
	localparam	Y4_DMEM_L2_ADDR_OFFSET	= 10'h0E8;
	localparam	Y5_DMEM_L2_ADDR_OFFSET	= 10'h122;
	localparam	Y6_DMEM_L2_ADDR_OFFSET	= 10'h15C;
	localparam	Y7_DMEM_L2_ADDR_OFFSET	= 10'h196;
	localparam	Y8_DMEM_L2_ADDR_OFFSET	= 10'h1D0;
	localparam	Y9_DMEM_L2_ADDR_OFFSET	= 10'h20A;
	localparam	Y10_DMEM_L2_ADDR_OFFSET	= 10'h244;
	localparam	Y11_DMEM_L2_ADDR_OFFSET	= 10'h27E;
	localparam	Y12_DMEM_L2_ADDR_OFFSET	= 10'h2B8;
	localparam	Y13_DMEM_L2_ADDR_OFFSET	= 10'h2F2;
	localparam	Y14_DMEM_L2_ADDR_OFFSET	= 10'h32C;
	localparam	Y15_DMEM_L2_ADDR_OFFSET	= 10'h366;
	localparam	Y16_DMEM_L2_ADDR_OFFSET	= 10'h3A0;
	localparam	RES_DMEM_L2_ADDR_OFFSET	= 10'h3DA;

	// Wires ///////////////////////////////////////////////
	wire	[MS_AXI_WIDTH-1:0]		m_axis_tdata_0;
	wire	[MS_AXI_WIDTH-1:0]		m_axis_tdata_1;

	wire							ext_cp_active_0;
	wire							cp_ext_nbusy_0;
	wire							cp_ext_done_0;

	wire	[IMEM_ADDR_WIDTH-1:0]	ext_imem_addr_0;
	wire							ext_imem_we_0;
	wire	[CP_I_WIDTH-1:0]		ext_imem_in_0;

	wire	[DMEM_ADDR_WIDTH-1:0]	ext_dmem_addr_0;
	wire	[CP_D_WIDTH-1:0]		ext_dmem_in_0;
	wire							ext_dmem_we_0;
	wire	[CP_D_WIDTH-1:0]		dmem_ext_out_0;

	wire							ext_cp_active_1;
	wire							cp_ext_nbusy_1;
	wire							cp_ext_done_1;

	wire	[IMEM_ADDR_WIDTH-1:0]	ext_imem_addr_1;
	wire							ext_imem_we_1;
	wire	[CP_I_WIDTH-1:0]		ext_imem_in_1;

	wire	[DMEM_ADDR_WIDTH-1:0]	ext_dmem_addr_1;
	wire	[CP_D_WIDTH-1:0]		ext_dmem_in_1;
	wire							ext_dmem_we_1;
	wire	[CP_D_WIDTH-1:0]		dmem_ext_out_1;

	wire	[DMEM_ADDR_WIDTH-1:0]	dmem_L1_addr_offset_0;
	wire	[DMEM_ADDR_WIDTH-1:0]	dmem_L1_addr_offset_1;

	wire	[DMEM_ADDR_WIDTH-1:0]	dmem_L2_addr_offset_0;
	wire	[DMEM_ADDR_WIDTH-1:0]	dmem_L2_addr_offset_1;

	wire	[DMEM_ADDR_WIDTH-1:0]	ext_dmem_L1_addr_a;
	wire	[CP_D_WIDTH-1:0]		ext_dmem_L1_in_a;
	wire							ext_dmem_L1_we_a;
	wire	[CP_D_WIDTH-1:0]		dmem_L1_ext_out_a;

	wire	[DMEM_ADDR_WIDTH-1:0]	ext_dmem_L1_addr_b;
	wire	[CP_D_WIDTH-1:0]		ext_dmem_L1_in_b;
	wire							ext_dmem_L1_we_b;
	wire	[CP_D_WIDTH-1:0]		dmem_L1_ext_out_b;

	wire							ext_dmem_L1_selector;

	wire	[DMEM_ADDR_WIDTH-1:0]	ext_dmem_addr_x;
	wire							ext_dmem_we_x;

	wire	[DMEM_ADDR_WIDTH-1:0]	ext_dmem_addr_y;
	wire							ext_dmem_we_y;

	// Registers ///////////////////////////////////////////
	reg								m_axis_tvalid;
	reg								m_axis_tlast;

	reg		[3:0]					state_0;
	reg		[3:0]					next_state_0;

	reg		[3:0]					state_1;
	reg		[3:0]					next_state_1;

	reg		[IMEM_ADDR_WIDTH-1:0]	imem_addr_cntr;

	reg		[7:0]					dmem_wr_len_cntr;
	reg		[DMEM_ADDR_WIDTH-1:0]	dmem_wr_addr_cntr;
	reg		[3:0]					dmem_wr_sub_cntr;

	reg		[7:0]					dmem_rd_len_cntr;
	reg		[DMEM_ADDR_WIDTH-1:0]	dmem_rd_addr_cntr;
	reg		[3:0]					dmem_rd_sub_cntr;

	reg		[MS_AXI_WIDTH-1:0]		cached_s_axis_tdata;
	reg		[MS_AXI_WIDTH-1:0]		cached_dmem_ext_out;

	reg		[DMEM_ADDR_WIDTH-1:0]	dmem_L1L2_rd_addr_cntr_x;
	reg		[DMEM_ADDR_WIDTH-1:0]	dmem_L1L2_rd_addr_cntr_y;

	// Assignments //////////////////////////////////////////////////
	assign status_0	= ((state_0 == CP_IMEM_LD) || (state_0 == CP_DMEM_L3L1_WR) || (state_0 == CP_DMEM_L3L1_RD)) ? 2'b01 : // L3L1_DMA_TRANSFERING
					  ((state_0 == L2_TRANSFERING) || (state_0 == CP_DMEM_L1L2_WR) || (state_0 == CP_DMEM_L1L2_RD)) ? 2'b10 : // L1L2_DMEM_TRANSFERING
					  (state_0 == WAIT_CMD_ZERO) ? 2'b11 : // CMD_ZERO_WAITTING
					  2'b00;

	assign status_1	= ((state_1 == CP_IMEM_LD) || (state_1 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_RD)) ? 2'b01 : // L3L1_DMA_TRANSFERING
					  ((state_1 == L2_TRANSFERING) || (state_1 == CP_DMEM_L1L2_WR) || (state_1 == CP_DMEM_L1L2_RD)) ? 2'b10 : // L1L2_DMEM_TRANSFERING
					  (state_1 == WAIT_CMD_ZERO) ? 2'b11 : // CMD_ZERO_WAITTING
					  2'b00;

	assign dmem_shared_trq_0 = ((state_0 == L2_TRANSFERING) || (state_0 == CP_DMEM_L1L2_WR) || (state_0 == CP_DMEM_L1L2_RD)) ? 1'b1 : 1'b0;
	assign dmem_shared_trq_1 = ((state_1 == L2_TRANSFERING) || (state_1 == CP_DMEM_L1L2_WR) || (state_1 == CP_DMEM_L1L2_RD)) ? 1'b1 : 1'b0;

	assign ext_cp_active_0 = (state_0 == CP_PROG_RUN) ? 1'b1 : 1'b0;
	assign ext_cp_active_1 = (state_1 == CP_PROG_RUN) ? 1'b1 : 1'b0;

	assign s_axis_tready = (((state_0 == CP_IMEM_LD) || (state_1 == CP_IMEM_LD)) && (s_axis_tvalid == 1'b1)) ? 1'b1 :
						   (((state_0 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_WR)) && (s_axis_tvalid == 1'b1)) ? 1'b1 :
						   1'b0;

	assign m_axis_tkeep = s_axis_tkeep;

	assign m_axis_tdata = (state_1 == CP_DMEM_L3L1_RD) ? m_axis_tdata_1 : m_axis_tdata_0;

	assign m_axis_tdata_0 = (dmem_rd_sub_cntr == 4'b0000) ? dmem_ext_out_0[(8*(CP_D_WIDTH/9))-1:0] :
							(dmem_rd_sub_cntr == 4'b0001) ? {dmem_ext_out_0[(7*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:7*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0010) ? {dmem_ext_out_0[(6*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:6*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0011) ? {dmem_ext_out_0[(5*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:5*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0100) ? {dmem_ext_out_0[(4*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:4*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0101) ? {dmem_ext_out_0[(3*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:3*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0110) ? {dmem_ext_out_0[(2*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:2*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0111) ? {dmem_ext_out_0[(1*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:1*(MS_AXI_WIDTH/8)]} :
							cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:0*(MS_AXI_WIDTH/8)];

	assign m_axis_tdata_1 = (dmem_rd_sub_cntr == 4'b0000) ? dmem_ext_out_1[(8*(CP_D_WIDTH/9))-1:0] :
							(dmem_rd_sub_cntr == 4'b0001) ? {dmem_ext_out_1[(7*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:7*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0010) ? {dmem_ext_out_1[(6*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:6*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0011) ? {dmem_ext_out_1[(5*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:5*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0100) ? {dmem_ext_out_1[(4*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:4*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0101) ? {dmem_ext_out_1[(3*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:3*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0110) ? {dmem_ext_out_1[(2*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:2*(MS_AXI_WIDTH/8)]} :
							(dmem_rd_sub_cntr == 4'b0111) ? {dmem_ext_out_1[(1*(CP_D_WIDTH/9))-1:0], cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:1*(MS_AXI_WIDTH/8)]} :
							cached_dmem_ext_out[(8*(MS_AXI_WIDTH/8))-1:0*(MS_AXI_WIDTH/8)];

	assign ext_imem_addr_0 = imem_addr_cntr;
	assign ext_imem_we_0 = ((state_0 == CP_IMEM_LD) && (s_axis_tvalid == 1'b1) && (s_axis_tready == 1'b1)) ? 1'b1 : 1'b0;
	
	assign ext_imem_in_0 = {s_axis_tdata[2*(MS_AXI_WIDTH/8)-1:1*(MS_AXI_WIDTH/8)],
							s_axis_tdata[3*(MS_AXI_WIDTH/8)-1:2*(MS_AXI_WIDTH/8)],
							s_axis_tdata[4*(MS_AXI_WIDTH/8)-1:3*(MS_AXI_WIDTH/8)],
							s_axis_tdata[5*(MS_AXI_WIDTH/8)-1:4*(MS_AXI_WIDTH/8)],
							s_axis_tdata[6*(MS_AXI_WIDTH/8)-1:5*(MS_AXI_WIDTH/8)],
							s_axis_tdata[7*(MS_AXI_WIDTH/8)-1:6*(MS_AXI_WIDTH/8)],
							s_axis_tdata[8*(MS_AXI_WIDTH/8)-1:7*(MS_AXI_WIDTH/8)]};

	assign ext_imem_addr_1 = imem_addr_cntr;
	assign ext_imem_we_1 = ((state_1 == CP_IMEM_LD) && (s_axis_tvalid == 1'b1) && (s_axis_tready == 1'b1)) ? 1'b1 : 1'b0;

	assign ext_imem_in_1 = {s_axis_tdata[2*(MS_AXI_WIDTH/8)-1:1*(MS_AXI_WIDTH/8)],
							s_axis_tdata[3*(MS_AXI_WIDTH/8)-1:2*(MS_AXI_WIDTH/8)],
							s_axis_tdata[4*(MS_AXI_WIDTH/8)-1:3*(MS_AXI_WIDTH/8)],
							s_axis_tdata[5*(MS_AXI_WIDTH/8)-1:4*(MS_AXI_WIDTH/8)],
							s_axis_tdata[6*(MS_AXI_WIDTH/8)-1:5*(MS_AXI_WIDTH/8)],
							s_axis_tdata[7*(MS_AXI_WIDTH/8)-1:6*(MS_AXI_WIDTH/8)],
							s_axis_tdata[8*(MS_AXI_WIDTH/8)-1:7*(MS_AXI_WIDTH/8)]};

	assign dmem_L1_addr_offset_0 =	(command_0[15:12] == 4'b0000) ? X0_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0001) ? X1_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0010) ? X2_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0011) ? X3_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0100) ? X4_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0101) ? X5_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0110) ? X6_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b0111) ? X7_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b1000) ? R2_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b1001) ? n2_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b1010) ? n_DMEM_L1_ADDR_OFFSET :
									(command_0[15:12] == 4'b1011) ? g_DMEM_L1_ADDR_OFFSET :
									RES_DMEM_L1_ADDR_OFFSET;

	assign dmem_L1_addr_offset_1 =	(command_1[15:12] == 4'b0000) ? X0_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0001) ? X1_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0010) ? X2_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0011) ? X3_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0100) ? X4_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0101) ? X5_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0110) ? X6_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b0111) ? X7_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b1000) ? R2_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b1001) ? n2_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b1010) ? n_DMEM_L1_ADDR_OFFSET :
									(command_1[15:12] == 4'b1011) ? g_DMEM_L1_ADDR_OFFSET :
									RES_DMEM_L1_ADDR_OFFSET;

	assign dmem_L2_addr_offset_0 =	(command_0[19:16] == 4'b0000) ? Y0_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0001) ? Y1_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0010) ? Y2_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0011) ? Y3_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0100) ? Y4_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0101) ? Y5_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0110) ? Y6_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b0111) ? Y7_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1000) ? Y8_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1001) ? Y9_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1010) ? Y10_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1011) ? Y11_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1100) ? Y12_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1101) ? Y13_DMEM_L2_ADDR_OFFSET :
									(command_0[19:16] == 4'b1110) ? Y14_DMEM_L2_ADDR_OFFSET :
									Y15_DMEM_L2_ADDR_OFFSET;

	assign dmem_L2_addr_offset_1 =	(command_1[19:16] == 4'b0000) ? Y0_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0001) ? Y1_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0010) ? Y2_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0011) ? Y3_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0100) ? Y4_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0101) ? Y5_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0110) ? Y6_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b0111) ? Y7_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1000) ? Y8_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1001) ? Y9_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1010) ? Y10_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1011) ? Y11_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1100) ? Y12_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1101) ? Y13_DMEM_L2_ADDR_OFFSET :
									(command_1[19:16] == 4'b1110) ? Y14_DMEM_L2_ADDR_OFFSET :
									Y15_DMEM_L2_ADDR_OFFSET;

	assign ext_dmem_addr_0 = (state_0 == CP_DMEM_L3L1_RD) ? (dmem_L1_addr_offset_0 + dmem_rd_addr_cntr) : (dmem_L1_addr_offset_0 + dmem_wr_addr_cntr);
	assign ext_dmem_addr_1 = (state_1 == CP_DMEM_L3L1_RD) ? (dmem_L1_addr_offset_1 + dmem_rd_addr_cntr) : (dmem_L1_addr_offset_1 + dmem_wr_addr_cntr);

	assign ext_dmem_we_0 = ((state_0 == CP_DMEM_L3L1_WR) && (s_axis_tvalid == 1'b1) && (s_axis_tready == 1'b1) && (dmem_wr_sub_cntr != 4'b0000)) ? 1'b1 : 1'b0;
	assign ext_dmem_we_1 = ((state_1 == CP_DMEM_L3L1_WR) && (s_axis_tvalid == 1'b1) && (s_axis_tready == 1'b1) && (dmem_wr_sub_cntr != 4'b0000)) ? 1'b1 : 1'b0;

	assign ext_dmem_in_0 =  (dmem_wr_sub_cntr == 4'b0001) ? {s_axis_tdata[(1*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:0*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0010) ? {s_axis_tdata[(2*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:1*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0011) ? {s_axis_tdata[(3*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:2*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0100) ? {s_axis_tdata[(4*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:3*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0101) ? {s_axis_tdata[(5*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:4*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0110) ? {s_axis_tdata[(6*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:5*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0111) ? {s_axis_tdata[(7*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:6*(MS_AXI_WIDTH/8)]} :
							{s_axis_tdata[(8*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:7*(MS_AXI_WIDTH/8)]};

	assign ext_dmem_in_1 =  (dmem_wr_sub_cntr == 4'b0001) ? {s_axis_tdata[(1*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:0*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0010) ? {s_axis_tdata[(2*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:1*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0011) ? {s_axis_tdata[(3*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:2*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0100) ? {s_axis_tdata[(4*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:3*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0101) ? {s_axis_tdata[(5*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:4*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0110) ? {s_axis_tdata[(6*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:5*(MS_AXI_WIDTH/8)]} :
							(dmem_wr_sub_cntr == 4'b0111) ? {s_axis_tdata[(7*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:6*(MS_AXI_WIDTH/8)]} :
							{s_axis_tdata[(8*(MS_AXI_WIDTH/8))-1:0], cached_s_axis_tdata[MS_AXI_WIDTH-1:7*(MS_AXI_WIDTH/8)]};

	assign ext_dmem_L1_selector_a = ((state_0 == DMA_SHARING) || (state_0 == CP_IMEM_LD) || (state_0 == CP_DMEM_L3L1_WR) || (state_0 == CP_DMEM_L3L1_RD)) ? 1'b1 : 1'b0;
	assign ext_dmem_L1_selector_b = ((state_1 == DMA_SHARING) || (state_1 == CP_IMEM_LD) || (state_1 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_RD)) ? 1'b1 : 1'b0;

	assign ext_dmem_L1_addr_a = (ext_dmem_L1_selector_a == 1'b1) ? ext_dmem_addr_0 : ext_dmem_addr_x;
	assign ext_dmem_L1_in_a = (ext_dmem_L1_selector_a == 1'b1) ? ext_dmem_in_0 : shared_dmem_out_0;
	assign ext_dmem_L1_we_a = (ext_dmem_L1_selector_a == 1'b1) ? ext_dmem_we_0 : ext_dmem_we_x;
	assign dmem_ext_out_0 = dmem_L1_ext_out_a;
	assign dmem_shared_in_0 = dmem_L1_ext_out_a;

	assign ext_dmem_L1_addr_b = (ext_dmem_L1_selector_b == 1'b1) ? ext_dmem_addr_1 : ext_dmem_addr_y;
	assign ext_dmem_L1_in_b = (ext_dmem_L1_selector_b == 1'b1) ? ext_dmem_in_1 : shared_dmem_out_1;
	assign ext_dmem_L1_we_b = (ext_dmem_L1_selector_b == 1'b1) ? ext_dmem_we_1 : ext_dmem_we_y;
	assign dmem_ext_out_1 = dmem_L1_ext_out_b;
	assign dmem_shared_in_1 = dmem_L1_ext_out_b;

	assign ext_dmem_addr_x = (state_0 == CP_DMEM_L1L2_WR) ? (dmem_L1_addr_offset_0 + dmem_L1L2_rd_addr_cntr_x) : (dmem_L1_addr_offset_0 + dmem_L1L2_rd_addr_cntr_x - 10'h1);
	assign dmem_shared_addr_0 = (state_0 == CP_DMEM_L1L2_WR) ? (dmem_L2_addr_offset_0 + dmem_L1L2_rd_addr_cntr_x - 10'h1) : (dmem_L2_addr_offset_0 + dmem_L1L2_rd_addr_cntr_x);
	assign dmem_shared_we_0 = ((state_0 == CP_DMEM_L1L2_WR) && (dmem_L1L2_rd_addr_cntr_x != 10'h0)) ? 1'b1 : 1'b0;
	assign ext_dmem_we_x = ((state_0 == CP_DMEM_L1L2_RD) && (dmem_L1L2_rd_addr_cntr_x != 10'h0)) ? 1'b1 : 1'b0;

	assign ext_dmem_addr_y = (state_1 == CP_DMEM_L1L2_WR) ? (dmem_L1_addr_offset_1 + dmem_L1L2_rd_addr_cntr_y) : (dmem_L1_addr_offset_1 + dmem_L1L2_rd_addr_cntr_y - 10'h1);
	assign dmem_shared_addr_1 = (state_1 == CP_DMEM_L1L2_WR) ? (dmem_L2_addr_offset_1 + dmem_L1L2_rd_addr_cntr_y - 10'h1) : (dmem_L2_addr_offset_1 + dmem_L1L2_rd_addr_cntr_y);
	assign dmem_shared_we_1 = ((state_1 == CP_DMEM_L1L2_WR) && (dmem_L1L2_rd_addr_cntr_y != 10'h0)) ? 1'b1 : 1'b0;
	assign ext_dmem_we_y = ((state_1 == CP_DMEM_L1L2_RD) && (dmem_L1L2_rd_addr_cntr_y != 10'h0)) ? 1'b1 : 1'b0;

	/////////////////////////////////////////////////////////////////
	// Sequential procedure /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				state_0 <= IDLE;
			end
		else
			begin
				state_0 <= next_state_0;
			end
	end

	//-----------------------------------------------------------
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				state_1 <= IDLE;
			end
		else
			begin
				state_1 <= next_state_1;
			end
	end

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(*)
	begin
		next_state_0 <= IDLE;

		case(state_0)
			//-----------------------------------------------------------
			IDLE:
				begin
					case (command_0[3:0])
						4'b0011:
							next_state_0 <= DMA_SHARING;

						4'b0101:
							next_state_0 <= DMA_SHARING;

						4'b0111:
							next_state_0 <= DMA_SHARING;

						4'b1001:
							next_state_0 <= L2_TRANSFERING;

						4'b1011:
							next_state_0 <= L2_TRANSFERING;

						4'b1101:
							next_state_0 <= CP_PROG_RUN;

						default:
							next_state_0 <= IDLE;
					endcase
				end
			
			//-----------------------------------------------------------
			DMA_SHARING:
				begin
					if ((state_1 != CP_IMEM_LD) && (state_1 != CP_DMEM_L3L1_WR) && (state_1 != CP_DMEM_L3L1_RD))
						begin
							case (command_0[3:0])
								4'b0011:
									next_state_0 <= CP_IMEM_LD;

								4'b0101:
									next_state_0 <= CP_DMEM_L3L1_WR;

								4'b0111:
									next_state_0 <= CP_DMEM_L3L1_RD;

								default:
									next_state_0 <= DMA_SHARING;
							endcase
						end
					else
						begin
							next_state_0 <= DMA_SHARING;
						end
				end

			//-----------------------------------------------------------
			CP_IMEM_LD:
				begin
					if ((imem_addr_cntr == IMEM_MAX_ADDR) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_0 <= CP_IMEM_LD;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L3L1_WR:
				begin
					if ((dmem_wr_len_cntr == command_0[11:4]) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_0 <= CP_DMEM_L3L1_WR;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L3L1_RD:
				begin
					if ((dmem_rd_len_cntr == command_0[11:4]) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_0 <= CP_DMEM_L3L1_RD;
						end
				end

			//-----------------------------------------------------------
			L2_TRANSFERING:
				begin
					if ((command_0[3:0] == 4'b1001) && (dmem_shared_tack_0 == 1'b1))
						begin
							next_state_0 <= CP_DMEM_L1L2_WR;
						end
					else if ((command_0[3:0] == 4'b1011) && (dmem_shared_tack_0 == 1'b1))
						begin
							next_state_0 <= CP_DMEM_L1L2_RD;
						end
					else
						begin
							next_state_0 <= L2_TRANSFERING;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L1L2_WR:
				begin
					if (dmem_L1L2_rd_addr_cntr_x == {2'b00, command_0[11:4]})
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_0 <= CP_DMEM_L1L2_WR;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L1L2_RD:
				begin
					if (dmem_L1L2_rd_addr_cntr_x == {2'b00, command_0[11:4]})
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_0 <= CP_DMEM_L1L2_RD;
						end
				end

			//-----------------------------------------------------------
			CP_PROG_RUN:
				begin
					if (cp_ext_nbusy_0 && cp_ext_done_0)
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_0 <= CP_PROG_RUN;
						end
				end

			//-----------------------------------------------------------
			WAIT_CMD_ZERO:
				begin
					if (command_0 == 22'h0)
						begin
							next_state_0 <= IDLE;
						end
					else
						begin
							next_state_0 <= WAIT_CMD_ZERO;
						end
				end       

			//-----------------------------------------------------------
			default:
				begin
					next_state_0 <= IDLE;
				end
		endcase
	end

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	always @(*)
	begin
		next_state_1 <= IDLE;

		case(state_1)
			//-----------------------------------------------------------
			IDLE:
				begin
					case (command_1[3:0])
						4'b0011:
							next_state_1 <= DMA_SHARING;

						4'b0101:
							next_state_1 <= DMA_SHARING;

						4'b0111:
							next_state_1 <= DMA_SHARING;

						4'b1001:
							next_state_1 <= L2_TRANSFERING;

						4'b1011:
							next_state_1 <= L2_TRANSFERING;

						4'b1101:
							next_state_1 <= CP_PROG_RUN;

						default:
							next_state_1 <= IDLE;
					endcase
				end
			
			//-----------------------------------------------------------
			DMA_SHARING:
				begin
					if (state_0 == DMA_SHARING)
						begin
							next_state_1 <= DMA_SHARING;
						end
					else if ((state_0 != CP_IMEM_LD) && (state_0 != CP_DMEM_L3L1_WR) && (state_0 != CP_DMEM_L3L1_RD))
						begin
							case (command_1[3:0])
								4'b0011:
									next_state_1 <= CP_IMEM_LD;

								4'b0101:
									next_state_1 <= CP_DMEM_L3L1_WR;

								4'b0111:
									next_state_1 <= CP_DMEM_L3L1_RD;

								default:
									next_state_1 <= DMA_SHARING;
							endcase
						end
					else
						begin
							next_state_1 <= DMA_SHARING;
						end
				end

			//-----------------------------------------------------------
			CP_IMEM_LD:
				begin
					if ((imem_addr_cntr == IMEM_MAX_ADDR) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_1 <= CP_IMEM_LD;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L3L1_WR:
				begin
					if ((dmem_wr_len_cntr == command_1[11:4]) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_1 <= CP_DMEM_L3L1_WR;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L3L1_RD:
				begin
					if ((dmem_rd_len_cntr == command_1[11:4]) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_1 <= CP_DMEM_L3L1_RD;
						end
				end

			//-----------------------------------------------------------
			L2_TRANSFERING:
				begin
					if ((command_1[3:0] == 4'b1001) && (dmem_shared_tack_1 == 1'b1))
						begin
							next_state_1 <= CP_DMEM_L1L2_WR;
						end
					else if ((command_1[3:0] == 4'b1011) && (dmem_shared_tack_1 == 1'b1))
						begin
							next_state_1 <= CP_DMEM_L1L2_RD;
						end
					else
						begin
							next_state_1 <= L2_TRANSFERING;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L1L2_WR:
				begin
					if (dmem_L1L2_rd_addr_cntr_y == {2'b00, command_1[11:4]})
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_1 <= CP_DMEM_L1L2_WR;
						end
				end

			//-----------------------------------------------------------
			CP_DMEM_L1L2_RD:
				begin
					if (dmem_L1L2_rd_addr_cntr_y == {2'b00, command_1[11:4]})
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_1 <= CP_DMEM_L1L2_RD;
						end
				end

			//-----------------------------------------------------------
			CP_PROG_RUN:
				begin
					if (cp_ext_nbusy_1 && cp_ext_done_1)
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state_1 <= CP_PROG_RUN;
						end
				end

			//-----------------------------------------------------------
			WAIT_CMD_ZERO:
				begin
					if (command_1 == 22'h0)
						begin
							next_state_1 <= IDLE;
						end
					else
						begin
							next_state_1 <= WAIT_CMD_ZERO;
						end
				end       

			//-----------------------------------------------------------
			default:
				begin
					next_state_1 <= IDLE;
				end
		endcase
	end

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				imem_addr_cntr <= 9'h0;
			end
		else
			begin
				if ((state_0 == CP_IMEM_LD) || (state_1 == CP_IMEM_LD))
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								imem_addr_cntr <= imem_addr_cntr + 9'h1;
							end
						else
							begin
								imem_addr_cntr <= imem_addr_cntr;
							end
					end
				else
					begin
						imem_addr_cntr <= 9'h0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				dmem_wr_len_cntr <= 8'h1;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_WR))
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								dmem_wr_len_cntr <= dmem_wr_len_cntr + 8'h1;
							end
						else
							begin
								dmem_wr_len_cntr <= dmem_wr_len_cntr;
							end
					end
				else
					begin
						dmem_wr_len_cntr <= 8'h1;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				dmem_wr_addr_cntr <= 0;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_WR))
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								if (dmem_wr_sub_cntr == 4'b0000)
									begin
										dmem_wr_addr_cntr <= dmem_wr_addr_cntr;
									end
								else
									begin
										dmem_wr_addr_cntr <= dmem_wr_addr_cntr + 10'h1;
									end
							end
						else
							begin
								dmem_wr_addr_cntr <= dmem_wr_addr_cntr;
							end
					end
				else
					begin
						dmem_wr_addr_cntr <= 0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				dmem_wr_sub_cntr <= 4'h0;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_WR))
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								if (dmem_wr_sub_cntr == 4'b1000)
									begin
										dmem_wr_sub_cntr <= 4'h0;
									end
								else
									begin
										dmem_wr_sub_cntr <= dmem_wr_sub_cntr + 4'h1;
									end
							end
						else
							begin
								dmem_wr_sub_cntr <= dmem_wr_sub_cntr;
							end
					end
				else
					begin
						dmem_wr_sub_cntr <= 4'h0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				cached_s_axis_tdata <= 0;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_WR) || (state_1 == CP_DMEM_L3L1_WR))
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								cached_s_axis_tdata <= s_axis_tdata;
							end
						else
							begin
								cached_s_axis_tdata <= cached_s_axis_tdata;
							end
					end
				else
					begin
						cached_s_axis_tdata <= 0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				m_axis_tvalid <= 1'b0;
			end
		else
			begin
				if (state_0 == CP_DMEM_L3L1_RD)
					begin
						if ((dmem_rd_len_cntr == command_0[11:4]) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								m_axis_tvalid <= 1'b0;
							end
						else
							begin
								m_axis_tvalid <= 1'b1;
							end
					end
				else if (state_1 == CP_DMEM_L3L1_RD)
					begin
						if ((dmem_rd_len_cntr == command_1[11:4]) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								m_axis_tvalid <= 1'b0;
							end
						else
							begin
								m_axis_tvalid <= 1'b1;
							end
					end
				else
					begin
						m_axis_tvalid <= 1'b0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				m_axis_tlast <= 1'b0;
			end
		else
			begin
				if (state_0 == CP_DMEM_L3L1_RD)
					begin
						if ((dmem_rd_len_cntr == (command_0[11:4] - 8'h1)) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								m_axis_tlast <= 1'b1;
							end
						else
							begin
								m_axis_tlast <= 1'b0;
							end
					end
				else if (state_1 == CP_DMEM_L3L1_RD)
					begin
						if ((dmem_rd_len_cntr == (command_1[11:4] - 8'h1)) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								m_axis_tlast <= 1'b1;
							end
						else
							begin
								m_axis_tlast <= 1'b0;
							end
					end
				else
					begin
						m_axis_tlast <= 1'b0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				dmem_rd_len_cntr <= 8'h1;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_RD) || (state_1 == CP_DMEM_L3L1_RD))
					begin
						if ((m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								dmem_rd_len_cntr <= dmem_rd_len_cntr + 8'h1;
							end
						else
							begin
								dmem_rd_len_cntr <= dmem_rd_len_cntr;
							end
					end
				else
					begin
						dmem_rd_len_cntr <= 8'h1;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				dmem_rd_addr_cntr <= 0;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_RD) || (state_1 == CP_DMEM_L3L1_RD))
					begin
						if (m_axis_tready == 1'b1)
							begin
								if (dmem_rd_sub_cntr == 4'b0111)
									begin
										dmem_rd_addr_cntr <= dmem_rd_addr_cntr;
									end
								else
									begin
										dmem_rd_addr_cntr <= dmem_rd_addr_cntr + 10'h1;
									end
							end
						else
							begin
								dmem_rd_addr_cntr <= dmem_rd_addr_cntr;
							end
					end
				else
					begin
						dmem_rd_addr_cntr <= 0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				dmem_rd_sub_cntr <= 4'h0;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L3L1_RD) || (state_1 == CP_DMEM_L3L1_RD))
					begin
						if ((m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								if (dmem_rd_sub_cntr == 4'b1000)
									begin
										dmem_rd_sub_cntr <= 4'h0;
									end
								else
									begin
										dmem_rd_sub_cntr <= dmem_rd_sub_cntr + 4'h1;
									end
							end
						else
							begin
								dmem_rd_sub_cntr <= dmem_rd_sub_cntr;
							end
					end
				else
					begin
						dmem_rd_sub_cntr <= 4'h0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				cached_dmem_ext_out <= 0;
			end
		else
			begin
				if (state_0 == CP_DMEM_L3L1_RD)
					begin
						if ((m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								cached_dmem_ext_out <= dmem_ext_out_0[(9*(CP_D_WIDTH/9))-1:1*(CP_D_WIDTH/9)];
							end
						else
							begin
								cached_dmem_ext_out <= cached_dmem_ext_out;
							end
					end
				else if (state_1 == CP_DMEM_L3L1_RD)
					begin
						if ((m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								cached_dmem_ext_out <= dmem_ext_out_1[(9*(CP_D_WIDTH/9))-1:1*(CP_D_WIDTH/9)];
							end
						else
							begin
								cached_dmem_ext_out <= cached_dmem_ext_out;
							end
					end
				else
					begin
						cached_dmem_ext_out <= 0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				dmem_L1L2_rd_addr_cntr_x <= 0;
			end
		else
			begin
				if ((state_0 == CP_DMEM_L1L2_WR) || (state_0 == CP_DMEM_L1L2_RD))
					begin
						dmem_L1L2_rd_addr_cntr_x <= dmem_L1L2_rd_addr_cntr_x + 10'h1;
					end
				else
					begin
						dmem_L1L2_rd_addr_cntr_x <= 0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				dmem_L1L2_rd_addr_cntr_y <= 0;
			end
		else
			begin
				if ((state_1 == CP_DMEM_L1L2_WR) || (state_1 == CP_DMEM_L1L2_RD))
					begin
						dmem_L1L2_rd_addr_cntr_y <= dmem_L1L2_rd_addr_cntr_y + 10'h1;
					end
				else
					begin
						dmem_L1L2_rd_addr_cntr_y <= 0;
					end
			end
	end

	// Instantiation ////////////////////////////////////////////////
	cp_top
	#(
		.CP_D_WIDTH(CP_D_WIDTH),
		.CP_I_WIDTH(CP_I_WIDTH),
		.DMEM_ADDR_WIDTH(DMEM_ADDR_WIDTH),
		.IMEM_ADDR_WIDTH(IMEM_ADDR_WIDTH)
	 )
	cp_core_0
	(
		.nreset(s_axis_aresetn),
		.clock(s_axis_aclk),

		.ext_cp_active(ext_cp_active_0),
		.cp_ext_nbusy(cp_ext_nbusy_0),
		.cp_ext_done(cp_ext_done_0),

		.ext_imem_addr(ext_imem_addr_0),
		.ext_imem_in(ext_imem_in_0),
		.ext_imem_we(ext_imem_we_0),

		.ext_dmem_addr(ext_dmem_L1_addr_a),
		.ext_dmem_in(ext_dmem_L1_in_a),
		.ext_dmem_we(ext_dmem_L1_we_a),
		.dmem_ext_out(dmem_L1_ext_out_a)
	);

	/////////////////////////////////////////////////////////////////
	cp_top
	#(
		.CP_D_WIDTH(CP_D_WIDTH),
		.CP_I_WIDTH(CP_I_WIDTH),
		.DMEM_ADDR_WIDTH(DMEM_ADDR_WIDTH),
		.IMEM_ADDR_WIDTH(IMEM_ADDR_WIDTH)
	 )
	cp_core_1
	(
		.nreset(s_axis_aresetn),
		.clock(s_axis_aclk),

		.ext_cp_active(ext_cp_active_1),
		.cp_ext_nbusy(cp_ext_nbusy_1),
		.cp_ext_done(cp_ext_done_1),

		.ext_imem_addr(ext_imem_addr_1),
		.ext_imem_in(ext_imem_in_1),
		.ext_imem_we(ext_imem_we_1),

		.ext_dmem_addr(ext_dmem_L1_addr_b),
		.ext_dmem_in(ext_dmem_L1_in_b),
		.ext_dmem_we(ext_dmem_L1_we_b),
		.dmem_ext_out(dmem_L1_ext_out_b)
	);

////////////////////////////////////////////////////////////////////////
endmodule
