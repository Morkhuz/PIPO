#include "EMSFilesystem.h"
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <windows.h>

using namespace std;

//void WriteTraceInfo(const char Targetfolder[], const unsigned int Version, const unsigned int filesperfolder, const unsigned int tracesperfile, const unsigned int samplespertrace, const unsigned char unitsize, const unsigned char numofplaintextunits, const unsigned char numofciphertextunits, const unsigned int numberoftraces, const unsigned char signedvalues)
void EMSFilesystem::WriteTraceInfo(const char Targetfolder[])
{
    strcpy(Folder, Targetfolder);
    CreateDirectory (Targetfolder, NULL);
    sprintf(FileName,"%s/traceinfo.dat",Targetfolder);
    File.open(FileName, ios::out|ios::binary);
    File.write(reinterpret_cast<char*>(&FSConfig),sizeof(FSConfig));
    File.close();
}

void EMSFilesystem::ReadTraceInfo(const char Targetfolder[])
{
    strcpy(Folder, Targetfolder);
    sprintf(FileName,"%s/traceinfo.dat",Targetfolder);
    File.open(FileName, ios::in|ios::binary);
    File.read(reinterpret_cast<char*>(&FSConfig),sizeof(FSConfig));
    File.close();
}

EMSFS_CONFIG EMSFilesystem::getTraceInfo()
{
    return FSConfig;
}

void EMSFilesystem::setTraceInfo(const EMSFS_CONFIG config)
{
    FSConfig=config;
}

void EMSFilesystem::printTraceInfo()
{
    printf("Configuration of the Filesystem is:\n");
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[0], FSConfig.Version, sizeof(FSConfig.Version));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[1], FSConfig.filesperfolder, sizeof(FSConfig.filesperfolder));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[2], FSConfig.tracesperfile, sizeof(FSConfig.tracesperfile));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[3], FSConfig.samplespertrace, sizeof(FSConfig.samplespertrace));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[4], FSConfig.unitsize, sizeof(FSConfig.unitsize));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[5], FSConfig.numofplaintextunits, sizeof(FSConfig.numofplaintextunits));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[6], FSConfig.numofciphertextunits, sizeof(FSConfig.numofciphertextunits));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[7], FSConfig.numberoftraces, sizeof(FSConfig.numberoftraces));
    printf("%28s: %8d (%2d Bytes)\n", EMSFS_CONFIG_DESCRIPTION[8], FSConfig.precision, sizeof(FSConfig.precision));
    printf("%28s: %8d (%2d Bytes)\n\n\n", EMSFS_CONFIG_DESCRIPTION[9], FSConfig.signedvalues, sizeof(FSConfig.signedvalues));
}


//Functions to be used during Measurement
char* EMSFilesystem::nextPlaintext(unsigned char unitID)
{
    return &plaintexts[measurementnumber*FSConfig.unitsize+unitID*FSConfig.numberoftraces*FSConfig.unitsize];
}

char* EMSFilesystem::nextCiphertext(unsigned char unitID)
{
    return &ciphertexts[measurementnumber*FSConfig.unitsize+unitID*FSConfig.numberoftraces*FSConfig.unitsize];
}

char* EMSFilesystem::nextTrace()
{
    return &Traces[(measurementnumber%FSConfig.tracesperfile)*FSConfig.samplespertrace*FSConfig.precision];
}

void EMSFilesystem::allocPlaintextMem()
{
    plaintexts=new char[FSConfig.numberoftraces*FSConfig.unitsize*FSConfig.numofplaintextunits];
}

void EMSFilesystem::allocCiphertextMem()
{
    ciphertexts=new char[FSConfig.numberoftraces*FSConfig.unitsize*FSConfig.numofciphertextunits];
}

void EMSFilesystem::allocTraceMem()
{
    Traces=new char[FSConfig.samplespertrace*FSConfig.precision*FSConfig.tracesperfile];
}


void EMSFilesystem::freePlaintextMem()
{
    writePlaintextfile();
    delete [] plaintexts;
    plaintexts=NULL;
}

void EMSFilesystem::freeCiphertextMem()
{
    writeCiphertextfile();
    delete [] ciphertexts;
    ciphertexts=NULL;
}

void EMSFilesystem::freeTraceMem()
{
    writeTraces();
    delete [] Traces;
    Traces=NULL;
}

void EMSFilesystem::nextMeasurement()
{
    measurementnumber++;
    if (measurementnumber<FSConfig.numberoftraces)
    {
        if ((measurementnumber%FSConfig.tracesperfile)==0)
        {
            writeTraces();
            FileID++;
            if ((FileID%FSConfig.filesperfolder)==0)
            {
                FileID=0;
                FolderID++;
            }
        }
    }
    else
        measurementnumber--;
}

void EMSFilesystem::resetMeasurement()
{
    measurementnumber=0;
    FileID=0;
    FolderID=0;
}

void EMSFilesystem::writeCiphertextfile()
{
    sprintf(FileName,"%s/ciphertexts.dat",Folder);
    File.open(FileName, ios::out|ios::binary);
    File.write(ciphertexts,FSConfig.numberoftraces*FSConfig.unitsize*FSConfig.numofciphertextunits);
    File.close();
}

void EMSFilesystem::writePlaintextfile()
{
    sprintf(FileName,"%s/plaintexts.dat",Folder);
    File.open(FileName, ios::out|ios::binary);
    File.write(plaintexts,FSConfig.numberoftraces*FSConfig.unitsize*FSConfig.numofplaintextunits);
    File.close();
}


void EMSFilesystem::writeTraces()
{

    sprintf(FileName,"%s/%.5d",Folder, FolderID);
    CreateDirectory (FileName, NULL);
    sprintf(FileName,"%s/%.5d/Traces%.5d.dat",Folder, FolderID, FileID);
    File.open(FileName, ios::out|ios::binary);
    File.write(Traces,FSConfig.samplespertrace*FSConfig.precision*FSConfig.tracesperfile);
    File.close();
}

void EMSFilesystem::readPlaintextfile()
{
    sprintf(FileName,"%s/plaintexts.dat",Folder);
    File.open(FileName, ios::in|ios::binary);
    File.read(plaintexts,FSConfig.numberoftraces*FSConfig.unitsize*FSConfig.numofplaintextunits);
    File.close();
}


void EMSFilesystem::readCiphertextfile()
{
    sprintf(FileName,"%s/ciphertexts.dat",Folder);
    File.open(FileName, ios::in|ios::binary);
    File.read(ciphertexts,FSConfig.numberoftraces*FSConfig.unitsize*FSConfig.numofciphertextunits);
    File.close();
}

char* EMSFilesystem::readTraces(unsigned long firstTrace, unsigned long tracecount, unsigned long firstPoint, unsigned long pointcount)
{
    char* result = new char[tracecount*pointcount*FSConfig.precision];
    int FileID=(firstTrace/FSConfig.tracesperfile)%FSConfig.filesperfolder;
    int FolderID=firstTrace/FSConfig.tracesperfile/FSConfig.filesperfolder;
    freeTraceMem();
    allocTraceMem();
    sprintf(FileName,"%s/%.5d/Traces%.5d.dat",Folder, FolderID, FileID);
    File.open(FileName, ios::in|ios::binary);
    File.read(Traces,FSConfig.samplespertrace*FSConfig.precision*FSConfig.tracesperfile);
    File.close();
    for (unsigned long position=firstTrace; position<firstTrace+tracecount; position++)
    {
        if (((position%FSConfig.tracesperfile)==0)&&position!=firstTrace)
        {
            FileID++;
            if ((FileID%FSConfig.filesperfolder)==0)
            {
                FileID=0;
                FolderID++;
            }
            sprintf(FileName,"%s/%.5d/Traces%.5d.dat",Folder, FolderID, FileID);
            File.open(FileName, ios::in|ios::binary);
            File.read(Traces,FSConfig.samplespertrace*FSConfig.precision*FSConfig.tracesperfile);
            File.close();
        }
        for(unsigned long sample=0; sample<pointcount; sample++)
        {
            result[(position-firstTrace)+sample*tracecount]=Traces[(position%FSConfig.tracesperfile)*FSConfig.precision*FSConfig.samplespertrace+firstPoint+sample];
        }

    }
    freeTraceMem();
    return result;
}

EMSFilesystem::EMSFilesystem()
{
    FSConfig.Version=1;
    FSConfig.filesperfolder=1000;
    FSConfig.tracesperfile=1000;
    FSConfig.samplespertrace=0;
    FSConfig.unitsize=1;
    FSConfig.numofplaintextunits=16;
    FSConfig.numofciphertextunits=16;
    FSConfig.numberoftraces=0;
    FSConfig.precision=1;
    FSConfig.signedvalues=1;
}

EMSFilesystem::~EMSFilesystem()
{
    freeCiphertextMem();
    freePlaintextMem();
    freeTraceMem();
}
