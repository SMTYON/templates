struct Player {
    std::string name;
    int score;

    // Must be marked 'const' and take a 'const reference'
    bool operator<(const Player& other) const {
        // Primary sort: lowest score first
        if (score != other.score) {
            return score < other.score; 
        }
        // Tie-breaker: alphabetical by name
        return name < other.name; 
    }
};