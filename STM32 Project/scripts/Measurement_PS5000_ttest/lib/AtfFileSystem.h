
#ifndef ATFFILESYSTEM_H
#define ATFFILESYSTEM_H

#include <fstream>
#include <stdio.h>
#include <stdint.h>

class AtfFileSystem
{
    public:
        uint8_t * getPlaintextBuffer();
        uint8_t * getCiphertextBuffer();
        int8_t * getTraceBuffer();
        void nextMeasurement();

        AtfFileSystem(char const * const directoryName, uint32_t const numberOfTracesPerFile, uint32_t const plaintextLength, uint32_t const ciphertextLength, uint32_t const traceLength);
        ~AtfFileSystem();

    private:
        uint32_t computeTraceLengthInFile();
        uint32_t computeFileLength();
        void createNewFileBuffer();
        void openNewTraceFile();
        void makeDirectory(char const * const directoryName);
        void computeOffsets();
        void writeTraces();

    private:
        uint32_t plaintextOffset;
        uint32_t ciphertextOffset;
        uint32_t traceOffset;
        uint32_t traceDataOffset;
        uint8_t * fileBuffer;
        uint32_t traceNumber;
        uint32_t traceNumberInFile;
        bool isFileModified;
        FILE * traceFile;

        char const * directoryName;
        uint32_t numberOfTracesPerFile;
        uint32_t plaintextLength;
        uint32_t ciphertextLength;
        uint32_t traceLength;
};

#endif // ATFFILESYSTEM_H
