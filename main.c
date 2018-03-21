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

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            scanf("%hu %hu %hu", &img.pixel[pixel_y][pixel_x][0],
                                 &img.pixel[pixel_y][pixel_x][1],
                                 &img.pixel[pixel_y][pixel_x][2]);
        }
    }
    return img;
}

int get_media(Image img, int pixel_y, int pixel_x, int media) {
  media = img.pixel[pixel_y][pixel_x][0] +
          img.pixel[pixel_y][pixel_x][1] +
          img.pixel[pixel_y][pixel_x][2];

  media = media / 3;
  return media;
}

Image change_grey_scale(Image img) {

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            int media = 0;

            media = get_media(img, pixel_y, pixel_x, media);

            img.pixel[pixel_y][pixel_x][0] = media;
            img.pixel[pixel_y][pixel_x][1] = media;
            img.pixel[pixel_y][pixel_x][2] = media;
        }
    }
    return img;
}

Image invert_colors(Image img) {

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            img.pixel[pixel_y][pixel_x][0] = 255 - img.pixel[pixel_y][pixel_x][0];
            img.pixel[pixel_y][pixel_x][1] = 255 - img.pixel[pixel_y][pixel_x][1];
            img.pixel[pixel_y][pixel_x][2] = 255 - img.pixel[pixel_y][pixel_x][2];
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

    for (int pixel_y = 0; pixel_y < info.height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < info.width; ++pixel_x) {
            cut.pixel[pixel_y][pixel_x][0] = img.pixel[pixel_y +
                info.starting_point_y][pixel_x + info.starting_point_x][0];
            cut.pixel[pixel_y][pixel_x][1] = img.pixel[pixel_y +
                info.starting_point_y][pixel_x + info.starting_point_x][1];
            cut.pixel[pixel_y][pixel_x][2] = img.pixel[pixel_y +
                info.starting_point_y][pixel_x + info.starting_point_x][2];
        }
    }
    return cut;
}

int check_pixel_size_max(int int_pixel) {
    int pixel_size = 0;

    if (255 > int_pixel) {
      pixel_size = int_pixel;
    } else {
      pixel_size = 255;
    }
    return pixel_size;
}

Image change_sepia_filter(Image img) {

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            unsigned short int pixel[3];
            int int_pixel = 0;
            
            pixel[0] = img.pixel[pixel_y][pixel_x][0];
            pixel[1] = img.pixel[pixel_y][pixel_x][1];
            pixel[2] = img.pixel[pixel_y][pixel_x][2];

            int_pixel =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            img.pixel[pixel_y][pixel_x][0] = check_pixel_size_max(int_pixel);

            int_pixel =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            img.pixel[pixel_y][pixel_x][1] = check_pixel_size_max(int_pixel);

            int_pixel =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            img.pixel[pixel_y][pixel_x][2] = check_pixel_size_max(int_pixel);
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

int get_size_blur() {
    int size = 0;

    scanf("%d", &size);
    return size;
}

Image blur(Image img) {
    int size = 0;

    size = get_size_blur();

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            Pixel media = {0, 0, 0};
            int size_height = 0;
            int size_width = 0;
            int check_height = 0;
            int check_width = 0;

            size_height = check_size_image(img.height, size, pixel_y);
            size_width = check_size_image(img.width, size, pixel_x);

            check_height = check_positive_number(size,pixel_y);
            check_width = check_positive_number(size,pixel_x);

            for (int pixel_y = check_height; pixel_y <= size_height; ++pixel_y) {
                for (int pixel_x = check_width; pixel_x <= size_width; ++pixel_x) {
                    media.red += img.pixel[pixel_y][pixel_x][0];
                    media.green += img.pixel[pixel_y][pixel_x][1];
                    media.blue += img.pixel[pixel_y][pixel_x][2];
                }
            }

            img.pixel[pixel_y][pixel_x][0] = get_color_pixel_per_area(media.red, size);
            img.pixel[pixel_y][pixel_x][1] = get_color_pixel_per_area(media.green, size);
            img.pixel[pixel_y][pixel_x][2] = get_color_pixel_per_area(media.blue, size);
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

Image rotate_photo(Image img) {

    int times = 0;
    scanf("%d", &times);
    times = times % 4;
    for (int rotate = 0; rotate < times; ++rotate) {
        img = rotate_90_right(img);
    }
    return img;
}

Image function_mirror(Image img, int horizontal, int width, int height) {
    for (int pixel_y = 0; pixel_y < height; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < width; ++pixel_x) {
            int var_height = 0;
            int var_width = 0;

            var_height = pixel_y;
            var_width = pixel_x;

            if (horizontal == 1) {
              var_width = img.width - 1 - pixel_x;
            } else {
              var_height = img.height - 1 - pixel_y;
            }

            Pixel pixel;
            pixel.red = img.pixel[pixel_y][pixel_x][0];
            pixel.green = img.pixel[pixel_y][pixel_x][1];
            pixel.blue = img.pixel[pixel_y][pixel_x][2];

            img.pixel[pixel_y][pixel_x][0] =
                img.pixel[var_height][var_width][0];
            img.pixel[pixel_y][pixel_x][1] =
                img.pixel[var_height][var_width][1];
            img.pixel[pixel_y][pixel_x][2] =
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
    int times_mirror = 0;

    scanf("%d", &horizontal);

    if (horizontal == 1) {
      times_mirror = width = width / 2;
    } else {
      times_mirror = height = height / 2;
    }

    width = img.width;
    height = img.height;

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
                img = blur(img);
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
