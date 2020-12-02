#include "wav_file.h"
#include "my_exception.cpp"

wav_file::wav_file(const char *file_name) {
    load(file_name);
}

void wav_file::setChunkId(std::string chunkId) {
    for (int i = 0; i < 4; i++)
        chunk_id[i] = chunkId[i];
}

void wav_file::setChunkSize(unsigned int chunkSize) {
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
       << "sub_chunk_2_size: " << file.getSubChunk2Size() << "\n";
    return os;
}

void wav_file::load(const char *file_name) {
    file = fopen(file_name, "r");
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
    //std::fseek(file, getSubChunk1Size() - 16, SEEK_CUR);
    check_chunk("data");
    setSubChunk2Id("data");
    setSubChunk2Size(read<unsigned int>());
    //setData(read<unsigned long>());
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

template<typename T>
void wav_file::write(T symbols, int size) {
    if (size == 0)
        size = sizeof(symbols);
    else
        size = 4;
    std::fwrite(&symbols, 1, size, file);
}

template<typename T>
void wav_file::merge_and_write(std::vector<wav_file> files, long long int min_data_size) {
    for (unsigned int i = 0; i < min_data_size; i++) {
        T left = 0, right = 0;
        for (unsigned int j = 0; j < files.size(); j += 2)
            left += files[j].read<T>();
        for (unsigned int j = 1; j < files.size(); j += 2)
            right += files[j].read<T>();
        left *= 2;
        right *= 2;
        write<T>(left, 0);
        write<T>(right, 0);
    }
}

wav_file::wav_file() noexcept = default;


void are_compatible(std::vector<wav_file> files, unsigned int i) {
    if (i + 1 < files.size()) {
        wav_file first = files[i];
        wav_file second = files[i + 1];
        if (first.getBitsPerSample() != second.getBitsPerSample() ||
            first.getSampleRate() != second.getSampleRate() ||
            first.getByteRate() != second.getByteRate())
            throw exception("Files are not compatible");
    }
}

void are_mono(std::vector<wav_file> files, unsigned int i) {
    if (files[i].getNumChannels() != 1)
        throw exception("I can work only with mono files");
}


void save(const char *file_name, std::vector<wav_file> files) {
    wav_file output_file;
    output_file.file = fopen(file_name, "w+");
    if (!output_file.file)
        throw exception("Can't open out-file");
    unsigned int min_data_size = UINT32_MAX;
    for (unsigned int i = 0; i < files.size(); i++) {
        std::fseek(files[i].file,
                   files[i].chunk_size - files[i].sub_chunk_2_size + // NOLINT(cppcoreguidelines-narrowing-conversions)
                   8, // NOLINT(cppcoreguidelines-narrowing-conversions)
                   SEEK_SET); // NOLINT(cppcoreguidelines-narrowing-conversions)
        are_compatible(files, i);
        are_mono(files, i);
        long long int data_size = files[i].getSubChunk2Size();
        if (min_data_size > data_size)
            min_data_size = data_size;
    }
    output_file.setChunkId("RIFF");
    output_file.write("RIFF", 4);
    output_file.setChunkSize(min_data_size + 28);
    output_file.write(min_data_size + 28, 0);
    output_file.setFormat("WAVE");
    output_file.write("WAVE", 4);
    output_file.setSubChunk1Id("fmt  ");
    output_file.write("fmt ", 4);
    output_file.setSubChunk1Size(16);
    output_file.write<unsigned int>(16, 0);
    output_file.setAudioFormat(1);
    output_file.write<unsigned short>(1, 0);
    output_file.setNumChannels(2);
    output_file.write(2, 0);
    output_file.setSampleRate(files[0].getSampleRate());
    output_file.write(files[0].getSampleRate(), 0);
    output_file.setByteRate(files[0].getByteRate());
    output_file.write(output_file.getByteRate(), 0);
    output_file.setBlockAlign(files[0].getBlockAlign());
    output_file.write(output_file.getBlockAlign(), 0);
    output_file.setBitsPerSample(files[0].getBitsPerSample());
    output_file.write(output_file.getBitsPerSample(), 0);
    output_file.setSubChunk2Id("data");
    output_file.write("data", 4);
    output_file.setSubChunk2Size(2 * min_data_size);
    output_file.write(2 * min_data_size, 0);
    switch (output_file.bits_per_sample) {
        case 8:
            output_file.merge_and_write<uint8_t>(files, min_data_size);
            break;
        case 16:
            output_file.merge_and_write<int16_t>(files, min_data_size);
            break;
        case 32:
            output_file.merge_and_write<int32_t>(files, min_data_size);
            break;
    }
}