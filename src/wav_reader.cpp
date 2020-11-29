#include "wav_reader.h"
#include "my_exception.cpp"

FILE *file;

// TODO: fix extra symbols at big endian fields
wav_file wav_reader::reader(char *file_name) {
    file = fopen(file_name, "r+");
    if (!file)
        throw exception("Can't open file");
    wav_file wavFile{};

    check_chunk("RIFF");
    wavFile.setChunkId("RIFF");
//    std::cout << wavFile.getChunkId() << "\n";

    auto chunk_size = read<uint32_t>(0);
    wavFile.setChunkSize(chunk_size);
//    std::cout << wavFile.getChunkSize() << "\n";

    check_chunk("WAVE");
    wavFile.setFormat("WAVE");
//    std::cout << wavFile.getFormat() << "\n";

    check_chunk("fmt ");
    wavFile.setSubChunk1Id("fmt ");
//    std::cout << wavFile.getSubChunk1Id() << "\n";

    auto sub_chunk_1_size = read<uint32_t>(0);
    wavFile.setSubChunk1Size(sub_chunk_1_size);
//    std::cout << wavFile.getSubChunk1Size() << "\n";

    auto audio_format = read<uint16_t>(0);
    wavFile.setAudioFormat(audio_format);
//    std::cout << wavFile.getAudioFormat() << "\n";

    auto num_channels = read<uint16_t>(0);
    wavFile.setNumChannels(num_channels);
//    std::cout << wavFile.getNumChannels() << "\n";

    auto sample_rate = read<uint32_t>(0);
    wavFile.setSampleRate(sample_rate);
//    std::cout << wavFile.getSampleRate() << "\n";

    auto byte_rate = read<uint32_t>(0);
    wavFile.setByteRate(byte_rate);
//    std::cout << wavFile.getByteRate() << "\n";

    auto block_align = read<uint16_t>(0);
    wavFile.setBlockAlign(block_align);
//    std::cout << wavFile.getBlockAlign() << "\n";

    auto bits_per_sample = read<uint16_t>(0);
    wavFile.setBitsPerSample(bits_per_sample);
//    std::cout << wavFile.getBitsPerSample() << "\n";

    check_chunk("data");
    wavFile.setSubChunk2Id("data");
//    std::cout << wavFile.getSubChunk2Id() << "\n";

    auto sub_chunk_2_size = read<uint32_t>(0);
    wavFile.setSubChunk2Size(sub_chunk_2_size);
//    std::cout << wavFile.getSubChunk2Size() << "\n";
    return wavFile;
}

void wav_reader::check_chunk(std::string chunk) {
    char id[4];
    if (std::fread(id, 1, 4, file) == 0)
        throw exception("Read error");
    for (int i = 0; i < 4; i++)
        if (id[i] != chunk[i])
            throw exception("Invalid file");
}

template<typename T>
T wav_reader::read(int size) {
    if (size == 0)
        size = sizeof(T);
    char mas[size];
    if (std::fread(mas, 1, size, file) == 0)
        throw exception("read error");
    return *reinterpret_cast<T *>(mas);
}
