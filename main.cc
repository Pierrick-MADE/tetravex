#include <iostream>
#include <chrono>
#include "tetravex.hh"

int main(int argc, const char *argv[]) {
    if (argc == 3) {
        Tetravex tetravex = Tetravex();
        if (!tetravex.load_file(argv[1]))
            return 1;
        tetravex.draw_simple_board();
        tetravex.draw_board();

        std::chrono::duration<double, std::milli> total_time;

        for (int i=0; i<10; i++)
        {
            auto t1 = std::chrono::high_resolution_clock::now();

            tetravex.generate_random_board(6);
            tetravex.randomize_board();
            std::cout << "score:" << tetravex.get_score() << std::endl;
            tetravex.solve(true);
            std::cout << "score:" << tetravex.get_score() << std::endl;

            auto t2 = std::chrono::high_resolution_clock::now();
            total_time += t2-t1;

            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            std::cout << "time:" << ms_double.count() << "ms" << std::endl;
        }
        std::cout << "mean time:" << (total_time / 10).count() << "ms" << std::endl;

        return 0;
    }

    std::cerr << "Usage: " << argv[0] << " in.txt out.txt" << std::endl;
    return 1;
}