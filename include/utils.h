#pragma once
#include <flags.h>
#include <iostream>
#include <charconv> 
#include <vector>
#include <sstream>
#include <bitset>


#ifndef _MSC_VER
    #define _In_		// MSC macro specific
    #define _Out_
#endif

#define NIBBLE_TO_BYTE 2
#define BITS_IN_BYTE 8
//#define EIGHT_BIT_MAX 255
//#define SIXTEEN_BIT_MAX 65535
//#define THIRTYTWO_BIT_MAX 4294967295
//#define SIXTYFOUR_BIT_MAX 18446744073709551615
#define DEFAULT_BIT_RANGE 8


enum class MAX
{
    EIGHT_BIT_MAX = 255,
    SIXTEEN_BIT_MAX = 65535,
    THIRTYTWO_BIT_MAX = 4294967295,
    SIXTYFOUR_BIT_MAX = 18446744073709551615
};

#define HEX_BASE 16
#define DECIMAL_BASE 10
#define BINARY_BASE 2
#define OCTAL_BASE 8



// Inputs are big and can only have 1 at a time
#define HEX_INPUT_FLAG 'X'
#define ASCII_INPUT_FLAG 'A'
#define DECIMAL_INPUT_FLAG 'I'
#define BINARY_INPUT_FLAG 'B'
#define OCTAL_INPUT_FLAG 'O'

// #define ARRAY_INPUT_TYPE_FLAG "array"
#define BYTE_INPUT_TYPE "bytes"

// Outputs are small
#define ASCII_OUTPUT_FLAG 'a'
#define HEX_OUTPUT_FLAG 'x'
#define DECIMAL_OUTPUT_FLAG 'i'
#define BINARY_OUTPUT_FLAG 'b'
#define OCTAL_OUTPUT_FLAG 'o'

#define HELP_FLAG_LONG "help"
#define HELP_FLAG_SHORT 'h'

#define ENDIANNESS_LSB "LSB"
#define ENDIANNESS_MSB "MSB"

namespace print
{
    template <typename Ttype>
    void ASCII(_In_ const std::vector<Ttype>& vInput)
    {
        std::cout << "ASCII: ";

        for (auto& byte : vInput)
        {
            if (byte <= '~' && byte >= ' ')
                std::cout << std::format("{:c}", byte);
            else
                std::cout << std::format("(0x{:X})", byte);
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    template <typename Ttype>
    void hex(_In_ const std::vector<Ttype>& vInput)
    {
        std::cout << "Hex: ";

        for (auto& byte : vInput)
            std::cout << std::format("0x{:X}, ", byte);

        std::cout << std::endl;
        std::cout << std::endl;
    }

    template <typename Ttype>
    void decimal(_In_ const std::vector<Ttype>& vInput, _In_opt_ const bool& bPrintNegative = true)
    {
        std::cout << "Unsigned decimal: ";

        for (auto& byte : vInput)
            std::cout << byte << ", ";

        std::cout << std::endl;

        if (bPrintNegative)
        {
            std::cout << "Signed decimal: ";

            for (auto& byte : vInput)
            {
                auto sval = static_cast<Ttype>(byte);
                std::cout << sval << ", ";
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    template <typename Ttype>
    void binary(_In_ const std::vector<Ttype>& vInput, _In_opt_ const unsigned& nBitRange = DEFAULT_BIT_RANGE)
    {
        std::cout << "Binary: ";

        for (auto& byte : vInput)
            std::cout << ConvertIntToBinaryString<Ttype>(byte, nBitRange) << ", ";

        std::cout << std::endl;
        std::cout << std::endl;
    }

    template <typename Ttype>
    void octal(_In_ const std::vector<Ttype>& vInput)
    {
        std::cout << "Octal: ";

        for (auto& byte : vInput)
            std::cout << std::format("{:o}, ", byte);
        

        std::cout << std::endl;
        std::cout << std::endl;
    }

};

namespace convert
{
    template <typename Ttype>
    std::vector<Ttype> ASCII(_In_ std::string_view sInput, _In_ const size_t& nInputSize)
    {
        std::vector<Ttype> result{ 0 };
        result.reserve(nInputSize);

        for (size_t i = 0; i < nInputSize; i++)
        {
            result.push_back(sInput.at(i));
        }

        return result;
    }

    template <typename Ttype>
    std::vector<Ttype> hex(_In_ const std::string& sInput, _In_ const size_t& nInputSize, _In_ const FlagParser& flags)
    {
        std::vector<Ttype> result;
        result.reserve(nInputSize);

        std::istringstream stream(sInput);
        std::string sToken;

        

        unsigned nByteSpacing = flags.hasValue(BYTE_INPUT_TYPE) ? (std::stoul(flags.getValue(BYTE_INPUT_TYPE)) * NIBBLE_TO_BYTE) : 0;

        std::string sByteSegment;

        while (std::getline(stream, sToken, ' '))
        {
            if (sToken.size() >= 2 && sToken[0] == '0' && (sToken[1] == 'x' || sToken[1] == 'X'))
                sToken.erase(0, 2);

            if (!sToken.empty() && (sToken.back() == 'h' || sToken.back() == 'H'))
                sToken.pop_back();

            if (nByteSpacing)
            {
                for (size_t i = 0; i < sToken.size(); i += nByteSpacing)
                {
                    sByteSegment = sToken.substr(i, nByteSpacing);

                    try {
                        result.push_back(std::stoull(sByteSegment, nullptr, HEX_BASE));
                    }
                    catch (const std::exception& eErrorMessage_HexSubTokens) {
                        std::cerr << "Error converting hex to out with byte seperation " << nByteSpacing << " : " << eErrorMessage_HexSubTokens.what() << std::endl;
                    }
                }
            }
            else
            {
                try {
                    result.push_back(std::stoull(sToken, nullptr, HEX_BASE));
                }
                catch (const std::exception& eErrorMessage_HexTokens) {
                    std::cerr << "Error converting hex to out: " << eErrorMessage_HexTokens.what() << std::endl;
                }

            }
        }

        return result;
    }

    template <typename Ttype>
    std::vector<Ttype> decimal(_In_ const std::string& sInput, _In_ const size_t& nInputSize)
    {
        std::vector<Ttype> result{ 0 };
        result.reserve(nInputSize);

        std::istringstream stream(sInput);

        std::string sToken;
        



        while (std::getline(stream, sToken, ' '))
        {

            try {
                result.push_back(std::stoull(sToken, nullptr, DECIMAL_BASE));
            }
            catch (const std::exception& eErrorMessage_intToken) {
                std::cerr << "Error converting decimal to out: " << eErrorMessage_intToken.what() << std::endl;
            }

        }

        return result;
    }

    template <typename Ttype>
    std::vector<Ttype> binary(_In_ const std::string& sInput, _In_ const size_t& nInputSize, _In_ const FlagParser& flags)
    {
        std::vector<Ttype> result{ 0 };
        result.reserve(nInputSize);

        std::istringstream stream(sInput);

        std::string sToken;
        

        unsigned nByteSpacing = flags.hasValue(BYTE_INPUT_TYPE) ? (std::stoul(flags.getValue(BYTE_INPUT_TYPE)) * BITS_IN_BYTE) : 0;


        std::string sByteSegment;

        while (std::getline(stream, sToken, ' '))
        {
            if (sToken.size() >= 2 && sToken[0] == '0' && (sToken[1] == 'b' || sToken[1] == 'B'))
                sToken.erase(0, 2);

            if (nByteSpacing)
            {
                for (size_t i = 0; i < sToken.size(); i += nByteSpacing)
                {
                    sByteSegment = sToken.substr(i, nByteSpacing);

                    try {
                        result.push_back(std::stoull(sByteSegment, nullptr, BINARY_BASE));
                    }
                    catch (const std::exception& eErrorMessage_HexSubTokens) {
                        std::cerr << "Error converting binary to out with byte seperation " << nByteSpacing << " : " << eErrorMessage_HexSubTokens.what() << std::endl;
                    }
                }
            }
            else
            {
                try {
                    result.push_back(std::stoull(sToken, nullptr, BINARY_BASE));
                }
                catch (const std::exception& eErrorMessage_intToken) {
                    std::cerr << "Error converting binary to out: " << eErrorMessage_intToken.what() << std::endl;
                }
            }
        }

        return result;
    }

    template <typename Ttype>
    std::vector<Ttype> octal(_In_ const std::string& sInput, _In_ const size_t& nInputSize)
    {
        std::vector<Ttype> result{ 0 };
        result.reserve(nInputSize);

        std::istringstream stream(sInput);
        std::string sToken;
        



        while (std::getline(stream, sToken, ' '))
        {
            if (sToken.size() >= 2 && sToken[0] == '0')
                sToken.erase(0, 1);

            try {
                result.push_back(std::stoull(sToken, nullptr, OCTAL_BASE));
            }
            catch (const std::exception& eErrorMessage_intToken) {
                std::cerr << "Error converting octal to out: " << eErrorMessage_intToken.what() << std::endl;
            }

        }

        return result;
    }

}

template <typename Ttype>
std::string ConvertIntToBinaryString(_In_ const Ttype& nInput, _In_opt_ const unsigned& nBitRange = DEFAULT_BIT_RANGE)
{

    std::bitset<64> binary(nInput);
    return binary.to_string().substr(64 - nBitRange);
}


inline void SetUpFlags(_In_ const int& argc, _In_ char* argv[], _Out_ FlagParser& flags)
{
    // Input flags (only 1)
    flags.addShortFlag(HEX_INPUT_FLAG, "Input is hexidecimal");
    flags.addShortFlag(ASCII_INPUT_FLAG, "Input is ASCII");
    flags.addShortFlag(DECIMAL_INPUT_FLAG, "Input is int");
    flags.addShortFlag(BINARY_INPUT_FLAG, "Input is binary");
    flags.addShortFlag(OCTAL_INPUT_FLAG, "Input is octal");

    // Input flag type (all as 1, array, etc)
    // flags.addLongFlag(ARRAY_INPUT_TYPE_FLAG, "Uses input as an array (must be exactly like this: uni_coverter \"0xFF, 0xAA, 0xFF\")");
    flags.addValueFlag(BYTE_INPUT_TYPE, "Allows you to select how many bytes to bytes to translate at a time. This is only for hex and binary (ex. --byte=1 [0xFFFF -> 0xFF 0xFF]). By default it takes input as 1 constant hex/binary");

    // Output flags
    flags.addShortFlag(ASCII_OUTPUT_FLAG, "Output is ASCII");
    flags.addShortFlag(HEX_OUTPUT_FLAG, "Output is hexidecimal");
    flags.addShortFlag(DECIMAL_OUTPUT_FLAG, "Output is numbers, the numbers are 64-bit by default");
    flags.addShortFlag(BINARY_OUTPUT_FLAG, "Output is binary (if not specified, bitset will be auto selected)");
    flags.addShortFlag(OCTAL_OUTPUT_FLAG, "Output is octal");

    flags.addLongFlag(HELP_FLAG_LONG, "Prints help command");
    flags.addShortFlag(HELP_FLAG_SHORT, "Prints help command");

    flags.addLongFlag(ENDIANNESS_LSB, "Makes input least significant byte");
    flags.addLongFlag(ENDIANNESS_MSB, "Makes input most significant byte (by default)");
    
    flags.parse(argc, argv);
}

inline std::vector<uint64_t> ConvertInputToVector(_In_ const std::string& sInput, _In_ const size_t& nInputSize, _In_ const FlagParser& flags)
{

    std::vector<uint64_t> result;

    if (flags.hasShortFlag(ASCII_INPUT_FLAG))
        result = convert::ASCII<uint64_t>(sInput, nInputSize);

    else if (flags.hasShortFlag(HEX_INPUT_FLAG))
        result = convert::hex<uint64_t>(sInput, nInputSize, flags);

    else if (flags.hasShortFlag(DECIMAL_INPUT_FLAG))
        result = convert::decimal<uint64_t>(sInput, nInputSize);

    else if (flags.hasShortFlag(BINARY_INPUT_FLAG))
        result = convert::binary<uint64_t>(sInput, nInputSize, flags);

    else if (flags.hasShortFlag(OCTAL_INPUT_FLAG))
        result = convert::octal<uint64_t>(sInput, nInputSize);

    if (flags.hasLongFlag(ENDIANNESS_LSB))
        std::reverse(result.begin(), result.end());

    return result;
}


inline void PrintOutput(_In_ const std::vector<uint64_t>& bytes, _In_ const FlagParser& flags)
{
    std::cout << std::endl;

    if (flags.hasShortFlag(ASCII_OUTPUT_FLAG))
        print::ASCII(bytes);

    if (flags.hasShortFlag(HEX_OUTPUT_FLAG))
        print::hex(bytes);

    if (flags.hasShortFlag(DECIMAL_OUTPUT_FLAG))
        print::decimal(bytes);

    if (flags.hasShortFlag(BINARY_OUTPUT_FLAG))
        print::binary(bytes);

    if (flags.hasShortFlag(OCTAL_OUTPUT_FLAG))
        print::octal(bytes);

}
