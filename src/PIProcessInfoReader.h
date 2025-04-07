#ifndef PI_PROCESS_INFO_READER_H 
#define PI_PROCESS_INFO_READER_H 1

#ifdef _WIN32
    #include <PIWindowsProcessInfoReader.h>
#else
    #include <PILinuxProcessInfoReader.h>
#endif

#endif /*PI_PROCESS_DATA_READER*/