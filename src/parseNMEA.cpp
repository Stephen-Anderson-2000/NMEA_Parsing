#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <vector>
#include <sstream>
#include <string>

using std::regex;
using std::string;
using std::vector;
using std::invalid_argument;

namespace NMEA
{

  bool isWellFormedSentence(string inputSentence)
  {
      // Uses regex to check for a valid NMEA sentence
      regex regexFullSentence("\\$GP[A-Z]{3}[\-A-Za-z0-9,.]*\\*[0-9A-Fa-f]{2}");
      return regex_match(inputSentence, regexFullSentence);
  }

  bool hasValidChecksum(string inputSentence)
  {
      int hexBase = 16;
      unsigned int endPoint = inputSentence.length() - 2;

      // Takes the actual checksum given by inputSentence
      string subStr = inputSentence.substr(endPoint);
      unsigned long hexVals = std::stoul(subStr, nullptr, hexBase);

      // Calculates a new checksum using a bitwise XOR
      unsigned int checksum = 0;
      for(unsigned int i = 1; i < endPoint - 1; i++)
      {
          checksum ^= inputSentence[i];
      }

      // Compares the two and returns the value
      return (checksum == hexVals);
  }

  SentenceData extractSentenceData(string inputSentence)
  {
      const int positionOffset = 10;
      // Finds the format of the sentence with starting position 3, spanning 3 characters
      string format = inputSentence.substr(3, 3);

      // Creates a vector for the position data
      // Finds the comma, and the exact end position of the positionaldata
      vector<string> positionalData;
      size_t comma = inputSentence.find(',');
      int endPosition = inputSentence.length() - positionOffset;

      // Uses stringstream to separate and add the data
      if (comma != string::npos)
      {
          std::stringstream separateStream(inputSentence.substr(comma + 1, endPosition));
          string section;
          while(separateStream.good())
          {
            getline(separateStream, section, ',');
            positionalData.push_back(section);
          }
      }

      return {format, {positionalData}};
  }

  GPS::Position positionFromSentenceData(SentenceData sentence)
  {
      // Fetches the main data that is going to be processed
      string format = sentence.first;
      vector<string> positionalData = sentence.second;

      unsigned int latLocation;
      unsigned int latDirLocation;
      unsigned int longLocation;
      unsigned int longDirLocation;

      string eleString = "0";

      // Sets the index locations for each format type
      if (format == "GLL" && positionalData.size() == 5)
      {   
          latLocation = 0;
          latDirLocation = 1;
          longLocation = 2;
          longDirLocation = 3;
      }
      else if (format == "GGA" && positionalData.size() == 14)
      {
          latLocation = 1;
          latDirLocation = 2;
          longLocation = 3;
          longDirLocation = 4;
          eleString = positionalData[8];
       }
       else if (format == "RMC" && positionalData.size() == 11)
       {
          latLocation = 2;
          latDirLocation = 3;
          longLocation = 4;
          longDirLocation = 5;
       }
       else { throw invalid_argument("Unsupported Format"); }

      // Gets the data that will be used
      char latDir = positionalData[latDirLocation][0];
      char longDir = positionalData[longDirLocation][0];

      if (latDir != 'N' && latDir != 'S') { throw invalid_argument("Invalid Northing"); }
      if (longDir != 'E' && longDir != 'W') { throw invalid_argument("Invalid Easting"); }

      // Returns a Position object based on given variables.
      return GPS::Position(positionalData[latLocation], latDir, positionalData[longLocation], longDir, eleString);
  }

  Route routeFromLog(std::istream &logStream)
  {
      // Declares the needed variables that are used multiple times
      string tempSentence;
      Route finalRoute;
      GPS::Position currentPosition(0, 0, 0);

      // Keeps looping until the end of the input stream
      while (logStream.good())
      {
          getline(logStream, tempSentence, '\n');
          if (isWellFormedSentence(tempSentence) && hasValidChecksum(tempSentence))
          {
              try
              {
                  // Attempts to make a Position object from the sentence,
                  // catching any exceptions from invalid data
                  currentPosition = positionFromSentenceData(extractSentenceData(tempSentence));
                  finalRoute.push_back(currentPosition);
              }
              catch (invalid_argument) { /*Needs to catch expected exceptions but not handle them*/}
          }
      }
      return finalRoute;
  }

}
