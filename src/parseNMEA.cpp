#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>

using std::string;


// REMEMBER TO REMOVE <iostream> ONCE COMPLETED


namespace NMEA
{

  bool isWellFormedSentence(string inputSentence)
  {
      std::regex regexFullSentence("\\$GP[A-Z]{3}[A-Za-z0-9,.]*\\*[0-9A-Fa-f]{2}");

      if (regex_match(inputSentence, regexFullSentence)) { return true; }
      else { return false; }
  }

  bool hasValidChecksum(string inputSentence)
  {
      string subStr = inputSentence.substr(inputSentence.length() - 2);
      unsigned long hexVals = std::stoul(subStr, nullptr, 16);

      unsigned int checksum = 0;
      for(unsigned long i = 1; i < inputSentence.length() - 3; i++)
      {
          checksum ^= inputSentence[i];
      }

      if (checksum == hexVals) { return true; }
      else { return false; }
  }

  SentenceData extractSentenceData(string inputSentence)
  {
      string format = inputSentence.substr(3, 3);

      std::vector<string> positionalData;

      size_t comma = inputSentence.find(',');
      if (comma != string::npos)
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
