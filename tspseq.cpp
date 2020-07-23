
class TSPPopulation {
    int** individuals;
public:
    TSPPopulation(int size) {
        individuals = new int*[size];
    }
    void init_population();
    int* fitness();
};

int main(int argc, char* argv[]) {

    if(argc != 5) {
        fprintf(stderr, "Usage: pop_size cross_prob mut_prob seed\n");
        return -1;
    }

    int pop_size = atoi(argv[1]);
    int cross_prob = atoi(argv[2]);
    int mut_prob = atoi(argv[3]);
    int seed = atoi(argv[4]);

    std::srand(seed);

    // Create initial population of tsp paths
    TSPPopulation population {pop_size};
    population.init_population();

    while(running) {
        // calculate fitness of each individual
        //
    }

    print_board(board, N, M);
    std::cout << "-----------------------" << std::endl;

    int alive;
    int** tmp; // Swap pointer
    float avg_it_time = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for(int it=0; it<iter; it++) {
        auto start_it = std::chrono::high_resolution_clock::now();
        // Iterate through each cell
        for(int i=1; i<N-1; i++) {
            for(int j=1; j<M-1; j++) {
                // Count alive neighbor cells
                alive = 0;
                for(int di=-1; di<=1; di++) {
                    for(int dj=-1; dj<=1; dj++) {
                        alive += board[i+di][j+dj];
                    }
                }
                alive -= board[i][j];

                // Update next board
                if(board[i][j]==ALIVE) {
                    if(alive<2 || alive>3) next_board[i][j] = DEAD;
                    else next_board[i][j] = ALIVE;
                } else {
                    if(alive==3) next_board[i][j] = ALIVE;
                    else next_board[i][j] = DEAD;
                }
            }
        }

        // Swap boards
        tmp = board;
        board = next_board;
        next_board = tmp;

        auto end_it = std::chrono::high_resolution_clock::now();
        avg_it_time += std::chrono::duration_cast<std::chrono::milliseconds>(end_it - start_it).count();

        // // Print updated board
        print_board(board, N, M);
        std::cout << "-----------------------" << std::endl;

        // Sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto tot_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    avg_it_time = avg_it_time / iter;
    std::cout << "Total time: " << tot_time << " ms" << std::endl;
    std::cout << "Average time per iteration: " << avg_it_time << " ms" << std::endl;

    // Free memory
    for(int i=0; i<N; i++) {
        delete [] board[i];
        delete [] next_board[i];
    }

    delete [] board;
    delete [] next_board;

    return 0;
}
