#include "AtfFileSystem.h"
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

AtfFileSystem::AtfFileSystem(char const * const directoryName, uint32_t const numberOfTracesPerFile, uint32_t const plaintextLength, uint32_t const ciphertextLength, uint32_t const traceLength) {
    this->directoryName = directoryName;
    this->numberOfTracesPerFile = numberOfTracesPerFile;
    this->plaintextLength = plaintextLength;
    this->ciphertextLength = ciphertextLength;
    this->traceLength = traceLength;
    this->traceOffset = 0;
    this->plaintextOffset = 0;
    this->ciphertextOffset = 0;
    this->traceNumber = 0;
    this->fileBuffer = 0;
    this->traceNumberInFile = 0;
    this->traceFile = 0;

    this->makeDirectory(directoryName);
    this->openNewTraceFile();
}

AtfFileSystem::~AtfFileSystem() {
    if(this->isFileModified == true) {
        this->writeTraces();
    }

    if(this->fileBuffer != 0) {
        free(this->fileBuffer);
        this->fileBuffer = 0;
    }

    if(this->traceFile != 0) {
        fclose(this->traceFile);
        this->traceFile = 0;
    }
}

uint8_t * AtfFileSystem::getPlaintextBuffer() {
    return &this->fileBuffer[this->plaintextOffset];
}

uint8_t * AtfFileSystem::getCiphertextBuffer() {
    return &this->fileBuffer[this->ciphertextOffset];
}

int8_t * AtfFileSystem::getTraceBuffer() {
    return (int8_t *) &this->fileBuffer[this->traceOffset];
}

void AtfFileSystem::nextMeasurement() {
    // Check if file is opened
    if(this->traceFile == 0) {
        this->openNewTraceFile();
    }

    // Set number of points
    uint32_t numPoints = this->traceLength;
    for(int i = 0; i < 4; i++)
        this->fileBuffer[traceDataOffset + i] = (numPoints >> (8 * i)) & 0xFF;

    // Prepare for next trace
    this->isFileModified = true;
    this->traceDataOffset += this->computeTraceLengthInFile();
    this->traceNumber++;
    this->traceNumberInFile++;

    if(this->traceNumber % this->numberOfTracesPerFile == 0) {
        this->writeTraces();
        this->openNewTraceFile();
    }
    else {
        // Recalculate offsets
        this->computeOffsets();
    }
}

void AtfFileSystem::writeTraces() {
    // Set number of traces
    uint32_t numTraces = this->traceNumberInFile;
    for(int i = 0; i < 4; i++)
        this->fileBuffer[i] = (uint8_t) ((numTraces >> (8 * i)) & 0xFF);

    // Write file buffer to file and open new file
    if(this->traceFile != 0) {
        uint32_t length = this->computeFileLength() - (this->numberOfTracesPerFile - numTraces) * this->computeTraceLengthInFile();
        fwrite((char *) this->fileBuffer, 1, length, this->traceFile);
    }

    fclose(this->traceFile);
    this->traceFile = 0;
    this->isFileModified = false;
}

void AtfFileSystem::computeOffsets() {
    this->traceOffset = this->traceDataOffset + 4;
    this->plaintextOffset = this->traceOffset + this->traceLength;
    this->ciphertextOffset = this->plaintextOffset + this->plaintextLength;
}

uint32_t AtfFileSystem::computeTraceLengthInFile() {
    uint32_t traceLengthInFile = 4;
    traceLengthInFile += this->traceLength;
    traceLengthInFile += this->plaintextLength;
    traceLengthInFile += this->ciphertextLength;

    return traceLengthInFile;
}

uint32_t AtfFileSystem::computeFileLength() {
    uint32_t fileLength = 4;
    fileLength += this->numberOfTracesPerFile * this->computeTraceLengthInFile();

    return fileLength;
}

void AtfFileSystem::createNewFileBuffer() {
    if(this->fileBuffer != 0) {
        free(this->fileBuffer);
        this->fileBuffer = 0;
    }

    this->fileBuffer = (uint8_t *) malloc(this->computeFileLength());
}

void AtfFileSystem::openNewTraceFile() {
    if(this->traceFile != 0) {
        fclose(this->traceFile);
        this->traceFile = 0;
    }

    char fileName[2049];
    uint32_t fileId = this->traceNumber / this->numberOfTracesPerFile;
    sprintf(fileName, "%s/Traces_%05u.dat", this->directoryName, fileId);
    fileName[2048] = 0;

    this->traceFile = fopen(fileName, "wb");
    if(this->traceFile == 0) {
        printf("##### ERROR: Failed to open trace file!\n");
    }

    this->createNewFileBuffer();
    this->traceDataOffset = 4;
    this->computeOffsets();
    this->traceNumberInFile = 0;
}

void AtfFileSystem::makeDirectory(char const * const directoryName) {
    CreateDirectory (directoryName, NULL);
}
