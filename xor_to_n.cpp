#include <iostream>
#include <cassert>
 
using namespace std;

int xor_to_n(int n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        case 3: return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace XORTest {
void XORTest() {
    for(int i = 0; i < 100; i++) {
        int expected = 0;
        for(int j = 0; j <= i; j++) expected ^= j;
        int output = xor_to_n(i);
        assert(output == expected);
        printf("PASS #%d\n", i);
    }
}

} // namespace XORTest

int main() {
    XORTest::XORTest();   
    return 0;
}