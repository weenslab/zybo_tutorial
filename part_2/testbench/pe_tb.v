`timescale 1ns / 1ps

module pe_tb();
    localparam T = 10;

    parameter WIDTH = 16;
    parameter FRAC_BIT = 10;
    
    reg signed [WIDTH-1:0] a_in;
    reg signed [WIDTH-1:0] y_in;
    reg signed [WIDTH-1:0] b;
    wire signed [WIDTH-1:0] a_out;
    wire signed [WIDTH-1:0] y_out;
    
    pe
    #( 
        .WIDTH(WIDTH),
        .FRAC_BIT(FRAC_BIT)
    )
    dut
    (
        .a_in(a_in),
        .y_in(y_in),
        .b(b),
        .a_out(a_out),
        .y_out(y_out)
    );
    
    initial
    begin
        a_in = 0;
        y_in = 0;
        b = 0;
        #T;
        a_in = 16'b0_00000_1000000000;
        y_in = 16'b0_00000_1000000000;
        b = 16'b0_00001_0000000000;
        #T; 
        a_in = 16'b0_00000_1010100001;
        y_in = 16'b1_11110_1110000110;
        b = 16'b0_00101_1010111100;
        #T;
        a_in = 0;
        y_in = 0;
        b = 0;
        #T;
    end
    
endmodule
