#include "./warp.hh"

Image::Image(int rhs_rows, int rhs_cols, int rhs_colors) {
    rows = rhs_rows;
    cols = rhs_cols;
    colors = rhs_colors;

    image.resize(rows, std::vector<std::vector<byte>>(cols, std::vector<byte>(colors, MIN_INTENSITY)));
}

Image::Image(std::string &filename, int rhs_rows, int rhs_cols, int rhs_colors) {
    rows = rhs_rows;
    cols = rhs_cols;
    colors = rhs_colors;

    image.resize(rows, std::vector<std::vector<byte>>(cols, std::vector<byte>(colors, MIN_INTENSITY)));
    read_image(filename);
    return;
}

Image::Image(Image &rhs) {
    rows = rhs.rows;
    cols = rhs.cols;
    colors = rhs.colors;

    image.resize(rows, std::vector<std::vector<byte>>(cols, std::vector<byte>(colors, MIN_INTENSITY)));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < colors; k++) {
                image[i][j][k] = rhs.image[i][j][k];
            }
        }
    }
}

void Image::read_image(std::string &filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file) {
        std::cerr << "File: " + filename + " could not be opened" << std::endl;
        exit(1); 
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < colors; k++) {
                file.read((char *) &image[i][j][k], 1);
            }
        }
    }

    file.close();
    return; 
}

void Image::write_image(std::string &filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file) {
        std::cerr << "File: " + filename + " could not be opened" << std::endl;
        exit(1); 
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < colors; k++) {
                file.write((char *) &image[i][j][k], 1);
            }
        }
    }

    file.close();
    return; 
}

byte float_to_byte(float intensity) {
    if (intensity >= MAX_INTENSITY) {
        return MAX_INTENSITY;
    }
    else if (intensity <= MIN_INTENSITY) {
        return MIN_INTENSITY;
    }
    else {
        return round(intensity); 
    }
}

std::pair<float, float> pixel_to_cartesian(const std::pair<float, float> &point, int J) {
    float j = point.first;
    float k = point.second;
    
    float x =  k - 0.5;
    float y = J + 0.5 - j;
    std::pair<float, float> result{x, y};

    return result;
}

std::pair<float, float> cartesian_to_pixel(const std::pair<float, float> &point, int J) {
    float x = point.first;
    float y = point.second;
    
    float j = J + 0.5 - y;
    float k =  x + 0.5;
    std::pair<float, float> result{j, k};

    return result;
}

std::pair<float, float> to_standard_range(const std::pair<float, float> &point, int max_x ,int max_y) {
    float x = point.first;
    float y = point.second;

    float new_x = (x - max_x / 2) / (max_x / 2);
    float new_y = (y - max_y / 2) / (max_y / 2);
    std::pair<float, float> result{new_x, new_y};

    return result;
}

std::pair<float, float> fromStandardRange(const std::pair<float, float> &point, int max_x ,int max_y) {
    float x = point.first;
    float y = point.second;

    float new_x = (x * max_x / 2) + (max_x / 2);
    float new_y = (y * max_y / 2) + (max_y / 2);
    std::pair<float, float> result{new_x, new_y};

    return result;
}

std::pair<float, float> forward_transform(const std::pair<float, float> &point) {
    float x = point.first;
    float y = point.second;

    float u = x * sqrt(1 - pow(y, 2) / 2);
    float v = y * sqrt(1 - pow(x, 2) / 2);
    std::pair<float, float> result{u, v};
    return result;
}

std::pair<float, float> inverse_transform(const std::pair<float, float> &point) {
    float u = point.first;
    float v = point.second;

    float a = sqrt(2 + pow(u, 2) - pow(v, 2) + 2 * sqrt(2) * u);
    float b = sqrt(2 + pow(u, 2) - pow(v, 2) - 2 * sqrt(2) * u);
    float c = sqrt(2 - pow(u, 2) + pow(v, 2) + 2 * sqrt(2) * v);
    float d = sqrt(2 - pow(u, 2) + pow(v, 2) - 2 * sqrt(2) * v);;

    float x = 0.5 * (a - b);
    float y = 0.5 * (c - d);
    std::pair<float, float> result{x, y};
    return result;
}

std::vector<int> inverse_mapping(const Image &source_image, const std::pair<float, float> &source_point) {
    int rows = source_image.rows;
    int cols = source_image.cols;
    int colors = source_image.colors;

    std::vector<int> intensities(colors, MIN_INTENSITY);

    float p = source_point.first;
    float q = source_point.second;

    int m = (int) floor(p);
    int n = (int) floor(q);

    float delta_x = p - m;
    float delta_y = q - n;

    if (((m >= 0) && ((m + 1) < rows)) && ((n >= 0) && ((n + 1) < cols))) {
        for (int c = 0; c < colors; c++) {
            intensities[c] = float_to_byte((delta_x * delta_y * source_image.image[m + 1][n + 1][c]) + \
                                (delta_x * (1 - delta_y) * source_image.image[m][n + 1][c]) + \
                                ((1 - delta_x) * delta_y * source_image.image[m + 1][n][c]) + \
                                ((1 - delta_x) * (1 - delta_y) * source_image.image[m][n][c]));
        }
        return intensities;
    }
    else {
        return intensities;
    }
}