/*! \file tinyalu_bfm.cpp
    \brief tinyalu bus functional model
*/
#include "tinyalu_bfm.h"

void tinyalu_bfm::reset_op(void) {
    reset_n = 0;
    wait(clk.negedge_event());
    reset_n = 1;
    start = 1;
}

tinyalu_bfm::tinyalu_bfm(sc_core::sc_module_name name) : sc_module(name) {}

void tinyalu_bfm::send_op(uint8_t iA, uint8_t iB, op_t iop, uint16_t &alu_result){
    op = op_value(iop);
    if (iop == op_t::rst_op) {
        wait(clk.posedge_event());
        reset_n = 0;
        start = 0;
        wait(clk.posedge_event());
        reset_n = 1;
    } else {
        wait(clk.negedge_event());
        A = iA;
        B = iB;
        start = 1;
        if (iop == op_t::no_op) {
            wait(clk.posedge_event());
            start = 0;           
        } else {
            do
                wait(clk.negedge_event());
            while (done == 0);
            alu_result = result;
            start = 0;
        }
    }
}
