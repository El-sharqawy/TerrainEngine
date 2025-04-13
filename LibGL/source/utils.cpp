#include "stdafx.h"
#include "utils.h"

bool IsGLVersionHigher(int MajorVer, int MinorVer)
{
    static int glMajorVersion = 0;
    static int glMinorVersion = 0;

    glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);

    return ((glMajorVersion >= MajorVer) && (glMinorVersion >= MinorVer));
}

std::string GetDirFromFilename(const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex;

#ifdef _WIN64
    SlashIndex = Filename.find_last_of("\\");

    if (SlashIndex == -1)
    {
        SlashIndex = Filename.find_last_of("/");
    }
#else
    SlashIndex = Filename.find_last_of("/");
#endif

    std::string Dir;

    if (SlashIndex == std::string::npos)
    {
        Dir = ".";
    }
    else if (SlashIndex == 0)
    {
        Dir = "/";
    }
    else
    {
        Dir = Filename.substr(0, SlashIndex);
    }

    return Dir;

}

char* ReadBinaryFile(const char* pFilename, int& size)
{
    FILE* f = NULL;

    errno_t err = fopen_s(&f, pFilename, "rb");

    if (!f)
    {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        sys_err("Error opening '%s': %s\n", pFilename, buf);
        exit(0);
    }

    struct stat stat_buf;
    int error = stat(pFilename, &stat_buf);

    if (error)
    {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        sys_err("Error getting file stats: %s\n", buf);
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);

    size_t bytes_read = fread(p, 1, size, f);

    if (bytes_read != size)
    {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        sys_err("Read file error file: %s\n", buf);
        exit(0);
    }

    sys_log("File %s Have Been Readed Successfully with size %d", pFilename, size);
    fclose(f);

    return p;
}

void WriteBinaryFile(const char* pFilename, const void* pData, int size)
{
    FILE* f = NULL;

    errno_t err = fopen_s(&f, pFilename, "wb");

    if (!f)
    {
        sys_err("Error opening '%s'\n", pFilename);
        exit(0);
    }

    size_t bytes_written = fwrite(pData, 1, size, f);

    if (bytes_written != size)
    {
        sys_err("Error write file\n");
        exit(0);
    }

    fclose(f);
}

std::string GetFullPath(const std::string& Dir, const aiString& Path)
{
    std::string p(Path.data);

    if (p == "C:\\\\")
    {
        p = "";
    }
    else if (p.substr(0, 2) == ".\\")
    {
        p = p.substr(2, p.size() - 2);
    }

    std::string FullPath = Dir + "/" + p;

    return FullPath;
}
