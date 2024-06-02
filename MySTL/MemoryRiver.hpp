#ifndef MEMORYRIVER_HPP
#define MEMORYRIVER_HPP
#include <fstream>
#include <iostream>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
template <class T, int info_len = 2>
class MemoryRiver
{
private:
   fstream file;
   string file_name;
   string recycle_file_name;
   int sizeofT = sizeof(T);
   int recycle_num;

public:
   MemoryRiver() = default;
   MemoryRiver(const string &file_name) : file_name(file_name)
   {
      initialise();
   }
   ~MemoryRiver()
   {
      file.open(recycle_file_name);
      file.seekp(0);
      file.write(reinterpret_cast<char *>(&recycle_num), sizeof(int));
      file.close();
   }
   void initialise(string FN = "")
   {
      if (FN != "")
      {
         file_name = FN;
         recycle_file_name = file_name + "_recycle";
      }
      file.open(file_name);
      if (!file)
      {
         file.close();
         file.open(file_name, std::ios::out);
         int tmp = 0;
         for (int i = 0; i < info_len; ++i)
         {
            file.seekp(i * sizeof(int));
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
         }
      }
      file.close();
      file.open(recycle_file_name);
      if (!file)
      {
         file.close();
         file.open(recycle_file_name, std::ios::out);
         recycle_num = 0;
         file.seekp(0);
         file.write(reinterpret_cast<char *>(&recycle_num), sizeof(int));
      }
      else
      {
         file.seekg(0);
         file.read(reinterpret_cast<char *>(&recycle_num), sizeof(int));
      }
      file.close();
   }
   void get_info(int &tmp, int n)
   {
      if (n > info_len)
         return;
      file.open(file_name);
      file.seekg((n - 1) * sizeof(int));
      file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
      file.close();
   }
   void write_info(int tmp, int n)
   {
      if (n > info_len)
         return;
      file.open(file_name);
      file.seekp((n - 1) * sizeof(int));
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
      file.close();
   }
   int write(T &t)
   {
      if (!recycle_num)
      {
         file.open(file_name);
         file.seekp(0, std::ios::end);
         unsigned long position = static_cast<unsigned long>(file.tellp());
         int index = (position - info_len * sizeof(int)) / sizeofT;
         file.write(reinterpret_cast<char *>(&t), sizeofT);
         file.close();
         return index;
      }
      else
      {
         int index;
         file.open(recycle_file_name);
         file.seekg(recycle_num-- * sizeof(int));
         file.read(reinterpret_cast<char *>(&index), sizeof(int));
         file.close();
         file.open(file_name);
         file.seekp(info_len * sizeof(int) + index * sizeofT);
         file.write(reinterpret_cast<char *>(&t), sizeofT);
         file.close();
         return index;
      }
   }
   void update(T &t, const int index)
   {
      file.open(file_name);
      file.seekp(info_len * sizeof(int) + index * sizeofT);
      file.write(reinterpret_cast<char *>(&t), sizeofT);
      file.close();
   }
   void read(T &t, const int index)
   {
      file.open(file_name);
      file.seekg(info_len * sizeof(int) + index * sizeofT);
      file.read(reinterpret_cast<char *>(&t), sizeofT);
      file.close();
   }
   void Delete(int index)
   {
      file.open(recycle_file_name);
      file.seekp(++recycle_num * sizeof(int));
      file.write(reinterpret_cast<char *>(&index), sizeof(int));
      file.close();
   }
   void clear()
   {
      recycle_num = 0;
      file.open(file_name, std::ios::out | std::ios::trunc);
      file.close();
      file.open(recycle_file_name, std::ios::out | std::ios::trunc);
      file.close();
   }
};

#endif 