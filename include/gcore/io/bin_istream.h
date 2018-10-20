//
// => gcore/io/bin_istream.h
//
//                                 GraphCore
//
// Copyright (c) 2018 Lorenzo Laneve
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef __graphcore_io_bin_istream
#define __graphcore_io_bin_istream

#include <cstdio>

#include <cstdint>
#include <cstring>

#include <glm/glm.hpp>

#define MAX_BUFF_SIZE 8192

namespace gcore {
    
    /*!
     \brief A single byte type (unsigned integer).
     */
    typedef uint8_t byte_t;
    
    /*!
     \brief Class implementing a binary input stream and convenience method to read numbers in big endian.
     */
    class BinaryInputStream {
        /*!
         \brief The C file pointer used to read the file.
         */
        FILE *fp;
        /*!
         \brief The pointer to the byte buffer.
         */
        byte_t *buf;
        /*!
         \brief The pointer to the next byte that has to be read.
         */
        byte_t *bufptr;
        /*!
         \brief The number of valid bytes left in the buffer to read.
         */
        size_t leftBytes;
        
    public:
        /*!
         \brief Initializes the stream with the file pointer to the given C file pointer.
         */
        explicit BinaryInputStream(FILE *fp) : fp(fp) {
            if (fp) {
                buf = new byte_t[MAX_BUFF_SIZE];
                leftBytes = 0;
            }
        }
        
        /*!
         \brief Initializes the stream with the file at the given filename.
         */
        explicit BinaryInputStream(const char *filename) : BinaryInputStream(fopen(filename, "rb")) {  }
        
        ~BinaryInputStream() {
            if (fp) {
                fclose(fp);
            }
            if (buf) {
                delete buf;
            }
        }
        
        BinaryInputStream &operator=(BinaryInputStream &) = delete;
        BinaryInputStream(BinaryInputStream &) = delete;
        
        
    private:
        /*!
         \brief Loads the next bytes in the file, until the buffer is full or the source runs out of bytes.
         */
        void load() {
            if (leftBytes > 0) {
                memcpy(buf, bufptr, sizeof(byte_t) * leftBytes);
            }
            
            leftBytes += fread(buf + leftBytes, sizeof(byte_t), MAX_BUFF_SIZE - leftBytes, fp);
            bufptr = buf;
        }
        
    public:
        
        /*!
         \brief Returns whether the stream is ready to read bytes
         */
        inline bool good() const {
            return fp != nullptr;
        }
        /*!
         \brief Returns whether the stream source has ran out of bytes to read.
         */
        inline bool eof() const {
            return feof(fp) && !leftBytes;
        }
        

        /*!
         \brief Returns a pointer to the stream buffer containing the next \c bytes bytes in the stream. If there are not enough bytes, then new data from the source is requested.
         */
        const byte_t *read(size_t bytes) {
            if (leftBytes < bytes) {
                load();
            }
            
            byte_t *ret = bufptr;
            bufptr += bytes;
            leftBytes -= bytes;
            
            return ret;
        }
        /*!
         \brief Returns the next byte from the buffer as unsigned integer type.
         */
        inline uint8_t readByte() {
            return *read(1);
        }
        /*!
         \brief Returns the next two bytes from the buffer as unsigned integer type, encoded as big endian.
         */
        uint16_t readInt16() {
            const byte_t *data = read(2);
            return ((uint32_t)data[0] << 8) | data[1];
        }
        /*!
         \brief Returns the next four bytes from the buffer as unsigned integer type, encoded as big endian.
         */
        uint32_t readInt32() {
            const byte_t *data = read(4);
            return ((uint32_t)data[0] << 24) |
            ((uint32_t)data[1] << 16) |
            ((uint32_t)data[2] << 8) | data[3];
        }
        /*!
         \brief Returns the next four bytes from the buffer as floating point type, encoded as big endian.
         */
        float readFloat() {
            uint32_t ix = readInt32();
            return *((float *)&ix);
        }
        /*!
         \brief Returns a \c glm::mat4 with the next 16 floats from the buffer, encoded as big endian. The layout of the floats for the matrix is column-major.
         */
        inline glm::mat4 readMat4() {
            return glm::mat4(readFloat(), readFloat(), readFloat(), readFloat(),
                             readFloat(), readFloat(), readFloat(), readFloat(),
                             readFloat(), readFloat(), readFloat(), readFloat(),
                             readFloat(), readFloat(), readFloat(), readFloat());
        }

    };
    
    
}

#endif
