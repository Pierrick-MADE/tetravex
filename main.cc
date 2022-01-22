#include <iostream>
#include <chrono>
#include "tetravex.hh"

#define LOG_RANDOM_BOARD_STATS true

void print_usage(const char *argv[])
{
    std::cerr << "Usage: " << argv[0] << " in.txt out.txt" << std::endl;
    std::cerr << "Or: " << argv[0] << " random BOARD_SIZE NB_BOARDS" << std::endl;
}

int main(int argc, const char *argv[]) {
    if (argc >= 3) {
        Tetravex tetravex = Tetravex();

        // if not random board (try to open the given file)
        if (std::string(argv[1]) != "random")
        {
            if (!tetravex.load_file(argv[1]))
                return 1;

            std::cout << "INITIAL BOARD :" << std::endl;
            tetravex.draw_board();

            auto t1 = std::chrono::high_resolution_clock::now();
            tetravex.solve();
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            std::cout << "The resolution took: " << ms_double.count() << "ms" << std::endl;
            std::cout << "nb iterations: " << tetravex.nb_iterations << std::endl;


            std::cout << "SOLVED BOARD :" << std::endl;
            tetravex.draw_board();

            tetravex.save_board(argv[2]);
            std::cout << "Solved board saved to : [" << argv[2] << "]" << std::endl;

            return 0;
        }

        // Generate and solve random boards

        int board_size, nb_board;
        try
        {
            board_size = std::stoi(argv[2]);
            nb_board = std::stoi(argv[3]);
        }
        catch(...)
        {
            std::cerr << "Could not parse board size or number of boards." << std::endl;
            print_usage(argv);
            return 1;
        }
        if (board_size < 2 || board_size > 6)
        {
            std::cerr << "Invalid board size (too small/large)." << std::endl;
            return 1;
        }

        std::chrono::duration<double, std::milli> total_time;
        int total_iterations = 0;
        std::ofstream log_all_file("log_random_" + std::to_string(board_size) + "_" + std::to_string(nb_board) + ".txt");

        for (int i=0; i < nb_board; i++)
        {
            std::cout << "Resolving the board of size " << board_size << ", number: " << i + 1 << "/" << nb_board << std::endl;

            tetravex.generate_random_board(board_size);
            tetravex.randomize_board();

            auto t1 = std::chrono::high_resolution_clock::now();
            tetravex.solve();
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            std::cout << "The resolution took: " << ms_double.count() << "ms" << std::endl;
            std::cout << "nb iterations: " << tetravex.nb_iterations << std::endl;
            total_time += t2-t1;
            total_iterations += tetravex.nb_iterations;

            if (LOG_RANDOM_BOARD_STATS)
                log_all_file << std::to_string(ms_double.count()) << ";"
                             << std::to_string(tetravex.nb_iterations) << std::endl;
        }
        std::cout << "Mean time: " << (total_time / nb_board).count() << "ms" << std::endl;
        std::cout << "Mean nb_iterations: " << total_iterations / nb_board << std::endl;

        std::cout << "Saved log file to : [" << "log_random_" + std::to_string(board_size) + "_" + std::to_string(nb_board) + ".txt" << "]" << std::endl;

        log_all_file.close();
        return 0;
    }

    print_usage(argv);
    return 1;
}