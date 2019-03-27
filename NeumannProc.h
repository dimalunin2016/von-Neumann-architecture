#include"CleverFile.h"
#include <cmath>
#include <unistd.h>

class MyStack {

  char *start_;
  int curr_ind = 0;

 public:

  MyStack() {}
  MyStack(char *raw_data, int start_ind) : start_(raw_data + start_ind) {}

  void PushBack(int x) {

    *reinterpret_cast<int *>(start_ + curr_ind) = x;
    curr_ind += sizeof(int);
  }

  int PopBack() {

    curr_ind -= sizeof(int);
    return *reinterpret_cast<int *>(start_ + curr_ind);
  }
};


class Processor {

  static const int MAXSIZE = 1e6;
  const int START_RAM = 3e5;
  const int PROC_STACK_START = 2e5;
  const int FUNC_STACK_START = 1e5;
  char raw_data_[MAXSIZE];
#define MAX_NUMBER_OF_REGISTERS_ 256
  size_t curr_insruction_ = 0;
  bool is_end_ = false;
  int register_array_[MAX_NUMBER_OF_REGISTERS_] = {0};
  MyStack proc_stack_;
  MyStack function_stack_;

#define cmd (static_cast<int>(                                      \
             raw_data_[curr_insruction_]) + 256) % 256
#define DEF_CMD(name, n, code)                                      \
  if (!is_end_ && cmd == n) {                                       \
    curr_insruction_ += 1;                                          \
    code                                                            \
  }


  void ExecuteProcCmds(const char* InstrFile) {

    size_t size_file  = 0;
    CleverFile read_file;
    read_file.FileBinaryToRead(InstrFile);
    size_file = read_file.GetFileSize();
    fread(raw_data_, sizeof(char), size_file, read_file.GetFile());
    for (curr_insruction_ = sizeof(int); curr_insruction_ < size_file && !is_end_;) {
#include "commands.h"
    }

  }

  void DeleterHelper() {

    curr_insruction_ = 0;
    is_end_ = false;
  }

  void PushInRam(int ind, int num) {
    *reinterpret_cast<int *>(raw_data_ + START_RAM + ind) = num;
  }


  int GetFromRam(int ind) {
    return *reinterpret_cast<int *>(raw_data_ + START_RAM + ind);
  }

 public:

  Processor() : proc_stack_(raw_data_ + 0, PROC_STACK_START),
                function_stack_(raw_data_ + 0, FUNC_STACK_START) {}

  void ExecuteNewProgramm(const char* InstrFile) {
     DeleterHelper();
     ExecuteProcCmds(InstrFile);
  }


  ~Processor() {
    DeleterHelper();
  }
#undef DEF_CMD
#undef cmd
};

