#include "wav_file.cpp"

struct wav_reader {
public:
    wav_file reader(char *file_name);

    virtual ~wav_reader();
private:
    static void check_chunk(std::string chunk);

    template<typename T>
    T read();
};