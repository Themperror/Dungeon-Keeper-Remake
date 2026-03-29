#include "utility/break.h"

#include "d3dincl.h"

void Utility::Break()
{
#if _DEBUG
	if (IsDebuggerPresent())
	{
		DebugBreak();
	}
#endif
}