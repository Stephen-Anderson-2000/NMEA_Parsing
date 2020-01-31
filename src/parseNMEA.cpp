#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>

using std::regex;
using std::string;
using std::vector;

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
      if(!isWellFormedSentence(inputSentence)) { return false; }

      std::string subStr = inputSentence.substr(inputSentence.length() - 2);
      unsigned int hexVals = std::stoul(subStr, nullptr, 16);

      int checksum = 0;
      for(int i = 1; i < inputSentence.length() - 3; i++)
      {
          checksum ^= inputSentence[i];
      }

      if (checksum == hexVals) { return true; }
      else { return false; }
  }

  SentenceData extractSentenceData(std::string inputSentence)
  {
      assert(isWellFormedSentence(inputSentence));

      std::string format = inputSentence.substr(3, 3);
      //std::cout << format << std::endl;
      //std::cout << inputSentence.length() << std::endl;

      vector<string> positionalData;

      size_t comma = inputSentence.find(',');
      if (comma != std::string::npos)
      {
          std::stringstream separateStream(inputSentence.substr(comma + 1, inputSentence.length() - 10));
          while(separateStream.good())
          {
            string section;
            getline(separateStream, section, ',');
            positionalData.push_back(section);
          }
      }

      return {format, {positionalData}};
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
