#ifndef XMLTOFLATLISTCONVERTER_XMLTOFLATLISTCONVERTER_HPP
#define XMLTOFLATLISTCONVERTER_XMLTOFLATLISTCONVERTER_HPP

#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include "api.hpp"

namespace xtfl
{

enum class XmlEltType
{
  BEACON,
  ENDOFBEACON,
  TEXT
};


struct XMLTOFLATLISTCONVERTER_API Text;

struct XMLTOFLATLISTCONVERTER_API XmlElt
{
  virtual ~XmlElt() {}

  const XmlEltType type;

  virtual std::string toStr() const = 0;
  virtual std::unique_ptr<XmlElt> clone() const = 0;

protected:
  XmlElt(XmlEltType pType);
};


struct XMLTOFLATLISTCONVERTER_API Beacon : public XmlElt
{
  Beacon(const std::string& pText,
         std::size_t pBegin,
         std::size_t pEnd);
  Beacon(const std::string& pName,
         const std::map<std::string, std::string>& pAttributes,
         bool pShouldBeClaused,
         std::size_t pSize);
  Beacon(const Beacon& pOther);

  std::string toStr() const;
  std::unique_ptr<XmlElt> clone() const;

  std::string getAttributeValueWithoutSurroundingQuotationMarks(const std::string& pAttributeName) const;

  std::string name;
  std::map<std::string, std::string> attributes;
  bool shouldBeClaused;
  std::size_t size;
};

struct XMLTOFLATLISTCONVERTER_API EndOfBeacon : public XmlElt
{
  EndOfBeacon(const std::string& pText,
              std::size_t pBegin,
              std::size_t pEnd);
  explicit EndOfBeacon(const std::string& pName);
  EndOfBeacon(const EndOfBeacon& pOther);

  std::string toStr() const;
  std::unique_ptr<XmlElt> clone() const;

  std::string name;
};


struct XMLTOFLATLISTCONVERTER_API Text : public XmlElt
{
  Text(const std::string& pText,
       std::size_t pBegin,
       std::size_t pEnd);
  Text(const std::string& pText);
  Text(const Text& pOther);

  std::string toStr() const;
  std::unique_ptr<XmlElt> clone() const;

  std::string txt;
};


XMLTOFLATLISTCONVERTER_API
std::string whiteColorForLinks();

XMLTOFLATLISTCONVERTER_API
std::string removeSurroundingQuotationMarks(const std::string& pStr);

XMLTOFLATLISTCONVERTER_API
std::size_t skipSpaces(const std::string& pStr,
                       std::size_t pPos);

XMLTOFLATLISTCONVERTER_API
std::size_t skipSpacesAndUnderscores(const std::string& pStr,
                                     std::size_t pPos);

XMLTOFLATLISTCONVERTER_API
std::list<std::unique_ptr<XmlElt>> textToFlatList(const std::string& pText);

XMLTOFLATLISTCONVERTER_API
std::string flatListToText(const std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
std::string flatListToTextWithoutBeacons(const std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
std::list<std::string> extractLines(const std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
std::string removeAllBeaconsThatShouldBeClausedFromStr(const std::string& pText);

XMLTOFLATLISTCONVERTER_API
void setAutoAdjustWidthOfBigElements(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                                     std::size_t pMinWidthToConsider,
                                     const std::string& pClassToControlWidth);

XMLTOFLATLISTCONVERTER_API
void collapseToTextStructExeptForBeacons(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                                         const std::vector<std::string>& pBeaconsToSkip);

XMLTOFLATLISTCONVERTER_API
void replaceParaByNewLines(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
void removeAllBeacons(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
void removeAllBeaconsThatBeginExclamationMark(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
void removeAllBeaconsThatShouldBeClaused(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
void removeBeacon(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                  const std::string& pBeaconName);

XMLTOFLATLISTCONVERTER_API
void insertText(std::list<std::unique_ptr<xtfl::XmlElt>>& pList,
                std::list<std::unique_ptr<xtfl::XmlElt>>::iterator pIt,
                const std::string& pText);

XMLTOFLATLISTCONVERTER_API
void insertNewLine(std::list<std::unique_ptr<xtfl::XmlElt>>& pList,
                   std::list<std::unique_ptr<xtfl::XmlElt>>::iterator pIt);

XMLTOFLATLISTCONVERTER_API
std::string* getFirstTextPtr(std::list<std::unique_ptr<XmlElt>>::iterator pItBegin,
                             std::list<std::unique_ptr<XmlElt>>::iterator pItEnd);

XMLTOFLATLISTCONVERTER_API
std::list<std::unique_ptr<XmlElt>>::iterator getEndingBeaconFromIts
(std::list<std::unique_ptr<XmlElt>>::iterator pItBeginOfABeacon,
 std::list<std::unique_ptr<XmlElt>>::iterator pItEnd);

XMLTOFLATLISTCONVERTER_API
std::list<std::unique_ptr<XmlElt>>::iterator getEndingBeacon
(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
 std::list<std::unique_ptr<XmlElt>>::iterator pItBeginOfABeacon);

XMLTOFLATLISTCONVERTER_API
void removeBeaconAndHisContent(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                               const std::string& pBeaconName);

XMLTOFLATLISTCONVERTER_API
void removeAttribute(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                     const std::string& pBeaconName,
                     const std::string& pAttributeName);

XMLTOFLATLISTCONVERTER_API
std::map<std::string, std::list<std::unique_ptr<XmlElt>>> getNamedLinks(const std::list<std::unique_ptr<XmlElt>>& pListOfElts);


XMLTOFLATLISTCONVERTER_API
void removeEmptyBeacons(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
void removeBlankBeaconsAndContent(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

XMLTOFLATLISTCONVERTER_API
void writeAllLinksInWhite(std::list<std::unique_ptr<XmlElt>>& pListOfElts);

} // !xtfl


#endif // XMLTOFLATLISTCONVERTER_XMLTOFLATLISTCONVERTER_HPP
