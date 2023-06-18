#ifndef WARP_HPP
#define WARP_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <algorithm>

#define MAX_INTENSITY 255
#define MIN_INTENSITY 0

typedef unsigned char byte;

class Image {
    public:
    int rows;
    int cols;
    int colors;
    std::vector<std::vector<std::vector<byte>>> image;

    Image(int rhs_rows, int rhs_cols, int rhs_colors);

    Image(std::string &filename, int rhs_rows, int rhs_cols, int rhs_colors);

    Image(Image &rhs);

    void write_image(std::string &filename);

    private:
    void read_image(std::string &filename);
    
};

byte float_to_byte(float intensity);

std::string get_image_filename(const std::string &full_path);

std::pair<float, float> pixel_to_cartesian(const std::pair<float, float> &point, int J);
std::pair<float, float> cartesian_to_pixel(const std::pair<float, float> &point, int J);

std::pair<float, float> to_standard_range(const std::pair<float, float> &point, int max_x ,int max_y);
std::pair<float, float> from_standard_range(const std::pair<float, float> &point, int max_x ,int max_y);


std::pair<float, float> forward_transform(const std::pair<float, float> &point);
std::pair<float, float> inverse_transform(const std::pair<float, float> &point);

std::vector<int> inverse_mapping(const Image &source_image, const std::pair<float, float> &source_point);

#endif