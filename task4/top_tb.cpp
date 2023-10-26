#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"   // include the vbuddy code

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vtop *top = new Vtop;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("top.vcd");

    // init Vbuddy
    if (vbdOpen() != 1) return (-1);
    vbdHeader("Task 4: top.sv");
    vbdSetMode(1);

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 1;

    // run simulation for many clock cycles
    for (i=0; i<1000; i++) {

        // dump variables into VCD file and toggle clock
        if (vbdFlag()) {
            for (clk=0; clk<2; clk++) {
                tfp->dump (2*i+clk);
                top->clk = !top->clk;
                top->eval ();
            }
        }

        // ++++ Send count value to Vbuddy
        vbdHex(4, (int(top->bcd) >> 16) & 0xF);
        vbdHex(3, (int(top->bcd) >> 8) & 0xF);
        vbdHex(2, (int(top->bcd) >> 4) & 0xF);
        vbdHex(1, int(top->bcd) & 0xF);
        vbdCycle(i + 1);
        // ---- end of Vbuddy output selection

        // change input stimuli
        top->rst = (i < 2) | (top->bcd >= 200);
        if (Verilated::gotFinish()) exit(0);
    }
    vbdClose();     // ++++ house keeping
    tfp->close();
    exit(0);
}