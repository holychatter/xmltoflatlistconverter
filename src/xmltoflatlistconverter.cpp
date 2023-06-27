#include <xmltoflatlistconverter/xmltoflatlistconverter.hpp>
#include <algorithm>
#include <sstream>
#include <set>
#include <xmltoflatlistconverter/lexical_cast.hpp>
#include <xmltoflatlistconverter/string.hpp>

namespace myu
{
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}


namespace xtfl
{

namespace
{
const std::string _spaceHtml = "&nbsp;";
const std::size_t _spaceHtml_size = _spaceHtml.size();

void _toLowerStr(std::string& pText)
{
  std::transform(pText.begin(), pText.end(), pText.begin(), ::tolower);
}

void _removeNewLinesStr(std::string& pText)
{
  replace_all(pText, "\r\n", "");
}

bool _hasOnlySpaces(const std::string& pStr)
{
  return skipSpaces(pStr, 0) == pStr.size();
}

}

XmlElt::XmlElt(XmlEltType pType)
 : type(pType)
{
}


Beacon::Beacon(const std::string& pText,
               std::size_t pBegin,
               std::size_t pEnd)
  : XmlElt(XmlEltType::BEACON),
    name(),
    attributes(),
    shouldBeClaused(true),
    size(0)
{
  size_t begOfInside = pBegin + 1;
  if (begOfInside >= pEnd)
    return;
  if (pText[pEnd - 1] == '/')
  {
    shouldBeClaused = false;
    --pEnd;
    if (begOfInside >= pEnd)
      return;
  }

  std::size_t endOfName = pText.find_first_of(" \r\n", begOfInside);
  if (endOfName > pEnd)
    endOfName = pEnd;
  name = pText.substr(begOfInside, endOfName - begOfInside);
  _toLowerStr(name);
  if (endOfName == pEnd)
    return;

  std::size_t begOfAttr = endOfName + 1;
  while (begOfAttr < pEnd &&
         (pText[begOfAttr] == ' ' ||
          pText[begOfAttr] == '\r' ||
          pText[begOfAttr] == '\n'))
    ++begOfAttr;

  while (begOfAttr < pEnd)
  {
    std::size_t equalPos = pText.find('=', begOfAttr);
    if (equalPos < pEnd)
    {
      size_t begOfValue = equalPos + 1;
      if ((begOfValue < pEnd))
      {
        bool beginWithQuotationMark = pText[begOfValue] == '"';
        const std::string charsToFind = beginWithQuotationMark ? "\"" : " >";
        std::size_t endOfAttr = pText.find_first_of(charsToFind, begOfValue + 1);
        if (beginWithQuotationMark && endOfAttr <= pEnd)
          ++endOfAttr;
        if (endOfAttr <= pEnd)
        {
          std::string attrName = pText.substr(begOfAttr, equalPos - begOfAttr);
          _toLowerStr(attrName);
          _removeNewLinesStr(attrName);
          std::string attrValue = pText.substr(begOfValue, endOfAttr - begOfValue);
          _removeNewLinesStr(attrValue);
          attributes.emplace(attrName, attrValue);
          if (attrName == "textlength")
          {
            const std::string textLengthStr = removeSurroundingQuotationMarks(attrValue);
            try
            {
              size = lexical_cast<std::size_t>(textLengthStr);
            } catch (...) {
              size = 0;
            }
          }
          begOfAttr = endOfAttr + 1;
          continue;
        }
      }
    }

    if (begOfAttr < pEnd)
      attributes.emplace(pText.substr(begOfAttr, pEnd - begOfAttr), "");
    break;
  }
}


Beacon::Beacon(const std::string& pName,
               const std::map<std::string, std::string>& pAttributes,
               bool pShouldBeClaused,
               std::size_t pSize)
  : XmlElt(XmlEltType::BEACON),
    name(pName),
    attributes(pAttributes),
    shouldBeClaused(pShouldBeClaused),
    size(pSize)
{
}

Beacon::Beacon(const Beacon& pOther)
  : XmlElt(XmlEltType::BEACON),
    name(pOther.name),
    attributes(pOther.attributes),
    shouldBeClaused(pOther.shouldBeClaused),
    size(0)
{
}


std::string Beacon::toStr() const
{
  std::stringstream ss;
  ss << "<" << name;
  for (const auto& currAttr : attributes)
  {
    ss << " " << currAttr.first;
    if (!currAttr.second.empty())
      ss << "=" << currAttr.second;
  }
  if (!shouldBeClaused)
    ss << " /";
  ss << ">";
  return ss.str();
}

std::string Beacon::getAttributeValueWithoutSurroundingQuotationMarks(const std::string& pAttributeName) const
{
  auto it = attributes.find(pAttributeName);
  if (it != attributes.end())
    return removeSurroundingQuotationMarks(it->second);
  return "";
}

std::unique_ptr<XmlElt> Beacon::clone() const
{
  return myu::make_unique<Beacon>(*this);
}


EndOfBeacon::EndOfBeacon(const std::string& pText,
                         std::size_t pBegin,
                         std::size_t pEnd)
  : XmlElt(XmlEltType::ENDOFBEACON),
    name()
{
  std::size_t begOfName = pBegin + 2;
  if (begOfName >= pEnd)
    return;
  name = pText.substr(begOfName, pEnd - begOfName);
  _toLowerStr(name);
}

EndOfBeacon::EndOfBeacon(const std::string& pName)
  : XmlElt(XmlEltType::ENDOFBEACON),
    name(pName)
{
}


EndOfBeacon::EndOfBeacon(const EndOfBeacon& pOther)
  : XmlElt(XmlEltType::ENDOFBEACON),
    name(pOther.name)
{
}


std::string EndOfBeacon::toStr() const
{
  return "</" + name + ">";
}

std::unique_ptr<XmlElt> EndOfBeacon::clone() const
{
  return myu::make_unique<EndOfBeacon>(*this);
}


Text::Text(const std::string& pText,
           std::size_t pBegin,
           std::size_t pEnd)
  : XmlElt(XmlEltType::TEXT),
    txt(pText.substr(pBegin, pEnd - pBegin))
{
  replace_all(txt, "\r\n", " ");
}


Text::Text(const std::string& pText)
  : XmlElt(XmlEltType::TEXT),
    txt(pText)
{
}


Text::Text(const Text& pOther)
  : XmlElt(XmlEltType::TEXT),
    txt(pOther.txt)
{
}


std::string Text::toStr() const
{
  return txt;
}

std::unique_ptr<XmlElt> Text::clone() const
{
  return myu::make_unique<Text>(*this);
}


bool _isTheBeginOfABeacon(const std::string& pText,
                          std::size_t pBeaconFirstChar)
{
  std::size_t firstCharInsideTheBeacon = pBeaconFirstChar + 1;
  if (firstCharInsideTheBeacon < pText.size())
    return pText[firstCharInsideTheBeacon] != '/';
  return true;
}


std::string whiteColorForLinks()
{
  return "Gainsboro";
}


std::string removeSurroundingQuotationMarks(const std::string& pStr)
{
  if (pStr.size() >= 2 && pStr[0] == '"' && pStr[pStr.size() - 1] == '"')
    return pStr.substr(1, pStr.size() - 2);
  return pStr;
}


std::size_t skipSpaces(const std::string& pStr,
                       std::size_t pPos)
{
  for (std::size_t i = pPos; i < pStr.size(); ++i)
  {
    if (pStr[i] == ' ')
      continue;
    if (pStr.compare(i, _spaceHtml_size, _spaceHtml) == 0)
    {
      i += _spaceHtml_size - 1;
      continue;
    }
    return i;
  }
  return pStr.size();
}

std::size_t skipSpacesAndUnderscores(const std::string& pStr,
                                     std::size_t pPos)
{
  for (std::size_t i = pPos; i < pStr.size(); ++i)
  {
    if (pStr[i] == ' ' || pStr[i] == '_')
      continue;
    if (pStr.compare(i, _spaceHtml_size, _spaceHtml) == 0)
    {
      i += _spaceHtml_size - 1;
      continue;
    }
    return i;
  }
  return pStr.size();
}

std::list<std::unique_ptr<XmlElt>> textToFlatList(const std::string& pText)
{
  std::list<std::unique_ptr<XmlElt>> res;
  std::size_t beginOfBeacon = std::string::npos;
  std::size_t nextCharToFlashPos = 0;

  for (std::size_t i = 0; i < pText.size(); ++i)
  {
    if (pText[i] == '<')
    {
      beginOfBeacon = i;
    }
    else if (pText[i] == '>' &&
             beginOfBeacon != std::string::npos)
    {
      if (nextCharToFlashPos < beginOfBeacon)
        res.emplace_back(myu::make_unique<Text>(pText, nextCharToFlashPos, beginOfBeacon));
      if (_isTheBeginOfABeacon(pText, beginOfBeacon))
        res.emplace_back(myu::make_unique<Beacon>(pText, beginOfBeacon, i));
      else
        res.emplace_back(myu::make_unique<EndOfBeacon>(pText, beginOfBeacon, i));
      nextCharToFlashPos = i + 1;
      beginOfBeacon = std::string::npos;
    }
  }

  if (nextCharToFlashPos < pText.size())
    res.emplace_back(myu::make_unique<Text>(pText, nextCharToFlashPos, pText.size()));
  return res;
}


std::string flatListToText(const std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  std::string res;
  for (const auto& currElt : pListOfElts)
    res += currElt->toStr();
  return res;
}


std::string flatListToTextWithoutBeacons(const std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  std::string res;
  for (const auto& currElt : pListOfElts)
    if (currElt->type == XmlEltType::TEXT)
      res += currElt->toStr();
  return res;
}


std::list<std::string> extractLines(const std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  std::list<std::string> res;
  std::string currStrRes;
  for (const auto& currEltPtr : pListOfElts)
  {
    const XmlElt& currElt = *currEltPtr;
    if (currElt.type == XmlEltType::TEXT)
    {
      currStrRes += currElt.toStr();
    }
    else if (currElt.type == XmlEltType::BEACON)
    {
      auto& beacon = *dynamic_cast<const Beacon*>(&currElt);
      if (!beacon.shouldBeClaused && beacon.name == "br" &&
          !currStrRes.empty())
      {
        res.emplace_back(currStrRes);
        currStrRes.clear();
      }
    }
  }
  if (!currStrRes.empty())
    res.emplace_back(currStrRes);
  return res;
}


std::string removeAllBeaconsThatShouldBeClausedFromStr(const std::string& pText)
{
  auto flatXmlList = textToFlatList(pText);
  removeAllBeaconsThatShouldBeClaused(flatXmlList);
  return flatListToText(flatXmlList);
}


void setAutoAdjustWidthOfBigElements(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                                     std::size_t pMinWidthToConsider,
                                     const std::string& pClassToControlWidth)
{
  static const std::string styleAttrName = "style";
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); ++it)
  {
    auto& currElt = *it;
    if (currElt->type == XmlEltType::BEACON)
    {
      Beacon& currBeacon = *dynamic_cast<Beacon*>(&*currElt);
      if (currBeacon.name == "iframe" || currBeacon.name == "img"|| currBeacon.name == "video")
      {
        std::size_t width = 0;
        auto itWidth = currBeacon.attributes.find("width");
        if (itWidth != currBeacon.attributes.end())
        {
          try
          {
            auto widthStr = removeSurroundingQuotationMarks(itWidth->second);
            width = lexical_cast<std::size_t>(widthStr);
            if (width < pMinWidthToConsider)
              continue;
          }
          catch (...)
          {
            continue;
          }
          std::size_t height = 0;
          auto itHeight = currBeacon.attributes.find("height");
          if (itHeight != currBeacon.attributes.end())
          {
            try
            {
              auto heightStr = removeSurroundingQuotationMarks(itHeight->second);
              height = lexical_cast<std::size_t>(heightStr);
            }
            catch (...)
            {
              continue;
            }

            auto itEnd = pListOfElts.end();
            auto itEndOfBeacon = getEndingBeaconFromIts(it, itEnd);
            if (itEndOfBeacon != itEnd)
            {
              auto& styleStr = currBeacon.attributes[styleAttrName];
              auto styleAttrWithoutQuotation = currBeacon.getAttributeValueWithoutSurroundingQuotationMarks(styleAttrName);
              styleStr = "\"";
              if (!styleAttrWithoutQuotation.empty())
                styleStr += styleAttrWithoutQuotation + " ";
              styleStr += "position: absolute; height:100%; width:100%;\"";

              float ratio = (height * 100) / static_cast<float>(width);
              std::stringstream rationSS;
              rationSS << ratio;
              auto divContainer = myu::make_unique<Beacon>("div", std::map<std::string, std::string>{}, true, 0u);
              std::string containerStyleStr = "position: relative; height:0; padding-bottom: " + rationSS.str() + "%;";
              divContainer->attributes.emplace(styleAttrName, "\"" + containerStyleStr + "\"");
              divContainer->attributes.emplace("class", "\"" + pClassToControlWidth + "\"");
              pListOfElts.insert(it, std::move(divContainer));
              auto endOfDivContainer = myu::make_unique<EndOfBeacon>("div");
              ++itEndOfBeacon;
              pListOfElts.insert(itEndOfBeacon, std::move(endOfDivContainer));
              if (itEndOfBeacon == itEnd)
                return;
              it = itEndOfBeacon;
            }
          }
        }
        else if ((currBeacon.name == "img" || currBeacon.name == "video") && currBeacon.attributes.count("height") == 0)
        {
          currBeacon.attributes.emplace("class", pClassToControlWidth);
        }
      }
    }
  }
}



void collapseToTextStructExeptForBeacons(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                                         const std::vector<std::string>& pBeaconsToSkip)
{
  std::string buffer;
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const auto& currElt = **it;
    switch (currElt.type)
    {
    case XmlEltType::TEXT:
    {
      buffer += currElt.toStr();
      it = pListOfElts.erase(it);
      break;
    }
    case XmlEltType::BEACON:
    case XmlEltType::ENDOFBEACON:
    {
      const std::string& name = currElt.type == XmlEltType::BEACON ? dynamic_cast<const Beacon*>(&currElt)->name :
                                                                     dynamic_cast<const EndOfBeacon*>(&currElt)->name;
      if (std::find(pBeaconsToSkip.begin(), pBeaconsToSkip.end(), name) != pBeaconsToSkip.end())
      {
        if (!buffer.empty())
          insertText(pListOfElts, it, buffer);
        buffer.clear();
        ++it;
      }
      else
      {
        buffer += currElt.toStr();
        it = pListOfElts.erase(it);
      }
      break;
    }
    }
  }
  if (!buffer.empty())
    insertText(pListOfElts, pListOfElts.end(), buffer);
}


void replaceParaByNewLines(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  bool prevWasANewLine = true;
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const auto& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      const Beacon& currBeacon = *dynamic_cast<const Beacon*>(&currElt);
      if (currBeacon.shouldBeClaused)
      {
        if (currBeacon.name == "p")
        {
          auto itEnding = getEndingBeacon(pListOfElts, it);
          if (itEnding != pListOfElts.end())
          {
            auto itAfterEnding = itEnding;
            if (itAfterEnding != pListOfElts.end())
              insertNewLine(pListOfElts, itEnding);
            if (!prevWasANewLine)
              insertNewLine(pListOfElts, it);
            prevWasANewLine = false;
            if (itEnding != it)
              pListOfElts.erase(itEnding);
            it = pListOfElts.erase(it);
            continue;
          }
        }
      }
      else
      {
        prevWasANewLine = currBeacon.name == "br";
        ++it;
        continue;
      }
    }
    prevWasANewLine = false;
    ++it;
  }
}


void removeAllBeacons(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON ||
        currElt.type == XmlEltType::ENDOFBEACON)
    {
      it = pListOfElts.erase(it);
      continue;
    }
    ++it;
  }
}


void removeAllBeaconsThatBeginExclamationMark(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      const std::string& name = dynamic_cast<const Beacon*>(&currElt)->name;
      if (!name.empty() && name[0] == '!')
      {
        it = pListOfElts.erase(it);
        continue;
      }
    }
    ++it;
  }
}


void removeAllBeaconsThatShouldBeClaused(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      const auto& currBeacon = dynamic_cast<const Beacon*>(&currElt);
      if (currBeacon->shouldBeClaused)
      {
        it = pListOfElts.erase(it);
        continue;
      }
    }
    if (currElt.type == XmlEltType::ENDOFBEACON)
    {
      it = pListOfElts.erase(it);
      continue;
    }
    ++it;
  }
}

void removeBeacon(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                  const std::string& pBeaconName)
{
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      if (dynamic_cast<const Beacon*>(&currElt)->name == pBeaconName)
      {
        it = pListOfElts.erase(it);
        continue;
      }
    }
    else if (currElt.type == XmlEltType::ENDOFBEACON)
    {
      if (dynamic_cast<const EndOfBeacon*>(&currElt)->name == pBeaconName)
      {
        it = pListOfElts.erase(it);
        continue;
      }
    }
    ++it;
  }
}


void insertText(std::list<std::unique_ptr<xtfl::XmlElt>>& pList,
                std::list<std::unique_ptr<xtfl::XmlElt>>::iterator pIt,
                const std::string& pText)
{
  pList.insert(pIt, myu::make_unique<Text>(pText));
}

void insertNewLine(std::list<std::unique_ptr<xtfl::XmlElt>>& pList,
                   std::list<std::unique_ptr<xtfl::XmlElt>>::iterator pIt)
{
  pList.insert(pIt, myu::make_unique<Beacon>("br", std::map<std::string, std::string>{}, false, 0u));
}

std::string* getFirstTextPtr(std::list<std::unique_ptr<XmlElt>>::iterator pItBegin,
                             std::list<std::unique_ptr<XmlElt>>::iterator pItEnd)
{
  for (auto it = pItBegin; it != pItEnd; ++it)
  {
    XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::TEXT)
      return &dynamic_cast<Text*>(&currElt)->txt;
  }
  return nullptr;
}

std::list<std::unique_ptr<XmlElt>>::iterator getEndingBeaconFromIts
(std::list<std::unique_ptr<XmlElt>>::iterator pItBeginOfABeacon,
 std::list<std::unique_ptr<XmlElt>>::iterator pItEnd)
{
  const XmlElt& beginElt = **pItBeginOfABeacon;
  std::string beaconName;
  if (beginElt.type == XmlEltType::BEACON)
  {
    const Beacon& beginBeacon = *dynamic_cast<const Beacon*>(&beginElt);
    if (!beginBeacon.shouldBeClaused)
      return pItBeginOfABeacon;
    beaconName = beginBeacon.name;
  }
  else
  {
    return pItBeginOfABeacon;
  }

  std::size_t stackCount = 1;
  ++pItBeginOfABeacon;
  for (auto it = pItBeginOfABeacon; it != pItEnd; ++it)
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      const Beacon& currBeacon = *dynamic_cast<const Beacon*>(&currElt);
      if (currBeacon.name == beaconName &&
          currBeacon.shouldBeClaused)
        ++stackCount;
    }
    else if (currElt.type == XmlEltType::ENDOFBEACON)
    {
      const EndOfBeacon& currEndOfBeacon = *dynamic_cast<const EndOfBeacon*>(&currElt);
      if (currEndOfBeacon.name == beaconName)
      {
        --stackCount;
        if (stackCount == 0)
          return it;
      }
    }
  }
  return pItEnd;
}


std::list<std::unique_ptr<XmlElt>>::iterator getEndingBeacon
(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
 std::list<std::unique_ptr<XmlElt>>::iterator pItBeginOfABeacon)
{
  return getEndingBeaconFromIts(pItBeginOfABeacon, pListOfElts.end());
}


void removeBeaconAndHisContent(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                               const std::string& pBeaconName)
{
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      const Beacon& currBeacon = *dynamic_cast<const Beacon*>(&currElt);
      if (currBeacon.name == pBeaconName)
      {
        auto itEndingBeacon = getEndingBeacon(pListOfElts, it);
        if (itEndingBeacon != pListOfElts.end())
        {
          auto itBeginBeacon = it;
          it = itEndingBeacon;
          ++it;
          while (itBeginBeacon != itEndingBeacon)
            itBeginBeacon = pListOfElts.erase(itBeginBeacon);
          pListOfElts.erase(itEndingBeacon);
          continue;
        }
      }
    }
    ++it;
  }
}


void removeAttribute(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                     const std::string& pBeaconName,
                     const std::string& pAttributeName)
{
  for (auto& currEltPtr : pListOfElts)
  {
    XmlElt& currElt = *currEltPtr;
    if (currElt.type == XmlEltType::BEACON)
    {
      Beacon& currBeacon = *dynamic_cast<Beacon*>(&currElt);
      if (currBeacon.name == pBeaconName)
      {
        auto attrIt = currBeacon.attributes.find(pAttributeName);
        if (attrIt != currBeacon.attributes.end())
          currBeacon.attributes.erase(attrIt);
      }
    }
  }
}


void copyListOfElts(std::list<std::unique_ptr<XmlElt>>& pNewListOfElts,
                    std::list<std::unique_ptr<XmlElt>>::iterator pPosToInsert,
                    std::list<std::unique_ptr<XmlElt>>::const_iterator pBeginIt,
                    std::list<std::unique_ptr<XmlElt>>::const_iterator pEndIt)
{
  for (auto it = pBeginIt; it != pEndIt; ++it)
    pNewListOfElts.insert(pPosToInsert, (*it)->clone());
}


std::map<std::string, std::list<std::unique_ptr<XmlElt>>> getNamedLinks(const std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  std::map<std::string, std::list<std::unique_ptr<XmlElt>>> res;
  std::string currLinkName;
  auto beginOfCurrNamedLink = pListOfElts.end();

  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); ++it)
  {
    XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      if (currLinkName.empty())
      {
        Beacon& currBeacon = *dynamic_cast<Beacon*>(&currElt);
        if (currBeacon.name == "a")
        {
          auto attrIt = currBeacon.attributes.find("name");
          if (attrIt != currBeacon.attributes.end())
          {
            currLinkName = attrIt->second;
            beginOfCurrNamedLink = it;
          }
        }
      }
    }
    else if (currElt.type == XmlEltType::ENDOFBEACON &&
             !currLinkName.empty())
    {
      EndOfBeacon& currEndOfBeacon = *dynamic_cast<EndOfBeacon*>(&currElt);
      if (currEndOfBeacon.name == "a")
      {
        ++beginOfCurrNamedLink;
        auto& newListOfElts = res[currLinkName];
        copyListOfElts(newListOfElts, newListOfElts.end(), beginOfCurrNamedLink, it);
        currLinkName.clear();
      }
    }
  }
  return res;
}


void _removeBeaconsWithContentThatMatch(std::list<std::unique_ptr<XmlElt>>& pListOfElts,
                                        const std::function<bool(const std::string&)>& pPatternFunc,
                                        bool pClearContent)
{
  bool somethingHasBeenRemoved = false;
  do
  {
    somethingHasBeenRemoved = false;
    for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
    {
      const XmlElt& currElt = **it;
      if (currElt.type == XmlEltType::BEACON)
      {
        const Beacon& currBeacon = *dynamic_cast<const Beacon*>(&currElt);
        if (currBeacon.shouldBeClaused)
        {
          auto nextIt = it;
          ++nextIt;
          if (nextIt == pListOfElts.end())
            break;

          std::list<std::list<std::unique_ptr<XmlElt>>::iterator> spaceToRemove;
          if ((*nextIt)->type == XmlEltType::TEXT &&
              pPatternFunc(dynamic_cast<const Text*>(&**nextIt)->txt))
          {
            if (pClearContent)
              spaceToRemove.emplace_back(nextIt);
            ++nextIt;
            if (nextIt == pListOfElts.end())
              break;
          }

          if ((*nextIt)->type == XmlEltType::ENDOFBEACON &&
              currBeacon.name == dynamic_cast<const EndOfBeacon*>(&**nextIt)->name)
          {
            for (auto& currItToDel : spaceToRemove)
              pListOfElts.erase(currItToDel);
            pListOfElts.erase(it);
            it = nextIt;
            ++it;
            pListOfElts.erase(nextIt);
            somethingHasBeenRemoved = true;
            continue;
          }
        }
      }
      ++it;
    }
  }
  while (somethingHasBeenRemoved);
}


void removeEmptyBeacons(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  _removeBeaconsWithContentThatMatch(pListOfElts,
                                     [](const std::string& pStr)
  {
    return pStr == " ";
  }, false);
}


void removeBlankBeaconsAndContent(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  _removeBeaconsWithContentThatMatch(pListOfElts, _hasOnlySpaces, true);
}


void writeAllLinksInWhite(std::list<std::unique_ptr<XmlElt>>& pListOfElts)
{
  for (auto it = pListOfElts.begin(); it != pListOfElts.end(); )
  {
    const XmlElt& currElt = **it;
    if (currElt.type == XmlEltType::BEACON)
    {
      const std::string& name = dynamic_cast<const Beacon*>(&currElt)->name;
      if (name == "a")
      {
        auto itEndingBeacon = getEndingBeacon(pListOfElts, it);
        if (itEndingBeacon != pListOfElts.end())
        {
          ++it;
          static const std::map<std::string, std::string> attributes{{"color", "\"" + whiteColorForLinks() + "\""}};
          pListOfElts.insert(it, myu::make_unique<Beacon>("font", attributes, true, 0u));
          pListOfElts.insert(itEndingBeacon, myu::make_unique<EndOfBeacon>("font"));
          it = itEndingBeacon;
        }
      }
    }
    ++it;
  }
}


} // !xtfl
