#include "CleverFile.h"
#include "AssmTypeRecognizer.h"
#include <cmath>
#include <string>
#include <unordered_map>

void PrintRegistrByNumber(int n, FILE **file) {

#define REGISTER(name, n_reg)                                   \
  if (n == n_reg) {                                             \
    fprintf(*file, #name);                                      \
  }
  #include "Registers.h"
#undef REGISTER

}

void DissasmFile(const char* InstrFile, const char *disFile) {


#define NamePrint(file, name)                                       \
  std::string curr_name = #name;                                    \
  fprintf(file, "%s", curr_name.c_str());

#define cmd (static_cast<int>(                                      \
               command_array_[ind]) + 256) % 256

#define DEF_CMD(name, n, code)                                      \
    if (ind < comm_size && cmd == n) {                              \
      ind += 1;                                                     \
      if (WorkWithNumbersOrWithNone(n)) {                           \
        if (#name == "push") {                                      \
          int num = *reinterpret_cast<int *>(                       \
          (ind += sizeof(int)), command_array_ +                    \
          (ind - sizeof(int)));                                     \
          NamePrint(file, name);                                    \
          fprintf(file, " ");                                       \
          Print(file, num);                                         \
        } else {                                                    \
          NamePrint(file, name);                                    \
          fprintf(file, "\n");                                      \
        }                                                           \
      }                                                             \
      else if (WorkWithRamByNumbers(n)) {                           \
        NamePrint(file, name);                                      \
        fprintf(file, " [");                                        \
        int num =  *reinterpret_cast<int *>(                        \
          (ind += sizeof(int)), command_array_ +                    \
          (ind - sizeof(int)));                                     \
        fprintf(file, "%d]\n", num);                                \
      }                                                             \
      else if (WorkWithRegisters(n)) {                              \
        NamePrint(file, name);                                      \
        fprintf(file, " ");                                         \
        int r_n = static_cast<int>(                                 \
        (ind += sizeof(char)), *(command_array_ +                   \
        (ind - sizeof(char))));                                     \
        PrintRegistrByNumber(r_n, &file);                           \
        fprintf(file, "\n");                                        \
      }                                                             \
      else if (WorkWithRamByRegisters(n)) {                         \
        NamePrint(file, name);                                      \
        fprintf(file, " [");                                        \
        int r_n = static_cast<int>(                                 \
        (ind += sizeof(char)), *(command_array_ +                   \
        (ind - sizeof(char))));                                     \
        PrintRegistrByNumber(r_n, &file);                           \
        fprintf(file, "]\n");                                       \
      }                                                             \
      else if (WorkWithLabels(n)) {                                 \
        int num = *reinterpret_cast<int *>(                         \
            (ind += sizeof(int)), command_array_ +                  \
            (ind - sizeof(int)));                                   \
        if (n != NUM_OF_EMPTY_LABEL) {                              \
          NamePrint(file, name);                                    \
          fprintf(file, " ");                                       \
        }                                                           \
        fprintf(file, ":%s\n", (labels[num]).c_str());              \
      } else if (WorkWithStaticTexts(n)) {                          \
        int num = *reinterpret_cast<int *>(                         \
            (ind += sizeof(int)), command_array_ +                  \
            (ind - sizeof(int)));                                   \
        NamePrint(file, name);                                      \
        fprintf(file, " ");                                         \
        fprintf(file, "%s\n", (text_static[num]).c_str());          \
      }                                                             \
    }

  const size_t MAX_SIZE = 256;
  char first_command[MAX_SIZE]  = "";
  strcat(first_command, "rm ");
  strcat(first_command, disFile);
  system(first_command);
  char *command_array_ = nullptr;
  size_t size_file  = 0;
  std::unordered_map<int, std::string> labels;
  std::unordered_map<int, std::string> text_static;
  std::unordered_map<int, std::string> text_text;
  CleverFile read_file;
  CleverFile write_file;
  read_file.FileBinaryToRead(InstrFile);
  write_file.FileToWriteInEnd(disFile);

  FILE *file = write_file.GetFile();
  size_file = read_file.GetFileSize();
  command_array_ = new char[size_file];
  fread(command_array_, sizeof(char), size_file, read_file.GetFile());
  int comm_size = *reinterpret_cast<int *>(command_array_);
  bool is_label = true;
  int here_text = 0;
  for (size_t ind = comm_size + 1; ind < size_file; ++ind) {

      if (command_array_[ind] == '\n') {
        ind++;
        here_text = ind;
        std::string curr = "";
        std::string text = "";
        int place_curr = 0;
        for (; ind < size_file; ++ind) {
            curr = "";
            text = "";
            for (; command_array_[ind] != ' ' && command_array_[ind] != '\n';
                ++ind) {

              curr += command_array_[ind];
            }
            if (curr == "") {
              break;
            }
            place_curr = ind + 1;
            ind++;
            for (; command_array_[ind] != '\n'; ++ind) {
              text += command_array_[ind];
            }
            if (curr != "\n") {
              text_static[place_curr] = curr;
              text_text[place_curr] = text;
            }
        }
        is_label = false;
      }
      if (is_label) {
        std::string curr_label = "";
        for (;command_array_[ind] != ' '; ++ind) {
          curr_label += command_array_[ind];
        }
        ++ind;
        int curr_ind = *reinterpret_cast<int *>(command_array_ + ind);
        labels[curr_ind] = curr_label;
        ind += sizeof(int);
      }
  }



  for (int ind = sizeof(int); ind < comm_size;) {
#include "commands.h"
  }

  fprintf(file, "\n.data\n");

  for (auto it = text_static.begin(); it != text_static.end(); ++it) {

    fprintf(file, "%s %s\n",
        (it->second).c_str(), text_text[it->first].c_str());
  }

#undef DEF_CMD
#undef cmd
#undef NamePrint
  delete command_array_;
}


