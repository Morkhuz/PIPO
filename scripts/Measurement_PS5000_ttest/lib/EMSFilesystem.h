#ifndef EMSFILESYSTEM_H
#define EMSFILESYSTEM_H

#include <fstream>


typedef struct
{
    unsigned int  Version;               //Version of the Filesystem
    unsigned int  filesperfolder;        //number of files in one folder
    unsigned int  tracesperfile;         //number of traces within a single file
    unsigned int  samplespertrace;       //number of samples in one trace
    unsigned char unitsize;              //number of bytes for each plaintext unit (1 for Bytewise, 8 for 32bit units and so on)
    unsigned char numofplaintextunits;   //number of plaintext units (16 of size 1 for AES, 2 of size 32 for DES)
    unsigned char numofciphertextunits;  //number of ciphertext units
    unsigned int  numberoftraces;        //number of traces available
    unsigned char precision;             //number of bytes per measured value
    unsigned char signedvalues;          //are the measurements signed or unsigned? 1 = signed 0 = unsigned
} EMSFS_CONFIG;


//TraceFile:
//TracesperFile x samplespertrace x precision

//InFile:
//numofplaintextunits x numberoftraces x  unitsize

//OutFile:
//numofciphertextunits x numberoftraces x  unitsize




const char EMSFS_CONFIG_DESCRIPTION [10][43]= {
                                "Version",
                                "Files per Folder",
                                "Traces per File",
                                "Samples per Trace",
                                "Unitsize of I/O in Bytes",
                                "Number of plaintext units",
                                "Number of ciphertext units",
                                "Number of Traces available",
                                "Precision (Bytes per value)",
                                "unsigned (0) or signed (1)"
                            };


class EMSFilesystem
{
    public:
        char* plaintexts;
        char* ciphertexts;
        char* Traces;

        char* nextPlaintext(unsigned char unitID);
        char* nextCiphertext(unsigned char unitID);
        char* nextTrace();

        void allocPlaintextMem();
        void allocCiphertextMem();
        void allocTraceMem();

        void freePlaintextMem();
        void freeCiphertextMem();
        void freeTraceMem();

        void writePlaintextfile();
        void writeCiphertextfile();
        void writeTraces();

        void readPlaintextfile();
        void readCiphertextfile();
        char* readTraces(unsigned long firstTrace, unsigned long tracecount, unsigned long firstPoint, unsigned long pointcount);

        void nextMeasurement();
        void resetMeasurement();

        EMSFilesystem();
        virtual ~EMSFilesystem();
        void WriteTraceInfo(const char Targetfolder[]);
        void ReadTraceInfo(const char Targetfolder[]);
        EMSFS_CONFIG getTraceInfo();
        void setTraceInfo(const EMSFS_CONFIG config);
        void printTraceInfo();

    protected:
    private:
        unsigned long measurementnumber;
        unsigned int FileID;
        unsigned int FolderID;
        EMSFS_CONFIG FSConfig;
        std::fstream File;
        char FileName[500];
        char Folder[500];
};

#endif // EMSFILESYSTEM_H
