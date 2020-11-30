# wav-output_file-combiner
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
## Reader
Считывет заголовок и проверяет файл на корректность
## Writer
1. Проверяет файлы на совместимость
1. Объединяет файлы:
   + Увеличивает громкость в два раза
   + Чётные записывает в левый канал
   + Нечётные записывает в правый канал