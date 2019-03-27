#include <algorithm>
#include <cstring>
#include <sys/stat.h>
#include <cassert>
#include <cstdlib>


const int MAX_COMMAND_SIZE = 100;


class TextLine {
 private:

  const char* symbols_;
  size_t size_;
 public:


  void CreateLine(const char* str) {
   
    symbols_ = str;
    size_    = strlen(symbols_);
  }


  void CreateLine(const char* str, size_t size) {
    
    symbols_ = str;
    size_    = size;
  }

  size_t Size() const {
    return size_;
  }

  char operator [](size_t ind) const {
    
    assert(("index out of range", ind < size_));
    return symbols_[ind];
  }


  const char* GetCharArr() const {
    
    return symbols_;
  }
};


/**
 * @brief class, which helps to work with texts from files
 */
class Text {
 private:
  
  char* text_             = nullptr;
  TextLine* origin_lines_ = nullptr;
  size_t num_of_lines_    = 0;
  size_t text_size_in_bytes_ = 0;

  void CopyText(const char* FileName) {
    
    CleverFile read_file;
    read_file.FileToRead(FileName);
    size_t size = read_file.GetFileSize();
    text_ = new char[size + 2];
    fread(text_, sizeof(char), size, read_file.GetFile()); 
    text_[size]         = '\n';
    text_[size + 1]     = '\0';
    text_size_in_bytes_ = size;
  }
  

  void CalcNumOfLines() {
   
    for (size_t i = 0; text_[i]; ++i) {
      if (text_[i] == '\n') {
         
        ++num_of_lines_;
      } 
    }
  }


  void PushLineInArr(
      size_t curr_index_of_line, 
      size_t curr_start_of_line,
      size_t size) {
    
    origin_lines_[curr_index_of_line].CreateLine(
      text_ + curr_start_of_line, size);
  }


  void PrepareTextAfterCopyng() {
    
    CalcNumOfLines();
    origin_lines_ = new TextLine[num_of_lines_];
    
    size_t curr_start_of_line = 0;
    size_t curr_index_of_line = 0;
    size_t size               = 0;
    for (size_t i = 0; text_[i]; ++i) {
      
      if (text_[i] == '\n') {
        text_[i] = '\0';
        size     = i - curr_start_of_line;
        PushLineInArr(curr_index_of_line, curr_start_of_line, size);
        ++curr_index_of_line;
        curr_start_of_line = i + 1;
      }
    }
  } 

  void PrintHelper(
      const char* FileName) const {
    
    CleverFile write_file;
    write_file.FileToWrite(FileName);
    for (size_t i = 0; i < num_of_lines_; ++i) {
      
      const char* a = origin_lines_[i].GetCharArr();
      fprintf(write_file.GetFile(), "%s\n", a);
    }
  }

 public:
  
  
  Text() {}


  /**
   * @brief decode input file in UTF-8 then copy it and prapare data 
   *    to work with it
   */
  void CreateText(const char* FileName) {
    CopyText(FileName);
    PrepareTextAfterCopyng();   
  }


  /**
   * @bref decode input file in UTF-8 then copy it and prapare data 
   *    to work with it
   */
	Text(const char* FileName) {
      
    CreateText(FileName);
	} 

  
  /**
  * @brief return const Textline by ind
  */
  const TextLine& operator [](size_t ind) const {
    
    if (num_of_lines_ <= ind) { 
        fprintf (stderr, "Access to an inaccesible piece of memory in const operator [] of Text\n");
        assert(0);
    }
    return origin_lines_[ind];
  }
  

  /**
  * @brief return pointer to the begining of current array of TextLine
  */
  const TextLine* operator *() const {
   
    return origin_lines_;
  }


  size_t GetNumberOfLines() const {
    
    return num_of_lines_;
  }


  /**
   * @brief print original text in file
   */
  void PrintText(const char* FileName) const {
    
    PrintHelper(FileName);
  }


  ~Text() {
    delete[]      origin_lines_;
    delete[]      text_;
    text_         = nullptr;
    origin_lines_ = nullptr;
  }
};

