#include "wav_writer.h"
#include "my_exception.cpp"

FILE *file;

wav_file wav_writer::writer(char *file_name, std::vector<wav_file> files) {
    file = fopen(file_name, "w");
    long long min_data_size = INT64_MAX;
    for (int i = 0; i < files.size(); i++) {
        are_compatible(files, i);
        are_mono(files, i);
        long long data_size = files[i].getSubChunk2Size();
        if (min_data_size < data_size)
            min_data_size = data_size;
    }
    min_data_size *= 2;
    std::fwrite("RIFF", 1, 4, file);
    write("RIFF", 4);
    write(min_data_size + 28, 0);
    write("WAVE", 4);
    write("fmt ", 4);
    write<uint32_t>(16, 0);
    write<uint16_t>(1, 0);
    write(2, 0);
    write(files[0].getSampleRate(), 0);
    write(files[0].getByteRate(), 0);
    write(files[0].getBlockAlign(), 0);
    write(files[0].getBitsPerSample(), 0);
    write("data", 4);
    write(2 * min_data_size, 0);
    return wav_file();
}

void wav_writer::are_compatible(std::vector<wav_file> files, int i) {
    if (i + 1 < files.size()) {
        wav_file first = files[i];
        wav_file second = files[i + 1];
        if (first.getBitsPerSample() != second.getBitsPerSample() ||
            first.getSampleRate() != second.getSampleRate() ||
            first.getByteRate() != second.getByteRate())
            throw exception("Files are not compatible");
    }
}

void wav_writer::are_mono(std::vector<wav_file> files, int i) {
    if (files[i].getNumChannels() != 1)
        throw exception("I can work only with mono files");
}

template<typename T>
void wav_writer::write(T symbols, int size) {
    if (size == 0)
        size = sizeof(symbols);
    std::fwrite(&symbols, 1, size, file);
}
