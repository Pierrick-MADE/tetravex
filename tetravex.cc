#include "tetravex.hh"

// GET / SET

void Tetravex::copy_movements_to(int *out_arr)
{
    std::copy(movements, movements + size * size,
              out_arr);
}

void Tetravex::load_movements_from(int *in_arr)
{
    std::copy(in_arr, in_arr + size * size,
              movements);
}

TILE& Tetravex::get_tile(int i, int j)
{
    assert(i * size + j < size * size);
    return board[movements[i * size + j]];
}

TILE& Tetravex::get_tile(int pos)
{
    assert(pos < size * size);
    return board[movements[pos]];
}

void Tetravex::set_tile(int i, int j, TILE& tile)
{
    assert(i * size + j < size * size);
    board[i * size + j] = tile;
}

void Tetravex::set_tile(int pos, TILE& tile)
{
    assert(pos < size * size);
    board[pos] = tile;
}

// MOVES

bool Tetravex::swap_tiles(int pos1, int pos2)
{
    assert(pos1 < size * size && pos2 < size * size);

    if (pos1 == pos2 ||
        board[movements[pos1]].fixed ||
        board[movements[pos2]].fixed)
        return false;

    std::swap(movements[pos1], movements[pos2]);

    return true;
}

void Tetravex::generate_random_board(int new_size)
{
    size = new_size;
    unsigned char random_row[size * (size + 1)];
    unsigned char random_col[size * (size + 1)];

    for (int i=0; i < size * (size + 1); i++)
    {
        random_row[i] = rand() % 10;
        random_col[i] = rand() % 10;
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            TILE tile = {n:random_row[i * size + j],
                         w:random_col[i * size + j],
                         e:random_col[i * size + (j+1)],
                         s:random_row[(i+1) * size + j]};
            set_tile(i, j, tile);
        }
    }
    for (int i = 0; i < size * size; i++)
        movements[i] = i;
}

void Tetravex::randomize_board(int nb_random_swaps)
{
    if (!nb_random_swaps)
        nb_random_swaps = size * size * 5 + rand()%1;
    int pos1 = 0;
    int pos2 = 0;

    for (int i = 0; i < nb_random_swaps; i++)
    {
        pos1 = rand() % (size * size);
        pos2 = rand() % (size * size);

        if (!swap_tiles(pos1, pos2))
            i--;
    }
}

// UTILS

bool Tetravex::save_board(const char* file_path)
{
    std::ofstream outfile (file_path);
    if (!outfile.is_open())
    {
        std::cerr << "Couldn't open output file: " << file_path << std::endl;
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            TILE& tile = get_tile(i, j);
            outfile << (int)tile.n << (int)tile.e << (int)tile.w << (int)tile.s;
            //if (tile.fixed)
            //    outfile << " @";
            outfile << "\n";
        }
    }
    outfile.close();
    return true;
}

void Tetravex::draw_board()
{
    printf("+");
    for (int j = 0; j < size; j++)
        printf("-----+");
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        printf("|");
        for (int j = 0; j < size; j++)
        {
            TILE& tile = get_tile(i, j);
            printf("  %d  |", tile.n);
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < size; j++)
        {
            TILE& tile = get_tile(i, j);
            printf(" %d %d |", tile.w, tile.e);
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < size; j++)
        {
            TILE& tile = get_tile(i, j);
            printf("  %d  |", tile.s);
        }
        printf("\n");

        printf("+");
        for (int j = 0; j < size; j++)
            printf("-----+");
        printf("\n");
    }
}

bool Tetravex::load_file(const char* file_path)
{
    std::ifstream file;
    file.open(file_path);
    std::string line;
    if (file.is_open())
    {
        std::cout << "Loading: " << file_path << std::endl;
        int pos = 0;
        while (std::getline(file, line))
        {
            if (line.length() == 0)
                continue;
            try
            {

                TILE tile = {(unsigned char)std::stoul(line.substr(0,1)),
                             (unsigned char)std::stoul(line.substr(1,1)),
                             (unsigned char)std::stoul(line.substr(2,1)),
                             (unsigned char)std::stoul(line.substr(3,1)),
                             line.find("@") != std::string::npos};
                set_tile(pos, tile);
            }
            catch(...)
            {
                std::cerr << "Couldn't load file: " << file_path << std::endl;
                return false;
            }
            pos++;
        }

        double sr = sqrt(pos);
        if (sr - floor(sr) != 0) {
            std::cerr << "The board doesn't have size * size lines.. " << std::endl;
            return false;
        }
        size = (int)sr;

        for (int i = 0; i < size * size; i++)
            movements[i] = i;

        return true;
    }

    std::cerr << "Couldn't open file: " << file_path << std::endl;
    return false;
}

// RULES

bool allowed_horizontal(TILE& t1, TILE& t2)
{
    return t1.e == t2.w;
}

bool allowed_vertical(TILE& t1, TILE& t2)
{
    return t1.s == t2.n;
}

bool Tetravex::check_board()
{
    // Check movements uniqueness
    std::set<int> movements_values;
    for (int i=0; i < size * size; i++)
    {
        movements_values.insert(movements[i]);
    }
    if (movements_values.size() != size * size)
        return false;

    // Check adjacent tiles values
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            TILE& t1 = get_tile(i, j);
            // Check horizontal neighbor tile
            if (j < size - 1)
            {
                TILE& t2 = get_tile(i, j + 1);
                if (!allowed_horizontal(t1, t2))
                    return false;
            }

            // Check vertical neighbor tile
            if (i < size - 1)
            {
                TILE& t2 = get_tile(i + 1, j);
                if (!allowed_vertical(t1, t2))
                    return false;
            }
        }
    }
    return true;
}

int Tetravex::get_score()
{
    int score = 0;

    // Check adjacent tiles values
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            TILE& t1 = get_tile(i, j);
            // Check horizontal neighbor tile
            if (j < size - 1)
            {
                TILE& t2 = get_tile(i, j + 1);
                if (!allowed_horizontal(t1, t2))
                    score++;
            }

            // Check vertical neighbor tile
            if (i < size - 1)
            {
                TILE& t2 = get_tile(i + 1, j);
                if (!allowed_vertical(t1, t2))
                    score++;
            }
        }
    }
    return score;
}

// SOLVER

float Tetravex::find_initial_t()
{
    int nb_samples = 30;
    int scores[nb_samples];
    int total = 0;
    for (int i=0; i<nb_samples; i++)
    {
        randomize_board();
        int score = get_score();
        scores[i] = score;
        total += scores[i];
    }

    float pstdev = 0.0;
    float mean = total / nb_samples;
    for (int i=0; i<nb_samples; i++)
    {
        pstdev += std::pow(scores[i] - mean, 2);
    }

    return std::pow(pstdev / nb_samples, 0.5);
}

void Tetravex::solve()
{
    int initial_state[MAX_SIZE * MAX_SIZE];
    copy_movements_to(initial_state);

    float initial_t = find_initial_t();
    float T = initial_t;
    float decrease_factor = (size<=3) ? 0.999:
                            (size<=4) ? 0.9999:
                            (size<=5) ? 0.99999:
                                        0.999;

    bool use_cooling = (size <= 5) ? false: true;
    int stuck = 0;

    load_movements_from(initial_state);
    int score = get_score();
    int reset_nb = 0;
    int iteration = 0;

    std::ofstream log_file("log_s" + std::to_string(size) + ".txt");

    while (score > 0)
    {
        if (VERBOSE && iteration % 1000 == 0)
        {
            std::cout << "iteration:" << iteration <<
                         " // T:" << T <<
                         " // score:" << score << std::endl;
        }

        // Select random tiles to swap
        int pos1 = rand() % (size * size);
        int pos2 = rand() % (size * size);

        // Do swap or loop if swap is not possible
        if (!swap_tiles(pos1, pos2))
            continue;

        int new_score = get_score();

        // end if we found a solution
        if (new_score == 0)
            break;

        // check if we should keep this move or not
        if (new_score < score ||
            (exp(-(new_score - score) / T)) * RAND_MAX >= rand())
        {
            score = new_score;
            stuck = 0;
        }
        else
        {
            swap_tiles(pos1, pos2);
            stuck += 1;
        }

        if (LOG_STAT)
            log_file << std::to_string(T) << ";" << std::to_string(score) << std::endl;


        // Cooling method :
        if (use_cooling)
        {
            // Increase the heat
            if (stuck == 10)
            {
                T /= 0.99;
                stuck = 0;
            }
            // Decrease the heat
            else
                T *= decrease_factor;
        }

        // Reset Heat method :
        else
        {
            // Decrease the heat
            T *= decrease_factor;

            // Reset the heat if we are at a local minimum
            if (T < 0.25)
            {
                T = initial_t;
                reset_nb++;
                // TODO Test other methods
                // T_init = 2
                // T:2 -> T:0.3 -> T:2 (restart with completly random)
                // T:2 -> T:0.3 -> T:0.8 (restart with sightly random board)
                // T:2 -> T:0.3 -> T:0.5 -> T:0.3 -> T:0.8 -> T:0.3 -> T:1 ...
            }
        }


        iteration++;
    }
    if (VERBOSE)
    {
        std::cout << "nb iterations: " << iteration << std::endl;
        std::cout << "reset_nb : " << reset_nb << std::endl;
    }
    log_file.close();

    return;
}