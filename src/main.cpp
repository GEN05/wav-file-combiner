#include <vector>
#include <iostream>
#include "wav_reader.cpp"
#include "wav_writer.cpp"

int main() {
    std::vector<wav_file> files;
    std::string file_name;
    wav_reader reader;
    files.reserve(4);
    for (int i = 0; i < 4; i++) {
        std::cout << "Input input-file name please\n";
        std::cin >> file_name;
        files.push_back(reader.reader(file_name.data()));
        std::cout << files[i];
    }
    std::cout << "Input output-file name please\n";
    wav_writer writer;
    std::cin >> file_name;
    writer.writer(file_name.data(), files);
    return 0;
}