`timescale 1 ns/10 ps

module gpioemu_tb;

    // Input registers
    reg n_reset = 1;
    reg [15:0] saddress = 0;
    reg srd = 0;
    reg swr = 0;
    reg [31:0] sdata_in = 0;
    reg [31:0] gpio_in = 0;
    reg gpio_latch = 0;
    reg clk = 0;
    integer i;

    // Outputs
    output [31:0] gpio_in_s_insp;
    output [31:0] gpio_out;
    
    // Initialize the VCD file for simulation
    initial begin
        $dumpfile("gpioemu.vcd");
        $dumpvars(0, gpioemu_tb);
        clk = 0;
    end
    
    // Generate clock signal
    initial begin 
        for (i = 0; i < 2500000; i = i + 1)
            #2 clk = ~clk;
    end

    // Main test sequence
    initial begin
        clk = 0; // Initial clock state
        #10;
        
        n_reset = 0; // Release reset
        #10;
        n_reset = 1;
        #10;
        n_reset = 0;

        // ---------------------------------------
        // Request the 5th prime number
        #100;
        saddress = 16'h100; // Address for input A
        swr = 1; // Write signal high
        sdata_in = 5; // Request the 5th prime number
        #100;
        swr = 0; // Write signal low
        #100;

        // Read the output
        saddress = 16'h110; // Address for output
        srd = 1; // Read signal high
        #100;
        srd = 0; // Read signal low
        #1000;
        // ---------------------------------------
        
        // Request the 10th prime number
        saddress = 16'h100; // Address for input A
        swr = 1; // Write signal high
        sdata_in = 10; // Request the 10th prime number
        #100;
        swr = 0; // Write signal low
        #200;

        // Read the output
        saddress = 16'h110; // Address for output
        srd = 1; // Read signal high
        #100;
        srd = 0; // Read signal low
        #1000;
        // ---------------------------------------
        
        // Request the 25th prime number
        saddress = 16'h100; // Address for input A
        swr = 1; // Write signal high
        sdata_in = 25; // Request the 25th prime number
        #100;
        swr = 0; // Write signal low
        #600;

        // Read the output
        saddress = 16'h110; // Address for output
        srd = 1; // Read signal high
        #100;
        srd = 0; // Read signal low
        #1000;
        // ---------------------------------------
        
        // Request the 60th prime number
        saddress = 16'h100; // Address for input A
        swr = 1; // Write signal high
        sdata_in = 60; // Request the 60th prime number
        #100;
        swr = 0; // Write signal low
        #1200;

        // Read the output
        saddress = 16'h110; // Address for output
        srd = 1; // Read signal high
        #100;
        srd = 0; // Read signal low
        #1000;
        // ---------------------------------------
        
        // Request the 100th prime number
        saddress = 16'h100; // Address for input A
        swr = 1; // Write signal high
        sdata_in = 100; // Request the 100th prime number
        #100;
        swr = 0; // Write signal low
        #2400;

        // Read the output
        saddress = 16'h110; // Address for output
        srd = 1; // Read signal high
        #100;
        srd = 0; // Read signal low
        #1000;
        // ---------------------------------------
        
        // Request the 450th prime number
        saddress = 16'h100; // Address for input A
        swr = 1; // Write signal high
        sdata_in = 450; // Request the 450th prime number
        #100;
        swr = 0; // Write signal low
        #13500;

        // Read the output
        saddress = 16'h110; // Address for output
        srd = 1; // Read signal high
        #100;
        srd = 0; // Read signal low
        #5000;
        
        $finish;
    end
    
    // Testbench wires
    wire [31:0] gpio_out_test;
    wire [31:0] sdata_out_test;
    wire [31:0] gpio_in_s_insp_test;
    wire [63:0] is_prime;
    wire [63:0] comparison_value;
    
    // Instantiate the DUT (Device Under Test)
   gpioemu test(n_reset, saddress, srd, swr, sdata_in, sdata_out_test, gpio_in, gpio_latch, gpio_out, clk, gpio_in_s_insp);

endmodule
