//Source: Daniel Gehringer, http://stackoverflow.com/a/4821494
//Licensed under CC BY-SA 3.0 Attribution (https://creativecommons.org/licenses/by-sa/3.0/)

#include <streambuf>
#include <iostream>

namespace GA
{
	class CountingStreamBuffer : public std::streambuf
	{
	public:
		// constructor
		CountingStreamBuffer( std::streambuf* sbuf );

		// Get current line number
		unsigned int        lineNumber() const;

		// Get line number of previously read character
		unsigned int        prevLineNumber() const;

		// Get current column
		unsigned int        column() const;

		// Get file position
		std::streamsize     filepos() const;

	protected:
		CountingStreamBuffer( const CountingStreamBuffer& ) = delete;
		CountingStreamBuffer& operator=(const CountingStreamBuffer&) = delete;

		// extract next character from stream w/o advancing read pos
		std::streambuf::int_type underflow();

		// extract next character from stream
		std::streambuf::int_type uflow();

		// put back last character
		std::streambuf::int_type pbackfail( std::streambuf::int_type c );

		// change position by offset, according to way and mode  
		virtual std::ios::pos_type seekoff( std::ios::off_type pos,
			std::ios_base::seekdir dir,
			std::ios_base::openmode mode );

		// change to specified position, according to mode
		virtual std::ios::pos_type seekpos( std::ios::pos_type pos,
			std::ios_base::openmode mode );


	private:
		std::streambuf*     streamBuf_;     // hosted streambuffer
		unsigned int        lineNumber_;    // current line number
		unsigned int        lastLineNumber_;// line number of last read character
		unsigned int        column_;        // current column
		unsigned int        prevColumn_;    // previous column
		std::streamsize     filePos_;       // file position
	};
}