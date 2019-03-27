const int NUM_OF_EMPTY_LABEL = 200;

bool WorkWithNumbersOrWithNone(int n) {

  return n / 50 == 0;
}


bool WorkWithRamByNumbers(int n) {

  return n / 50 == 1;
}


bool WorkWithRegisters(int n) {

  return n / 50 == 2;
}


bool WorkWithRamByRegisters(int n) {

  return n / 50 == 3;
}


bool WorkWithLabels(int n) {

  return (n / 50 == 4) && n < 250;
}

bool WorkWithStaticTexts(int n) {

  return n >= 250;
}

