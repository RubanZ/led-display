#include "HandlerBase.h"

void HandlerBase::parseArray(std::string fmessage, std::string delimiter, int *buffer)
{
    size_t pos = 0;
    size_t count = 0;
    std::string token;
    while ((pos = fmessage.find(delimiter)) != std::string::npos)
    {
        token = fmessage.substr(0, pos);
        buffer[count] = atoi(token.c_str());
        count++;
        fmessage.erase(0, pos + delimiter.length());
    }
}

std::string HandlerBase::parseValue(std::string fmessage, std::string delimiter)
{
    size_t pos = 0;
    if ((pos = fmessage.find(delimiter)) != std::string::npos)
        return fmessage.substr(0, pos);
    else
        return "";
}