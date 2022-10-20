#include "Console.hpp"

#ifdef WC_ENABLE_DEBUG_CONSOLE

__ConsoleOutputHandler DebugConsole::Out = __ConsoleOutputHandler();
HANDLE DebugConsole::Handle = NULL;

#endif