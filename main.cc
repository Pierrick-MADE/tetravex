#include <iostream>
#include "tetravex.hh"

int main(int argc, const char *argv[]) {
    if (argc == 3) {
        Tetravex tetravex = Tetravex();
        if (!tetravex.load_file(argv[1]))
            return 1;
        tetravex.draw_simple_board();

        tetravex.draw_board();
        std::cout << (tetravex.check_board() ? "true": "false") << std::endl;
        std::cout << "score:" << tetravex.get_score() << std::endl;

        tetravex.solve();

        tetravex.draw_board();
        std::cout << (tetravex.check_board() ? "true": "false") << std::endl;
        std::cout << "score:" << tetravex.get_score() << std::endl;
        return 0;
    }

    std::cerr << "Usage: " << argv[0] << " in.txt out.txt" << std::endl;
    return 1;
}