#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>
#include <iostream>

#define INVALID_DEGREE -360.f
#define INVALID_VERTICAL_DEGREE {0.0, INVALID_DEGREE}
#define INVALID_HORIZONTAL_DEGREE {INVALID_DEGREE, 0.0}
#define DATA_WORD_SIZE 2
#define DATA_LINE_SIZE 1201
#define sqr(a) a*a
#define DATA_FILE_SIZE sqr(DATA_LINE_SIZE)

struct degreeInfo
{
   degreeInfo(): mind(0), maxd(180){}

   bool correctVal(int a) const
   { return mind <= a and a <= maxd; }

   int mind;
   int maxd;
};

struct readingInfo
{
   readingInfo()=default;

   degreeInfo n;
   degreeInfo s;
   degreeInfo e;
   degreeInfo w;
};

struct HGTdata
{
   float verticalAngle;
   float horizontalAngle;
   std::vector<float> data;
};

class HGTreader 
{
public:
   HGTreader()=default;

   // set data

   void readDataFromFile(const std::string fileName, const readingInfo& ri, std::vector<Tile>& tiles, int lod)
   {
      auto data = std::vector<float>(DATA_FILE_SIZE);
      std::string name = std::filesystem::path(fileName).stem().string();
      auto [verticalAngle, horizontalAngle] = getAngles(name, ri);

      if(verticalAngle == INVALID_DEGREE || horizontalAngle == INVALID_DEGREE)
         return;

      // reading data
      uint16_t word;
      std::ifstream ifile(fileName.c_str(), std::ios::binary);
      if (ifile.good())
      {
         for(int i=0; i<DATA_LINE_SIZE; i++)
            for (int j=0; j<DATA_LINE_SIZE; j++)
            {
               unsigned char buffer[2];
               ifile.read(reinterpret_cast<char*>(buffer), DATA_WORD_SIZE);
               word = ((uint16_t)buffer[0] << 8) | buffer[1];
               data[i*DATA_LINE_SIZE + j] = (float)word;
            }
      }
      else 
         throw std::runtime_error("File is not good :(");

      tiles.emplace_back(verticalAngle, horizontalAngle, data, lod);
      std::cout<<"Successfully read "<<fileName<<".."<<std::endl;
   }

   void readDataFromDirectory(char* dirPathStr, const readingInfo& ri, std::vector<Tile>& tiles, int lod)
   {
      // użycie C++ 17
      for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(dirPathStr)) {
         std::filesystem::path entryPath = entry.path();
         if (entryPath.extension().string() == ".hgt") {
            auto path = entryPath.string();
            readDataFromFile(path, ri, tiles, lod);
         }
         else 
         {
            std::cout<<"file "<<entryPath<<" is not a HGT file!"<<std::endl;
         }
      }
   }

   // returning data

   HGTdata readDataFromFile(const std::string fileName)
   {
      auto data = std::vector<float>(DATA_FILE_SIZE);
      std::string name = std::filesystem::path(fileName).stem().string();
      auto [verticalAngle, horizontalAngle] = getAngles(name);

      // reading data
      uint16_t word;
      std::ifstream ifile(fileName.c_str(), std::ios::binary);
      if (ifile.good())
      {
         for(int i=0; i<DATA_LINE_SIZE; i++)
            for (int j=0; j<DATA_LINE_SIZE; j++)
            {
               unsigned char buffer[2];
               ifile.read(reinterpret_cast<char*>(buffer), DATA_WORD_SIZE);
               word = ((uint16_t)buffer[0] << 8) | buffer[1];
               data[i*DATA_LINE_SIZE + j] = (float)word;
            }
      }
      else 
         throw std::runtime_error("File is not good :(");

      return {verticalAngle, horizontalAngle, data};
   }

   std::vector<HGTdata> readDataFromDirectory(std::string dirPathStr)
   {
      std::vector<HGTdata> data;
      // użycie C++ 17
      for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(dirPathStr)) {
         std::filesystem::path entryPath = entry.path();
         if (entryPath.extension().string() == ".hgt") {
            auto path = entryPath.string();
            data.push_back(readDataFromFile(path));
            std::cout<<"Successfully read "<<path<<".."<<std::endl;
         }
         else 
         {
            std::cout<<"file "<<entryPath<<" is not a HGT file!"<<std::endl;
         }
      }

      return data;
   }

   std::pair<float,float> getAngles(const std::string& name, const readingInfo& ri)
   {
      char vertSym, horSym;
      float vertAngle, horAngle;

      vertSym = name[0];
      vertAngle = std::atof(name.substr(1,2).c_str());
      horSym = name[3];
      horAngle = std::atof(name.substr(4,3).c_str());

      if(vertSym == 'N' and not ri.n.correctVal(vertAngle)) return INVALID_VERTICAL_DEGREE;
      if(vertSym == 'S' and not ri.s.correctVal(vertAngle)) return INVALID_VERTICAL_DEGREE;
      if(horSym == 'E' and not ri.e.correctVal(horAngle)) return INVALID_HORIZONTAL_DEGREE;
      if(horSym == 'W' and not ri.w.correctVal(horAngle)) return INVALID_HORIZONTAL_DEGREE;

      if (vertSym == 'S')
         vertAngle *= -1;
      if (horSym == 'W')
         horSym *= -1;

      return {vertAngle, horAngle};
   }

   std::pair<float,float> getAngles(const std::string& name)
   {
      char vertSym, horSym;
      float vertAngle, horAngle;

      vertSym = name[0];
      vertAngle = std::atof(name.substr(1,2).c_str());
      horSym = name[3];
      horAngle = std::atof(name.substr(4,3).c_str());

      if (vertSym == 'S')
         vertAngle *= -1;
      if (horSym == 'W')
         horSym *= -1;

      return {vertAngle, horAngle};
   }
};