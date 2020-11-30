
#include <ostream>

struct wav_file {
    /*
     * Contains the letters "RIFF" in ASCII form
     * (0x52494646 big-endian form).
     */
    char chunk_id[4];

    /*
     * 36 + SubChunk2Size, or more precisely:
     * 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
     * This is the size of the rest of the chunk
     * following this number.  This is the size of the
     * entire file in bytes minus 8 bytes for the
     * two fields not included in this count:
     * ChunkID and ChunkSize.
     */
    unsigned long chunk_size;


    /*
     * Contains the letters "WAVE"
     * (0x57415645 big-endian form).
     */
    char format[4];

    //  The "WAVE" format consists of two subchunks: "fmt " and "data":
    //  The "fmt " subchunk describes the sound data's format:

    /*
     * Contains the letters "fmt "
     * (0x666d7420 big-endian form).
     */
    char sub_chunk_1_id[4];

    /*
     * 16 for PCM.  This is the size of the
     * rest of the Subchunk which follows this number.
     */
    unsigned long sub_chunk_1_size;

    /*
     * PCM = 1 (i.e. Linear quantization)
     * Values other than 1 indicate some
     * form of compression.
     */
    unsigned short audio_format;

    /*
     * Mono = 1, Stereo = 2, etc.
     */
    unsigned short num_channels;

    /*
     * 8000, 44100, etc.
     */
    unsigned long sample_rate;

    /*
     * == SampleRate * NumChannels * BitsPerSample/8
     */
    unsigned long byte_rate;

    /*
     * == NumChannels * BitsPerSample/8
     * The number of bytes for one sample including
     * all channels. I wonder what happens when
     * this number isn't an integer?
     */
    unsigned short block_align;

    /*
     * 8 bits = 8, 16 bits = 16, etc.
     */
    unsigned short bits_per_sample;

    //  The "data" subchunk contains the size of the data and the actual sound:

    /*
     * Contains the letters "data"
     * (0x64617461 big-endian form).
     */
    char sub_chunk_2_id[4];

    /*
     * == NumSamples * NumChannels * BitsPerSample/8
     * This is the number of bytes in the data.
     * You can also think of this as the size
     * of the read of the subchunk following this
     * number.
     */
    unsigned long sub_chunk_2_size;

    unsigned long data;

    void setChunkId(std::string chunkId);

    void setChunkSize(unsigned long chunkSize);

    void setFormat(const char *format2);

    void setSubChunk1Id(const char *subChunk1Id);

    void setSubChunk1Size(unsigned long subChunk1Size);

    void setAudioFormat(unsigned short audioFormat);

    void setNumChannels(unsigned short numChannels);

    void setSampleRate(unsigned long sampleRate);

    void setByteRate(unsigned long byteRate);

    void setBlockAlign(unsigned short blockAlign);

    void setBitsPerSample(unsigned short bitsPerSample);

    void setSubChunk2Id(const char *subChunk2Id);

    void setSubChunk2Size(unsigned long subChunk2Size);

    void setData(unsigned long data2);

    [[nodiscard]] const char *getChunkId() const;

    [[nodiscard]] unsigned long getChunkSize() const;

    [[nodiscard]] const char *getFormat() const;

    [[nodiscard]] const char *getSubChunk1Id() const;

    [[nodiscard]] unsigned long getSubChunk1Size() const;

    [[nodiscard]] unsigned short getAudioFormat() const;

    [[nodiscard]] unsigned short getNumChannels() const;

    [[nodiscard]] unsigned long getSampleRate() const;

    [[nodiscard]] unsigned long getByteRate() const;

    [[nodiscard]] unsigned short getBlockAlign() const;

    [[nodiscard]] unsigned short getBitsPerSample() const;

    [[nodiscard]] const char *getSubChunk2Id() const;

    [[nodiscard]] unsigned long getSubChunk2Size() const;

    [[nodiscard]] unsigned long getData() const;

    friend std::ostream &operator<<(std::ostream &os, const wav_file &file);
};