module adder_2bit
    (
        input wire [1:0]  a,
        input wire [1:0]  b,
        output wire [2:0] y
    );
    
    wire sum_0, sum_1;
    wire carry_0, carry_1;
    
    full_adder full_adder_0
    (
        .a(a[0]),
        .b(b[0]),
        .c(0),
        .sum(sum_0),
        .carry(carry_0) 
    );
    
    full_adder full_adder_1
    (
        .a(a[1]),
        .b(b[1]),
        .c(carry_0),
        .sum(sum_1),
        .carry(carry_1) 
    );
    
    assign y = {carry_1, sum_1, sum_0};
    
endmodule
