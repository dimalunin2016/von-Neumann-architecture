#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <cstdio>

/**
 * @brief class interpreter of FILE* which close itself in destructor
 */
class CleverFile {
 private:
  
  FILE* file_ = nullptr;
  const char* FileName_;
 public:
 
  /**
   * @brief open file to read
   */
  void FileToRead(const char* FileName) {
 		
    file_ = fopen(FileName, "r");
    if (!file_) {
      fprintf (stderr, "Can not open file %s\n", FileName);
      assert(file_);
    }
    FileName_ = FileName;
  }
 

  void FileBinaryToRead(const char* FileName) {
 		
    file_ = fopen(FileName, "rb");
    if (!file_) {
      fprintf (stderr, "Can not open file %s\n", FileName);
      assert(file_);
    }
    FileName_ = FileName;
  }
  /**
   * @brief open file to write in it
   */  
  void FileToWrite(const char* FileName) {

    file_ = fopen(FileName, "w");
    if (!file_) {
      fprintf (stderr, "Can not open file %s\n", FileName);
      assert(file_);
    }
    FileName_ = FileName;
  }


  void FileToWriteInEnd(const char* FileName) {

    file_ = fopen(FileName, "a");
    if (!file_) {
      fprintf (stderr, "Can not open file %s\n", FileName);
      assert(file_);
    }
    FileName_ = FileName;
  }
  /**
   * @return pointer to file
   */
  FILE* GetFile() {

    return file_;
  }  

  
  size_t GetFileSize() const { 
    
    struct stat stat_buf;
    int exit = stat(FileName_, &stat_buf); 
    if (exit != 0) {
      fprintf (stderr, "Can not open file %s\n", FileName_);
      assert(exit == 0);
    }
    
    return stat_buf.st_size;
  }


  /**
   * @brief close file in the end
   */
  ~CleverFile() {
    
    fclose(file_);
    file_ = nullptr;
  }

};

