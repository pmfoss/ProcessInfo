#ifndef PROCESS_DATA_READER_H 
#define PROCESS_DATA_READER_H 1

#ifdef _WIN32
#include <WindowsProcessDataReader.h>
using ProcessDataReader = WindowsProcessDataReader;
#else
#include <LinuxProcessDataReader.h>
using ProcessDataReader = LinuxProcessDataReader;
#endif

#endif /*PROCESS_DATA_READER*/