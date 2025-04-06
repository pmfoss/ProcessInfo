#ifndef PROCESS_INFO_READER_H 
#define PROCESS_INFO_READER_H 1

#ifdef _WIN32
#include <WindowsProcessInfoReader.h>
using ProcessInfoReader = WindowsProcessInfoReader;
#else
#include <LinuxProcessInfoReader.h>
using ProcessInfoReader = LinuxProcessInfoReader;
#endif

#endif /*PROCESS_DATA_READER*/