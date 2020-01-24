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
      regex regexSentenceCheck("($GP)");

      string prefix = inputSentence.substr(0, 3);

      std::cout << prefix << std::endl;

      if (regex_match(prefix, regexSentenceCheck)) { std::cout << "Worked" << std::endl; }
      else { std::cout << "Failed" << std::endl; }

      if (regex_match(inputSentence, regexSentenceCheck)) { return true; }
      else { return false; }
  }

  bool hasValidChecksum(std::string)
  {
      // Stub definition, needs implementing
      return false;
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
