#include "score.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "globals.h"

#define SCORE_FILE_PATH "savedata.bin"
#define SCORE_FILE_COUNT 16
#define SCORE_SLOT_COUNT 6
#define SCORE_RECORD_SIZE 720
#define SCORE_CHECKSUM_OFFSET 716
#define SCORE_INDEX_SIZE 4

static FILE *scoreFiles[SCORE_FILE_COUNT];
static Sint32 selectedScoreIndex = -1;

static void WriteU16LE(Uint8 *dst, Uint16 value) {
    dst[0] = (Uint8)(value & 0xff);
    dst[1] = (Uint8)((value >> 8) & 0xff);
}

static void WriteU32LE(Uint8 *dst, Uint32 value) {
    dst[0] = (Uint8)(value & 0xff);
    dst[1] = (Uint8)((value >> 8) & 0xff);
    dst[2] = (Uint8)((value >> 16) & 0xff);
    dst[3] = (Uint8)((value >> 24) & 0xff);
}

static Uint16 ReadU16LE(const Uint8 *src) {
    return (Uint16)(src[0] | (src[1] << 8));
}

static Uint32 ReadU32LE(const Uint8 *src) {
    return (Uint32)((Uint32)src[0] | ((Uint32)src[1] << 8) |
                    ((Uint32)src[2] << 16) | ((Uint32)src[3] << 24));
}

static void PutU8(Uint8 *dst, size_t *offset, Uint8 value) {
    dst[*offset] = value;
    ++*offset;
}

static void PutU16(Uint8 *dst, size_t *offset, Uint16 value) {
    WriteU16LE(&dst[*offset], value);
    *offset += 2;
}

static void PutU32(Uint8 *dst, size_t *offset, Uint32 value) {
    WriteU32LE(&dst[*offset], value);
    *offset += 4;
}

static void PutBytes(Uint8 *dst, size_t *offset, const void *src, size_t size) {
    memcpy(&dst[*offset], src, size);
    *offset += size;
}

static Uint8 GetU8(const Uint8 *src, size_t *offset) {
    Uint8 value = src[*offset];
    ++*offset;
    return value;
}

static Uint16 GetU16(const Uint8 *src, size_t *offset) {
    Uint16 value = ReadU16LE(&src[*offset]);
    *offset += 2;
    return value;
}

static Uint32 GetU32(const Uint8 *src, size_t *offset) {
    Uint32 value = ReadU32LE(&src[*offset]);
    *offset += 4;
    return value;
}

static void GetBytes(const Uint8 *src, size_t *offset, void *dst, size_t size) {
    memcpy(dst, &src[*offset], size);
    *offset += size;
}

static Sint32 ScoreChecksum(const Uint8 *record) {
    Sint32 checksum = 0;
    size_t i;

    for (i = 0; i < SCORE_CHECKSUM_OFFSET; ++i) {
        checksum += (Sint8)record[i];
    }
    return checksum;
}

static int IsValidIndex(Sint32 index) {
    return index >= 0 && index < SCORE_SLOT_COUNT;
}

static void InitScoreData(score_data *score, Sint32 index) {
    int round;
    int zone;
    int timeIndex;
    static const char playerName[12] = {'P', 'L', 'A', 'Y', 'E', 'R',
                                        '_', '1', ' ', ' ', ' ', ' '};

    memset(score, 0, sizeof(*score));
    memcpy(score->player, playerName, sizeof(score->player));
    if (IsValidIndex(index)) {
        score->player[7] = (char)('1' + index);
    }

    for (round = 0; round < 7; ++round) {
        for (zone = 0; zone < 3; ++zone) {
            for (timeIndex = 0; timeIndex < 3; ++timeIndex) {
                score->timeattack[round][zone][timeIndex].time = 18000;
                memcpy(score->timeattack[round][zone][timeIndex].name, "AAA", 4);
            }
        }
    }

    for (round = 0; round < 7; ++round) {
        for (timeIndex = 0; timeIndex < 3; ++timeIndex) {
            score->special[round][timeIndex].time = 18000;
            memcpy(score->special[round][timeIndex].name, "AAA", 4);
        }
    }

    score->total = 378000;
}

static void SerializeTimeData(Uint8 *record, size_t *offset,
                              const time_data *timeData) {
    PutU32(record, offset, timeData->time);
    PutBytes(record, offset, timeData->name, sizeof(timeData->name));
}

static void DeserializeTimeData(const Uint8 *record, size_t *offset,
                                time_data *timeData) {
    timeData->time = GetU32(record, offset);
    GetBytes(record, offset, timeData->name, sizeof(timeData->name));
}

static void SerializeScoreData(score_data *score, Uint8 *record) {
    size_t offset = 0;
    int round;
    int zone;
    int timeIndex;
    Sint32 checksum;

    memset(record, 0, SCORE_RECORD_SIZE);
    PutU32(record, &offset, score->saved);
    PutBytes(record, &offset, score->player, sizeof(score->player));
    PutU32(record, &offset, score->roundNo);
    PutU16(record, &offset, score->year);
    PutU16(record, &offset, score->month);
    PutU16(record, &offset, score->date);
    PutU16(record, &offset, score->hour);
    PutU16(record, &offset, score->min);
    PutU16(record, &offset, score->sec);

    for (round = 0; round < 7; ++round) {
        for (zone = 0; zone < 3; ++zone) {
            for (timeIndex = 0; timeIndex < 3; ++timeIndex) {
                SerializeTimeData(
                    record, &offset,
                    &score->timeattack[round][zone][timeIndex]);
            }
        }
    }

    for (round = 0; round < 7; ++round) {
        for (timeIndex = 0; timeIndex < 3; ++timeIndex) {
            SerializeTimeData(record, &offset,
                              &score->special[round][timeIndex]);
        }
    }

    PutU32(record, &offset, score->total);
    PutU8(record, &offset, score->clrspflg_save);
    PutU8(record, &offset, score->clrgood);
    PutU8(record, &offset, score->stagenm);
    PutU8(record, &offset, score->reserved1);
    PutU8(record, &offset, score->reserved2);

    checksum = ScoreChecksum(record);
    score->checkSum = (Uint32)checksum;
    WriteU32LE(&record[SCORE_CHECKSUM_OFFSET], score->checkSum);
}

static void DeserializeScoreData(const Uint8 *record, score_data *score) {
    size_t offset = 0;
    int round;
    int zone;
    int timeIndex;

    memset(score, 0, sizeof(*score));
    score->saved = GetU32(record, &offset);
    GetBytes(record, &offset, score->player, sizeof(score->player));
    score->roundNo = GetU32(record, &offset);
    score->year = GetU16(record, &offset);
    score->month = GetU16(record, &offset);
    score->date = GetU16(record, &offset);
    score->hour = GetU16(record, &offset);
    score->min = GetU16(record, &offset);
    score->sec = GetU16(record, &offset);

    for (round = 0; round < 7; ++round) {
        for (zone = 0; zone < 3; ++zone) {
            for (timeIndex = 0; timeIndex < 3; ++timeIndex) {
                DeserializeTimeData(
                    record, &offset,
                    &score->timeattack[round][zone][timeIndex]);
            }
        }
    }

    for (round = 0; round < 7; ++round) {
        for (timeIndex = 0; timeIndex < 3; ++timeIndex) {
            DeserializeTimeData(record, &offset,
                                &score->special[round][timeIndex]);
        }
    }

    score->total = GetU32(record, &offset);
    score->clrspflg_save = GetU8(record, &offset);
    score->clrgood = GetU8(record, &offset);
    score->stagenm = GetU8(record, &offset);
    score->reserved1 = GetU8(record, &offset);
    score->reserved2 = GetU8(record, &offset);
    score->checkSum = ReadU32LE(&record[SCORE_CHECKSUM_OFFSET]);
}

static long ScoreRecordOffset(Sint32 index) {
    return SCORE_INDEX_SIZE + (long)index * SCORE_RECORD_SIZE;
}

static int WriteScoreRecord(FILE *file, Sint32 index, score_data *score) {
    Uint8 record[SCORE_RECORD_SIZE];

    if (!file || !IsValidIndex(index)) {
        return 0;
    }
    SerializeScoreData(score, record);
    if (fseek(file, ScoreRecordOffset(index), SEEK_SET) != 0) {
        return 0;
    }
    return fwrite(record, 1, sizeof(record), file) == sizeof(record);
}

static int ReadScoreRecord(FILE *file, Sint32 index, score_data *score) {
    Uint8 record[SCORE_RECORD_SIZE];
    Sint32 checksum;

    if (!file || !IsValidIndex(index)) {
        return 0;
    }
    if (fseek(file, ScoreRecordOffset(index), SEEK_SET) != 0) {
        return 0;
    }
    if (fread(record, 1, sizeof(record), file) != sizeof(record)) {
        return 0;
    }
    checksum = ScoreChecksum(record);
    if ((Uint32)checksum != ReadU32LE(&record[SCORE_CHECKSUM_OFFSET])) {
        return 0;
    }
    DeserializeScoreData(record, score);
    return 1;
}

static int WriteScoreIndex(FILE *file, Sint32 index) {
    Uint8 encoded[SCORE_INDEX_SIZE];

    if (!file) {
        return 0;
    }
    WriteU32LE(encoded, (Uint32)index);
    if (fseek(file, 0, SEEK_SET) != 0) {
        return 0;
    }
    return fwrite(encoded, 1, sizeof(encoded), file) == sizeof(encoded);
}

static int ReadScoreIndex(FILE *file, Sint32 *index) {
    Uint8 encoded[SCORE_INDEX_SIZE];

    if (!file || !index) {
        return 0;
    }
    if (fseek(file, 0, SEEK_SET) != 0) {
        return 0;
    }
    if (fread(encoded, 1, sizeof(encoded), file) != sizeof(encoded)) {
        return 0;
    }
    *index = (Sint32)ReadU32LE(encoded);
    return 1;
}

static int CreateDefaultScoreFile(void) {
    FILE *file;
    Sint32 index;

    file = fopen(SCORE_FILE_PATH, "wb+");
    if (!file) {
        return 0;
    }
    if (!WriteScoreIndex(file, -1)) {
        fclose(file);
        return 0;
    }
    for (index = 0; index < SCORE_SLOT_COUNT; ++index) {
        score_data emptyScore;
        InitScoreData(&emptyScore, index);
        if (!WriteScoreRecord(file, index, &emptyScore)) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

static int EnsureScoreFile(void) {
    FILE *file = fopen(SCORE_FILE_PATH, "rb");

    if (file) {
        fclose(file);
        return 1;
    }
    return CreateDefaultScoreFile();
}

static FILE *GetScoreFile(Uint32 existingFile) {
    if (existingFile == 0 || existingFile >= SCORE_FILE_COUNT) {
        return NULL;
    }
    return scoreFiles[existingFile];
}

static Uint32 StoreScoreFile(FILE *file) {
    Uint32 i;

    if (!file) {
        return 0;
    }
    for (i = 1; i < SCORE_FILE_COUNT; ++i) {
        if (!scoreFiles[i]) {
            scoreFiles[i] = file;
            return i;
        }
    }
    fclose(file);
    return 0;
}

Sint32 ReadScoreData(Sint32 index, score_data *score, Uint32 existingFile) {
    FILE *file;
    score_data *target = score ? score : &scoreData;
    int closeFile = 0;

    if (!IsValidIndex(index)) {
        InitScoreData(target, index);
        return 0;
    }

    file = GetScoreFile(existingFile);
    if (!file) {
        file = fopen(SCORE_FILE_PATH, "rb");
        closeFile = 1;
    }
    if (!file || !ReadScoreRecord(file, index, target)) {
        if (closeFile && file) {
            fclose(file);
        }
        InitScoreData(target, index);
        return 0;
    }
    if (closeFile) {
        fclose(file);
    }
    if (target->saved == 0) {
        InitScoreData(target, index);
    }
    return 1;
}

Sint32 ReadScoreIndx(Uint32 existingFile) {
    FILE *file;
    Sint32 index = -1;
    int closeFile = 0;

    file = GetScoreFile(existingFile);
    if (!file) {
        file = fopen(SCORE_FILE_PATH, "rb");
        closeFile = 1;
    }
    if (!file || !ReadScoreIndex(file, &index)) {
        if (closeFile && file) {
            fclose(file);
        }
        return -1;
    }
    if (closeFile) {
        fclose(file);
    }
    selectedScoreIndex = index;
    return index;
}

Sint32 WriteScoreData(Sint32 index, score_data *score, Uint32 existingFile) {
    FILE *file;
    score_data *source = score ? score : &scoreData;
    int closeFile = 0;
    int ok;

    if (!IsValidIndex(index) || !EnsureScoreFile()) {
        return 0;
    }
    file = GetScoreFile(existingFile);
    if (!file) {
        file = fopen(SCORE_FILE_PATH, "rb+");
        closeFile = 1;
    }
    if (!file) {
        return 0;
    }
    ok = WriteScoreRecord(file, index, source);
    fflush(file);
    if (closeFile) {
        fclose(file);
    }
    return ok ? 1 : 0;
}

Uint32 WriteScoreIndx(Sint32 index, Uint32 existingFile) {
    FILE *file;
    int closeFile = 0;
    int ok;

    if (!EnsureScoreFile()) {
        return 0;
    }
    file = GetScoreFile(existingFile);
    if (!file) {
        file = fopen(SCORE_FILE_PATH, "rb+");
        closeFile = 1;
    }
    if (!file) {
        return 0;
    }
    selectedScoreIndex = index;
    ok = WriteScoreIndex(file, index);
    fflush(file);
    if (closeFile) {
        fclose(file);
    }
    return ok ? 1U : 0U;
}

Uint32 OpenScoreData(Sint32 mode) {
    FILE *file;

    if (mode != 0 && !EnsureScoreFile()) {
        return 0;
    }
    file = fopen(SCORE_FILE_PATH, mode == 0 ? "rb" : "rb+");
    return StoreScoreFile(file);
}

Uint32 CloseScoreData(Uint32 file) {
    if (file == 0 || file >= SCORE_FILE_COUNT || !scoreFiles[file]) {
        return 0;
    }
    fclose(scoreFiles[file]);
    scoreFiles[file] = NULL;
    return 1;
}

Uint32 CreateScoreData(void) {
    FILE *file = fopen(SCORE_FILE_PATH, "wb+");
    return StoreScoreFile(file);
}

void SetScoreDate(score_data *score) {
    time_t currentTime;
    struct tm calendarTime;
    score_data *target = score ? score : &scoreData;

    time(&currentTime);
#if defined(_MSC_VER)
    if (localtime_s(&calendarTime, &currentTime) != 0) {
        return;
    }
#elif defined(__unix__) || defined(__APPLE__)
    if (!localtime_r(&currentTime, &calendarTime)) {
        return;
    }
#else
    struct tm *fallbackTime;
    fallbackTime = localtime(&currentTime);
    if (!fallbackTime) {
        return;
    }
    calendarTime = *fallbackTime;
#endif

    target->year = (Uint16)(calendarTime.tm_year + 1900);
    target->month = (Uint16)(calendarTime.tm_mon + 1);
    target->date = (Uint16)calendarTime.tm_mday;
    target->hour = (Uint16)calendarTime.tm_hour;
    target->min = (Uint16)calendarTime.tm_min;
    target->sec = (Uint16)calendarTime.tm_sec;
}

Sint32 NewScoreData(void) {
    Sint32 index;

    if (!EnsureScoreFile()) {
        InitScoreData(&scoreData, 0);
        SetScoreDate(&scoreData);
        scoreData.saved = 1;
        return 1;
    }

    for (index = 0; index < SCORE_SLOT_COUNT; ++index) {
        score_data candidate;

        if (!ReadScoreData(index, &candidate, 0)) {
            if (!CreateDefaultScoreFile()) {
                break;
            }
            index = -1;
            continue;
        }

        if (candidate.saved == 0) {
            InitScoreData(&scoreData, index);
            SetScoreDate(&scoreData);
            scoreData.saved = 1;
            if (!WriteScoreData(index, &scoreData, 0)) {
                return 0;
            }
            if (!WriteScoreIndx(index, 0)) {
                return 0;
            }
            selectedScoreIndex = index;
            return 1;
        }
    }

    return 0;
}
