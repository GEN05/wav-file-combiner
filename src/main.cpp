#include <vector>
#include "wav_file.cpp"

int main(int argc, char **argv) {
    if (argc != 6)
        throw exception("wrong number of arguments");
    std::vector<wav_file> files;
    files.reserve(4);
    for (int i = 1; i < argc - 1; i++) {
        files[i - 1].load(argv[i]);
    }
    return 0;
}