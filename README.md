# von-Neumann-architecture
Реализация архитектуры Фон-неймана
Программа на моем ассемблере, вычисляющая числа фиббоначи - `Fib.txt`

бинарный файл, полученный ассемблером - `Fibbin.txt`

файл, полученный дизассемблером из бинарного файла - `Fibcopy.txt`

Описание команд и их реализация в `commands.h`

Запуск:
1) Компиляция

`g++ -std=c++11 create_bin_cmd.cpp -o bincr`

`g++ -std=c++11 main.cpp -o launch_programm`

`g++ -std=c++11 dissasm.cpp -o dissasm`

2) получение бинарника

`./bincr -I <откуда читать> -O <куда писать> `

пример:

`./bincr -I Fib.txt -O Fibbin.txt`

3) исполнение программы

`./launch_programm -I <бинарный файл, откуда брать программу>`

пример:

`./launch_programm -I Fibbin.txt`

4) дизассемблер

`./dissasm -I <бинарник для чтения> -O <файл-ответ>`

пример:

`./dissasm -I Fibbin.txt -O Fibcopy.txt`
