#ifndef PDH_EXCEPTION_H
#define PDH_EXCEPTION_H 1

#include <exception>
#include <string>
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>

class PdhException : public std::exception 
{
    public:
        PdhException(DWORD pStatus);

        DWORD getStatus() const;
        virtual const char* what() const noexcept override;
    
    private:
       DWORD mStatus;
       std::string mMessage;
};

/*constructors / destructors*/
PdhException::PdhException(DWORD pStatus)
    : mStatus(pStatus)
{
    char* lMessageBuffer;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        pStatus,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lMessageBuffer,
        0, NULL);

    mMessage = lMessageBuffer;

    LocalFree(lMessageBuffer);  // Free the buffer allocated by FormatMessage
}

/*public methods*/
DWORD PdhException::getStatus() const
{
    return mStatus;
}

const char* PdhException::what() const noexcept
{
    return mMessage.c_str();
}

#endif /*PDH_EXCEPTION_H*/
