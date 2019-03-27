#ifndef REGISTER
#define REGISTER(...)
#define REGISTER_TO_BE_REMOVED
#endif

#define flag_register 10

REGISTER(rba, 1);
REGISTER(rbb, 2);
REGISTER(rbc, 3);
REGISTER(rbd, 4);
REGISTER(rbx, 5);
REGISTER(rby, 6);
REGISTER(rbn, 7);
REGISTER(rbz, 8);
REGISTER(rbf, 10);

#ifdef REGISTER_TO_BE_REMOVED
#undef REGISTER_TO_BE_REMOVED
#undef REGISTER
#endif
