#include <vector>
#include "wav_file.cpp"

int main(int argc, char *argv[]) {
    if (argc == 1)
        throw exception("No arguments");
    else if (argv[1][0] != '-') {
        std::cout << argv[1];
        throw exception("No input flag (--input)");
    }
    bool flag = false;
    int position = 0;
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            flag = true;
            position = i;
            break;
        }
    }
    if (!flag)
        throw exception("No output flag (--output)");
    if (argc - position > 2)
        throw exception("More than 1 output file");
    std::vector<wav_file> files;
    files.reserve(4);
    try {
        for (int i = 2; i < argc - 2; i++) {
            files.emplace_back(argv[i]);
            std::cout << files[i - 2];
        }
    } catch (exception e) { // NOLINT(misc-throw-by-value-catch-by-reference)
        perror(e.what());
        return 1;
    }
    try {
        save(argv[argc - 1], files);
    } catch (exception e) { // NOLINT(misc-throw-by-value-catch-by-reference)
        perror(e.what());
        return 1;
    }
    return 0;
}