#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>

using std::string;
using std::vector;

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

      vector<string> positionalData;

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

  GPS::Position positionFromSentenceData(SentenceData sentence)
  {
      string format = sentence.first;
      std::cout << format << std::endl;
      vector<string> positionalData = sentence.second;


      if (format != "GLL" && format != "GGA" && format != "RMC")
      {
          throw std::invalid_argument("");
      }




      return GPS::Earth::NorthPole;
  }

  Route routeFromLog(std::istream &)
  {
      // Takes input stream as a parameter
      // Need to iterate through stream
      // Assert each previous function to the given sentence
      // Use each sentence to get the GPS::Position
      // Save the position to the Route (vector<GPS::Position>


      // Stub definition, needs implementing
      return {};
  }

}
