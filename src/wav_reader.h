#include "wav_file.cpp"

struct wav_reader {
public:
    wav_file reader(char *file_name);

private:
    static void check_chunk(std::string chunk);

    template<typename T>
    T read(int size);
};