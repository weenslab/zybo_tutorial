`timescale 1ns / 1ps

module fibonacci_tb();
    localparam T = 10;
    
    reg clk;
    reg rst_n;
    reg start;
    reg [4:0] i;
    wire ready;
    wire done_tick;
    wire [19:0] f;

    fibonacci dut
    (
        .clk(clk),
        .rst_n(rst_n),
        .start(start),
        .i(i),
        .ready(ready),
        .done_tick(done_tick),
        .f(f)
    );
    
    always
    begin
        clk = 0;
        #(T/2);
        clk = 1;
        #(T/2);
    end
    
    initial
    begin
        start = 0;
        i = 0;
    
        rst_n = 0;
        #T;
        rst_n = 1;
        #T;
        
        // Calculate fib(10)
        start = 1;
        i = 10;
        #T;     
    end
    
endmodule
