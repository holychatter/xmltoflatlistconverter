#ifndef XMLTOFLATLISTCONVERTER_STRING_HPP
#define XMLTOFLATLISTCONVERTER_STRING_HPP

#include <string>
#include "api.hpp"


namespace xtfl
{

/**
 * @brief Replace all occurences of a string to another string.
 * @param str Input string to modify inplace.
 * @param oldStr String to search.
 * @param newStr String to put instead of the string ti search.
 */
XMLTOFLATLISTCONVERTER_API
void replace_all(std::string& str,
                 const std::string& oldStr,
                 const std::string& newStr);


} // End of namespace xtfl

#endif // XMLTOFLATLISTCONVERTER_STRING_HPP
