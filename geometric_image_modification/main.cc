#include "./warp.hh"

int main(int argc, char** argv) {
    if (argc != 5){
        printf("[Usage]: ./main input_filename rows cols colors\n");
        exit(1);
    }

    std::string input_filename = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    int colors = atoi(argv[4]);
    std::string warped_output_filename = "./output_images/warped_" + get_image_filename(input_filename);

    Image input_image(input_filename, rows, cols, colors);

    int canvas_rows = rows * 1;
    int canvas_cols = cols * 1;
    Image canvas(canvas_rows, canvas_cols, colors);

    for (int i = 0; i < canvas_rows; i++) {
        for (int j = 0; j < canvas_cols; j++) {
            std::pair<float, float> image_point{(float)i, (float) j};
            std::pair<float, float> cartesian_point = pixel_to_cartesian(image_point, canvas_rows);
            std::pair<float, float> standard_cartesian_point = to_standard_range(cartesian_point, canvas_rows, canvas_cols);

            std::pair<float, float> transformed_point = inverse_transform(standard_cartesian_point);
            std::pair<float, float> normal_cartesian_point = from_standard_range(transformed_point, canvas_rows, canvas_cols);
            std::pair<float, float> normal_image_point = cartesian_to_pixel(normal_cartesian_point, canvas_rows);

            std::vector<int> new_colors = inverse_mapping(input_image, normal_image_point);

            for (int k = 0; k < colors; k++) {
                canvas.image[i][j][k] += new_colors[k];
            }
        }
    }
    canvas.write_image(warped_output_filename);

    // std::string reverse_warped_output_filename = "./output_images/reverse_warped_" + get_image_filename(input_filename);
    // Image reverse_canvas(canvas_rows, canvas_cols, colors);
    // for (int i = 0; i < canvas_rows; i++) {
    //     for (int j = 0; j < canvas_cols; j++) {
    //         std::pair<float, float> image_point{(float)i, (float) j};
    //         std::pair<float, float> cartesian_point = pixel_to_cartesian(image_point, canvas_rows);
    //         std::pair<float, float> standard_cartesian_point = to_standard_range(cartesian_point, canvas_rows, canvas_cols);

    //         std::pair<float, float> transformed_point = inverse_transform(standard_cartesian_point);
    //         std::pair<float, float> normal_cartesian_point = from_standard_range(transformed_point, canvas_rows, canvas_cols);
    //         std::pair<float, float> normal_image_point = cartesian_to_pixel(normal_cartesian_point, canvas_rows);

    //         std::vector<int> new_colors = inverse_mapping(input_image, normal_image_point);

    //         for (int k = 0; k < colors; k++) {
    //             canvas.image[i][j][k] += new_colors[k];
    //         }
    //     }
    // }

    // reverse_canvas.write_image(reverse_warped_output_filename);

    return 0;
}