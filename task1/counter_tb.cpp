#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    int holding_on_0x9 = 0; //number of cycles that we have been holding clk on 0x9

    // run simulation for many clock cycles
    for (i=0; i<300; i++) {

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        holding_on_0x9 = ((top->count == 9) && (holding_on_0x9 != 3)) ? (holding_on_0x9 + 1) : 0;

        top->rst = !(i % 32);
        top->en = (i > 4) && !holding_on_0x9;

        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}