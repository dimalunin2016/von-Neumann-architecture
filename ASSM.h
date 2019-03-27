#include"CleverFile.h"
#include "AssmTypeRecognizer.h"
#include "TextsAndFiles/Text.h"
#include <unordered_map>
#include <string>

const size_t MAX_SIZE = 100;
const size_t MAX_NUM_LABELS = 100;

void ReadNum(const char* number, int n,
             int *curr_ind_in_command_array,
             FILE** file, bool need_to_write) {

  if (need_to_write) {
    fwrite(&n, sizeof(char), 1, *file);
    int num = atoi(number);
    fwrite(&num, sizeof(int), 1, *file);
  } else {
    *curr_ind_in_command_array += 1;
    *curr_ind_in_command_array += sizeof(int);
  }
}

void ReadRegister(const char* registr, int n,
                  int *curr_ind_in_command_array,
                  FILE** file , bool need_to_write) {

  if (need_to_write) {
#define REGISTER(name, n_reg)                                   \
  if (!strncmp(#name, registr, strlen(#name))) {                \
    fwrite(&n, sizeof(char), 1, *file);                         \
    int r_n = n_reg;                                            \
    fwrite(&r_n, sizeof(char), 1, *file);                       \
  }
  #include "Registers.h"
#undef REGISTER
  }
  else {
    *curr_ind_in_command_array += 2;
  }
}


void ReadLabel(int label_ind, int n,
               int *curr_ind_in_command_array,
               FILE** file, bool need_to_write) {

  if (need_to_write) {
    fwrite(&n, sizeof(char), 1, *file);
    fwrite(&label_ind, sizeof(int), 1, *file);
  } else {
    *curr_ind_in_command_array += 5;
  }
}


void ReadText(int text_place, int n,
               int *curr_ind_in_command_array,
               FILE** file, bool need_to_write) {

  if (need_to_write) {
    fwrite(&n, sizeof(char), 1, *file);
    fwrite(&text_place, sizeof(int), 1, *file);
  } else {
    *curr_ind_in_command_array += 5;
  }
}

void ReadNone(int n, int *curr_ind_in_command_array,
    FILE** file, bool need_to_write) {

  if (need_to_write) {
    fwrite(&n, sizeof(char), 1, *file);
  } else {
    *curr_ind_in_command_array += 1;
  }
}


std::string CreateCorrectString(const char* curr) {

  size_t ind = 0;
  while(isspace(*(curr + ind))) {
    ind++;
  }
  std::string ans = curr + ind;
  std::string whitespaces (" \t\f\v\n\r");
  std::size_t found = ans.find_last_not_of(whitespaces);
  ans.resize (found + 1);
  return ans;
}


void CreateAssemblerBinaryInstructions(const char* FileName,
                                       const Text& commands) {

  CleverFile write_file;
  char first_command[MAX_SIZE]  = "";
  strcat(first_command, "rm ");
  strcat(first_command, FileName);
  system(first_command);
  int max_label = 0;
  int maxt_text = 0;
  std::unordered_map<std::string, int> labels;
  std::unordered_map<std::string, int> text_static_map;
  std::string texts[MAX_NUM_LABELS];
  std::string texts_static_rem[MAX_NUM_LABELS];
  int texts_starts[MAX_NUM_LABELS];

  write_file.FileToWriteInEnd(FileName);
  FILE *file = write_file.GetFile();
  size_t num_of_commands = commands.GetNumberOfLines();
  int label_array[MAX_NUM_LABELS] = {0};
  int curr_ind_in_command_array = sizeof(int);
  bool second_time = false;
#define DEF_CMD(name, n, code)                                  \
  if (!strncmp(#name, cmd, strlen(#name))) {                    \
    if (WorkWithNumbersOrWithNone(n)) {                         \
      if (strlen(#name) != strlen(cmd)) {                       \
        const char *numb = cmd + strlen(#name) + 1;             \
        if (numb[0] == '-' ||                                   \
            (numb[0] >= '0' && numb[0] <= '9')) {               \
          ReadNum(numb, n, &curr_ind_in_command_array, &file,   \
                  second_time);                                 \
        }                                                       \
      }                                                         \
      else {                                                    \
        ReadNone(n, &curr_ind_in_command_array, &file,          \
            second_time);                                       \
      }                                                         \
    }                                                           \
    else if (WorkWithRamByNumbers(n)) {                         \
      const char *numb = cmd + strlen(#name) + 2;               \
      if (numb[0] >= '0' && numb[0] <= '9' &&                   \
          numb[strlen(numb) - 1] == ']') {                      \
        char *copy_str = new char[strlen(numb)];                \
        strcpy(copy_str, numb);                                 \
        copy_str[strlen(numb) - 1] = 0;                         \
        ReadNum(copy_str, n, &curr_ind_in_command_array,        \
            &file, second_time);                                \
        delete[] copy_str;                                      \
      }                                                         \
    }                                                           \
    else if (WorkWithRegisters(n)) {                            \
      const char *reg = cmd + strlen(#name) + 1;                \
      if (strlen(#name) != strlen(cmd) &&                       \
          reg[0] >= 'a' && reg[0] <= 'z') {                     \
        ReadRegister(reg, n,                                    \
            &curr_ind_in_command_array, &file, second_time);    \
      }                                                         \
    }                                                           \
    else if (WorkWithRamByRegisters(n)) {                       \
      const char *reg = cmd + strlen(#name) + 2;                \
      if (reg[0] >= 'a' && reg[0] <= 'z' &&                     \
          reg[strlen(reg) - 1] == ']') {                        \
        char *copy_str = new char[strlen(reg)];                 \
        strcpy(copy_str, reg);                                  \
        copy_str[strlen(reg) - 1] = 0;                          \
        ReadRegister(copy_str, n,                               \
            &curr_ind_in_command_array, &file, second_time);    \
        delete[] copy_str;                                      \
      }                                                         \
    }                                                           \
    else if (WorkWithLabels(n)) {                               \
      if (cmd[strlen(#name) + 1] == ':' || cmd[0] == ':') {     \
        std::string label((cmd[0] == ':') ? cmd + 1 :           \
            cmd + strlen(#name) + 2);                           \
        int curr_lab_ind = 0;                                   \
        if (!second_time &&labels.find(label) ==                \
            labels.end()) {                                     \
          labels[label] = max_label++;                          \
        }                                                       \
        if (!second_time && n == NUM_OF_EMPTY_LABEL) {          \
          label_array[labels[label]] =                          \
          curr_ind_in_command_array + 5;                        \
        }                                                       \
        curr_lab_ind = label_array[labels[label]];              \
        ReadLabel(curr_lab_ind, n,                              \
            &curr_ind_in_command_array, &file, second_time);    \
      }                                                         \
    }                                                           \
    else if (WorkWithStaticTexts(n)) {                          \
      if (cmd[strlen(#name) + 1] == '@') {                      \
        std::string text_static = cmd + strlen(#name) + 1;      \
        int curr_text_ind = text_static_map[text_static];       \
        int curr_place_of_text = texts_starts[curr_text_ind];   \
        ReadText(curr_place_of_text, n,                         \
            &curr_ind_in_command_array, &file, second_time);    \
      }                                                         \
    }                                                           \
  }

  for (size_t ind = 0; ind < num_of_commands; ++ind) {

    const char* cmd = commands[ind].GetCharArr();
    std::string curr = CreateCorrectString(cmd);
    cmd = curr.c_str();
    if (strlen(cmd) != 0) {
#include "commands.h"
    }

    if (curr == ".data") {
      ind += 1;
      int text_place = curr_ind_in_command_array;
      for (auto it = labels.begin(); it != labels.end(); ++it) {

        text_place += 1;
        int sz = (it->first).size();
        text_place += sz;
        text_place += 5;
      }
      text_place += 3;
      for (; ind < num_of_commands; ++ind) {
        const char* cmd = commands[ind].GetCharArr();
        std::string curr = CreateCorrectString(cmd);
        std::string text_static = "";
        std::string text = "";
        bool is_space = false;
        for (int i = 0; cmd[i]; ++i) {
          if (!is_space && cmd[i] == ' ') {
            is_space = true;
          }
          else if (!is_space) {
            text_static += cmd[i];
          } else if(is_space) {
            text += cmd[i];
          }
        }

        text_static_map[text_static] = maxt_text++;
        texts[maxt_text - 1] = text;
        texts_static_rem[maxt_text - 1] = text_static;
        texts_starts[maxt_text - 1] = text_place + text_static.size();
        text_place += text.size() + text_static.size() + 1;
        text_place += 1;
      }
    }
  }

  fwrite(&curr_ind_in_command_array, sizeof(int), 1, file);
  second_time = true;
  for (size_t ind = 0; ind < num_of_commands; ++ind) {

    const char* cmd = commands[ind].GetCharArr();
    std::string curr = CreateCorrectString(cmd);
    cmd = curr.c_str();
    if (curr == ".data") {
      break;
    }
    if (strlen(cmd) != 0) {

#include "commands.h"
    }
  }

  for (auto it = labels.begin(); it != labels.end(); ++it) {

    fwrite("\n", sizeof(char), 1, file);
    int sz = (it->first).size();
    fwrite((it->first).c_str(), sizeof(char), sz, file);
    fwrite(" ", sizeof(char), 1, file);
    fwrite(&(label_array[it->second]), sizeof(int), 1, file);
  }
  fwrite("\n", sizeof(char), 1, file);
  for (int i = 0; i < maxt_text; ++i) {

    fwrite("\n", sizeof(char), 1, file);
    int sz = texts_static_rem[i].size();
    fwrite(texts_static_rem[i].c_str(), sizeof(char), sz, file);
    fwrite(" ", sizeof(char), 1, file);
    sz = texts[i].size();
    fwrite(texts[i].c_str(), sizeof(char), sz, file);
  }
  fwrite("\n", sizeof(char), 1, file);
#undef DEF_CMD

}

