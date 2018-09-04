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

#ifndef VIDEO_SMK_PLAYER_H
#define VIDEO_SMK_PLAYER_H

#include <cstdint>
#include <BitStream.h>
#include <memstream.h>
namespace Smacker 
{
	class BigHuffmanTree;


	template<int depth>
	struct ColorComponent {
	};

	template<>
	struct ColorComponent<0> {
		static inline uint expand(uint value) {
			return 0;
		}
	};

	template<>
	struct ColorComponent<1> {
		static inline uint expand(uint value) {
			value &= 1;
			return value |
				(value << 1) |
				(value << 2) |
				(value << 3) |
				(value << 4) |
				(value << 5) |
				(value << 6) |
				(value << 7);
		}
	};

	template<>
	struct ColorComponent<2> {
		static inline uint expand(uint value) {
			value &= 3;
			return value |
				(value << 2) |
				(value << 4) |
				(value << 6);
		}
	};

	template<>
	struct ColorComponent<3> {
		static inline uint expand(uint value) {
			value &= 7;
			return (value << 5) |
				(value << 2) |
				(value >> 1);
		}
	};

	template<>
	struct ColorComponent<4> {
		static inline uint expand(uint value) {
			value &= 15;
			return value |
				(value << 4);
		}
	};

	template<>
	struct ColorComponent<5> {
		static inline uint expand(uint value) {
			value &= 31;
			return (value << 3) |
				(value >> 2);
		}
	};

	template<>
	struct ColorComponent<6> {
		static inline uint expand(uint value) {
			value &= 63;
			return (value << 2) |
				(value >> 4);
		}
	};

	template<>
	struct ColorComponent<7> {
		static inline uint expand(uint value) {
			value &= 127;
			return (value << 1) |
				(value >> 6);
		}
	};

	template<>
	struct ColorComponent<8> {
		static inline uint expand(uint value) {
			return value & 255;
		}
	};
	struct PixelFormat {
		byte bytesPerPixel; /**< Number of bytes used in the pixel format. */

		byte rLoss, gLoss, bLoss, aLoss; /**< Precision loss of each color component. */
		byte rShift, gShift, bShift, aShift; /**< Binary left shift of each color component in the pixel value. */

		inline PixelFormat() {
			bytesPerPixel =
				rLoss = gLoss = bLoss = aLoss =
				rShift = gShift = bShift = aShift = 0;
		}

		inline PixelFormat(byte BytesPerPixel,
			byte RBits, byte GBits, byte BBits, byte ABits,
			byte RShift, byte GShift, byte BShift, byte AShift) {
			bytesPerPixel = BytesPerPixel;
			rLoss = 8 - RBits;
			gLoss = 8 - GBits;
			bLoss = 8 - BBits;
			aLoss = 8 - ABits;
			rShift = RShift;
			gShift = GShift;
			bShift = BShift;
			aShift = AShift;
		}

		static inline PixelFormat createFormatCLUT8() {
			return PixelFormat(1, 0, 0, 0, 0, 0, 0, 0, 0);
		}

		inline bool operator==(const PixelFormat &fmt) const {
			// TODO: If aLoss==8, then the value of aShift is irrelevant, and should be ignored.
			return bytesPerPixel == fmt.bytesPerPixel &&
				rLoss == fmt.rLoss &&
				gLoss == fmt.gLoss &&
				bLoss == fmt.bLoss &&
				aLoss == fmt.aLoss &&
				rShift == fmt.rShift &&
				gShift == fmt.gShift &&
				bShift == fmt.bShift &&
				aShift == fmt.aShift;
		}

		inline bool operator!=(const PixelFormat &fmt) const {
			return !(*this == fmt);
		}

		inline uint32 RGBToColor(uint8 r, uint8 g, uint8 b) const {
			return
				((0xFF >> aLoss) << aShift) |
				((r >> rLoss) << rShift) |
				((g >> gLoss) << gShift) |
				((b >> bLoss) << bShift);
		}

		inline uint32 ARGBToColor(uint8 a, uint8 r, uint8 g, uint8 b) const {
			return
				((a >> aLoss) << aShift) |
				((r >> rLoss) << rShift) |
				((g >> gLoss) << gShift) |
				((b >> bLoss) << bShift);
		}

		inline void colorToRGB(uint32 color, uint8 &r, uint8 &g, uint8 &b) const {
			r = expand(rBits(), color >> rShift);
			g = expand(gBits(), color >> gShift);
			b = expand(bBits(), color >> bShift);
		}

		inline void colorToARGB(uint32 color, uint8 &a, uint8 &r, uint8 &g, uint8 &b) const {
			a = (aBits() == 0) ? 0xFF : expand(aBits(), color >> aShift);
			r = expand(rBits(), color >> rShift);
			g = expand(gBits(), color >> gShift);
			b = expand(bBits(), color >> bShift);
		}

		//////////////////////////////////////////////////////////////////////
		// Convenience functions for getting number of color component bits //
		//////////////////////////////////////////////////////////////////////

		inline byte rBits() const {
			return (8 - rLoss);
		}

		inline byte gBits() const {
			return (8 - gLoss);
		}

		inline byte bBits() const {
			return (8 - bLoss);
		}

		inline byte aBits() const {
			return (8 - aLoss);
		}

		inline byte bpp() const {
			return rBits() + gBits() + bBits() + aBits();
		}

		////////////////////////////////////////////////////////////////////////
		// Convenience functions for getting color components' maximum values //
		////////////////////////////////////////////////////////////////////////

		inline uint rMax() const {
			return (1 << rBits()) - 1;
		}

		inline uint gMax() const {
			return (1 << gBits()) - 1;
		}

		inline uint bMax() const {
			return (1 << bBits()) - 1;
		}

		inline uint aMax() const {
			return (1 << aBits()) - 1;
		}

		/** Expand a given bit-depth component to a full 8-bit component */
		static inline uint expand(uint bits, uint color) {
			switch (bits) {
			case 0:
				return ColorComponent<0>::expand(color);
			case 1:
				return ColorComponent<1>::expand(color);
			case 2:
				return ColorComponent<2>::expand(color);
			case 3:
				return ColorComponent<3>::expand(color);
			case 4:
				return ColorComponent<4>::expand(color);
			case 5:
				return ColorComponent<5>::expand(color);
			case 6:
				return ColorComponent<6>::expand(color);
			case 7:
				return ColorComponent<7>::expand(color);
			case 8:
				return ColorComponent<8>::expand(color);
			}

			// Unsupported
			return 0;
		}
	};
	struct Surface {
		/*
		* IMPORTANT implementation specific detail:
		*
		* ARM code relies on the layout of the first 3 of these fields. Do not
		* change them.
		*/

		/**
		* The width of the surface.
		*/
		uint16 w;

		/**
		* The height of the surface.
		*/
		uint16 h;

		/**
		* The number of bytes a pixel line has.
		*
		* Note that this might not equal w * bytesPerPixel.
		*/
		uint16 pitch;

	protected:
		/**
		* The surface's pixel data.
		*/
		void *pixels;

	public:
		/**
		* The pixel format of the surface.
		*/
		PixelFormat format;

		/**
		* Construct a simple Surface object.
		*/
		Surface() : w(0), h(0), pitch(0), pixels(0), format() {
		}

		/**
		* Return a pointer to the pixel data.
		*
		* @return Pointer to the pixel data.
		*/
		inline const void *getPixels() const {
			return pixels;
		}

		/**
		* Return a pointer to the pixel data.
		*
		* @return Pointer to the pixel data.
		*/
		inline void *getPixels() {
			return pixels;
		}

		/**
		* Sets the pixel data.
		*
		* Note that this is a simply a setter. Be careful what you are doing!
		*
		* @param newPixels The new pixel data.
		*/
		void setPixels(void *newPixels) { pixels = newPixels; }

		/**
		* Return a pointer to the pixel at the specified point.
		*
		* @param x The x coordinate of the pixel.
		* @param y The y coordinate of the pixel.
		* @return Pointer to the pixel.
		*/
		inline const void *getBasePtr(int x, int y) const {
			return (const byte *)(pixels)+y * pitch + x * format.bytesPerPixel;
		}

		/**
		* Return a pointer to the pixel at the specified point.
		*
		* @param x The x coordinate of the pixel.
		* @param y The y coordinate of the pixel.
		* @return Pointer to the pixel.
		*/
		inline void *getBasePtr(int x, int y) {
			return static_cast<byte *>(pixels) + y * pitch + x * format.bytesPerPixel;
		}

		/**
		* Allocate memory for the pixel data of the surface.
		*
		* Note that you are responsible for calling free yourself.
		* @see free
		*
		* @param width Width of the surface object.
		* @param height Height of the surface object.
		* @param format The pixel format the surface should use.
		*/
		void create(uint16 width, uint16 height, const PixelFormat &f) {
			free();

			w = width;
			h = height;
			format = f;
			pitch = w * format.bytesPerPixel;

			if (width && height) {
				pixels = calloc(width * height, format.bytesPerPixel);
			}
		}

		void free()
		{
			::free(pixels);
			pixels = 0;
			w = h = pitch = 0;
			format = PixelFormat();
		}

		/**
		* Set up the Surface with user specified data.
		*
		* Note that this simply sets the 'internal' attributes of the Surface. It
		* will not take care of freeing old data via free or similar!
		*
		* @param width Width of the pixel data.
		* @param height Height of the pixel data.
		* @param pitch The pitch of the pixel data.
		* @param pixels The pixel data itself.
		* @param format The pixel format of the pixel data.
		*/
		void init(uint16 width, uint16 height, uint16 pitch, void *pixels, const PixelFormat &format);

	};
	enum AudioCompression {
		kCompressionNone,
		kCompressionDPCM,
		kCompressionRDFT,
		kCompressionDCT
	};
	struct AudioInfo {
		AudioCompression compression;
		bool hasAudio;
		bool is16Bits;
		bool isStereo;
		uint32_t sampleRate;
	};
	class SmackerAudioTrack
	{
	public:
		SmackerAudioTrack(const AudioInfo &audioInfo);
		~SmackerAudioTrack();
		void DecompressDPCM(unsigned char *buffer, uint32_t bufferSize, uint32_t unpackedSize);
		AudioInfo _audioInfo;
		unsigned char* _buffer;
		uint32_t _bufferSize;
	};
	class SmackerVideoTrack
	{
	public:
		SmackerVideoTrack(uint32_t width, uint32_t height, uint32_t frameCount, float frameRate, uint32_t flags, uint32_t signature);
		~SmackerVideoTrack();

		bool isRewindable() const { return true; }
		bool rewind() { _curFrame = -1; return true; }

		uint16_t getWidth() const;
		uint16_t getHeight() const;
		PixelFormat getPixelFormat() const;
		int getCurFrame() const { return _curFrame; }
		int getFrameCount() const { return _frameCount; }
		const Surface *decodeNextFrame() { return _surface; }
		const byte *getPalette() const { _dirtyPalette = false; return _palette; }
		bool hasDirtyPalette() const { return _dirtyPalette; }

		bool endOfTrack()
		{
			return _curFrame == _frameCount;
		}
		void readTrees(BitStreamMemory8LSB &bs, uint32 mMapSize, uint32 mClrSize, uint32 fullSize, uint32 typeSize);
		void increaseCurFrame() { _curFrame++; }
		void decodeFrame(BitStreamMemory8LSB &bs);
		void unpackPalette(SeekableReadStream *stream);

		float getFrameRate() const { return _frameRate; }

		Surface *_surface;

		float _frameRate;
		uint32 _flags, _signature;

		byte _palette[3 * 256];
		mutable bool _dirtyPalette;

		int _curFrame;
		uint32 _frameCount;

		BigHuffmanTree *_MMapTree;
		BigHuffmanTree *_MClrTree;
		BigHuffmanTree *_FullTree;
		BigHuffmanTree *_TypeTree;

		// Possible runs of blocks
		static uint getBlockRun(int index) { return (index <= 58) ? index + 1 : 128 << (index - 59); }
	};
	class SmackerDecoder
	{
	public:
		SmackerDecoder();
		~SmackerDecoder();

		bool loadStream(SeekableReadStream *stream);
		void close();
		bool rewind();
		void readNextPacket();
		bool supportsAudioTrackSwitching() const { return true; }
		SmackerAudioTrack* getAudioTrack(int index) { return audio[index]; }
		SmackerVideoTrack* getVideoTrack() { return video; }
		void handleAudioTrack(byte track, uint32 chunkSize, uint32 unpackedSize);

		SmackerVideoTrack *createVideoTrack(uint32 width, uint32 height, uint32 frameCount,  float frameRate, uint32 flags, uint32 signature) const;

		SeekableReadStream *_fileStream;
		
		struct {
			uint32 signature;
			uint32 flags;
			uint32 audioSize[7];
			uint32 treesSize;
			uint32 mMapSize;
			uint32 mClrSize;
			uint32 fullSize;
			uint32 typeSize;
			AudioInfo audioInfo[7];
			uint32 dummy;
		} _header;
		SmackerVideoTrack* video;
		SmackerAudioTrack* audio[7];
		uint32 *_frameSizes;
		// The FrameTypes section of a Smacker file contains an array of bytes, where
		// the 8 bits of each byte describe the contents of the corresponding frame.
		// The highest 7 bits correspond to audio frames (bit 7 is track 6, bit 6 track 5
		// and so on), so there can be up to 7 different audio tracks. When the lowest bit
		// (bit 0) is set, it denotes a frame that contains a palette record
		byte *_frameTypes;
		uint32 _firstFrameStart;
	};

} // End of namespace Video

#endif