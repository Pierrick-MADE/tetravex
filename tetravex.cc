#include "tetravex.hh"

TILE Tetravex::get_tile(int i, int j)
{
    assert(i * size + j < size * size);
    return board[i * size + j];
}

TILE Tetravex::get_tile(int pos)
{
    assert(pos < size * size);
    return board[pos];
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
        for (int j = 0; j < size; j++) {
            TILE tile = get_tile(i, j);
            printf("  %d  |", tile.n);
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < size; j++) {
            TILE tile = get_tile(i, j);
            printf(" %d %d |", tile.w, tile.e);
        }
        printf("\n");

        printf("|");
        for (int j = 0; j < size; j++) {
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

int Tetravex::load_file(const char* file_path)
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
            try {

                TILE tile = {(unsigned char)std::stoul(line.substr(0,1)),
                             (unsigned char)std::stoul(line.substr(1,1)),
                             (unsigned char)std::stoul(line.substr(2,1)),
                             (unsigned char)std::stoul(line.substr(3,1)),
                             line.find("@") != std::string::npos};
                set_tile(pos, tile);
            }
            catch(...){
                std::cerr << "Couldn't load file: " << file_path << std::endl;
                return 1;
            }
            pos++;
        }
        double sr = sqrt(pos);
        if (sr - floor(sr) != 0) {
            std::cerr << "The board doesn't have size * size lines.. " << std::endl;
            return 1;
        }
        size = (int)sr;
        return 0;
    }

    std::cerr << "Couldn't open file: " << file_path << std::endl;
    return 1;
}

bool allowed_horizontal(TILE t1, TILE t2) {
    return t1.e == t2.w;
}

bool allowed_vertical(TILE t1, TILE t2) {
    return t1.s == t2.n;
}

bool Tetravex::check_board() {
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            TILE t1 = get_tile(i, j);
            if (j < size - 1) {
                TILE t2 = get_tile(i, j + 1);
                if (!allowed_horizontal(t1, t2))
                    return false;
            }

            if (i < size - 1) {
                TILE t2 = get_tile(i + 1, j);
                if (!allowed_vertical(t1, t2))
                    return false;
            }
        }
    }
    return true;
}