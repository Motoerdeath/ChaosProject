class Task {
    public:
    void virtual execute() = 0;

    private:
    void virtual setup() = 0;
    void virtual run() = 0;
    void virtual cleanup() = 0;
};