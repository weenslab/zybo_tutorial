`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Kyushu Institute of Technology
// Engineer: DSP Lab
// 
// Create Date:    10:43:18 10/11/2017 
// Design Name:    Neural Network (using backpropagation)
// Module Name:    w3_22
// Project Name:   LSI Design Contest in Okinawa 2018
// Target Devices: 
// Tool versions: 
//
// Description: 
//		Calculation of w3_22, when the select initial signal is active, 
// 	    the output will be the initial value of w3_22, and when the select update 
//		signal is active, the output will be the new value of w3_22
//	Input: 
//		clk	    : 1 bit
//		reset	: 1 bit : high active
//		dw3_22	: 16 bits 00_0000.0000_0000_00 signed	 : delta weight3_22
//		select_initial	: 1 bit	: high active
//		select_update	: 1 bit	: high active
//	Output:
//		w3_22	: 16 bits 00_0000.0000_0000_00 signed : weight3_22
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module w3_22(clk, reset, dw3_22, select_initial, select_update, w3_22);
	
	input clk, reset;
	input select_initial, select_update;
	input signed [15:0] dw3_22;
	output signed [15:0] w3_22;
	
	reg signed [15:0] zero = 16'b0;
	reg signed [15:0] w3_22;
	reg signed [15:0] init_w3_22 = 16'b00_0000_1000_0000_00;   //initial w3_22 = 0,5
	reg signed [15:0] net1, net2;
		
	always @(posedge clk)
		begin
			if(reset==1)
				begin
					net1=0;
					net2=0;
					w3_22=0;
				end
			else
				begin
					net1 = select_update? dw3_22 : zero;
					net2 = w3_22 + net1;
					w3_22 = select_initial? init_w3_22 : net2;
				end
		end
endmodule
