// the engine
//int 
mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

//unsigned long long
mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

//(between 0 and 2^32 - 1)
rng() 

// Generates a random integer between 1 and 6
uniform_int_distribution<int> rng_rand(1, 6);
rng_rand(rng);

/// Generates a random double between 0.0 and 1.0
uniform_real_distribution<double> real_rand(0.0, 1.0);
real_rand(rng);