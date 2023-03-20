#include "shared.h"

void Console::WriteLine(const char* fmt, ...)
{
	char buffer[512];

	va_list params;
	va_start(params, fmt);
	vsprintf(buffer, fmt, params);
	va_end(params);

#ifdef SOURCEMOD_BUILD
	g_pSM->LogMessage(myself, "%s", buffer);
#else
    printf("%s\n", buffer);
#endif
}

void Console::Error(const char* fmt, ...)
{
    char buffer[512];

	va_list params;
	va_start(params, fmt);
	vsprintf(buffer, fmt, params);
	va_end(params);

#ifdef SOURCEMOD_BUILD
	g_pSM->LogError(myself, "%s", buffer);
#else
    printf("%s\n", buffer);
#endif
}