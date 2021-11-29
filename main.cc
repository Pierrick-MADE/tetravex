#include <iostream>
#include "tetravex.hh"

int main(int argc, const char *argv[]) {
    if (argc == 2) {
        Tetravex tetravex = Tetravex();
        if (!tetravex.load_file(argv[1]))
            return 1;
        tetravex.draw_simple_board();
        tetravex.draw_board();
        tetravex.set_movement(1, 3);
        tetravex.draw_board();

        tetravex.set_movement(2, 1);
        tetravex.draw_board();

        tetravex.set_movement(3, 2);
        tetravex.draw_board();
        std::cout << (tetravex.check_board() ? "true": "false") << std::endl;
        return 0;
    }

    std::cerr << "Usage: " << argv[0] << " FILE_PATH" << std::endl;
    return 1;
}