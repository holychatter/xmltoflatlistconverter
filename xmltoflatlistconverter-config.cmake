get_filename_component(_xmltoflatlistconverter_root "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_xmltoflatlistconverter_root "${_xmltoflatlistconverter_root}" ABSOLUTE)


set(XMLROFLATLISTCONVERTER_FOUND TRUE)

set(
  XMLROFLATLISTCONVERTER_INCLUDE_DIRS
  ${_xmltoflatlistconverter_root}/include
  CACHE INTERNAL "" FORCE
)

set(
  XMLROFLATLISTCONVERTER_LIBRARIES
  "xmltoflatlistconverter"
)


