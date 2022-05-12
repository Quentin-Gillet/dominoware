#pragma once

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/filesystem.h#L689
class IFileSystem
{
public:
	virtual int				Read(void* pOutput, int size, void* file) = 0;
	// if pathID is NULL, all paths will be searched for the file
	virtual void*			Open(const char* pFileName, const char* pOptions, const char* pathID = 0) = 0;
	virtual void			Close(void* file) = 0;
	virtual unsigned int	Size(void* pFileName) = 0;
};