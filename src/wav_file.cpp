#include "wav_file.h"
#include "my_exception.cpp"

void wav_file::setChunkId(std::string chunkId) {
    for (int i = 0; i < 4; i++)
        chunk_id[i] = chunkId[i];
}

void wav_file::setChunkSize(unsigned long chunkSize) {
    chunk_size = chunkSize;
}

void wav_file::setFormat(const char *format2) {
    for (int i = 0; i < 4; i++)
        format[i] = format2[i];
}


void wav_file::setSubChunk1Id(const char *subChunk1Id) {
    for (int i = 0; i < 4; i++)
        sub_chunk_1_id[i] = subChunk1Id[i];
}

void wav_file::setSubChunk1Size(unsigned long subChunk1Size) {
    sub_chunk_1_size = subChunk1Size;
}

void wav_file::setAudioFormat(unsigned short audioFormat) {
    audio_format = audioFormat;
}

void wav_file::setNumChannels(unsigned short numChannels) {
    num_channels = numChannels;
}

void wav_file::setSampleRate(unsigned long sampleRate) {
    sample_rate = sampleRate;
}

void wav_file::setByteRate(unsigned long byteRate) {
    byte_rate = byteRate;
}

void wav_file::setBlockAlign(unsigned short blockAlign) {
    block_align = blockAlign;
}

void wav_file::setBitsPerSample(unsigned short bitsPerSample) {
    bits_per_sample = bitsPerSample;
}

void wav_file::setSubChunk2Id(const char *subChunk2Id) {
    for (int i = 0; i < 4; i++)
        sub_chunk_2_id[i] = subChunk2Id[i];
}

void wav_file::setSubChunk2Size(unsigned long subChunk2Size) {
    sub_chunk_2_size = subChunk2Size;
}

const char *wav_file::getChunkId() const {
    return chunk_id;
}

unsigned long wav_file::getChunkSize() const {
    return chunk_size;
}

const char *wav_file::getFormat() const {
    return format;
}

const char *wav_file::getSubChunk1Id() const {
    return sub_chunk_1_id;
}

unsigned long wav_file::getSubChunk1Size() const {
    return sub_chunk_1_size;
}

unsigned short wav_file::getAudioFormat() const {
    return audio_format;
}

unsigned short wav_file::getNumChannels() const {
    return num_channels;
}

unsigned long wav_file::getSampleRate() const {
    return sample_rate;
}

unsigned long wav_file::getByteRate() const {
    return byte_rate;
}

unsigned short wav_file::getBlockAlign() const {
    return block_align;
}

unsigned short wav_file::getBitsPerSample() const {
    return bits_per_sample;
}

const char *wav_file::getSubChunk2Id() const {
    return sub_chunk_2_id;
}

unsigned long wav_file::getSubChunk2Size() const {
    return sub_chunk_2_size;
}

std::ostream &operator<<(std::ostream &os, const wav_file &file) {
    os << "chunk_id: " << file.getChunkId() << "\n"
       << "chunk_size: " << file.getChunkSize() << "\n"
       << "format: " << file.getFormat() << "\n"
       << "sub_chunk_1_id: " << file.getSubChunk1Id() << "\n"
       << "sub_chunk_1_size: " << file.getSubChunk1Size() << "\n"
       << "audio_format: " << file.getAudioFormat() << "\n"
       << "num_channels: " << file.getNumChannels() << "\n"
       << "sample_rate: " << file.getSampleRate() << "\n"
       << "byte_rate: " << file.getByteRate() << "\n"
       << "block_align: " << file.getBlockAlign() << "\n"
       << "bits_per_sample: " << file.getBitsPerSample() << "\n"
       << "sub_chunk_2_id: " << file.getSubChunk2Id() << "\n"
       << "sub_chunk_2_size: " << file.getSubChunk2Size() << "\n"
       << "data: " << file.getData() << "\n";
    return os;
}

unsigned long wav_file::getData() const {
    return data;
}

void wav_file::setData(unsigned long data2) {
    wav_file::data = data2;
}

void wav_file::load(const char *file_name) {
    file = fopen(file_name, "r+");
    if (!file)
        throw exception("Can't open in-file");
    check_chunk("RIFF");
    setChunkId("RIFF");
    setChunkSize(read<unsigned int>());
    check_chunk("WAVE");
    setFormat("WAVE");
    check_chunk("fmt ");
    setSubChunk1Id("fmt ");
    setSubChunk1Size(read<unsigned int>());
    setAudioFormat(read<unsigned short>());
    setNumChannels(read<unsigned short>());
    setSampleRate(read<unsigned int>());
    setByteRate(read<unsigned int>());
    setBlockAlign(read<unsigned short>());
    setBitsPerSample(read<unsigned short>());
    check_chunk("data");
    setSubChunk2Id("data");
    setSubChunk2Size(read<unsigned int>());
    setData(read<unsigned long>());
}

void wav_file::check_chunk(std::string chunk) const {
    char chunk_from_file[4];
    if (std::fread(chunk_from_file, 1, 4, file) == 0)
        throw exception("Read error");
    for (int i = 0; i < 4; i++)
        if (chunk_from_file[i] != chunk[i])
            throw exception("Invalid input_file");
}

template<typename T>
T wav_file::read() {
    int size = sizeof(T);
    char array[size];
    if (std::fread(array, 1, size, file) == 0)
        throw exception("read error");
    return *reinterpret_cast<T *>(array);
}

void wav_file::save(const char *) {

}
