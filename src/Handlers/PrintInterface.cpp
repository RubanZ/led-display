#include "PrintInterface.h"

void PrintInterface::logo(Interface *interface)
{
    interface->write("\r\n\r\n");
    interface->write("                                          ___           ___                         ___           ___           ___     \r\n");
    interface->write("      _____          ___                 /  /\\         /__/\\         _____         /  /\\         /__/\\         /  /\\    \r\n");
    interface->write("     /  /::\\        /__/|               /  /::\\        \\  \\:\\       /  /::\\       /  /::\\        \\  \\:\\       /  /::|   \r\n");
    interface->write("    /  /:/\\:\\      |  |:|              /  /:/\\:\\        \\  \\:\\     /  /:/\\:\\     /  /:/\\:\\        \\  \\:\\     /  /:/:|   \r\n");
    interface->write("   /  /:/~/::\\     |  |:|             /  /:/~/:/    ___  \\  \\:\\   /  /:/~/::\\   /  /:/~/::\\   _____\\__\\:\\   /  /:/|:|__ \r\n");
    interface->write("  /__/:/ /:/\\:|  __|__|:|            /__/:/ /:/___ /__/\\  \\__\\:\\ /__/:/ /:/\\:| /__/:/ /:/\\:\\ /__/::::::::\\ /__/:/ |:| /\\\r\n");
    interface->write("  \\  \\:\\/:/~/:/ /__/::::\\            \\  \\:\\/:::::/ \\  \\:\\ /  /:/ \\  \\:\\/:/~/:/ \\  \\:\\/:/__\\/ \\  \\:\\~~\\~~\\/ \\__\\/  |:|/:/\r\n");
    interface->write("   \\  \\::/ /:/     ~\\~~\\:\\            \\  \\::/~~~~   \\  \\:\\  /:/   \\  \\::/ /:/   \\  \\::/       \\  \\:\\  ~~~      |  |:/:/ \r\n");
    interface->write("    \\  \\:\\/:/        \\  \\:\\            \\  \\:\\        \\  \\:\\/:/     \\  \\:\\/:/     \\  \\:\\        \\  \\:\\          |  |::/  \r\n");
    interface->write("     \\  \\::/          \\__\\/             \\  \\:\\        \\  \\::/       \\  \\::/       \\  \\:\\        \\  \\:\\         |  |:/   \r\n");
    interface->write("      \\__\\/                              \\__\\/         \\__\\/         \\__\\/         \\__\\/         \\__\\/         |__|/    \r\n");
    interface->write("\r\n\r\n");
}

size_t PrintInterface::format(Interface *interface, const char *format, ...)
{
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if (len < 0)
    {
        va_end(arg);
        return 0;
    };
    if (len >= sizeof(loc_buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
        {
            va_end(arg);
            return 0;
        }
        len = vsnprintf(temp, len + 1, format, arg);
    }
    va_end(arg);
    len = interface->write(temp, len);
    if (temp != loc_buf)
    {
        free(temp);
    }
    return len;
}