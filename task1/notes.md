# Test yourself challenge 1

Changed the reset condition to be periodic to effectively test counter checking. Implemented a new integer variable holding_on_0x9 to track the number of cycles that the counter has stopped counting after reaching 0x9. Also added logic to control enable signal using this.

```c++
    holding_on_0x9 = ((top->count == 9) && (holding_on_0x9 != 3)) ? (holding_on_0x9 + 1) : 0;

    top->rst = !(i % 32);
    top->en = (i > 4) && !holding_on_0x9;
```

# Test yourself challenge 2

Added a 'posedge rst' condition to the always @() statement in counter.sv. This has the counter reset to 0 asynchronously, as soon as rst goes high. In GTK wave, we see this as the reset taking effect half a cycle early, on the falling edge of the clock cycle where rst goes high. This is because this is the first simulated point in time after rst goes high (only rising/falling clock edges are simulated with this testbench). In a physical cicuit, the reset would occur earlier, immediately after the rising edge of the clock.