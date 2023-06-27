#include <xmltoflatlistconverter/string.hpp>


namespace xtfl
{

void replace_all(std::string& str,
                 const std::string& oldStr,
                 const std::string& newStr)
{
  std::string::size_type pos = 0u;
  while ((pos = str.find(oldStr, pos)) != std::string::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}


} // End of namespace xtfl

