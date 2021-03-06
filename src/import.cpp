
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <inc/import.h>

size_t import_bin_file(std::string filename, memory_t& memory, uint32_t address) {

    struct stat st;
    stat(filename.c_str(), &st);
    auto sz = st.st_size;

    // prepare a character buffer
    std::vector<char> buf;
    buf.resize(sz);

    std::ifstream is(filename, std::ios::binary);
    is.read(buf.data(), sz);

    for(char c : buf) {
        memory.store_i8(address, c);
        address++;
    }

    auto iter = buf.end() - 4;

    union {
        unsigned int isz;
        char cb[4];
    };

    for(int i = 0; i < 4; i++) {
        cb[i] = *(iter + i);
    }

    return isz;
}
