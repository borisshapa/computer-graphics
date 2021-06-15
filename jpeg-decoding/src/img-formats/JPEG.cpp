//
// Created by borisshapa on 05.06.2021.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <cassert>
#include <cmath>
#include "JPEG.h"

namespace jpeg {
    typedef std::array<std::array<std::pair<uint8_t, std::vector<uint8_t>>, 16>, 2> DCACHuffmanTables;

    void Log(const std::string &str) {
        std::cout << str << "\n";
    }

    uint8_t ReadMarker(std::istream &in) {
        uint8_t ff_byte = in.get();
        if (ff_byte != BYTE_FF) {
            std::ostringstream error_message;
            error_message << "Expected byte " << BYTE_FF << ", found " << static_cast<int>(ff_byte);
            throw std::runtime_error(error_message.str());
        }
        return in.get();
    }

    uint16_t Read2Bytes(std::istream &in) {
        return (in.get() << 8) + in.get();
    }

    void SkipSegment(std::istream &in) {
        uint16_t size = Read2Bytes(in);
        for (size_t i = 0; i < size - 2; i++) {
            in.get();
        }
    }

    void ReadQuantizationTable(std::istream &in, std::array<uint8_t, 64> &quantization_table) {
        int size = Read2Bytes(in);
        size -= 2;

        while (size > 0) {
            uint8_t table_info = in.get();
            size--;
            uint8_t table_id = table_info & 0x0F;

            if (table_id != 0) {
                std::ostringstream error_message;
                error_message << "Expected table id 0 (only grayscale images are supported), found: "
                              << static_cast<int>(table_id);
                throw std::runtime_error(error_message.str());
            }

            if (table_info >> 4 != 0) {
                std::ostringstream error_message;
                error_message << "Expected represent precision 8 bit (only Baseline JPEG is supported), found: "
                              << static_cast<int>(table_id);
                throw std::runtime_error(error_message.str());
            }

            for (size_t i = 0; i < 64; i++) {
                quantization_table[zig_zag_map[i]] = in.get();
            }
            size -= 64;
        }
    }

    std::pair<uint16_t, uint16_t> ReadStartOfFrame(std::istream &in) {
        Read2Bytes(in);
        uint8_t precision = in.get();
        if (precision != 8) {
            std::ostringstream error_message;
            error_message << "Expected precision 8, found " << static_cast<int>(precision);
            throw std::runtime_error(error_message.str());
        }

        uint16_t height = Read2Bytes(in);
        uint16_t width = Read2Bytes(in);
        uint8_t channel_count = in.get();
        if (channel_count != 1) {
            throw std::runtime_error("Only grayscale images are supported");
        }

        uint8_t component_id = in.get();
        if (component_id != 0 && component_id != 1) {
            std::ostringstream error_message;
            error_message << "Expected component id equals 1, found " << static_cast<int>(component_id);
            throw std::runtime_error(error_message.str());
        }

        in.get(); // sampling factor
        uint8_t quantization_table_id = in.get();
        if (quantization_table_id != 0) {
            std::ostringstream error_message;
            error_message << "Expected quantization id equals 0, found " << static_cast<int>(quantization_table_id);
            throw std::runtime_error(error_message.str());
        }
        return std::make_pair(height, width);
    }

    uint16_t ReadRestartInterval(std::istream &in) {
        Read2Bytes(in);
        return Read2Bytes(in);
    }

    void ReadHuffmanTable(std::istream &in, DCACHuffmanTables &huffman_tables) {
        uint16_t size = Read2Bytes(in);
        size -= 2;

        while (size > 0) {
            uint8_t table_info = in.get();
            uint8_t table_id = table_info & 0x0F;
            if (table_id != 0) {
                throw std::runtime_error("Only grayscale images are supported");
            }
            uint8_t table_type = table_info >> 4;

            size_t total_symbol_count = 0;

            for (size_t i = 0; i < 16; i++) {
                uint8_t code_count = in.get();
                huffman_tables[table_type][i].first = code_count;
                total_symbol_count += code_count;
            }

            size_t symbols = 0;
            size_t code_size = 0;
            while (symbols < total_symbol_count) {
                uint8_t symbol_code = in.get();
                while (huffman_tables[table_type][code_size].first == 0) {
                    code_size++;
                }
                huffman_tables[table_type][code_size].second.push_back(symbol_code);
                symbols++;
                if (huffman_tables[table_type][code_size].first ==
                    huffman_tables[table_type][code_size].second.size()) {
                    code_size++;
                }
            }
            size -= 17 + total_symbol_count;
        }
    }

    void ReadStartOfScan(std::istream &in) {
        Read2Bytes(in); // size

        uint8_t component_count = in.get();
        if (component_count != 1) {
            throw std::runtime_error("Only grayscale images are supported");
        }

        uint8_t component_id = in.get();
        if (component_id != 0 && component_id != 1) {
            std::ostringstream error_message;
            error_message << "Expected component id equals 1, found " << static_cast<int>(component_id);
            throw std::runtime_error(error_message.str());
        }

        uint8_t huffman_table_ids = in.get();
        if (huffman_table_ids != 0) {
            throw std::runtime_error("Only grayscale images are supported");
        }

        uint8_t start_of_selection = in.get();
        uint8_t end_of_selection = in.get();
        if (start_of_selection != 0 || end_of_selection != 63) {
            throw std::runtime_error("Baseline JPEG doesn't use spectral selection");
        }

        uint8_t successive_approximation = in.get();
        if (successive_approximation != 0) {
            throw std::runtime_error("Baseline JPEG doesn't use successive approximation");
        }
    }

    struct BitReader {
        explicit BitReader(std::vector<uint8_t> data) : data_(std::move(data)) {};

        uint8_t ReadBit() {
            if (byte_ind_ >= data_.size()) {
                throw std::runtime_error("Invalid value");
            }
            uint8_t bit = (data_[byte_ind_] >> (7 - bit_ind_)) & 1;
            bit_ind_++;
            if (bit_ind_ == 8) {
                bit_ind_ = 0;
                byte_ind_++;
            }
            return bit;
        }

        size_t ReadBits(size_t count) {
            size_t bits = 0;
            for (size_t i = 0; i < count; i++) {
                uint8_t bit = ReadBit();
                bits = (bits << 1) | bit;
            }
            return bits;
        }

        void NextByte() {
            if (byte_ind_ >= data_.size()) {
                return;
            }
            if (bit_ind_ != 0) {
                bit_ind_ = 0;
                byte_ind_++;
            }
        }

    private:
        size_t byte_ind_ = 0;
        uint8_t bit_ind_ = 0;
        std::vector<uint8_t> data_;
    };


    uint8_t GetNextSymbol(
            const std::array<std::vector<size_t>, 16> &codes,
            const std::array<std::pair<uint8_t, std::vector<uint8_t>>, 16> &huffman_table,
            BitReader &b
    ) {
        size_t code = 0;
        for (size_t i = 0; i < 16; i++) {
            assert(codes[i].size() == huffman_table[i].second.size());

            uint8_t bit = b.ReadBit();
            code = (code << 1) | bit;
            for (size_t j = 0; j < huffman_table[i].first; j++) {
                if (code == codes[i][j]) {
                    return huffman_table[i].second[j];
                }
            }
        }
        throw std::runtime_error("The symbol wasn't found");
    }

    std::vector<std::array<int, 64>> GetMCU(
            const DCACHuffmanTables &huffman_tables,
            const std::vector<uint8_t> &huffman_data,
            size_t height,
            size_t width,
            size_t restart_interval
    ) {
        std::array<std::array<std::vector<size_t>, 16>, 2> codes;
        for (size_t table_type = 0; table_type < huffman_tables.size(); table_type++) {
            size_t code = 0;
            for (size_t i = 0; i < huffman_tables[table_type].size(); i++) {
                for (size_t j = 0; j < huffman_tables[table_type][i].first; j++) {
                    codes[table_type][i].push_back(code);
                    code += 1;
                }
                code <<= 1;
            }
        }

        BitReader b(huffman_data);

        size_t mcu_height = (height + 7) / 8;
        size_t mcu_width = (width + 7) / 8;
        std::vector<std::array<int, 64>> mcu(mcu_height * mcu_width);

        int prev_dc = 0;

        for (size_t i = 0; i < mcu_height * mcu_width; i++) {
            if (restart_interval != 0 && i % restart_interval == 0) {
                prev_dc = 0;
                b.NextByte();
            }

            uint8_t size = GetNextSymbol(codes[0], huffman_tables[0], b);
            int coeff = b.ReadBits(size);

            if (size != 0 && coeff < (1 << (size - 1))) {
                coeff -= (1 << size) - 1;
            }
            mcu[i][0] = coeff + prev_dc;
            prev_dc = mcu[i][0];

            size_t j = 1;
            while (j < 64) {
                uint8_t symbol = GetNextSymbol(codes[1], huffman_tables[1], b);
                if (symbol == 0x00) {
                    for (; j < 64; j++) {
                        mcu[i][zig_zag_map[j]] = 0;
                    }
                    break;
                }

                uint8_t zero_count = (symbol >> 4);
                uint8_t coeff_size = (symbol & 0x0F);
                if (symbol == 0xF0) {
                    zero_count = 16;
                }
                if (j + zero_count >= 64) {
                    throw std::runtime_error("TODO");
                }
                for (size_t k = 0; k < zero_count; k++, j++) {
                    mcu[i][zig_zag_map[j]] = 0;
                }
                if (coeff_size != 0) {
                    coeff = b.ReadBits(coeff_size);
                    if (coeff < (1 << (coeff_size - 1))) {
                        coeff -= (1 << coeff_size) - 1;
                    }
                    mcu[i][zig_zag_map[j]] = coeff;
                    j++;
                }
            }
        }
        return mcu;
    }

    void Dequantize(
            size_t height,
            size_t width,
            std::array<uint8_t, 64> quantization_table,
            std::vector<std::array<int, 64>> &mcu) {
        size_t mcu_height = (height + 7) / 8;
        size_t mcu_width = (width + 7) / 8;

        for (size_t i = 0; i < mcu_height * mcu_width; i++) {
            for (size_t j = 0; j < 64; j++) {
                mcu[i][j] *= quantization_table[j];
            }
        }
    }

    void InverseDCT(
            size_t height,
            size_t width,
            std::vector<std::array<int, 64>> &mcu
    ) {
        size_t mcu_height = (height + 7) / 8;
        size_t mcu_width = (width + 7) / 8;

        for (size_t i = 0; i < mcu_height * mcu_width; i++) {
            std::array<int, 64> result{};
            for (size_t x = 0; x < 8; x++) {
                for (size_t y = 0; y < 8; y++) {
                    double sum = 0;
                    for (size_t mcu_row = 0; mcu_row < 8; mcu_row++) {
                        for (size_t mcu_col = 0; mcu_col < 8; mcu_col++) {
                            double cv = (mcu_row == 0) ? (1 / std::sqrt(2)) : 1.0;
                            double cu = (mcu_col == 0) ? (1 / std::sqrt(2)) : 1.0;
                            sum += cv * cu * mcu[i][mcu_row * 8 + mcu_col]
                                   * std::cos(
                                    (2 * static_cast<double>(x) + 1) * static_cast<double>(mcu_row) * M_PI / 16)
                                   * std::cos(
                                    (2 * static_cast<double>(y) + 1) * static_cast<double>(mcu_col) * M_PI / 16);
                        }
                    }
                    sum /= 4;
                    result[x * 8 + y] = static_cast<int>(sum);
                }
            }

            for (size_t j = 0; j < 64; j++) {
                mcu[i][j] = result[j];
            }
        }
    }

    std::vector<uint8_t> ReadHuffmanData(std::istream &in) {
        std::vector<uint8_t> huffman_data;

        while (true) {
            uint8_t byte = in.get();
            if (byte == BYTE_FF) {
                uint8_t next_byte = in.get();
                if (next_byte == EOI) {
                    break;
                } else if (next_byte == 0) {
                    huffman_data.push_back(byte);
                }
            } else {
                huffman_data.push_back(byte);
            }
        }
        return huffman_data;
    }

    Image GetImage(size_t height, size_t width, std::vector<std::array<int, 64>> mcu) {
        size_t mcu_width = (width + 7) / 8;
        auto *image_data = new uint8_t[height * width];
        std::cout << height << ' ' << width << '\n';
        for (size_t i = 0; i < height; i++) {
            size_t mcu_row = i / 8;
            size_t pixel_row = i % 8;
            for (size_t j = 0; j < width; j++) {
                size_t mcu_col = j / 8;
                size_t pixel_col = j % 8;
                size_t mcu_index = mcu_row * mcu_width + mcu_col;
                size_t pixel_index = pixel_row * 8 + pixel_col;

                image_data[i * width + j] = mcu[mcu_index][pixel_index] + 128;
            }
        }

        return Image(width, height, 1, 255, image_data);
    }

    Image read(const std::string &file_name) {
        std::ifstream in;
        in.open(file_name, std::ios::binary);

        uint8_t marker_type = ReadMarker(in);

        if (marker_type != SOI) {
            std::ostringstream error_message;
            error_message << "Expected " << SOI << " " << ", found: " << static_cast<int>(marker_type);
            throw std::runtime_error(error_message.str());
        }

        size_t height, width;
        size_t restart_interval = 0;
        DCACHuffmanTables huffman_tables;
        std::array<uint8_t, 64> quantization_table{};

        while (true) {
            marker_type = ReadMarker(in);
            if (marker_type >= APP0 && marker_type <= APP15) {
                Log("APP");
                SkipSegment(in);
            } else if (marker_type == COM) {
                Log("COM");
                SkipSegment(in);
            } else if (marker_type == DQT) {
                Log("DQT");
                ReadQuantizationTable(in, quantization_table);
            } else if (marker_type == SOF0) {
                Log("SOF0");
                std::tie(height, width) = ReadStartOfFrame(in);
            } else if (marker_type == DHT) {
                Log("DHT");
                ReadHuffmanTable(in, huffman_tables);
            } else if (marker_type == DRI) {
                Log("DRI");
                restart_interval = ReadRestartInterval(in);
            } else if (marker_type == SOS) {
                Log("SOS");
                ReadStartOfScan(in);
                std::vector<uint8_t> huffman_data = ReadHuffmanData(in);
                std::vector<std::array<int, 64>> mcu = GetMCU(
                        huffman_tables,
                        huffman_data,
                        height,
                        width,
                        restart_interval
                        );
                Dequantize(height, width, quantization_table, mcu);
                InverseDCT(height, width, mcu);
                return GetImage(height, width, mcu);
            }
        }
    }
}