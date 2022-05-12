#pragma once

class ILocalize
{
public:
	wchar_t* Find(const char* szTokenName)
	{
		return MEM::CallVFunc<wchar_t*>(this, 11, szTokenName);
	}

	const wchar_t* FindSafe(const char* szTokenName)
	{
		return MEM::CallVFunc<wchar_t*>(this, 12, szTokenName);
	}

	int ConvertUnicodeToAnsi(const wchar_t* unicode, char* ansi, int ansiBufferSize)
	{
		return MEM::CallVFunc<int>(this, 16, unicode, ansi, ansiBufferSize);
	}
};

template <std::size_t BufferSize = 4096>
class ToUtf8Converter {
public:
	ToUtf8Converter(ILocalize& localize) : localize{ localize } {}

	[[nodiscard]] std::string ConvertUnicodeToAnsi(const wchar_t* string)
	{
		localize.ConvertUnicodeToAnsi(string, buffer.data(), buffer.size());
		buffer.back() = '\0';
		return buffer.data();
	}

private:
	ILocalize& localize;
	std::array<char, BufferSize> buffer;
};
