#ifndef STRINGCONVERSION_H
#define STRINGCONVERSION_H

#include "conversionexception.h"
#include "binaryconversion.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <initializer_list>
#include <list>
#include <vector>

#include <bits/codecvt.h>

namespace ConversionUtilities
{

LIB_EXPORT void truncateString(std::string &str, char terminationChar = '\0');

/*!
 * \brief Joins the given \a strings using the specified \a delimiter.
 *
 * The strings will be enclosed using the provided closures \a leftClosure and \a rightClosure.
 *
 * \param strings The string parts to be joined.
 * \param delimiter Specifies a delimiter to be used (empty string by default).
 * \param omitEmpty Indicates whether empty part should be omitted.
 * \param leftClosure Specifies a string to be inserted before each string (empty string by default).
 * \param rightClosure Specifies a string to be appendend after each string (empty string by default).
 * \tparam Container The STL-container used to provide the \a strings.
 * \returns Returns the joined string.
 */
template <class Container = std::initializer_list<std::string> > LIB_EXPORT
typename Container::value_type joinStrings(const Container &strings, const typename Container::value_type &delimiter = typename Container::value_type(), bool omitEmpty = false, const typename Container::value_type &leftClosure = typename Container::value_type(), const typename Container::value_type &rightClosure = typename Container::value_type())
{
    typename Container::value_type res;
    if(strings.size()) {
        size_t entries = 0;
        size_t size = 0;
        for(const auto &str : strings) {
            if(!omitEmpty || !str.empty()) {
                size += str.size();
                ++entries;
            }
        }
        if(entries) {
            size += (entries * leftClosure.size()) + (entries * rightClosure.size()) + ((entries - 1) * delimiter.size());
            res.reserve(size);
            for(const auto &str : strings) {
                if(!omitEmpty || !str.empty()) {
                    if(!res.empty()) {
                        res.append(delimiter);
                    }
                    res.append(leftClosure);
                    res.append(str);
                    res.append(rightClosure);
                }
            }
        }
    }
    return res;
}

/*!
 * \brief Specifies the role of empty parts when splitting strings.
 */
enum class EmptyPartsTreat
{
    Keep, /**< empty parts are kept */
    Omit, /**< empty parts are omitted */
    Merge /**< empty parts are omitted but cause the adjacent parts being joined using the delimiter */
};

/*!
 * \brief Splits the given \a string at the specified \a delimiter.
 *
 * \param strings The string to be splitted.
 * \param delimiter Specifies the delimiter.
 * \param skipEmpty Indicates whether empty parts should be skipped.
 * \param maxParts Specifies the maximal number of parts. Values less or equal zero indicate an unlimited number of parts.
 * \tparam Container The STL-container used to return the parts.
 * \returns Returns the parts.
 */
template <class Container = std::list<std::string> > LIB_EXPORT
Container splitString(const typename Container::value_type &string, const typename Container::value_type &delimiter, EmptyPartsTreat emptyPartsRole = EmptyPartsTreat::Keep, int maxParts = -1)
{
    --maxParts;
    Container res;
    bool merge = false;
    for(typename Container::value_type::size_type i = 0, end = string.size(), delimPos; i < end; i = delimPos + delimiter.size()) {
        delimPos = string.find(delimiter, i);
        if(!merge && maxParts >= 0 && res.size() == static_cast<typename Container::value_type::size_type>(maxParts)) {
            if(delimPos == i && emptyPartsRole == EmptyPartsTreat::Merge) {
                if(!res.empty()) {
                    merge = true;
                    continue;
                }
            }
            delimPos = Container::value_type::npos;
        }
        if(delimPos == Container::value_type::npos) {
            delimPos = string.size();
        }
        if(emptyPartsRole == EmptyPartsTreat::Keep || i != delimPos) {
            if(merge) {
                res.back().append(delimiter);
                res.back().append(string.substr(i, delimPos - i));
                merge = false;
            } else {
                res.emplace_back(string.substr(i, delimPos - i));
            }
        } else if(emptyPartsRole == EmptyPartsTreat::Merge) {
            if(!res.empty()) {
                merge = true;
            }
        }
    }
    return res;
}

/*!
 * \brief Returns whether \a str starts with \a phrase.
 */
template <typename StringType> LIB_EXPORT bool startsWith(const StringType &str, const StringType &phrase)
{
    if(str.size() < phrase.size()) {
        return false;
    }
    for(auto stri = str.cbegin(), strend = str.cend(), phrasei = phrase.cbegin(), phraseend = phrase.cend(); stri != strend; ++stri, ++phrasei) {
        if(phrasei == phraseend) {
            return true;
        } else if(*stri != *phrasei) {
            return false;
        }
    }
    return false;
}

/*!
 * \brief Converts the given \a number to its equivalent std::string representation using the specified \a base.
 *
 * \tparam NumberType The data type of the given number.
 * \tparam StringType The string type (should be an instantiation of the basic_string class template).
 *
 * \sa numberToWString()
 * \sa stringToNumber()
 * \sa wstringToNumber()
 */
template <typename NumberType, typename StringType = std::string> LIB_EXPORT StringType numberToString(NumberType number, int base = 10)
{
    std::basic_stringstream<typename StringType::value_type> ss;
    ss << std::setbase(base) << number;
    return ss.str();
}

/*!
 * \brief Converts the given \a string to a numeric value using the specified \a base.
 *
 * \tparam NumberType The data type used to store the converted value.
 * \tparam StringType The string type (should be an instantiation of the basic_string class template).
 *
 * \throws A ConversionException will be thrown if the provided string is not a valid number.
 *
 * \sa numberToString()
 * \sa numberToWString()
 * \sa wstringToNumber()
 */
template <typename NumberType, typename StringType> LIB_EXPORT NumberType stringToNumber(const StringType &string, int base = 10)
{
    std::basic_stringstream<typename StringType::value_type> ss;
    ss << std::setbase(base) << string;
    NumberType result;
    if(ss >> result) {
        return result;
    } else {
        throw ConversionException("The specified string is no valid number.");
    }
}

/*!
 * \brief Interprets the given \a integer at the specified position as std::string using the specified byte order.
 *
 * Example: Interpretation of ID3v2 frame IDs (stored as 32-bit integer) as string
 *  - 0x54495432/1414091826 will be interpreted as "TIT2" using big endian byte order.
 *  - 0x00545432/5526578 will be interpreted as "TT2" using big endian byte order and start offset 1 to "exclude" the first byte.
 *
 * \tparam T The data type of the integer to be interpreted.
 */
template <typename T> LIB_EXPORT std::string interpretIntegerAsString(T integer, int startOffset = 0)
{
    char buffer[sizeof(T)];
    ConversionUtilities::BE::getBytes(integer, buffer);
    return std::string(buffer + startOffset, sizeof(T));
}

LIB_EXPORT std::string dataSizeToString(int64 sizeInByte);
LIB_EXPORT std::string bitrateToString(double speedInKbitsPerSecond, bool useByteInsteadOfBits = false);
LIB_EXPORT std::string encodeBase64(const std::vector<char> &bytes);
LIB_EXPORT std::vector<char> decodeBase64(const std::string &encoded);

}

#endif // STRINGCONVERSION_H