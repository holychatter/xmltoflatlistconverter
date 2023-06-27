#ifndef XMLTOFLATLISTCONVERTER_API_HPP
#define XMLTOFLATLISTCONVERTER_API_HPP

#include "macro.hpp"

#if !defined(SWIG) && defined(xmltoflatlistconverter_EXPORTS)
# define XMLTOFLATLISTCONVERTER_API XMLTOFLATLISTCONVERTER_LIB_API_EXPORTS(xmltoflatlistconverter)
#elif !defined(SWIG)
# define XMLTOFLATLISTCONVERTER_API XMLTOFLATLISTCONVERTER_LIB_API(xmltoflatlistconverter)
#else
# define XMLTOFLATLISTCONVERTER_API
#endif

#endif // XMLTOFLATLISTCONVERTER_API_HPP
