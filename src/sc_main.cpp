/*! \file sc_main.cpp
    \brief SystemC entry point
*/
#include <csignal>
#include <iomanip>
#include <iostream>

#include <systemc>
#include <uvm>

#include <verilated.h>
#include <verilated_vcd_sc.h>

#include "top.h"

top* top_p;
VerilatedVcdSc* tfp = nullptr;

void signal_handler(int signal) {
    if (signal == SIGABRT) {
        if (tfp) {
            tfp->flush();
        }
    }
    std::_Exit(EXIT_FAILURE);
}

int sc_main(int argc, char* argv[]) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);
    sc_set_time_resolution(1, SC_PS);
    top_p = new top("top");
    const char* flag = Verilated::commandArgsPlusMatch("trace");
    if (flag && strcmp(flag, "+trace") == 0) {
        std::cout << "SC_MAIN: VCD open" << std::endl;
        tfp = new VerilatedVcdSc;
        top_p->dut->trace(tfp, 99);
        tfp->open("tb_v0.vcd");
    }
    std::signal(SIGABRT, signal_handler);
  uvm::uvm_set_verbosity_level(uvm::UVM_FULL);
  uvm::uvm_default_printer = uvm::uvm_default_table_printer;
  uvm::uvm_root::get()->enable_print_topology();
  uvm::uvm_config_db<tinyalu_bfm*>::set(NULL,"*","bfm",top_p->bfm);
  uvm::run_test("fibonacci_test");
    if (tfp) {
        std::cout << "SC_MAIN: VCD close" << std::endl;
        tfp->close();
    }
    return 0;
}



