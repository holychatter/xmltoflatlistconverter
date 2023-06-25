#include <xmltoflatlistconverter/test_xmltoflatlistconverter.hpp>
#include <string>
#include <xmltoflatlistconverter/xmltoflatlistconverter.hpp>
#include <assert.h>
#include <iostream>

namespace
{
std::string rePrintXml(const std::string pText)
{
  return xtfl::flatListToText(xtfl::textToFlatList(pText));
}

std::string rePrintXmlWithoutExlamationMark(const std::string pText)
{
  auto list = xtfl::textToFlatList(pText);
  xtfl::removeAllBeaconsThatBeginExclamationMark(list);
  return xtfl::flatListToText(list);
}

std::string rePrintXmlWithoutBeacon(const std::string pText,
                                    const std::string& pBeaconName)
{
  auto list = xtfl::textToFlatList(pText);
  xtfl::removeBeacon(list, pBeaconName);
  return xtfl::flatListToText(list);
}

std::string rePrintXmlWithoutBodyBeaconAndHisContent(const std::string pText,
                                                     const std::string& pBeaconName)
{
  auto list = xtfl::textToFlatList(pText);
  xtfl::removeBeaconAndHisContent(list, pBeaconName);
  return xtfl::flatListToText(list);
}

std::string rePrintXmlWithoutAttribute(const std::string pText,
                                       const std::string& pBeaconName,
                                       const std::string& pAttributeName)
{
  auto list = xtfl::textToFlatList(pText);
  xtfl::removeAttribute(list, pBeaconName, pAttributeName);
  return xtfl::flatListToText(list);
}


std::string rePrintXmlWithoutEmptyBeacons(const std::string pText)
{
  auto list = xtfl::textToFlatList(pText);
  xtfl::removeEmptyBeacons(list);
  return xtfl::flatListToText(list);
}

bool _check_if_true(const std::string& pExpected,
                    const std::string& pObtained)
{
  if (pExpected != pObtained)
  {
    std::cout << "Expected:\n" << pExpected << "\nObtained:\n" << pObtained << std::endl;
    return false;
  }
  return true;
}

}


void test_readWrite()
{
  assert(_check_if_true("", rePrintXml("")));
  assert(_check_if_true("a", rePrintXml("a")));
  assert(_check_if_true("a ", rePrintXml("a ")));
  assert(_check_if_true("a <!-- comment -->", rePrintXml("a <!-- comment -->")));
  assert(_check_if_true("<html>a</html><!-- comment -->", rePrintXml("<html>a</html><!-- comment -->")));
  assert(_check_if_true("a <", rePrintXml("a <")));
  assert(_check_if_true("a <b", rePrintXml("a <b")));
  assert(_check_if_true("a <b>", rePrintXml("a <b>")));
  assert(_check_if_true("a <b />", rePrintXml("a <b/>")));
  assert(_check_if_true("a <b />", rePrintXml("a <b />")));
  assert(_check_if_true("a <b><", rePrintXml("a <b><")));
  assert(_check_if_true("a <b><>", rePrintXml("a <b><>")));
  assert(_check_if_true("a <b></>", rePrintXml("a <b></>")));
  assert(_check_if_true("a <b></b>", rePrintXml("a <b></b>")));
  assert(_check_if_true("a <b></b>", rePrintXml("a <B></B>")));
  assert(_check_if_true("a <b>f</b>", rePrintXml("a <b>f</b>")));
  assert(_check_if_true("a <b>f</b>g", rePrintXml("a <b>f</b>g")));
  assert(_check_if_true("a <b r>f</b>g", rePrintXml("a <b r>f</b>g")));
  assert(_check_if_true("a <b r=>f</b>g", rePrintXml("a <b r=>f</b>g")));
  assert(_check_if_true("a <b r=d>f</b>g", rePrintXml("a <b r=d>f</b>g")));
  assert(_check_if_true("a <b r=d z>f</b>g", rePrintXml("a <b r=d z>f</b>g")));
  assert(_check_if_true("a <b r=d z=>f</b>g", rePrintXml("a <b r=d z=>f</b>g")));
  assert(_check_if_true("a <b r=d z=e>f</b>g", rePrintXml("a <b r=d z=e>f</b>g")));
  assert(_check_if_true("a <b r=d z=e>f</b>g", rePrintXml("a <b R=d z=e>f</b>g")));
  assert(_check_if_true("a <b allowfullscreen r=d z=e>f</b>g", rePrintXml("a <b R=d z=e allowfullscreen>f</b>g")));
}


void test_removeAllBeaconsThatBeginExclamationMark()
{
  assert(_check_if_true("<html>a<z>b</z></html>",
                        rePrintXmlWithoutExlamationMark("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">"
                                                        "<html>a<z>b</z></html>")));
  assert(_check_if_true("<html></html>",
                        rePrintXmlWithoutExlamationMark("<html></html><!-- comment -->")));
  assert(_check_if_true("<html>a<z>b</z></html>",
                        rePrintXmlWithoutExlamationMark("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">"
                                                        "<html>a<z>b</z><!-- comment --></html>")));
}


void test_removeBeacon()
{
  assert(_check_if_true("b", rePrintXmlWithoutBeacon("<a>b</a>", "a")));
  assert(_check_if_true("b", rePrintXmlWithoutBeacon("<a>b", "a")));
  assert(_check_if_true("b", rePrintXmlWithoutBeacon("b</a>", "a")));
  assert(_check_if_true("<a>b</a>", rePrintXmlWithoutBeacon("<a>b</a>", "b")));
  assert(_check_if_true("<a />b</a>", rePrintXmlWithoutBeacon("<a />b</a>", "b")));
}


void test_removeBeaconAndHisContent()
{
  assert(_check_if_true("<a>b</a>", rePrintXmlWithoutBodyBeaconAndHisContent("<a>b</a>", "body")));
  assert(_check_if_true("", rePrintXmlWithoutBodyBeaconAndHisContent("<body>b</body>", "body")));
  assert(_check_if_true("<body>b", rePrintXmlWithoutBodyBeaconAndHisContent("<body>b", "body")));
  assert(_check_if_true("b", rePrintXmlWithoutBodyBeaconAndHisContent("<body />b", "body")));
  assert(_check_if_true("a b", rePrintXmlWithoutBodyBeaconAndHisContent("a<body>b</body> b", "body")));
  assert(_check_if_true("a b", rePrintXmlWithoutBodyBeaconAndHisContent("a<body>b</BODY> b", "body")));
  assert(_check_if_true("a b", rePrintXmlWithoutBodyBeaconAndHisContent("a<BODY>b</BODY> b", "body")));
  assert(_check_if_true("a b", rePrintXmlWithoutBodyBeaconAndHisContent("a<BODY>b</body> b", "body")));
  assert(_check_if_true("a<p>b</body> b", rePrintXmlWithoutBodyBeaconAndHisContent("a<p>b</body> b", "body")));
  assert(_check_if_true("a E", rePrintXmlWithoutBodyBeaconAndHisContent("a<p>b<p>c</p>d</p> E", "p")));
  assert(_check_if_true("ad</p> E", rePrintXmlWithoutBodyBeaconAndHisContent("a<p>b<p />c</p>d</p> E", "p")));
}


void test_removeAttribute()
{
  assert(_check_if_true("<p>b</p>", rePrintXmlWithoutAttribute("<p>b</p>", "p", "class")));
  assert(_check_if_true("<p t=g>b</p>", rePrintXmlWithoutAttribute("<p t=g>b</p>", "p", "class")));
  assert(_check_if_true("<p>b</p>", rePrintXmlWithoutAttribute("<p class=d>b</p>", "p", "class")));
  assert(_check_if_true("<p t=g>b</p>", rePrintXmlWithoutAttribute("<p class=d t=g>b</p>", "p", "class")));
}


void test_removeEmptyBeacons()
{
  assert(_check_if_true("a<b>d</b>c", rePrintXmlWithoutEmptyBeacons("a<b>d</b>c")));
  assert(_check_if_true("ac", rePrintXmlWithoutEmptyBeacons("a<b></b>c")));
  assert(_check_if_true("a<b />b", rePrintXmlWithoutEmptyBeacons("a<b />b")));
  assert(_check_if_true("ac", rePrintXmlWithoutEmptyBeacons("a<p><b></b></p>c")));
  assert(_check_if_true("ac", rePrintXmlWithoutEmptyBeacons("a<p class=MsoNormal><b style='mso-bidi-font-weight:normal'></b></p>c")));
}


namespace xtfl
{

void test_runAll()
{
  test_readWrite();
  test_removeAllBeaconsThatBeginExclamationMark();
  test_removeBeacon();
  test_removeBeaconAndHisContent();
  test_removeAttribute();
  test_removeEmptyBeacons();
}

}
