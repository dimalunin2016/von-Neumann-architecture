#include "Registers.h"

#ifndef DEF_CMD
#define DEF_CMD(...)
#define DEF_CMD_TO_BE_REMOVED
#endif

#define Print(file, x) fprintf(file, "%d\n", x)

#define Scan(x) scanf("%d", &x);

#define Push(x)                                                       \
  proc_stack_.PushBack(x)

#define Pop                                                           \
  proc_stack_.PopBack()

#define PushInRAM(ind, val)                                           \
  (*this).PushInRam(ind, val)

#define GetFromRAM(ind)                                               \
  (*this).GetFromRam(ind)

#define GetIntCmd                                                     \
   *reinterpret_cast<int *>(                                          \
       (curr_insruction_ += sizeof(int)), raw_data_ +            \
       (curr_insruction_ - sizeof(int)))

#define GetRegCmd                                                     \
  static_cast<int>(                                                   \
       (curr_insruction_ += sizeof(char)), *(raw_data_ +         \
       (curr_insruction_ - sizeof(char))))

#define IfJmp(cond); {                                                \
  if (static_cast<int>(register_array_[flag_register]) cond 0) {      \
    curr_insruction_ = GetIntCmd;                                     \
  } else {                                                            \
    GetIntCmd;                                                        \
  }                                                                   \
}

DEF_CMD(end, 0, {
  is_end_ = true; // shows, where programm must stop
});


DEF_CMD(push, 1, {
    int num = GetIntCmd; // push simple number in stack
    Push(num);
    });

DEF_CMD(push, 51, {

    int num = GetFromRAM(GetIntCmd); // push number from RAM where index is number in stack
    Push(num);
    });

DEF_CMD(push, 101, {
    Push(register_array_[GetRegCmd]); // push number from register in stack
    });

DEF_CMD(push, 151, {
    Push(GetFromRAM(register_array_[GetRegCmd])); // push number from Ram by index from register in stack
    });

///////////////////////////////////////////////////////////////////////
DEF_CMD(pop, 2, {
      Pop; // pop number from stack
    });

DEF_CMD(pop, 52, {
    int num = GetIntCmd;
    PushInRAM(num, Pop); // pop from stack in ram where index is number
    });

DEF_CMD(pop, 102, {
    register_array_[GetRegCmd] = Pop; // pop from stack in register
    });

DEF_CMD(pop, 152, {
    PushInRAM(register_array_[GetRegCmd], Pop); // pop from stack in ram where index from ram
    });

/////////////////////////////////////////////////////////////////////////

DEF_CMD(Plus, 3, {
    Push(Pop + Pop); // pop 2 last elements from stack and push sum
    });

DEF_CMD(Minus, 4, {
    Push(Pop - Pop); // pop 2 last elements from stack and push difference
    });

DEF_CMD(in, 5, {
    int xin = 0;
    Scan(xin); // scan number and push it in stack
    Push(xin);
    });

DEF_CMD(Multiply, 6, { //  pop 2 last elements from stack and push multiplication of them
    Push(Pop * Pop);
    });

DEF_CMD(Divide, 7, { // --//-- division of them
    Push(Pop / Pop);
    });

DEF_CMD(out, 10, {
    int top = Pop;  // Print number from top of stack and drop it from stack
    Print(stdout, top);
  });

DEF_CMD(Bool_equal, 14, { // Push on the top 1 if 2 last elements are equal else 0
    Push(Pop == Pop);
    });

DEF_CMD(pseudo_call, 47, { // in recursion to save current variables, pop the last element from stack and save in
    function_stack_.PushBack(Pop);  // recursion stack, variable return by pseudo_ret
  });
DEF_CMD(pseudo_ret, 48, { // push saved in the beginning of recursion variable back to stack
    Push(function_stack_.PopBack());
    });

DEF_CMD(ret, 49, { // move instruction to saved pointer of the function start from special function stack
    curr_insruction_ = function_stack_.PopBack(); // and continue to execute programm
  });

DEF_CMD(call, 210, { // save pointer where you need to return and jump into function section of code
    function_stack_.PushBack(curr_insruction_ + sizeof(int));
    curr_insruction_ = GetIntCmd;
  });

DEF_CMD(, 200, { // read label and do nothing (example string command: ':label')
    GetIntCmd;
  });

DEF_CMD(jmp, 201, { // jump where is label situated
    curr_insruction_ = GetIntCmd;
  });

DEF_CMD(je, 202, { // jmp if register rbf is equal to 0
    IfJmp(==);
  });

DEF_CMD(jne, 203, { // jmp if register rbf is not equal to 0
    IfJmp(!=);
  });

DEF_CMD(ja, 204, { // jmp if register rbf > 0
    IfJmp(>);
  });

DEF_CMD(jae, 205, { // jmp if register rbf >= 0
    IfJmp(>=);
  });

DEF_CMD(jb, 206, { // jmp if register rbf < 0
    IfJmp(<);
  });

DEF_CMD(jbe, 207, { // jmp if register rbf < 0
    IfJmp(<=);
  });

DEF_CMD(textout, 250, { // print static text from .data
    int num = GetIntCmd;
    for (; raw_data_[num] != '\n'; num++) {
      fprintf(stdout, "%c", raw_data_[num]);
    }
    fprintf(stdout, "\n");
  });

#undef Print
#undef Scan
#undef Push
#undef Pop
#undef PushInRam
#undef GetFromRAM
#undef GetIntCmd
#undef GetRegCmd
#ifdef DEF_CMD_TO_BE_REMOVED
#undef DEF_CMD_TO_BE_REMOVED
#undef DEF_CMD
#endif
