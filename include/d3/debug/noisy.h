#pragma once
#include <iostream>

namespace Dlugosz::d3::debug {

template<const char* label>
class noisy {
    void yell (const char* action, const void* other = nullptr) {
        std::cout << label << ": " << action << ' ' << (const void*)this;
        if (other)  std::cout << " <== " << other;
        std::cout << '\n';    }
public:
    noisy()  { yell ("construct"); }
    noisy (const noisy& other) { yell ("copy-construct", &other); }
    noisy (noisy&& other) { yell ("move-construct", &other); }
    void operator= (const noisy& other) { yell ("assign", &other); }
    void operator= (noisy&& other) { yell ("move-assign", &other); }
    ~noisy() { yell ("destruct"); }
};

}