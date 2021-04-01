`timescale 1ns/1ps
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
module pcp_ip_v1_0
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

		command,
		status
	);

	// Ports ////////////////////////////////////////////////////////
	// Slave_AXI_Stream port
	input			s_axis_aresetn;
	input			s_axis_aclk;
	input			s_axis_tvalid;
	output			s_axis_tready;
	input	[63:0]	s_axis_tdata;
	input	[7:0]	s_axis_tkeep;
	input			s_axis_tlast;

	// Master_AXI_Stream port
	input			m_axis_aresetn;
	input			m_axis_aclk;
	output			m_axis_tvalid;
	input			m_axis_tready;
	output	[63:0]	m_axis_tdata;
	output	[7:0]	m_axis_tkeep;
	output			m_axis_tlast;

	input	[31:0]	command;
	output	[31:0]	status;

	// Local parameters /////////////////////////////////////////////	
	localparam		IDLE		  = 3'b000;
    localparam		PCP_IMEM_LD	  = 3'b001;
    localparam		PCP_DMEM_WR	  = 3'b010;
	localparam		PCP_DMEM_RD	  = 3'b011;
	localparam		PCP_PROG_RUN  = 3'b100;
    localparam		WAIT_CMD_ZERO = 3'b101;

	// Wires ///////////////////////////////////////////////
	wire			ext_pcp_active;
	wire			pcp_ext_nbusy;
	wire			pcp_ext_done;

	wire	[9:0]	ext_imem_addr;
	wire			ext_imem_we;
	wire	[74:0]	ext_imem_in;

	wire	[9:0]	ext_dmem_addr;
	wire	[255:0]	ext_dmem_in;
	wire			ext_dmem_we;
	wire	[255:0]	dmem_ext_out;

	// Registers ///////////////////////////////////////////
	reg				m_axis_tvalid;
	reg				m_axis_tlast;

	reg		[2:0]	state;
	reg		[2:0]	next_state;

	reg		[10:0]	imem_addr_cntr;
	reg		[10:0]	cached_s_axis_tdata;

	reg		[1:0]	dmem_wr_len_cntr;
	reg		[63:0]	cached_wr_dmem_data0;
	reg		[63:0]	cached_wr_dmem_data1;
	reg		[63:0]	cached_wr_dmem_data2;

	reg		[1:0]	dmem_rd_len_cntr;

	// Assignments //////////////////////////////////////////////////
	assign status = (state == WAIT_CMD_ZERO) ? 32'h00000001 : 32'h00000000;

	assign ext_pcp_active = (state == PCP_PROG_RUN) ? 1'b1 : 1'b0;

	assign s_axis_tready = ((state == PCP_IMEM_LD) && (s_axis_tvalid == 1'b1)) ? 1'b1 :
						   ((state == PCP_DMEM_WR) && (s_axis_tvalid == 1'b1)) ? 1'b1 : 1'b0;
						   
	assign m_axis_tkeep = s_axis_tkeep;

	assign ext_imem_addr = imem_addr_cntr[10:1];
	assign ext_imem_we = ((state == PCP_IMEM_LD) && (s_axis_tvalid == 1'b1) && (s_axis_tready == 1'b1) && (imem_addr_cntr[0:0] == 1'b1)) ? 1'b1 : 1'b0;
	assign ext_imem_in = {cached_s_axis_tdata, {s_axis_tdata[7:0], s_axis_tdata[15:8], s_axis_tdata[23:16], s_axis_tdata[31:24],
													  s_axis_tdata[39:32], s_axis_tdata[47:40], s_axis_tdata[55:48], s_axis_tdata[63:56]}};

	assign ext_dmem_addr = command[13:4];
	assign ext_dmem_we = ((state == PCP_DMEM_WR) && (s_axis_tvalid == 1'b1) && (s_axis_tready == 1'b1) && (dmem_wr_len_cntr == 2'h3)) ? 1'b1 : 1'b0;	
	assign ext_dmem_in = {cached_wr_dmem_data0, cached_wr_dmem_data1, cached_wr_dmem_data2, {s_axis_tdata[7:0], s_axis_tdata[15:8], s_axis_tdata[23:16], s_axis_tdata[31:24],
																							 s_axis_tdata[39:32], s_axis_tdata[47:40], s_axis_tdata[55:48], s_axis_tdata[63:56]}};

	assign m_axis_tdata =
	(dmem_rd_len_cntr == 2'h0) ? {dmem_ext_out[199:192], dmem_ext_out[207:200], dmem_ext_out[215:208], dmem_ext_out[223:216],
								  dmem_ext_out[231:224], dmem_ext_out[239:232], dmem_ext_out[247:240], dmem_ext_out[255:248]} :
	(dmem_rd_len_cntr == 2'h1) ? {dmem_ext_out[135:128], dmem_ext_out[143:136], dmem_ext_out[151:144], dmem_ext_out[159:152],
								  dmem_ext_out[167:160], dmem_ext_out[175:168], dmem_ext_out[183:176], dmem_ext_out[191:184]} :
	(dmem_rd_len_cntr == 2'h2) ? {dmem_ext_out[71:64], dmem_ext_out[79:72], dmem_ext_out[87:80], dmem_ext_out[95:88],
								  dmem_ext_out[103:96], dmem_ext_out[111:104], dmem_ext_out[119:112], dmem_ext_out[127:120]} :
								 {dmem_ext_out[7:0], dmem_ext_out[15:8], dmem_ext_out[23:16], dmem_ext_out[31:24],
								  dmem_ext_out[39:32], dmem_ext_out[47:40], dmem_ext_out[55:48], dmem_ext_out[63:56]};

	/////////////////////////////////////////////////////////////////
	// Sequential procedure /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
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
					case (command[3:0])
						4'b0011:
							next_state <= PCP_IMEM_LD;

						4'b0101:
							next_state <= PCP_DMEM_WR;

						4'b0111:
							next_state <= PCP_DMEM_RD;

						4'b1001:
							next_state <= PCP_PROG_RUN;

						default:
							next_state <= IDLE;
					endcase
				end

			//-----------------------------------------------------------
			PCP_IMEM_LD:
				begin
					if ((imem_addr_cntr == 11'h7FF) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
						begin
							next_state <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state <= PCP_IMEM_LD;
						end
				end

			//-----------------------------------------------------------
			PCP_DMEM_WR:
				begin
					if ((dmem_wr_len_cntr == 2'h3) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
						begin
							next_state <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state <= PCP_DMEM_WR;
						end
				end

			//-----------------------------------------------------------
			PCP_DMEM_RD:
				begin
					if ((dmem_rd_len_cntr == 2'h3) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
						begin
							next_state <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state <= PCP_DMEM_RD;
						end
				end

			//-----------------------------------------------------------
			PCP_PROG_RUN:
				begin
					if (pcp_ext_nbusy && pcp_ext_done)
						begin
							next_state <= WAIT_CMD_ZERO;
						end
					else
						begin
							next_state <= PCP_PROG_RUN;
						end
				end

			//-----------------------------------------------------------
			WAIT_CMD_ZERO:
				begin
					if (command == 32'h0)
						begin
							next_state <= IDLE;
						end
					else
						begin
							next_state <= WAIT_CMD_ZERO;
						end
				end       

			//-----------------------------------------------------------
			default:
				begin
					next_state <= IDLE;
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
				imem_addr_cntr <= 11'h0;
			end
		else
			begin
				if (state == PCP_IMEM_LD)
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								imem_addr_cntr <= imem_addr_cntr + 11'h1;
							end
						else
							begin
								imem_addr_cntr <= imem_addr_cntr;
							end
					end
				else
					begin
						imem_addr_cntr <= 11'h0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				cached_s_axis_tdata <= 32'h0;
			end
		else
			begin
				if ((state == PCP_IMEM_LD) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1) && (imem_addr_cntr[0:0] == 1'b0))
					begin
						cached_s_axis_tdata <= {s_axis_tdata[50:48], s_axis_tdata[63:56]};
					end
				else
					begin
						cached_s_axis_tdata <= cached_s_axis_tdata;
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
				dmem_wr_len_cntr <= 2'h0;
			end
		else
			begin
				if (state == PCP_DMEM_WR)
					begin
						if ((s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1))
							begin
								dmem_wr_len_cntr <= dmem_wr_len_cntr + 2'h1;
							end
						else
							begin
								dmem_wr_len_cntr <= dmem_wr_len_cntr;
							end
					end
				else
					begin
						dmem_wr_len_cntr <= 2'h0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge s_axis_aclk)
	begin
		if (!s_axis_aresetn)
			begin
				cached_wr_dmem_data0 <= 64'h0;
				cached_wr_dmem_data1 <= 64'h0;
				cached_wr_dmem_data2 <= 64'h0;
			end
		else
			begin
				if ((state == PCP_DMEM_WR) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1) && (dmem_wr_len_cntr == 2'h0))
					begin
						cached_wr_dmem_data0 <= {s_axis_tdata[7:0], s_axis_tdata[15:8], s_axis_tdata[23:16], s_axis_tdata[31:24],
												s_axis_tdata[39:32], s_axis_tdata[47:40], s_axis_tdata[55:48], s_axis_tdata[63:56]};
					end
				else if ((state == PCP_DMEM_WR) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1) && (dmem_wr_len_cntr == 2'h1))
					begin
						cached_wr_dmem_data1 <= {s_axis_tdata[7:0], s_axis_tdata[15:8], s_axis_tdata[23:16], s_axis_tdata[31:24],
												s_axis_tdata[39:32], s_axis_tdata[47:40], s_axis_tdata[55:48], s_axis_tdata[63:56]};
					end
				else if ((state == PCP_DMEM_WR) && (s_axis_tready == 1'b1) && (s_axis_tvalid == 1'b1) && (dmem_wr_len_cntr == 2'h2))
					begin
						cached_wr_dmem_data2 <= {s_axis_tdata[7:0], s_axis_tdata[15:8], s_axis_tdata[23:16], s_axis_tdata[31:24],
												s_axis_tdata[39:32], s_axis_tdata[47:40], s_axis_tdata[55:48], s_axis_tdata[63:56]};
					end
				else
					begin
						cached_wr_dmem_data0 <= cached_wr_dmem_data0;
						cached_wr_dmem_data1 <= cached_wr_dmem_data1;
						cached_wr_dmem_data2 <= cached_wr_dmem_data2;
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
				dmem_rd_len_cntr <= 2'h0;
			end
		else
			begin
				if (state == PCP_DMEM_RD)
					begin
						if ((m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
							begin
								dmem_rd_len_cntr <= dmem_rd_len_cntr + 2'h1;
							end
						else
							begin
								dmem_rd_len_cntr <= dmem_rd_len_cntr;
							end
					end
				else
					begin
						dmem_rd_len_cntr <= 2'h0;
					end
			end
	end

	/////////////////////////////////////////////////////////////////
	always @(posedge m_axis_aclk)
	begin
		if (!m_axis_aresetn)
			begin
				m_axis_tvalid <= 1'b0;
			end
		else
			begin
				if (state == PCP_DMEM_RD)
					begin
						if ((m_axis_tready == 1'b1) && (dmem_rd_len_cntr == 2'h3))
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
				if (state == PCP_DMEM_RD)
					begin
						if ((dmem_rd_len_cntr == 2'h2) && (m_axis_tready == 1'b1) && (m_axis_tvalid == 1'b1))
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

	// Instantiation ////////////////////////////////////////////////
	pcp_top pcp_top_u
	(
		.nreset(s_axis_aresetn),
		.clock(s_axis_aclk),

		.ext_pcp_active(ext_pcp_active),
		.ext_pcp_imem_offset(command[17:14]),
		.pcp_ext_nbusy(pcp_ext_nbusy),
		.pcp_ext_done(pcp_ext_done),

		.ext_imem_addr(ext_imem_addr),
		.ext_imem_in(ext_imem_in),
		.ext_imem_we(ext_imem_we),

		.ext_dmem_addr(ext_dmem_addr),
		.ext_dmem_in(ext_dmem_in),
		.ext_dmem_we(ext_dmem_we),
		.dmem_ext_out(dmem_ext_out)
	);

////////////////////////////////////////////////////////////////////////
endmodule
