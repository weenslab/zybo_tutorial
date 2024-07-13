module fibonacci
    (
        input wire         clk,
        input wire         rst_n,
        input wire         start,
        input wire [4:0]   i,
        output wire        ready,
        output reg         done_tick,
        output wire [19:0] f
    );
    
    // Symbolic state declaration
    localparam [1:0] IDLE = 2'b00,
                     OP   = 2'b01,
                     DONE = 2'b10;
    
    // Signal declaration
    reg start_reg;
    wire start_tick;
    reg [1:0] state_reg, state_next;
    reg [19:0] t0_reg, t0_next;
    reg [19:0] t1_reg, t1_next;
    reg [19:0] n_reg, n_next;

    // Rising edge detector
    always @(posedge clk)
    begin
        if (!rst_n)
            start_reg <= 0;
        else
            start_reg <= start;
    end
    assign start_tick = start & ~start_reg; 
    
    // FSMD state and data registers
    always @(posedge clk)
    begin
        if (!rst_n)
        begin
            state_reg <= IDLE;
            t0_reg <= 0;
            t1_reg <= 0;
            n_reg <= 0;
        end
        else
        begin
            state_reg <= state_next;
            t0_reg <= t0_next;
            t1_reg <= t1_next;
            n_reg <= n_next;  
        end
    end
    
    // FSMD next state logic
    always @(*)
    begin
        state_next = state_reg;
        t0_next = t0_reg;
        t1_next = t1_reg;
        n_next = n_reg;      
        done_tick = 1'b0;
        case (state_reg)
            IDLE:
            begin
                if (start_tick)
                begin
                    t0_next = 0;
                    t1_next = 20'd1;
                    n_next = i;
                    state_next = OP;
                end
            end
            OP:
            begin
                if (n_reg == 0)
                begin
                    t1_next = 0;
                    state_next = DONE;
                end
                else if (n_reg == 1)
                begin
                    state_next = DONE;
                end
                else
                begin
                    t0_next = t1_reg;
                    t1_next = t0_reg + t1_reg;
                    n_next = n_reg - 1;                    
                end
            end
            DONE:
            begin
                done_tick = 1'b1;
                state_next = IDLE;
            end
        endcase
    end
    
    // Output
    assign f = t1_reg;
    assign ready = (state_reg == 0) ? 1 : 0;
    
endmodule
