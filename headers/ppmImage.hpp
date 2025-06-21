#include <vector>


class PPMImage{
    public:
        std::vector<std::vector<std::vector<int>>> image;

        void render();
        bool storeImageToFile();

    private:
};