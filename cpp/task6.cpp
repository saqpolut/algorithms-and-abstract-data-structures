#include "genealogy.h"
#include <string>

int main(int argc, char* argv[]) {
    HashMethod method = DOUBLE_HASHING;
    bool showStats = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--linear") method = LINEAR_PROBING;
        if (arg == "--stats") showStats = true;
    }

    runGenealogy(method, showStats);
    return 0;
}
