#include "tetravex.hh"

double rand_zero_to_one()
{
    return rand() / (RAND_MAX + 1.);
}

float Tetravex::find_initial_t()
{
    int nb_samples = 10;
    int scores[nb_samples];
    int total = 0;
    for (int i=0; i<nb_samples; i++)
    {
        randomize_board();
        int score = get_score();
        scores[i] = score;
        total += scores[i];
    }
    std::cout << std::endl;

    float pstdev = 0.0;
    float mean = total / nb_samples;
    for (int i=0; i<nb_samples; i++)
    {
        pstdev += std::pow(scores[i] - mean, 2);
    }

    return std::pow(pstdev / nb_samples, 0.5);
}


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


TILE Tetravex::get_tile(int i, int j)
{
    assert(i * size + j < size * size);
    return board[movements[i * size + j]];
}

TILE Tetravex::get_tile(int pos)
{
    assert(pos < size * size);
    return board[movements[pos]];
}

void Tetravex::set_tile(int i, int j, TILE tile)
{
    assert(i * size + j < size * size);
    board[i * size + j] = tile;
}

void Tetravex::set_tile(int pos, TILE tile)
{
    assert(pos < size * size);
    board[pos] = tile;
}

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

void Tetravex::draw_simple_board()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            TILE tile = get_tile(i, j);
            printf("%d%d%d%d", tile.n, tile.e, tile.w, tile.s);
            if (tile.fixed)
                printf(" @");
            printf("\n");
        }
    }
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
            TILE tile = get_tile(i, j);
            printf("  %d  |", tile.n);
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < size; j++)
        {
            TILE tile = get_tile(i, j);
            printf(" %d %d |", tile.w, tile.e);
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < size; j++)
        {
            TILE tile = get_tile(i, j);
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

bool allowed_horizontal(TILE t1, TILE t2)
{
    return t1.e == t2.w;
}

bool allowed_vertical(TILE t1, TILE t2)
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
            TILE t1 = get_tile(i, j);
            // Check horizontal neighbor tile
            if (j < size - 1)
            {
                TILE t2 = get_tile(i, j + 1);
                if (!allowed_horizontal(t1, t2))
                    return false;
            }

            // Check vertical neighbor tile
            if (i < size - 1)
            {
                TILE t2 = get_tile(i + 1, j);
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
            TILE t1 = get_tile(i, j);
            // Check horizontal neighbor tile
            if (j < size - 1)
            {
                TILE t2 = get_tile(i, j + 1);
                if (!allowed_horizontal(t1, t2))
                    score++;
            }

            // Check vertical neighbor tile
            if (i < size - 1)
            {
                TILE t2 = get_tile(i + 1, j);
                if (!allowed_vertical(t1, t2))
                    score++;
            }
        }
    }
    return score;
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

void Tetravex::solve()
{
    float T = find_initial_t();
    float decrease_factor = 0.99999;
    int nb_stuck = 0;
    int iteration = 0;

    int score = get_score();

    while (score > 0)
    {
        if (iteration % 1000 == 0)
        {
            std::cout << "iteration:" << iteration <<
                         " // T:" << T <<
                         " // score:" << score << std::endl;
        }

        int nb_moves = size*size;
        int diff_scores_after_moves = 0;

        for (int i=0; i < nb_moves; i++)
        {
            int pos1 = rand() % (size * size);
            int pos2 = rand() % (size * size);

            if (!swap_tiles(pos1, pos2))
            {
                i--;
                continue;
            }

            int new_score = get_score();

            if (new_score == 0)
                return;

            if (exp((score - new_score) / T ) > rand_zero_to_one())
            {
                diff_scores_after_moves += 1;
                score = new_score;
            }
            else
                swap_tiles(pos1, pos2);
        }

        T *= decrease_factor;

        if (diff_scores_after_moves == 0)
        {
            nb_stuck += 1;
            if (T < 0.01)
            {
                T = 5;
                nb_stuck = 0;
            }
        }
        iteration++;
    }
    return;
}