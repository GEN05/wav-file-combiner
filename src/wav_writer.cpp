#include "wav_writer.h"

FILE *output_file;

void wav_writer::writer(char *file_name, std::vector<wav_file> files) {
    output_file = fopen(file_name, "w");
    if (!output_file)
        throw exception("Can't open out-file");
    long long int min_data_size = INT64_MAX;
    for (int i = 0; i < files.size(); i++) {
        are_compatible(files, i);
        are_mono(files, i);
        long long int data_size = files[i].getSubChunk2Size();
        if (min_data_size < data_size)
            min_data_size = data_size;
    }
    min_data_size *= 2;
    write("RIFF", 4);
    write(min_data_size + 28, 0);
    write("WAVE", 4);
    write("fmt ", 4);
    write<unsigned int>(16, 0);
    write<unsigned short>(1, 0);
    write(2, 0);
    write(files[0].getSampleRate(), 0);
    write(files[0].getByteRate(), 0);
    write(files[0].getBlockAlign(), 0);
    write(files[0].getBitsPerSample(), 0);
    write("data", 4);
    write(2 * min_data_size, 0);
    merge_and_write(files, min_data_size);
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
    else
        size = 4;
    std::fwrite(&symbols, 1, size, output_file);
}

void wav_writer::merge_and_write(std::vector<wav_file> files, long long min_data_size) {
    for (unsigned int i = 0; i < min_data_size; i++) {
        long long int left = 0, right = 0;
        unsigned int left_channels = 0, right_channels = 0;
        for (unsigned int j = 0; j < files.size(); j += 2) {
            left += files[j].getData();
            left_channels++;
        }
        for (unsigned int j = 1; j < files.size(); j += 2) {
            right += files[j].getData();
            right_channels++;
        }
        if (left != 0)
            left /= left_channels;
        if (right != 0)
            right /= right_channels;
        left *= 2;
        right *= 2;
        write<long long int>(left, 0);
        write<long long int>(right, 0);
    }
}