// example PPM file format generator

#include <iostream>

int main() {

        // res setup

        const int image_width = 256;
        const int image_height = 256;

        // render

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int i = image_height - 1; i>=0; i--) {
                for (int j = 0; j < image_width; j++) {
                        
                        // value normalization between 0.0 ~ 1.0

                        auto r = double(i) / (image_width - 1);      // red top to bottom
                        auto g = double(j) / (image_height - 1);     // green left to right
                        auto b = 0.25;

                        // convert to (0,255) range and cast to int

                        int ir = static_cast<int>(255.999 * r);
                        int ig = static_cast<int>(255.999 * g);
                        int ib = static_cast<int>(255.999 * b);

                        std::cout << ir << ' ' << ig << ' ' << ib << '\n';
                }
        }

        return 0;
}

