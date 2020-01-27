#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <iostream>

using std::regex;
using std::string;

namespace NMEA
{

  bool isWellFormedSentence(std::string inputSentence)
  {
      // Intend on using regex to check the string
      regex regexFullSentence("\\$GP[A-Z]{3}[A-Za-z0-9,.]*\\*[0-9A-Fa-f]{2}");

      if (regex_match(inputSentence, regexFullSentence)) { return true; }
      else { return false; }
  }

  bool hasValidChecksum(std::string inputSentence)
  {
      if(!isWellFormedSentence(inputSentence)) return false;

      std::size_t ampersand = inputSentence.find('$');
      std::size_t asterisk = inputSentence.find('*');

      std::string subStr = inputSentence.substr(inputSentence.length() - 2);
      std::cout << subStr << std::endl;
      unsigned int hexVals = std::stoul(subStr, nullptr, 16);
      std::cout << std::hex << hexVals << std::endl;

      std::string valuesToCalc = inputSentence.substr(++ampersand, asterisk - 2);
      int checksum = 0;
      for(int i = 1; i < inputSentence.length() - 3; i++)
      {
          checksum ^= inputSentence[i];
          std::cout << std::hex << checksum << std::endl;
      }

      if (checksum == hexVals)
      {
          return true;
      }
      else
      {
          return false;
      }

      /*string checksum = "";
      string hexVals = inputSentence.substr(inputSentence.length() - 2);

      for(int i = 1; i < inputSentence.length() - 3; i++)
      {
            int calcChecksum = 0;
            checksum = calcChecksum ^= i;
      }*/
  }

  SentenceData extractSentenceData(std::string)
  {
      // Stub definition, needs implementing
      return {"",{}};
  }

  GPS::Position positionFromSentenceData(SentenceData)
  {
      // Stub definition, needs implementing
      return GPS::Earth::NorthPole;
  }

  Route routeFromLog(std::istream &)
  {
      // Stub definition, needs implementing
      return {};
  }

}
