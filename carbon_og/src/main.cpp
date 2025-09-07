#include <iostream>
#include "msutil.hpp"
#include "game/game_main.hpp"

i32 main() {
    if (game_main())
        std::cout << "failed to execute game!" << std::endl;




    return 0;
}