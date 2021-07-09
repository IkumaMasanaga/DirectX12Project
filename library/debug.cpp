#include <windows.h>
#include "debug.h"


namespace lib {

	void Debug::trace(const char* str, ...) {
		char buff[256] = { 0 };
		WCHAR wbuff[256] = { 0 };
		va_list argptr;
		va_start(argptr, str);
		vsprintf_s(buff, sizeof(buff), str, argptr);
		va_end(argptr);
		MultiByteToWideChar(CP_ACP, 0, buff, -1, wbuff, 256);
		wbuff[255] = 0;
		OutputDebugString(wbuff);
	}

}
