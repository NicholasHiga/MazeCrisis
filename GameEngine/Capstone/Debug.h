#pragma once

#ifdef _DEBUG
#include <algorithm>
#ifdef _MSC_VER
# define eTB_CriticalBreakPoint() if (IsDebuggerPresent ()) __debugbreak ();
#else
# define eTB_CriticalBreakPoint() asm (" int $3");
#endif

const char*
ETB_GL_DEBUG_SOURCE_STR(GLenum source)
{
	static const char* sources[] = {
		"API",   "Window System", "Shader Compiler", "Third Party",
		"Application", "Other", "Unknown"
	};

	int str_idx =
		std::min(source - GL_DEBUG_SOURCE_API,
			sizeof(sources) / sizeof(const char *));

	return sources[str_idx];
}

const char*
ETB_GL_DEBUG_TYPE_STR(GLenum type)
{
	static const char* types[] = {
		"Error",       "Deprecated Behavior", "Undefined Behavior",
		"Portability", "Performance", "Other", "Unknown"
	};

	int str_idx =
		std::min(type - GL_DEBUG_TYPE_ERROR,
			sizeof(types) / sizeof(const char *));

	return types[str_idx];
}

const char*
ETB_GL_DEBUG_SEVERITY_STR(GLenum severity)
{
	static const char* severities[] = {
		"High", "Medium", "Low", "Unknown"
	};

	int str_idx =
		std::min(severity - GL_DEBUG_SEVERITY_HIGH,
			sizeof(severities) / sizeof(const char *));

	return severities[str_idx];
}

void
ETB_GL_ERROR_CALLBACK(GLenum        source,
	GLenum        type,
	GLuint        id,
	GLenum        severity,
	GLsizei       length,
	const GLchar* message,
	GLvoid*       userParam)
{
	printf("OpenGL Error:\n");
	printf("=============\n");

	printf(" Object ID: ");
	printf("%d\n", id);

	printf(" Severity:  ");
	printf("%s\n",
		ETB_GL_DEBUG_SEVERITY_STR(severity));

	printf(" Type:      ");
	printf("%s\n", ETB_GL_DEBUG_TYPE_STR(type));

	printf(" Source:    ");
	printf("%s\n", ETB_GL_DEBUG_SOURCE_STR(source));

	printf(" Message:   ");
	printf("%s\n\n", message);

	// Force the console to flush its contents before executing a breakpoint
	//eTB_FlushConsole();

	// Trigger a breakpoint in gDEBugger...
	glFinish();

	// Trigger a breakpoint in traditional debuggers...
	eTB_CriticalBreakPoint();
}
#endif
