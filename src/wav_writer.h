#include <vector>
#include "wav_file.cpp"

struct wav_writer {
public:
    wav_file writer(char *file_name, std::vector<wav_file> files);

private:
    static void are_compatible(std::vector<wav_file> files, int i);

    static void are_mono(std::vector<wav_file> files, int i);

    template<typename T>
    void write(T symbols, int size);
};