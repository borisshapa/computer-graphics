//
// Created by borisshapa on 05.06.2021.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <vector>
#include <cassert>
#include <complex>
#include "JPEG.h"

namespace jpeg {
    typedef std::array<std::array<std::pair<int, std::vector<int>>, 16>, 2> DCACHuffmanTables;

    void Log(const std::string &str) {
        std::cout << str << "\n";
    }

    void CheckIfSizeIsZero(int size) {
        if (size != 0) {
            throw std::runtime_error("Wrong segment size");
        }
    }

    int ReadMarker(std::istream &in) {
        int ff_byte = in.get();
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
        for (int i = 0; i < size - 2; i++) {
            in.get();
        }
    }

    std::array<int, 64> ReadQuantizationTable(std::istream &in) {
        uint16_t size = Read2Bytes(in);
        size -= 2;

        std::array<int, 64> quantization_table{};
        while (size > 0) {
            int table_info = in.get();
            size--;
            int table_id = table_info & 0x0F;

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

            for (int i = 0; i < 64; i++) {
                quantization_table[zig_zag_map[i]] = in.get();
            }
            size -= 64;
        }
        return quantization_table;
    }

    std::pair<int, int> ReadStartOfFrame(std::istream &in) {
        Read2Bytes(in);
        int precision = in.get();
        if (precision != 8) {
            std::ostringstream error_message;
            error_message << "Expected precision 8, found " << static_cast<int>(precision);
            throw std::runtime_error(error_message.str());
        }

        uint16_t height = Read2Bytes(in);
        uint16_t width = Read2Bytes(in);
        int channel_count = in.get();
        if (channel_count != 1) {
            throw std::runtime_error("Only grayscale images are supported");
        }

        int component_id = in.get();
        if (component_id != 0 && component_id != 1) {
            std::ostringstream error_message;
            error_message << "Expected component id equals 1, found " << static_cast<int>(component_id);
            throw std::runtime_error(error_message.str());
        }

        in.get(); // sampling factor
        int quantization_table_id = in.get();
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

    DCACHuffmanTables ReadHuffmanTable(std::istream &in) {
        uint16_t size = Read2Bytes(in);
        size -= 2;

        DCACHuffmanTables huffman_tables{};

        while (size > 0) {
            int table_info = in.get();
            int table_id = table_info & 0x0F;
            if (table_id != 0) {
                throw std::runtime_error("Only grayscale images are supported");
            }
            int table_type = table_info >> 4;

            int total_symbol_count = 0;

            for (int i = 0; i < 16; i++) {
                int code_count = in.get();
                huffman_tables[table_type][i].first = code_count;
                total_symbol_count += code_count;
            }

            int symbols = 0;
            int code_size = 0;
            while (symbols < total_symbol_count) {
                int symbol_code = in.get();
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
        CheckIfSizeIsZero(size);
        return huffman_tables;
    }

    void ReadStartOfScan(std::istream &in) {
        uint16_t size = Read2Bytes(in);

        int component_count = in.get();
        if (component_count != 1) {
            throw std::runtime_error("Only grayscale images are supported");
        }

        int component_id = in.get();
        if (component_id != 0 && component_id != 1) {
            std::ostringstream error_message;
            error_message << "Expected component id equals 1, found " << static_cast<int>(component_id);
            throw std::runtime_error(error_message.str());
        }

        int huffman_table_ids = in.get();
        if (huffman_table_ids != 0) {
            throw std::runtime_error("Only grayscale images are supported");
        }

        int start_of_selection = in.get();
        int end_of_selection = in.get();
        if (start_of_selection != 0 || end_of_selection != 63) {
            throw std::runtime_error("Baseline JPEG doesn't use spectral selection");
        }

        int successive_approximation = in.get();
        if (successive_approximation != 0) {
            throw std::runtime_error("Baseline JPEG doesn't use successive approximation");
        }
    }

    struct BitReader {
        explicit BitReader(std::vector<int> data) : data_(data) {};

        int ReadBit() {
            if (next_byte_ >= data_.size()) {
                throw std::runtime_error("Invalid value");
            }
            int bit = (data_[next_byte_] >> (7 - next_bit_)) & 1;
            next_bit_++;
            if (next_bit_ == 8) {
                next_bit_ = 0;
                next_byte_++;
            }
            return bit;
        }

        int ReadBits(int count) {
            int bits = 0;
            for (int i = 0; i < count; i++) {
                int bit = ReadBit();
                bits = (bits << 1) | bit;
            }
            return bits;
        }

        void Align() {
            if (next_byte_ >= data_.size()) {
                return;
            }
            if (next_bit_ != 0) {
                next_bit_ = 0;
                next_byte_++;
            }
        }

    private:
        int next_byte_ = 0;
        int next_bit_ = 0;
        std::vector<int> data_;
    };


    int GetNextSymbol(
            const std::array<std::vector<int>, 16> &codes,
            const std::array<std::pair<int, std::vector<int>>, 16> &huffman_table,
            BitReader &b
    ) {
        int code = 0;
        for (int i = 0; i < 16; i++) {
            assert(codes[i].size() == huffman_table[i].second.size());

            int bit = b.ReadBit();
            code = (code << 1) | bit;
            for (int j = 0; j < huffman_table[i].first; j++) {
                if (code == codes[i][j]) {
                    return huffman_table[i].second[j];
                }
            }
        }
        throw std::runtime_error("The symbol wasn't found");
    }

    std::vector<std::array<int, 64>> GetMCU(
            const DCACHuffmanTables &huffman_tables,
            const std::vector<int> &huffman_data,
            int height,
            int width,
            int restart_interval
    ) {
        std::array<std::array<std::vector<int>, 16>, 2> codes;
        for (int table_type = 0; table_type < huffman_tables.size(); table_type++) {
            int code = 0;
            for (int i = 0; i < huffman_tables[table_type].size(); i++) {
                for (int j = 0; j < huffman_tables[table_type][i].first; j++) {
                    codes[table_type][i].push_back(code);
                    code += 1;
                }
                code <<= 1;
            }
        }

        BitReader b(huffman_data);

        int mcu_height = (height + 7) / 8;
        int mcu_width = (width + 7) / 8;
        std::vector<std::array<int, 64>> mcu(mcu_height * mcu_width);

        int prev_dc = 0;

        for (int i = 0; i < mcu_height * mcu_width; i++) {
            std::cout << prev_dc << ' ';
            if (restart_interval != 0 && i % restart_interval == 0) {
                prev_dc = 0;
                b.Align();
            }

            int size = GetNextSymbol(codes[0], huffman_tables[0], b);
            if (size > 11) {
                std::ostringstream os;
                os << "Expected size less or equal than 11, found " << size;
                throw std::runtime_error(os.str());
            }
            int coeff = b.ReadBits(size);

            if (size != 0 && coeff < (1 << (size - 1))) {
                coeff -= (1 << size) - 1;
            }
            mcu[i][0] = coeff + prev_dc;
            prev_dc = mcu[i][0];

            int j = 1;
            while (j < 64) {
                int symbol = GetNextSymbol(codes[1], huffman_tables[1], b);
//                std::cout << (int)symbol << ' ';
                if (symbol == 0x00) {
                    for (; j < 64; j++) {
                        mcu[i][zig_zag_map[j]] = 0;
                    }
                    break;
                }

                int zero_count = (symbol >> 4);
                int coeff_size = (symbol & 0x0F);
                coeff = 0;
                if (symbol == 0xF0) {
                    zero_count = 16;
                }
                if (j + zero_count >= 64) {
                    throw std::runtime_error("TODO");
                }
                for (int k = 0; k < zero_count; k++, j++) {
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

//        for (int i = 0; i < 100; i++) {
//            for (int j = 0; j < 64; j++) {
//                std::cout << mcu[i][j] << ' ';
//            }
//            std::cout << '\n';
//        }
        return mcu;
    }

    void Dequantize(
            int height,
            int width,
            std::array<int, 64> quantization_table,
            std::vector<std::array<int, 64>> &mcu) {
        int mcu_height = (height + 7) / 8;
        int mcu_width = (width + 7) / 8;

        for (int i = 0; i < mcu_height * mcu_width; i++) {
            for (int j = 0; j < 64; j++) {
                mcu[i][j] *= quantization_table[j];
            }
        }
    }

    void InverseDCT(
            int height,
            int width,
            std::vector<std::array<int, 64>> &mcu
    ) {
        int mcu_height = (height + 7) / 8;
        int mcu_width = (width + 7) / 8;

        for (int i = 0; i < mcu_height * mcu_width; i++) {
            std::array<int, 64> result{};
            for (int ii = 0; ii < 8; ii++) {
                for (int jj = 0; jj < 8; jj++) {
                    double sum = 0;
                    for (int mcu_row = 0; mcu_row < 8; mcu_row++) {
                        for (int mcu_col = 0; mcu_col < 8; mcu_col++) {
                            double cv = (mcu_row == 0) ? (1 / std::sqrt(2)) : 1.0;
                            double cu = (mcu_col == 0) ? (1 / std::sqrt(2)) : 1.0;
                            sum += cv * cu * mcu[i][mcu_row * 8 + mcu_col]
                                   * std::cos((2 * static_cast<double>(ii) + 1) * static_cast<double>(jj) * M_PI / 16)
                                   * std::cos((2 * static_cast<double>(jj) + 1) * static_cast<double>(ii) * M_PI / 16);
                        }
                    }
                    sum /= 4;
                    result[ii * 8 + jj] = static_cast<int>(sum);
                }
            }

            for (int j = 0; j < 64; j++) {
                mcu[i][j] = result[j];
            }
        }
    }

    Image read(const std::string &file_name) {
        std::ifstream in;
        in.open(file_name, std::ios::binary);

        int marker_type = ReadMarker(in);

        if (marker_type != SOI) {
            std::ostringstream error_message;
            error_message << "Expected " << SOI << " " << ", found: " << static_cast<int>(marker_type);
            throw std::runtime_error(error_message.str());
        }

        int height, width;
        int restart_interval = 0;
        DCACHuffmanTables huffman_tables;
        std::array<int, 64> quantization_table{};

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
                quantization_table = ReadQuantizationTable(in);
            } else if (marker_type == SOF0) {
                Log("SOF0");
                std::tie(height, width) = ReadStartOfFrame(in);
            } else if (marker_type == DHT) {
                Log("DHT");
                huffman_tables = ReadHuffmanTable(in);
//                for (auto &huffman_table : huffman_tables) {
//                    for (int j = 0; j < huffman_table.size(); j++) {
//                        std::cout << j << " " << (int) huffman_table[j].first << "    ";
//                        for (unsigned char k : huffman_table[j].second) {
//                            std::cout << (int) k << ' ';
//                        }
//                        std::cout << '\n';
//                    }
//                    std::cout << "===========\n\n";
//                }
            } else if (marker_type == SOS) {
                Log("SOS");
                ReadStartOfScan(in);
                break;
            } else if (marker_type == DRI) {
                Log("DRI");
                restart_interval = ReadRestartInterval(in);
            }
        }

        std::vector<int> huffman_data;

        while (true) {
            int byte = in.get();
            if (byte == BYTE_FF) {
                int next_byte = in.get();
                if (next_byte == EOI) {
                    break;
                } else if (next_byte == 0) {
                    huffman_data.push_back(byte);
                }
            } else {
                huffman_data.push_back(byte);
            }
        }

//        for (auto x : huffman_data) {
//            std::cout << (int)x << ' ';
//        }
//        std::cout << '\n';
//        for (int i = 0; i < 2; i++) {
//            for (int j = 0; j < 16; j++) {
//                std::cout << j << "   ";
//                for (int k = 0; k < codes[i][j].size(); k++) {
//                    std::cout << codes[i][j][k] << ' ';
//                }
//                std::cout << '\n';
//            }
//            std::cout << "=======\n";
//        }

        std::vector<std::array<int, 64>> mcu = GetMCU(huffman_tables, huffman_data, height, width, restart_interval);
        Dequantize(height, width, quantization_table, mcu);
        InverseDCT(height, width, mcu);

        int mcu_width = (width + 7) / 8;
        auto *image_data = new uint8_t[height * width];
        std::cout << height << ' ' << width << '\n';
        for (int i = 0; i < height; i++) {
            int mcu_row = i / 8;
            int pixel_row = i % 8;
            for (int j = 0; j < width; j++) {
                int mcu_col = j / 8;
                int pixel_col = j % 8;
                int mcu_index = mcu_row * mcu_width + mcu_col;
                int pixel_index = pixel_row * 8 + pixel_col;

                image_data[i * width + j] = mcu[mcu_index][pixel_index];
            }
        }

        return Image(width, height, 1, 255, image_data);
    }
}