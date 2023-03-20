#pragma once

#ifndef CONSOLE_H
#define CONSOLE_H

namespace Console
{
	void WriteLine(const char* fmt, ...);
    void Error(const char* fmt, ...);
}

#endif