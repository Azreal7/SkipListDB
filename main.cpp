#include "SkipList.h"
#include "operations.h"
#include <iostream>

int main()
{
    SkipList<int, int> *skip_list = new SkipList<int, int>(16);
    std::string inst;
    Operations operations(skip_list);

    while (std::cin >> inst)
    {
        operations.execute_command(inst);
    }

    return 0;
}