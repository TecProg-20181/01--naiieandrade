#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

typedef struct _info_cut_image {
    int starting_point_x;
    int starting_point_y;
    int width;
    int height;
} InfoCutImage;

Image read_pixels_image(Image img) {

    for (unsigned int aux_height = 0; aux_height < img.height; ++aux_height) {
        for (unsigned int aux_width = 0; aux_width < img.width; ++aux_width) {
            scanf("%hu %hu %hu", &img.pixel[aux_height][aux_width][0],
                                 &img.pixel[aux_height][aux_width][1],
                                 &img.pixel[aux_height][aux_width][2]);
        }
    }
    return img;
}

Image change_grey_scale(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            media /= 3;
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }
    return img;
}

Image rotate_90_right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][0] = img.pixel[x][y][0];
            rotated.pixel[i][j][1] = img.pixel[x][y][1];
            rotated.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }
    return rotated;
}

Image invert_colors(Image img) {

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j][0] = 255 - img.pixel[i][j][0];
            img.pixel[i][j][1] = 255 - img.pixel[i][j][1];
            img.pixel[i][j][2] = 255 - img.pixel[i][j][2];
        }
    }
    return img;

}

InfoCutImage get_info_to_cut_image(){
    InfoCutImage img;

    scanf("%d %d", &img.starting_point_x, &img.starting_point_y);
    scanf("%d %d", &img.width, &img.height);

    return img;
}

Image cut_image(Image img) {
    Image cut;
    InfoCutImage info;
    info = get_info_to_cut_image();

    cut.width = info.width;
    cut.height = info.height;

    for (int aux_height = 0; aux_height < info.height; ++aux_height) {
        for (int aux_width = 0; aux_width < info.width; ++aux_width) {
            cut.pixel[aux_height][aux_width][0] = img.pixel[aux_height +
                info.starting_point_y][aux_width + info.starting_point_x][0];
            cut.pixel[aux_height][aux_width][1] = img.pixel[aux_height +
                info.starting_point_y][aux_width + info.starting_point_x][1];
            cut.pixel[aux_height][aux_width][2] = img.pixel[aux_height +
                info.starting_point_y][aux_width + info.starting_point_x][2];
        }
    }
    return cut;
}

int get_pixel_size(int int_pixel) {
    int pixel_size = 0;

    if (255 > int_pixel) {
      pixel_size = int_pixel;
    } else {
      pixel_size = 255;
    }
    return pixel_size;
}

Image change_sepia_filter(Image img) {

    for (unsigned int height = 0; height < img.height; ++height) {
        for (unsigned int width = 0; width < img.width; ++width) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[height][width][0];
            pixel[1] = img.pixel[height][width][1];
            pixel[2] = img.pixel[height][width][2];

            int int_pixel =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            img.pixel[height][width][0] = get_pixel_size(int_pixel);

            int_pixel =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            img.pixel[height][width][1] = get_pixel_size(int_pixel);

            int_pixel =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            img.pixel[height][width][2] = get_pixel_size(int_pixel);
        }
    }
    return img;
}

int check_size_image(int img, int size, int var) {
    int size_img;

    if (img - 1 > var + size/2) {
        size_img = var + size/2;
    } else {
        size_img =  img - 1;
    }
    return size_img;
}

int check_positive_number(int size, int var) {
    int number = 0;

    if (0 > var-size/2) {
        number = 0;
    } else {
        number = var - size/2;
    }
    return number;
}

int get_color_pixel_per_area(unsigned short int color_pixel, int size) {
    int area = 0;
    area = size * size;
    return (color_pixel = color_pixel / area);
}

Image blur(Image img, int size) {

    for (unsigned int height = 0; height < img.height; ++height) {
        for (unsigned int width = 0; width < img.width; ++width) {
            Pixel media = {0, 0, 0};
            int size_height = 0;
            int size_width = 0;
            int check_height = 0;
            int check_width = 0;

            size_height = check_size_image(img.height, size, height);
            size_width = check_size_image(img.width, size, width);

            check_height = check_positive_number(size,height);
            check_width = check_positive_number(size,width);

            for (int aux_height = check_height; aux_height <= size_height; ++aux_height) {
                for (int aux_width = check_width; aux_width <= size_width; ++aux_width) {
                    media.red += img.pixel[aux_height][aux_width][0];
                    media.green += img.pixel[aux_height][aux_width][1];
                    media.blue += img.pixel[aux_height][aux_width][2];
                }
            }

            img.pixel[height][width][0] = get_color_pixel_per_area(media.red, size);
            img.pixel[height][width][1] = get_color_pixel_per_area(media.green, size);
            img.pixel[height][width][2] = get_color_pixel_per_area(media.blue, size);
        }
    }
    return img;
}

Image change_blur(Image img) {
    int size = 0;

    scanf("%d", &size);
    img = blur(img, size);
    return img;
}

Image rotate_photo(Image img) {

    int times = 0;
    scanf("%d", &times);
    times = times % 4;
    for (int rotate = 0; rotate < times; ++rotate) {
        img = rotate_90_right(img);
    }
    return img;
}

int choose_kind_of_mirror(int horizontal, int width, int height) {
    int times_mirror = 0;

    if (horizontal == 1) {
      times_mirror = width = width / 2;
    } else {
      times_mirror = height = height / 2;
    }
    return times_mirror;
}

Image function_mirror(Image img, int horizontal, int width, int height) {
    for (int aux_height = 0; aux_height < height; ++aux_height) {
        for (int aux_width = 0; aux_width < width; ++aux_width) {
            int var_height = 0;
            int var_width = 0;

            var_height = aux_height;
            var_width = aux_width;

            if (horizontal == 1) {
              var_width = img.width - 1 - aux_width;
            } else {
              var_height = img.height - 1 - aux_height;
            }

            Pixel pixel;
            pixel.red = img.pixel[aux_height][aux_width][0];
            pixel.green = img.pixel[aux_height][aux_width][1];
            pixel.blue = img.pixel[aux_height][aux_width][2];

            img.pixel[aux_height][aux_width][0] =
                img.pixel[var_height][var_width][0];
            img.pixel[aux_height][aux_width][1] =
                img.pixel[var_height][var_width][1];
            img.pixel[aux_height][aux_width][2] =
                img.pixel[var_height][var_width][2];

            img.pixel[var_height][var_width][0] = pixel.red;
            img.pixel[var_height][var_width][1] = pixel.green;
            img.pixel[var_height][var_width][2] = pixel.blue;
        }
    }
    return img;
}

Image mirror(Image img) {
    int horizontal = 0;
    int width = 0;
    int height = 0;

    scanf("%d", &horizontal);

    width = img.width;
    height = img.height;

    choose_kind_of_mirror(horizontal, width, height);
    img = function_mirror(img, horizontal, width, height);

    return img;
}

void print(Image img){

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int height = 0; height < img.height; ++height) {
        for (unsigned int width = 0; width < img.width; ++width) {
            printf("%hu %hu %hu ", img.pixel[height][width][0],
                                   img.pixel[height][width][1],
                                   img.pixel[height][width][2]);

        }
        printf("\n");
    }
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    img = read_pixels_image(img);

    int number_options;
    scanf("%d", &number_options);

    for(int number_option = 0; number_option < number_options; ++number_option) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Grey Scale
                img = change_grey_scale(img);
                break;
            }
            case 2: { // Sepia Filter
                img = change_sepia_filter(img);
                break;
            }
            case 3: { // Blur
                img = change_blur(img);
                break;
            }
            case 4: { // Rotate Photo
                img = rotate_photo(img);
                break;
            }
            case 5: { // Mirror Photo
                img = mirror(img);
                break;
            }
            case 6: { // Color Inversion
                img = invert_colors(img);
                break;
            }
            case 7: { // Cut Image
                img = cut_image(img);
                break;
            }
        }
    }

    print(img);

    return 0;
}
