# WAV-FILE-COMBINER
Программа объединяет несколько `mono` файлов в один `stereo` файл,
увеличивая при этом громкость в 2 раза
Левая сторона - четные каналы, правая - нечетные.  
Входные и выходные файлы поступают в качестве аргументов программы в виде:
```
--input file_name_1.wav file_name_2.wav file_name_3.wav file_name_4.wav --output file_name.wav
```
## Header
| Endian | File offset (bytes) | Field name      | Field Size (bytes) |
|:------:|:-------------------:|:---------------:|:------------------:|
| big    | 0                   | Chunk ID        | 4                  |
| little | 4                   | Chunk Size      | 4                  |
| big    | 8                   | Format          | 4                  |
| big    | 12                  | SubChunk 1 ID   | 4                  |
| little | 16                  | SubChunk 1 Size | 4                  |
| little | 20                  | Audio Format    | 2                  |
| little | 22                  | Num Channels    | 2                  |
| little | 24                  | Sample Rate     | 4                  |
| little | 28                  | Byte Rate       | 4                  |
| little | 32                  | Block Align     | 2                  |
| little | 34                  | Bits Per Sample | 2                  |
| big    | 36                  | SubChunk 2 ID   | 4                  |
| little | 40                  | SubChunk 2 Size | 4                  |
| little | 44                  | DATA            | SubChunk 2 Size    |