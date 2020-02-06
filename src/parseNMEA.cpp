#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using std::regex;
using std::string;
using std::vector;
using std::stod;

// REMEMBER TO REMOVE <iostream> ONCE COMPLETED


namespace NMEA
{

  bool isWellFormedSentence(string inputSentence)
  {
      regex regexFullSentence("\\$GP[A-Z]{3}[A-Za-z0-9,.]*\\*[0-9A-Fa-f]{2}");

      return regex_match(inputSentence, regexFullSentence);
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
      vector<string> positionalData = sentence.second;

      double latitude = 0;
      double longitude = 0;

      string latString = "";
      string longString = "";

      char latDir;
      char longDir;

      // GLL does not has elevation

      if (sentence.second.empty()) { throw std::invalid_argument("Missing Data"); }

      if (format == "GLL")
      {
          latString = sentence.second[0];
          latDir = sentence.second[1][0];

          longString = sentence.second[2];
          longDir = sentence.second[4][0];
      }
      else if (format == "GGA")
      {
          latString = sentence.second[1];
          latDir = sentence.second[2][0];

          longString = sentence.second[3];
          longDir = sentence.second[4][0];
      }
      else if (format == "RMC")
      {
          latString = sentence.second[3];
          latDir = sentence.second[4][0];

          longString = sentence.second[5];
          longDir = sentence.second[6][0];
      }
      else
      {
          throw std::invalid_argument("Invalid Format");
      }

      latitude = GPS::ddmTodd(latString);
      if (sentence.second[1][0] == 'S') { latitude = 0 - latitude; }
      longitude = GPS::ddmTodd(longString);
      if (sentence.second[3][0] == 'W') { longitude = 0 - longitude; }


      return GPS::Position(latitude, longitude);
  }

  Route routeFromLog(std::istream &)
  {
      // Takes input stream as a parameter
      // Need to iterate through stream
      // Assert each previous function to the given sentence
      // Use each sentence to get the GPS::Position
      // Save the position to the Route (vector<GPS::Position>

      // Catch each invalid argument thrown by positionFromSentenceData()


      // Stub definition, needs implementing
      return {};
  }

}
