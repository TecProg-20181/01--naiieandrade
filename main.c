#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
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
            scanf("%hu %hu %hu", &img.pixel[pixel_y][pixel_x].red,
                                 &img.pixel[pixel_y][pixel_x].green,
                                 &img.pixel[pixel_y][pixel_x].blue);
        }
    }
    return img;
}

int get_media(Image img, int pixel_y, int pixel_x, int media) {
    media = img.pixel[pixel_y][pixel_x].red +
            img.pixel[pixel_y][pixel_x].green +
            img.pixel[pixel_y][pixel_x].blue;

    media = media/3;

    return media;
}

Image change_grey_scale(Image img) {

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            int media = 0;

            media = get_media(img, pixel_y, pixel_x, media);

            img.pixel[pixel_y][pixel_x].red = media;
            img.pixel[pixel_y][pixel_x].green = media;
            img.pixel[pixel_y][pixel_x].blue = media;
        }
    }
    return img;
}

Image invert_colors(Image img) {

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            img.pixel[pixel_y][pixel_x].red = 255 - img.pixel[pixel_y][pixel_x].red;
            img.pixel[pixel_y][pixel_x].green = 255 - img.pixel[pixel_y][pixel_x].green;
            img.pixel[pixel_y][pixel_x].blue = 255 - img.pixel[pixel_y][pixel_x].blue;
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
            cut.pixel[pixel_y][pixel_x].red = img.pixel[pixel_y +
                info.starting_point_y][pixel_x + info.starting_point_x].red;
            cut.pixel[pixel_y][pixel_x].green = img.pixel[pixel_y +
                info.starting_point_y][pixel_x + info.starting_point_x].green;
            cut.pixel[pixel_y][pixel_x].blue = img.pixel[pixel_y +
                info.starting_point_y][pixel_x + info.starting_point_x].blue;
        }
    }
    return cut;
}

int check_min(int compare, int number_min) {
    int number = 0;

    if (compare > number_min) {
        number = number_min;
    } else {
        number = compare;
    }
    return number;
}

int check_max(int number_max, int compare) {
    int number = 0;

    if (number_max > compare) {
        number = number_max;
    } else {
        number = compare;
    }
    return number;
}

Image change_sepia_filter(Image img) {

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            unsigned short int pixel[3];
            int int_pixel = 0;

            pixel[0] = img.pixel[pixel_y][pixel_x].red;
            pixel[1] = img.pixel[pixel_y][pixel_x].green;
            pixel[2] = img.pixel[pixel_y][pixel_x].blue;

            int_pixel =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            img.pixel[pixel_y][pixel_x].red = check_min(255, int_pixel);

            int_pixel =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            img.pixel[pixel_y][pixel_x].green = check_min(255, int_pixel);

            int_pixel =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            img.pixel[pixel_y][pixel_x].blue = check_min(255, int_pixel);
        }
    }
    return img;
}

int get_number_blur() {
    int number = 0;

    scanf("%d", &number);
    return number;
}

int get_color_pixel_per_area(unsigned short int color_pixel, int size) {
    int area = 0;
    area = size * size;
    color_pixel = color_pixel / area;
    return color_pixel;
}

Image blur(Image img) {
    int number_blur = 0;

    number_blur = get_number_blur();

    for (unsigned int pixel_y = 0; pixel_y < img.height; ++pixel_y) {
        for (unsigned int pixel_x = 0; pixel_x < img.width; ++pixel_x) {
            Pixel media = {0, 0, 0};
            int min_height = 0;
            int min_width = 0;
            int check_height = 0;
            int check_width = 0;

            int min_y = 0;
            min_y = pixel_y + number_blur/2;
            int min_x = 0;
            min_x = pixel_x + number_blur/2;

            min_height = check_min(img.height-1, min_y);
            min_width = check_min(img.width-1, min_x);

            int max_y = 0;
            max_y = pixel_y - number_blur/2;
            int max_x = 0;
            max_x = pixel_x - number_blur/2;

            check_height = check_max(0, max_y);
            check_width = check_max(0, max_x);

            for (int pixel_y = check_height; pixel_y <= min_height; ++pixel_y) {
                for (int pixel_x = check_width; pixel_x <= min_width; ++pixel_x) {
                    media.red += img.pixel[pixel_y][pixel_x].red;
                    media.green += img.pixel[pixel_y][pixel_x].green;
                    media.blue += img.pixel[pixel_y][pixel_x].blue;
                }
            }

            img.pixel[pixel_y][pixel_x].red = get_color_pixel_per_area(media.red, number_blur);
            img.pixel[pixel_y][pixel_x].green = get_color_pixel_per_area(media.green, number_blur);
            img.pixel[pixel_y][pixel_x].blue = get_color_pixel_per_area(media.blue, number_blur);
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
            rotated.pixel[i][j].red = img.pixel[x][y].red;
            rotated.pixel[i][j].green = img.pixel[x][y].green;
            rotated.pixel[i][j].blue = img.pixel[x][y].blue;
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

Image function_mirror(Image img, int horizontal, int inversion_point_x,
                                                          int inversion_point_y) {
    for (int pixel_y = 0; pixel_y < inversion_point_y; ++pixel_y) {
        for (int pixel_x = 0; pixel_x < inversion_point_x; ++pixel_x) {
            int inversion_point_y = 0;
            int inversion_point_x = 0;

            if (horizontal == 1) {
              inversion_point_x = img.width - 1 - pixel_x;
              inversion_point_y = pixel_y;
            } else {
              inversion_point_y = img.height - 1 - pixel_y;
              inversion_point_x = pixel_x;
            }

            Pixel pixel;

            pixel.red = img.pixel[pixel_y][pixel_x].red;
            pixel.green = img.pixel[pixel_y][pixel_x].green;
            pixel.blue = img.pixel[pixel_y][pixel_x].blue;

            img.pixel[pixel_y][pixel_x].red =
                img.pixel[inversion_point_y][inversion_point_x].red;
            img.pixel[pixel_y][pixel_x].green =
                img.pixel[inversion_point_y][inversion_point_x].green;
            img.pixel[pixel_y][pixel_x].blue =
                img.pixel[inversion_point_y][inversion_point_x].blue;

            img.pixel[inversion_point_y][inversion_point_x].red = pixel.red;
            img.pixel[inversion_point_y][inversion_point_x].green = pixel.green;
            img.pixel[inversion_point_y][inversion_point_x].blue = pixel.blue;
        }
    }
    return img;
}

Image mirror(Image img) {
    int horizontal = 0;
    int inversion_point_x = 0;
    int inversion_point_y = 0;

    scanf("%d", &horizontal);

    if (horizontal == 1) {
        inversion_point_x = img.width / 2;
        inversion_point_y = img.height;
    } else {
        inversion_point_y = img.height / 2;
        inversion_point_x = img.width;
    }

    img = function_mirror(img, horizontal, inversion_point_x, inversion_point_y);

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
            printf("%hu %hu %hu ", img.pixel[height][width].red,
                                   img.pixel[height][width].green,
                                   img.pixel[height][width].blue);

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
