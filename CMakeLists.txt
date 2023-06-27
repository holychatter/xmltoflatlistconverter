project(xmltoflatlistconverter)
cmake_minimum_required(VERSION 2.8)


# Making a lib
set(XMLROFLATLISTCONVERTER_HPPS
    include/xmltoflatlistconverter/api.hpp
    include/xmltoflatlistconverter/test_xmltoflatlistconverter.hpp
    include/xmltoflatlistconverter/xmltoflatlistconverter.hpp
    include/xmltoflatlistconverter/lexical_cast.hpp
    include/xmltoflatlistconverter/macro.hpp
    include/xmltoflatlistconverter/string.hpp
)


set(XMLROFLATLISTCONVERTER_SRCS
    src/xmltoflatlistconverter.cpp
    src/test_xmltoflatlistconverter.cpp
    src/string.cpp
)


include_directories(include)



add_library(xmltoflatlistconverter SHARED
  ${XMLROFLATLISTCONVERTER_HPPS}
  ${XMLROFLATLISTCONVERTER_SRCS}
)

install(TARGETS xmltoflatlistconverter DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)


