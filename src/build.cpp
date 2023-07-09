#include <windows.h>
#include <direct.h>

#define SYSTEM_LIBRARIES "user32.lib kernel32.lib WinMM.Lib GDI32.lib OpenGL32.lib shell32.lib msvcrt.lib libcmt.lib"
#define LINKER_OPTIONS "/link glfw/glfw3.lib " SYSTEM_LIBRARIES " /LIBPATH:../lib /NODEFAULTLIB:library"
#define HEADER_PATHS "/I../include"
#define LIBRARY_SOURCE_FILES "../lib/glad/glad.c"
#define COMPILER_OUTPUT_DIRS "/Fd../build/stealth.pdb /Fe../build/stealth.exe /Fo../build/"
#define PREPROCESSOR_DEFINITIONS "/DWINAPI_PARTITION_DESKTOP"

#define SOURCE_FILES "main.cpp draw.cpp"

// build with "cl build.cpp"

int main() {
    CreateDirectoryA("../build", NULL);
    int compiled = system(
        "cl.exe /Zi /std:c++20 "
        PREPROCESSOR_DEFINITIONS " "
        COMPILER_OUTPUT_DIRS " " 
        HEADER_PATHS " " 
        SOURCE_FILES " " 
        LIBRARY_SOURCE_FILES " " 
        LINKER_OPTIONS
    );
    if (!compiled) {
        system("..\\build\\stealth.exe");
    }
    return 0;
}
