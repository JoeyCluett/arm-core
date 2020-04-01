#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>
#include <inc/decode_structure.h>
#include <inc/decode_16.h>
#include <inc/decode_32.h>
#include <inc/asm_math_interface.h>
#include <inc/fetch.h>

#include "main.h"

using namespace std;

int main(int argc, char* argv[]) {

    // for the assembly portion to work properly with the rest of the 
    // system, certain alignment requirements must be met:
    static_assert(sizeof(results_t) == 8,          "sizeof results_t incorrect (should be 8)");
    static_assert(offsetof(results_t, flags) == 0, "alignment of .flags in results_t incorrect (should be 0)");
    static_assert(offsetof(results_t, u32) == 4,   "alignment of .u32 in results_t incorrect (should be 4)");
    static_assert(offsetof(results_t, i32) == 4,   "alignment of .i32 in results_t incorrect (should be 4)");

    //armv7_m3 armcpu;
    //armcpu.set_current_mode(armv7_m3::mode_16);

    test_decode_fns("test/testfile.branch.txt");
    test_decode_fns("test/testfile.bottom.txt");
    test_decode_fns("test/testfile.txt");

    std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;

    memory_t mem(memory_t::big_endian);

    for(auto cptr : { "test/input/assembly-code.txt", "test/input/memory.txt" }) {
        load_memory_file(cptr, mem);
        std::cout << mem << std::endl;
    }

    // starting address for machine code

    for(address_t addr = 0x00000224; addr <= 0x000002d4;) {
        uint32_t instruction_word = mem.load_u16(addr);

        int prefix = (instruction_word >> 11) & 0x1F;

        if(prefix == 0x1D || prefix == 0x1E || prefix == 0x1F) {
            // 32-bit instruction
            cout << "<32-BIT THUMB INSTRUCTION>\n";
            addr += 4;
        }
        else {
            auto decoded_inst = decode_16bit_instruction(addr, instruction_word);
            cout << decoded_inst << endl;
            addr += 2;
        }
    }

    return 0;
}

void print_flag_results(int flags) {

    cout << "    " << (flags & 0x0001 ? "1 : CY(Carry)"           : "0 : NC(No Carry)") << endl;
    cout << "    " << (flags & 0x0004 ? "1 : PE(Parity Even)"     : "0 : PO(Parity Odd)") << endl;
    cout << "    " << (flags & 0x0010 ? "1 : AC(Auxiliary Carry)" : "0 : NA(No Auxiliary Carry)") << endl;
    cout << "    " << (flags & 0x0040 ? "1 : ZR(Zero)"            : "0 : NZ(Not Zero)") << endl;
    cout << "    " << (flags & 0x0080 ? "1 : NG(Negative)"        : "0 : PL(Positive)") << endl;
    cout << "    " << (flags & 0x0800 ? "1 : OV(Overflow)"        : "0 : NV(Not Overflow)") << endl;

}