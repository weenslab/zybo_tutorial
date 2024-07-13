`timescale 1ns / 1ps

module adder_2bit_tb();
    localparam T = 10;
    
    reg [1:0] a;
    reg [1:0] b;
    wire [2:0] y;
    
    adder_2bit adder_2bit_0
    (
        .a(a),
        .b(b),
        .y(y)
    );
    
    initial
    begin
        a = 0; b = 0;
        #T;
        a = 0; b = 1;
        #T;
        a = 0; b = 2;
        #T;
        a = 0; b = 3;
        #T;
        a = 1; b = 0;
        #T;
        a = 1; b = 1;
        #T;
        a = 1; b = 2;
        #T;
        a = 1; b = 3;
        #T;
        a = 2; b = 0;
        #T;
        a = 2; b = 1;
        #T;
        a = 2; b = 2;
        #T;
        a = 2; b = 3;
        #T;  
        a = 3; b = 0;
        #T;
        a = 3; b = 1;
        #T;
        a = 3; b = 2;
        #T;
        a = 3; b = 3;
        #T;            
    end
    
endmodule