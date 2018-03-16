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


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
}


Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

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

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (height - 1 > i + T/2) ? i + T/2 : height - 1;
            int min_w = (width - 1 > j + T/2) ? j + T/2 : width - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            pixel[i][j][0] = media.red;
            pixel[i][j][1] = media.green;
            pixel[i][j][2] = media.blue;
        }
    }
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void invert_colors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cut_image(Image img, int starting_point_x, int starting_point_y, int width, int height) {
    Image cut;

    cut.width = width;
    cut.height = height;

    for(int aux_height = 0; aux_height < height; ++aux_height) {
        for(int aux_width = 0; aux_width < width; ++aux_width) {
            cut.pixel[aux_height][aux_width][0] = img.pixel[aux_height + starting_point_y][aux_width + starting_point_x][0];
            cut.pixel[aux_height][aux_width][1] = img.pixel[aux_height + starting_point_y][aux_width + starting_point_x][1];
            cut.pixel[aux_height][aux_width][2] = img.pixel[aux_height + starting_point_y][aux_width + starting_point_x][2];
        }
    }

    return cut;
}

int tamanho_do_pixel(int p){
    int menor_r = 0;

    if(255 > p){
      menor_r = p;
    } else {
      menor_r = 255;
    }
    return menor_r;
}

Image change_sepia_filter(Image img) {
    for (unsigned int x = 0; x < img.height; ++x) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j][0];
            pixel[1] = img.pixel[x][j][1];
            pixel[2] = img.pixel[x][j][2];

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            img.pixel[x][j][0] = tamanho_do_pixel(p);

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            img.pixel[x][j][1] = tamanho_do_pixel(p);

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            img.pixel[x][j][2] = tamanho_do_pixel(p);
        }
    }
    return img;
}

Image change_blur(Image img) {
    int size = 0;
    scanf("%d", &size);
    blur(img.height, img.pixel, size, img.width);
    return img;
}

Image rotate_photo(Image img) {

    int times = 0;
    scanf("%d", &times);
    times = times % 4;
    for (int rotate = 0; rotate < times; ++rotate) {
        img = rotacionar90direita(img);
    }
    return img;
}

int choose_kind_of_mirror(int horizontal, int width, int height){

    if (horizontal == 1) {
      return width = width / 2;
    } else {
      return height = height / 2;
    }
}

Image function_mirror(Image img, int horizontal, int width, int height) {
    for (int aux_height = 0; aux_height < height; ++aux_height) {
        for (int aux_width = 0; aux_width < width; ++aux_width) {
            int x = aux_height;
            int y = aux_width;

            if (horizontal == 1) {
              y = img.width - 1 - aux_width;
            } else {
              x = img.height - 1 - aux_height;
            }

            Pixel pixel;
            pixel.red = img.pixel[aux_height][aux_width][0];
            pixel.green = img.pixel[aux_height][aux_width][1];
            pixel.blue = img.pixel[aux_height][aux_width][2];

            img.pixel[aux_height][aux_width][0] = img.pixel[x][y][0];
            img.pixel[aux_height][aux_width][1] = img.pixel[x][y][1];
            img.pixel[aux_height][aux_width][2] = img.pixel[x][y][2];

            img.pixel[x][y][0] = pixel.red;
            img.pixel[x][y][1] = pixel.green;
            img.pixel[x][y][2] = pixel.blue;
        }
    }
    return img;
}

Image mirror(Image img) {

    int horizontal = 0;
    scanf("%d", &horizontal);

    int width = img.width;
    int height = img.height;

    choose_kind_of_mirror(horizontal, width, height);
    img = function_mirror(img, horizontal, width, height);

    return img;
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
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = change_sepia_filter(img);
                break;
            }
            case 3: { // Blur
                img = change_blur(img);
                break;
            }
            case 4: { // Rotacao
                img = rotate_photo(img);
                break;
            }
            case 5: { // Espelhamento
                img = mirror(img);
                break;
            }
            case 6: { // Inversao de Cores
                invert_colors(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int starting_point_x;
                int starting_point_y;
                scanf("%d %d", &starting_point_x, &starting_point_y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cut_image(img, starting_point_x, starting_point_y, width, height);
                break;
            }
        }

    }

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
    return 0;
}
