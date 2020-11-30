#include <vector>

struct wav_writer {
public:
    void writer(char *file_name, std::vector<wav_file> files);

private:
    static void are_compatible(std::vector<wav_file> files, int i);

    static void are_mono(std::vector<wav_file> files, int i);

    template<typename T>
    void write(T symbols, int size);

    void merge_and_write(std::vector<wav_file> files, long long min_data_size);
};