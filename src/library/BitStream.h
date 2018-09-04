/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

// Based on eos' BitStream implementation
#pragma once

#ifndef FORCEINLINE
#define FORCEINLINE __inline
#endif

#include <stdlib.h>
#include <cstdint>
#include <stdio.h>
#include <memory>
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;
typedef uint32_t uint;
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint8_t byte;
typedef uint8_t uint8;
typedef int8_t int8;

__inline uint16 READ_UINT16(const void *ptr) {
	return *(const uint16 *)(ptr);
}

__inline uint32 READ_UINT32(const void *ptr) {
	return *(const uint32 *)(ptr);
}

__inline void WRITE_UINT16(void *ptr, uint16 value) {
	*(uint16 *)(ptr) = value;
}

__inline void WRITE_UINT32(void *ptr, uint32 value) {
	*(uint32 *)(ptr) = value;
}

__inline uint64 READ_UINT64(const void *ptr) {
	return *(const uint64 *)(ptr);
}

__inline void WRITE_UINT64(void *ptr, uint64 value) {
	*(uint64 *)(ptr) = value;
}
__inline uint32 SWAP_BYTES_32(uint32 a) {
	return _byteswap_ulong(a);
}
__inline uint16 SWAP_BYTES_16(const uint16 a) {
	return (a >> 8) | (a << 8);
}
__inline uint32 READ_BE_UINT32(const void *ptr) {
	return SWAP_BYTES_32(READ_UINT32(ptr));
}
__inline uint64 SWAP_BYTES_64(uint64 a) {
	return _byteswap_uint64(a);
}
inline void WRITE_BE_UINT16(void *ptr, uint16 value) {
	WRITE_UINT16(ptr, SWAP_BYTES_16(value));
}

#define READ_LE_UINT16(a) READ_UINT16(a)
#define READ_LE_UINT32(a) READ_UINT32(a)

#define WRITE_LE_UINT16(a, v) WRITE_UINT16(a, v)
#define WRITE_LE_UINT32(a, v) WRITE_UINT32(a, v)

#define FROM_LE_32(a) ((uint32)(a))
#define FROM_LE_16(a) ((uint16)(a))

#define FROM_BE_32(a) SWAP_BYTES_32(a)
#define FROM_BE_16(a) SWAP_BYTES_16(a)

#define TO_LE_32(a) ((uint32)(a))
#define TO_LE_16(a) ((uint16)(a))

#define TO_BE_32(a) SWAP_BYTES_32(a)
#define TO_BE_16(a) SWAP_BYTES_16(a)

#define CONSTANT_LE_32(a) ((uint32)(a))
#define CONSTANT_LE_16(a) ((uint16)(a))

#define CONSTANT_BE_32(a) SWAP_CONSTANT_32(a)
#define CONSTANT_BE_16(a) SWAP_CONSTANT_16(a)

#define READ_LE_UINT64(a) READ_UINT64(a)
#define WRITE_LE_UINT64(a, v) WRITE_UINT64(a, v)
#define FROM_LE_64(a) ((uint64)(a))
#define FROM_BE_64(a) SWAP_BYTES_64(a)
#define TO_LE_64(a) ((uint64)(a))
#define TO_BE_64(a) SWAP_BYTES_64(a)
#define CONSTANT_LE_64(a) ((uint64)(a))
#define CONSTANT_BE_64(a) SWAP_CONSTANT_64(a)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

namespace Smacker 
{
	/**
	* Virtual base class for both ReadStream and WriteStream.
	*/
	class SeekableReadStream;
	class Stream {
	public:
		virtual ~Stream() {}

		/**
		* Returns true if an I/O failure occurred.
		* This flag is never cleared automatically. In order to clear it,
		* client code has to call clearErr() explicitly.
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C ferror().
		*/
		virtual bool err() const { return false; }

		/**
		* Reset the I/O error status as returned by err().
		* For a ReadStream, also reset the end-of-stream status returned by eos().
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C clearerr().
		*/
		virtual void clearErr() {}
	};

	/**
	* Generic interface for a writable data stream.
	*/
	class WriteStream : virtual public Stream {
	public:
		/**
		* Write data into the stream. Subclasses must implement this
		* method; all other write methods are implemented using it.
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C fwrite().
		*
		* @param dataPtr	pointer to the data to be written
		* @param dataSize	number of bytes to be written
		* @return the number of bytes which were actually written.
		*/
		virtual uint32 write(const void *dataPtr, uint32 dataSize) = 0;

		/**
		* Commit any buffered data to the underlying channel or
		* storage medium; unbuffered streams can use the default
		* implementation.
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C fflush().
		*
		* @return true on success, false in case of a failure
		*/
		virtual bool flush() { return true; }

		/**
		* Finalize and close this stream. To be called right before this
		* stream instance is deleted. The goal here is to enable calling
		* code to detect and handle I/O errors which might occur when
		* closing (and this flushing, if buffered) the stream.
		*
		* After this method has been called, no further writes may be
		* performed on the stream. Calling err() is allowed.
		*
		* By default, this just flushes the stream.
		*/
		virtual void finalize() {
			flush();
		}

		/**
		* Obtains the current value of the stream position indicator of the
		* stream.
		*
		* @return the current position indicator, or -1 if an error occurred.
		*/
		virtual int32 pos() const = 0;


		// The remaining methods all have default implementations; subclasses
		// need not (and should not) overload them.

		void writeByte(byte value) {
			write(&value, 1);
		}

		void writeSByte(int8 value) {
			write(&value, 1);
		}

		void writeUint16LE(uint16 value) {
			value = TO_LE_16(value);
			write(&value, 2);
		}

		void writeUint32LE(uint32 value) {
			value = TO_LE_32(value);
			write(&value, 4);
		}

		void writeUint64LE(uint64 value) {
			value = TO_LE_64(value);
			write(&value, 8);
		}

		void writeUint16BE(uint16 value) {
			value = TO_BE_16(value);
			write(&value, 2);
		}

		void writeUint32BE(uint32 value) {
			value = TO_BE_32(value);
			write(&value, 4);
		}

		void writeUint64BE(uint64 value) {
			value = TO_BE_64(value);
			write(&value, 8);
		}

		FORCEINLINE void writeSint16LE(int16 value) {
			writeUint16LE((uint16)value);
		}

		FORCEINLINE void writeSint32LE(int32 value) {
			writeUint32LE((uint32)value);
		}

		FORCEINLINE void writeSint64LE(int64 value) {
			writeUint64LE((uint64)value);
		}

		FORCEINLINE void writeSint16BE(int16 value) {
			writeUint16BE((uint16)value);
		}

		FORCEINLINE void writeSint32BE(int32 value) {
			writeUint32BE((uint32)value);
		}

		FORCEINLINE void writeSint64BE(int64 value) {
			writeUint64BE((uint64)value);
		}


		/**
		* Write the given 32-bit floating point value stored
		* in little endian(LSB first) order into the stream.
		*/
		FORCEINLINE void writeFloatLE(float value) {
			uint32 n;

			memcpy(&n, &value, 4);

			writeUint32LE(n);
		}


		/**
		* Write the given 32-bit floating point value stored
		* in big endian order into the stream.
		*/
		FORCEINLINE void writeFloatBE(float value) {
			uint32 n;

			memcpy(&n, &value, 4);

			writeUint32BE(n);
		}
	};

	/**
	* Generic interface for a readable data stream.
	*/
	class ReadStream : virtual public Stream {
	public:
		/**
		* Returns true if a read failed because the stream end has been reached.
		* This flag is cleared by clearErr().
		* For a SeekableReadStream, it is also cleared by a successful seek.
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C feof(). In particular, in a stream
		* with N bytes, reading exactly N bytes from the start should *not*
		* set eos; only reading *beyond* the available data should set it.
		*/
		virtual bool eos() const = 0;

		/**
		* Read data from the stream. Subclasses must implement this
		* method; all other read methods are implemented using it.
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C fread(), in particular where
		* it concerns setting error and end of file/stream flags.
		*
		* @param dataPtr	pointer to a buffer into which the data is read
		* @param dataSize	number of bytes to be read
		* @return the number of bytes which were actually read.
		*/
		virtual uint32 read(void *dataPtr, uint32 dataSize) = 0;


		// The remaining methods all have default implementations; subclasses
		// in general should not overload them.

		/**
		* Read an unsigned byte from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		byte readByte() {
			byte b = 0; // FIXME: remove initialisation
			read(&b, 1);
			return b;
		}

		/**
		* Read a signed byte from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int8 readSByte() {
			return (int8)readByte();
		}

		/**
		* Read an unsigned 16-bit word stored in little endian (LSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		uint16 readUint16LE() {
			uint16 val;
			read(&val, 2);
			return FROM_LE_16(val);
		}

		/**
		* Read an unsigned 32-bit word stored in little endian (LSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		uint32 readUint32LE() {
			uint32 val;
			read(&val, 4);
			return FROM_LE_32(val);
		}

		/**
		* Read an unsigned 64-bit word stored in little endian (LSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		uint64 readUint64LE() {
			uint64 val;
			read(&val, 8);
			return FROM_LE_64(val);
		}

		/**
		* Read an unsigned 16-bit word stored in big endian (MSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		uint16 readUint16BE() {
			uint16 val;
			read(&val, 2);
			return FROM_BE_16(val);
		}

		/**
		* Read an unsigned 32-bit word stored in big endian (MSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		uint32 readUint32BE() {
			uint32 val;
			read(&val, 4);
			return FROM_BE_32(val);
		}

		/**
		* Read an unsigned 64-bit word stored in big endian (MSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		uint64 readUint64BE() {
			uint64 val;
			read(&val, 8);
			return FROM_BE_64(val);
		}

		/**
		* Read a signed 16-bit word stored in little endian (LSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int16 readSint16LE() {
			return (int16)readUint16LE();
		}

		/**
		* Read a signed 32-bit word stored in little endian (LSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int32 readSint32LE() {
			return (int32)readUint32LE();
		}

		/**
		* Read a signed 64-bit word stored in little endian (LSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int64 readSint64LE() {
			return (int64)readUint64LE();
		}

		/**
		* Read a signed 16-bit word stored in big endian (MSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int16 readSint16BE() {
			return (int16)readUint16BE();
		}

		/**
		* Read a signed 32-bit word stored in big endian (MSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int32 readSint32BE() {
			return (int32)readUint32BE();
		}

		/**
		* Read a signed 64-bit word stored in big endian (MSB first) order
		* from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE int64 readSint64BE() {
			return (int64)readUint64BE();
		}

		/**
		* Read a 32-bit floating point value stored in little endian (LSB first)
		* order from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE float readFloatLE() {
			uint32 n = readUint32LE();
			float f;

			memcpy(&f, &n, 4);

			return f;
		}

		/**
		* Read a 32-bit floating point value stored in big endian
		* order from the stream and return it.
		* Performs no error checking. The return value is undefined
		* if a read error occurred (for which client code can check by
		* calling err() and eos() ).
		*/
		FORCEINLINE float readFloatBE() {
			uint32 n = readUint32BE();
			float f;

			memcpy(&f, &n, 4);

			return f;
		}

		/**
		* Read the specified amount of data into a malloc'ed buffer
		* which then is wrapped into a MemoryReadStream.
		* The returned stream might contain less data than requested,
		* if reading more failed, because of an I/O error or because
		* the end of the stream was reached. Which can be determined by
		* calling err() and eos().
		*/
		SeekableReadStream *readStream(uint32 dataSize);

	};


	/**
	* Interface for a seekable & readable data stream.
	*
	* @todo Get rid of SEEK_SET, SEEK_CUR, or SEEK_END, use our own constants
	*/
	class SeekableReadStream : virtual public ReadStream {
	public:

		/**
		* Obtains the current value of the stream position indicator of the
		* stream.
		*
		* @return the current position indicator, or -1 if an error occurred.
		*/
		virtual int32 pos() const = 0;

		/**
		* Obtains the total size of the stream, measured in bytes.
		* If this value is unknown or can not be computed, -1 is returned.
		*
		* @return the size of the stream, or -1 if an error occurred
		*/
		virtual int32 size() const = 0;

		/**
		* Sets the stream position indicator for the stream. The new position,
		* measured in bytes, is obtained by adding offset bytes to the position
		* specified by whence. If whence is set to SEEK_SET, SEEK_CUR, or
		* SEEK_END, the offset is relative to the start of the file, the current
		* position indicator, or end-of-file, respectively. A successful call
		* to the seek() method clears the end-of-file indicator for the stream.
		*
		* @note The semantics of any implementation of this method are
		* supposed to match those of ISO C fseek().
		*
		* @param offset	the relative offset in bytes
		* @param whence	the seek reference: SEEK_SET, SEEK_CUR, or SEEK_END
		* @return true on success, false in case of a failure
		*/
		virtual bool seek(int32 offset, int whence = SEEK_SET) = 0;

		/**
		* TODO: Get rid of this??? Or keep it and document it
		* @return true on success, false in case of a failure
		*/
		virtual bool skip(uint32 offset) { return seek(offset, SEEK_CUR); }

		/**
		* Reads at most one less than the number of characters specified
		* by bufSize from the and stores them in the string buf. Reading
		* stops when the end of a line is reached (CR, CR/LF or LF), and
		* at end-of-file or error. The newline, if any, is retained (CR
		* and CR/LF are translated to LF = 0xA = '\n'). If any characters
		* are read and there is no error, a `\0' character is appended
		* to end the string.
		*
		* Upon successful completion, return a pointer to the string. If
		* end-of-file occurs before any characters are read, returns NULL
		* and the buffer contents remain unchanged.  If an error occurs,
		* returns NULL and the buffer contents are indeterminate.
		* This method does not distinguish between end-of-file and error;
		* callers must use err() or eos() to determine which occurred.
		*
		* @note This methods is closely modeled after the standard fgets()
		*       function from stdio.h.
		*
		* @param s	the buffer to store into
		* @param bufSize	the size of the buffer
		* @return a pointer to the read string, or NULL if an error occurred
		*/
		virtual char *readLine(char *s, size_t bufSize);
	};

	/**
	* This is a ReadStream mixin subclass which adds non-endian read
	* methods whose endianness is set during the stream creation.
	*/
	class ReadStreamEndian : virtual public ReadStream {
	private:
		const bool _bigEndian;

	public:
		ReadStreamEndian(bool bigEndian) : _bigEndian(bigEndian) {}

		bool isBE() const { return _bigEndian; }

		uint16 readUint16() {
			uint16 val;
			read(&val, 2);
			return (_bigEndian) ? TO_BE_16(val) : TO_LE_16(val);
		}

		uint32 readUint32() {
			uint32 val;
			read(&val, 4);
			return (_bigEndian) ? TO_BE_32(val) : TO_LE_32(val);
		}

		uint64 readUint64() {
			uint64 val;
			read(&val, 8);
			return (_bigEndian) ? TO_BE_64(val) : TO_LE_64(val);
		}

		FORCEINLINE int16 readSint16() {
			return (int16)readUint16();
		}

		FORCEINLINE int32 readSint32() {
			return (int32)readUint32();
		}

		FORCEINLINE int64 readSint64() {
			return (int64)readUint64();
		}
	};

	/**
	* This is a SeekableReadStream subclass which adds non-endian read
	* methods whose endianness is set during the stream creation.
	*/
	class SeekableReadStreamEndian : public SeekableReadStream, public ReadStreamEndian {
	public:
		SeekableReadStreamEndian(bool bigEndian) : ReadStreamEndian(bigEndian) {}
	};
/**
 * A template implementing a bit stream for different data memory layouts.
 *
 * Such a bit stream reads valueBits-wide values from the data stream and
 * gives access to their bits, one at a time.
 *
 * For example, a bit stream with the layout parameters 32, true, false
 * for valueBits, isLE and isMSB2LSB, reads 32bit little-endian values
 * from the data stream and hands out the bits in the order of LSB to MSB.
 */
template<class STREAM, int valueBits, bool isLE, bool isMSB2LSB>
class BitStreamImpl {
private:
	STREAM *_stream;			///< The input stream.
	bool _disposeAfterUse; ///< Should we delete the stream on destruction?

	uint32 _value;   ///< Current value.
	uint8  _inValue; ///< Position within the current value.
	uint32 _size;    ///< Total bitstream size (in bits)
	uint32 _pos;     ///< Current bitstream position (in bits)

	/** Read a data value. */
	inline uint32 readData() {
		if (isLE) {
			if (valueBits ==  8)
				return _stream->readByte();
			if (valueBits == 16)
				return _stream->readUint16LE();
			if (valueBits == 32)
				return _stream->readUint32LE();
		} else {
			if (valueBits ==  8)
				return _stream->readByte();
			if (valueBits == 16)
				return _stream->readUint16BE();
			if (valueBits == 32)
				return _stream->readUint32BE();
		}

		return 0;
	}

	/** Read the next data value. */
	inline void readValue() {

		_value = readData();

		// If we're reading the bits MSB first, we need to shift the value to that position
		if (isMSB2LSB)
			_value <<= 32 - valueBits;
		}

public:
	/** Create a bit stream using this input data stream and optionally delete it on destruction. */
	BitStreamImpl(STREAM *stream, bool disposeAfterUse = false) :
		_stream(stream), _disposeAfterUse(disposeAfterUse), _value(0), _inValue(0), _pos(0) {


		_size = (_stream->size() & ~((uint32) ((valueBits >> 3) - 1))) * 8;
	}

	/** Create a bit stream using this input data stream. */
	BitStreamImpl(STREAM &stream) :
		_stream(&stream), _disposeAfterUse(false), _value(0), _inValue(0), _pos(0) {


		_size = (_stream->size() & ~((uint32) ((valueBits >> 3) - 1))) * 8;
	}

	~BitStreamImpl() {
		if (_disposeAfterUse == true)
			delete _stream;
	}

private:
	uint32 getBit_internal() {
		// Get the current bit
		uint32 b = 0;
		if (isMSB2LSB)
			b = ((_value & 0x80000000) == 0) ? 0 : 1;
		else
			b = ((_value & 1) == 0) ? 0 : 1;

		// Shift to the next bit
		if (isMSB2LSB)
			_value <<= 1;
		else
			_value >>= 1;

		return b;
	}

public:
	/** Read a bit from the bit stream. */
	uint32 getBit() {
		// Check if we need the next value
		if (_inValue == 0)
			readValue();

		uint32 b = getBit_internal();

		// Increase the position within the current value
		_inValue = (_inValue + 1) % valueBits;
		_pos++;

		return b;
	}

	/**
	 * Read a multi-bit value from the bit stream.
	 *
	 * The value is read as if just taken as a whole from the bitstream.
	 *
	 * For example:
	 * Reading a 4-bit value from an 8-bit bitstream with the contents 01010011:
	 * If the bitstream is MSB2LSB, the 4-bit value would be 0101.
	 * If the bitstream is LSB2MSB, the 4-bit value would be 0011.
	 */
	uint32 getBits(uint8 n) {
		if (n == 0)
			return 0;

		if (n > 32)
			printf("BitStreamImpl::getBits(): Too many bits requested to be read");

		// Read the number of bits
		uint32 v = 0;

		uint8 nOrig = n;
		if (_inValue) {
			int count = MIN((int)n, valueBits - _inValue);
			for (int i = 0; i < count; ++i) {
				if (isMSB2LSB) {
					v = (v << 1) | getBit_internal();
				} else {
					v = (v >> 1) | (getBit_internal() << 31);
				}
			}

			n -= count;
		}

		while (n > 0) {
			// NB: readValue doesn't care that _inValue is incorrect here
			readValue();

			int count = MIN((int)n, valueBits);
			for (int i = 0; i < count; ++i) {
				if (isMSB2LSB) {
					v = (v << 1) | getBit_internal();
				} else {
					v = (v >> 1) | (getBit_internal() << 31);
				}
			}

			n -= count;
		}

		_inValue = (_inValue + nOrig) % valueBits;
		_pos += nOrig;

		if (!isMSB2LSB)
			v >>= (32 - nOrig);

		return v;
	}

	/** Read a bit from the bit stream, without changing the stream's position. */
	uint32 peekBit() {
		uint32 value   = _value;
		uint8  inValue = _inValue;
		uint32 curStreamPos  = _stream->pos();
		uint32 curPos = _pos;

		uint32 v = getBit();

		_pos     = curPos;
		_stream->seek(curStreamPos);
		_inValue = inValue;
		_value   = value;

		return v;
	}

	/**
	 * Read a multi-bit value from the bit stream, without changing the stream's position.
	 *
	 * The bit order is the same as in getBits().
	 */
	uint32 peekBits(uint8 n) {
		uint32 value   = _value;
		uint8  inValue = _inValue;
		uint32 curStreamPos  = _stream->pos();
		uint32 curPos = _pos;

		uint32 v = getBits(n);

		_pos     = curPos;
		_stream->seek(curStreamPos);
		_inValue = inValue;
		_value   = value;

		return v;
	}

	/**
	 * Add a bit to the value x, making it an n+1-bit value.
	 *
	 * The current value is shifted and the bit is added to the
	 * appropriate place, dependant on the stream's bitorder.
	 *
	 * For example:
	 * A bit y is added to the value 00001100 with size 4.
	 * If the stream's bitorder is MSB2LSB, the resulting value is 0001100y.
	 * If the stream's bitorder is LSB2MSB, the resulting value is 000y1100.
	 */
	void addBit(uint32 &x, uint32 n) {
		if (n >= 32)
			error("BitStreamImpl::addBit(): Too many bits requested to be read");

		if (isMSB2LSB)
			x = (x << 1) | getBit();
		else
			x = (x & ~(1 << n)) | (getBit() << n);
	}

	/** Rewind the bit stream back to the start. */
	void rewind() {
		_stream->seek(0);

		_value   = 0;
		_inValue = 0;
		_pos     = 0;
	}

	/** Skip the specified amount of bits. */
	void skip(uint32 n) {
		while (n-- > 0)
			getBit();
	}

	/** Skip the bits to closest data value border. */
	void align() {
		while (_inValue)
			getBit();
	}

	/** Return the stream position in bits. */
	uint32 pos() const {
		return _pos;
	}

	/** Return the stream size in bits. */
	uint32 size() const {
		return _size;
	}

	bool eos() const {
		return _stream->eos() || (_pos >= _size);
	}
};



/**
 * A cut-down version of MemoryReadStream specifically for use with BitStream.
 * It removes the virtual call overhead for reading bytes from a memory buffer,
 * and allows directly inlining this access.
 *
 * The code duplication with MemoryReadStream is not ideal.
 * It might be possible to avoid this by making this a final subclass of
 * MemoryReadStream, but that is a C++11 feature.
 */
class BitStreamMemoryStream {
private:
	const byte * const _ptrOrig;
	const byte *_ptr;
	const uint32 _size;
	uint32 _pos;
	bool _disposeMemory;
	bool _eos;

public:
	BitStreamMemoryStream(const byte *dataPtr, uint32 dataSize, bool disposeMemory = false) :
		_ptrOrig(dataPtr),
		_ptr(dataPtr),
		_size(dataSize),
		_pos(0),
		_disposeMemory(disposeMemory),
		_eos(false) {}

	~BitStreamMemoryStream()
	{
		if (_disposeMemory)
		{
			free(const_cast<byte *>(_ptrOrig));
		}
	}

	bool eos() const {
		return _eos;
	}

	bool err() const {
		return false;
	}

	int32 pos() const {
		return _pos;
	}

	int32 size() const {
		return _size;
	}

	bool seek(uint32 offset) {
		_eos = false;
		_pos = offset;
		_ptr = _ptrOrig + _pos;
		return true;
	}

	byte readByte() {
		if (_pos >= _size) {
			_eos = true;
			return 0;
		}

		_pos++;
		return *_ptr++;
	}

	uint16 readUint16LE() {
		if (_pos + 2 > _size) {
			_eos = true;
			if (_pos < _size) {
				_pos++;
				return *_ptr++;
			} else {
				return 0;
			}
		}

		uint16 val = READ_LE_UINT16(_ptr);

		_pos += 2;
		_ptr += 2;

		return val;
	}

	uint16 readUint16BE() {
		if (_pos + 2 > _size) {
			_eos = true;
			if (_pos < _size) {
				_pos++;
				return (*_ptr++) << 8;
			} else {
				return 0;
			}
		}

		uint16 val = READ_LE_UINT16(_ptr);

		_pos += 2;
		_ptr += 2;

		return val;
	}

	uint32 readUint32LE() {
		if (_pos + 4 > _size) {
			uint32 val = readByte();
			val |= (uint32)readByte() << 8;
			val |= (uint32)readByte() << 16;
			val |= (uint32)readByte() << 24;

			return val;
		}

		uint32 val = READ_LE_UINT32(_ptr);

		_pos += 4;
		_ptr += 4;

		return val;
	}

	uint32 readUint32BE() {
		if (_pos + 4 > _size) {
			uint32 val = (uint32)readByte() << 24;
			val |= (uint32)readByte() << 16;
			val |= (uint32)readByte() << 8;
			val |= (uint32)readByte();

			return val;
		}

		uint32 val = READ_BE_UINT32(_ptr);

		_pos += 4;
		_ptr += 4;

		return val;
	}

};

// typedefs for various memory layouts.

/** 8-bit data, MSB to LSB. */
typedef BitStreamImpl<SeekableReadStream, 8, false, true > BitStream8MSB;
/** 8-bit data, LSB to MSB. */
typedef BitStreamImpl<SeekableReadStream, 8, false, false> BitStream8LSB;

/** 16-bit little-endian data, MSB to LSB. */
typedef BitStreamImpl<SeekableReadStream, 16, true , true > BitStream16LEMSB;
/** 16-bit little-endian data, LSB to MSB. */
typedef BitStreamImpl<SeekableReadStream, 16, true , false> BitStream16LELSB;
/** 16-bit big-endian data, MSB to LSB. */
typedef BitStreamImpl<SeekableReadStream, 16, false, true > BitStream16BEMSB;
/** 16-bit big-endian data, LSB to MSB. */
typedef BitStreamImpl<SeekableReadStream, 16, false, false> BitStream16BELSB;

/** 32-bit little-endian data, MSB to LSB. */
typedef BitStreamImpl<SeekableReadStream, 32, true , true > BitStream32LEMSB;
/** 32-bit little-endian data, LSB to MSB. */
typedef BitStreamImpl<SeekableReadStream, 32, true , false> BitStream32LELSB;
/** 32-bit big-endian data, MSB to LSB. */
typedef BitStreamImpl<SeekableReadStream, 32, false, true > BitStream32BEMSB;
/** 32-bit big-endian data, LSB to MSB. */
typedef BitStreamImpl<SeekableReadStream, 32, false, false> BitStream32BELSB;

/** 8-bit data, MSB to LSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 8, false, true > BitStreamMemory8MSB;
/** 8-bit data, LSB to MSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 8, false, false> BitStreamMemory8LSB;

/** 16-bit little-endian data, MSB to LSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 16, true , true > BitStreamMemory16LEMSB;
/** 16-bit little-endian data, LSB to MSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 16, true , false> BitStreamMemory16LELSB;
/** 16-bit big-endian data, MSB to LSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 16, false, true > BitStreamMemory16BEMSB;
/** 16-bit big-endian data, LSB to MSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 16, false, false> BitStreamMemory16BELSB;

/** 32-bit little-endian data, MSB to LSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 32, true , true > BitStreamMemory32LEMSB;
/** 32-bit little-endian data, LSB to MSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 32, true , false> BitStreamMemory32LELSB;
/** 32-bit big-endian data, MSB to LSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 32, false, true > BitStreamMemory32BEMSB;
/** 32-bit big-endian data, LSB to MSB. */
typedef BitStreamImpl<BitStreamMemoryStream, 32, false, false> BitStreamMemory32BELSB;


} 