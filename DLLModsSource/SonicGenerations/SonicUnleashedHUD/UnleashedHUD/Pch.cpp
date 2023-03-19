#include "Pch.h"

#if !_DEBUG
void __cdecl boost::throw_exception(class stdext::exception const&) {}
#endif