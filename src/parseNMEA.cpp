#include "earth.h"
#include "parseNMEA.h"

#include <regex>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <cmath>

using std::regex;
using std::string;
using std::vector;

// REMEMBER TO REMOVE <iostream> ONCE COMPLETED


namespace NMEA
{

  bool isWellFormedSentence(string inputSentence)
  {
      //A regex statement to cover all possible combinations of NMEA sentences,
      //which is then checked against the input sentence.

      regex regexFullSentence("\\$GP[A-Z]{3}[\\-A-Za-z0-9,.]*\\*[0-9A-Fa-f]{2}");

      return regex_match(inputSentence, regexFullSentence);
  }

  bool hasValidChecksum(string inputSentence)
  {
      int hexBase = 16;
      unsigned int endPoint = inputSentence.length() - 2;

      //Takes the actual checksum given by inputSentence
      string subStr = inputSentence.substr(endPoint);
      unsigned long hexVals = std::stoul(subStr, nullptr, hexBase);

      //Calculates a new checksum using XOR
      unsigned int checksum = 0;
      for(unsigned long i = 1; i < endPoint - 1; i++)
      {
          checksum ^= inputSentence[i];
      }

      //Compares the two
      if (checksum == hexVals) { return true; }
      else { return false; }
  }

  SentenceData extractSentenceData(string inputSentence)
  {
      const int posOffset = 10;
      //Finds the format with starting position 3, spanning 3 characters.
      string format = inputSentence.substr(3, 3);

      //Creates a vector for the positional and finds the
      //Finds the comma, and the exact end position of the positionaldata.
      vector<string> positionalData;
      size_t comma = inputSentence.find(',');
      int endPosition = inputSentence.length() - posOffset;

      //Uses stringstream to separate the data and adds
      //the data to the vector separately.
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
      string format = sentence.first;
      vector<string> positionalData = sentence.second;

      string latString = "";
      string longString = "";
      string eleString = "0";

      char latDir;
      char longDir;

      if (positionalData.empty()) { throw std::invalid_argument("Missing Data"); }

      //Gathers the relevant positional data and assigns it to relevant variables.
      if (format == "GLL" && positionalData.size() == 5)
      {
          assert (positionalData.size() == 5);
          latString = positionalData[0];
          latDir = positionalData[1][0];


          longString = positionalData[2];
          longDir = positionalData[3][0];
      }
      else if (format == "GGA" && positionalData.size() == 14)
      {
          assert (positionalData.size() == 14);
          latString = positionalData[1];
          latDir = positionalData[2][0];

          longString = positionalData[3];
          longDir = positionalData[4][0];

          eleString = positionalData[8];
       }
       else if (format == "RMC" && positionalData.size() == 11)
       {
          assert (positionalData.size() == 11);
          latString = positionalData[2];
          latDir = positionalData[3][0];

          longString = positionalData[4];
          longDir = positionalData[5][0];
       }
       else
       {
          throw std::invalid_argument("Unsupported Format");
       }
      if (latDir != 'N' && latDir != 'S') { throw std::invalid_argument("Invalid Northing"); }
      if (longDir != 'E' && longDir != 'W') { throw std::invalid_argument("Invalid Easting"); }

      //Returns a Position based on given variables.
      return GPS::Position(latString, latDir, longString, longDir, eleString);
  }

  Route routeFromLog(std::istream &logStream)
  {
      // Takes input stream as a parameter
      // Need to iterate through stream
      // Assert each previous function to the given sentence
      // Use each sentence to get the GPS::Position
      // Save the position to the Route (vector<GPS::Position>

      string tempSentence;
      Route finalRoute;
      GPS::Position currentPosition(0, 0, 0);
      int counter1 = 0;
      int counter2 = 0;



      while (logStream.good()) {
          getline(logStream, tempSentence, '\n');

          if (isWellFormedSentence(tempSentence))
          {
              counter1++;
              if (hasValidChecksum(tempSentence))
              {
                  counter2++;
                  {
                          try
                          {
                           currentPosition = positionFromSentenceData(extractSentenceData(tempSentence));
                           finalRoute.push_back(currentPosition);
                          }
                          catch (...)
                          {

                          }
                      }
              }
          }

        }
      // Stub definition, needs implementing
      return finalRoute;
  }

}
