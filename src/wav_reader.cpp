#include "wav_reader.h"
#include "my_exception.cpp"

FILE *input_file;

wav_file wav_reader::reader(char *file_name) {
    input_file = fopen(file_name, "r");
    if (!input_file)
        throw exception("Can't open in-file");
    wav_file wavFile{};

    check_chunk("RIFF");
    wavFile.setChunkId("RIFF");
    wavFile.setChunkSize(read<unsigned int>());
    check_chunk("WAVE");
    wavFile.setFormat("WAVE");
    check_chunk("fmt ");
    wavFile.setSubChunk1Id("fmt ");
    wavFile.setSubChunk1Size(read<unsigned int>());
    wavFile.setAudioFormat(read<unsigned short>());
    wavFile.setNumChannels(read<unsigned short>());
    wavFile.setSampleRate(read<unsigned int>());
    wavFile.setByteRate(read<unsigned int>());
    wavFile.setBlockAlign(read<unsigned short>());
    wavFile.setBitsPerSample(read<unsigned short>());
    check_chunk("data");
    wavFile.setSubChunk2Id("data");
    wavFile.setSubChunk2Size(read<unsigned int>());
    wavFile.setData(read<unsigned long>());
    return wavFile;
}

void wav_reader::check_chunk(std::string chunk) {
    char chunk_from_file[4];
    if (std::fread(chunk_from_file, 1, 4, input_file) == 0)
        throw exception("Read error");
    for (int i = 0; i < 4; i++)
        if (chunk_from_file[i] != chunk[i])
            throw exception("Invalid input_file");
}

template<typename T>
T wav_reader::read() {
    int size = sizeof(T);
    char array[size];
    if (std::fread(array, 1, size, input_file) == 0)
        throw exception("read error");
    return *reinterpret_cast<T *>(array);
}

wav_reader::~wav_reader() {
    if (input_file)
        fclose(input_file);
}

// RIFF0x63fd50WAVEfmt 0x63fd700x63fd580x63fd700x63fd700x63fd700x63fd580x63fd58data0x63fd50