/* verilator lint_off UNUSED */ 
/* verilator lint_off UNDRIVEN */ 
/* verilator lint_off MULTIDRIVEN */ 
/* verilator lint_off COMBDLY */
/* verilator lint_off WIDTH */
/* verilator lint_off BLKSEQ */

module gpioemu(
    input           n_reset,
    input [15:0]    saddress,
    input           srd,
    input           swr,
    input [31:0]    sdata_in,
    output [31:0]   sdata_out,
    input [31:0]    gpio_in,
    input           gpio_latch,
    output [31:0]   gpio_out,
    input           clk,
    output [31:0]   gpio_in_s_insp
);

    reg [31:0] sdata_out_s;    //stan magistrali danych - wyjscie
    reg [31:0] gpio_in_s;      //stan peryferii wyjsciowych (do polaczenia z np.: klawiszami)
    reg [31:0] gpio_out_s;     //stan peryferii wejsciowych (stan wyjsc - ale nie laczony z np.: LED'ami)
    reg [2:0]  state;          // status
    reg [31:0] A;              // ilość liczb pierwszych do wyznaczenia
    reg [31:0] S;              // stan automatu -> 0-inicjacja, 1-liczenie, 2-wyliczono
    reg [31:0] W;              // wynik, po znalezieniu liczby pierwszej

    integer counter;
    integer is_prime; // 1-prime, 0-not prime
    integer b;
    integer prime;
    integer i;


    // Zerowanie i inicjacja zmiennych
    always @(negedge n_reset) begin
        gpio_in_s <= 0;
        gpio_out_s <= 0;
        sdata_out_s <= 0;
        W <= 0;
        A <= 0;
        i = 0;
        counter = 0;
    end

    // Obsługa zapisu
    always @(posedge swr) begin
        if (saddress == 16'h100) begin
            A <= sdata_in;
            W <= 0;
            S <= 0;
            state <= 0;
        end
    end

    // Obsługa odczytu
    always @(posedge srd) begin
        case (saddress)
            16'h110: sdata_out_s <= W[31:0];
            16'h120: sdata_out_s <= S[31:0];
            default:   sdata_out_s <= 0;
        endcase
    end

    // Obsługa logiki automatu
    always @(posedge clk) begin
        case(state)
            0: begin
                S <= 1;
                is_prime = 1;
                b = 2;
                state <= 1;
                i = 2;
            end
            1: begin
                if (counter < A) begin
                    is_prime = 1;
                    b = 2;
                    while (b <= i**(0.5)) begin
                        if (i % b == 0) begin
                            is_prime = 0;
                        end
                        b = b + 1;
                    end
                    if (is_prime) begin
                        prime = i;
                        W <= prime;
                        counter = counter + 1;
                    end
                    i = i + 1;
                end else begin
                    state <= 2;
                end
            end
            2: begin
                S <= 2;
                W <= prime;
                gpio_out_s <= gpio_out_s + counter;
                counter = 0;
                prime = 0;
                is_prime = 0;
                b = 0;
                i = 0;
                state <= 3;
            end    
        endcase
    end

    // Przypisanie sygnałów wyjściowych
    assign gpio_out = {16'h0, gpio_out_s[15:0]};
    assign gpio_in_s_insp = gpio_in_s;
    assign sdata_out = sdata_out_s;

endmodule