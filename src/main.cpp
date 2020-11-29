#include <vector>
#include <iostream>
#include "wav_reader.cpp"

int main() {
    std::vector<wav_file> files;
    std::string file_name;
    wav_reader reader;
    files.reserve(4);
    for (int i = 0; i < 4; i++) {
        std::cin >> file_name;
        files.push_back(reader.reader(file_name.data()));
        std::cout << files[i];
    }
    return 0;
}