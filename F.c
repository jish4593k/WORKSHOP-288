#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stb_image.h"
#include "stb_image_write.h"

#define MAX_FILE_EXTENSIONS 3
#define MAX_FILE_EXTENSION_LENGTH 5

typedef struct {
    char *name;
    int width;
    int height;
    unsigned char *data;
} Image;

typedef struct {
    char **items;
    int size;
} StringArray;

void put_watermark_on_single_file(const char *input_path, const char *output_path);
void delete_images_from_directory(StringArray *images);
StringArray get_files_from_in_directory(const char *directory, const char *file_extensions[], int num_extensions);

int main() {
    const char *file_extensions[MAX_FILE_EXTENSIONS] = {".jpg", ".png", ".jpeg"};
    const char *in_directory = "pre_photos";
    const char *out_directory = "post_photos";

    StringArray pictures = get_files_from_in_directory(in_directory, file_extensions, MAX_FILE_EXTENSIONS);

    for (int i = 0; i < pictures.size; ++i) {
        char input_path[256];
        sprintf(input_path, "%s/%s", in_directory, pictures.items[i]);

        char output_path[256];
        sprintf(output_path, "%s/marked_%s", out_directory, pictures.items[i]);

        put_watermark_on_single_file(input_path, output_path);
    }

    delete_images_from_directory(&pictures);

    return 0;
}

void put_watermark_on_single_file(const char *input_path, const char *output_path) {
    int image_width, image_height, image_channels;
    unsigned char *image_data = stbi_load(input_path, &image_width, &image_height, &image_channels, 0);

    if (image_data != NULL) {
        // Add watermarking logic here (simplified in this example)

        // Save the marked image
        stbi_write_png(output_path, image_width, image_height, image_channels, image_data, image_width * image_channels);
        
        stbi_image_free(image_data);
    } else {
        fprintf(stderr, "Error loading image: %s\n", input_path);
    }
}

StringArray get_files_from_in_directory(const char *directory, const char *file_extensions[], int num_extensions) {
    StringArray result;
    result.size = 0;
    result.items = NULL;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directory)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                const char *file_name = ent->d_name;
                int len = strlen(file_name);

                for (int i = 0; i < num_extensions; ++i) {
                    const char *extension = file_extensions[i];
                    int ext_len = strlen(extension);

                    if (len > ext_len && strcmp(file_name + len - ext_len, extension) == 0) {
                        result.size++;
                        result.items = realloc(result.items, result.size * sizeof(char *));
                        result.items[result.size - 1] = strdup(file_name);
                        break;
                    }
                }
            }
        }

        closedir(dir);
    } else {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    return result;
}

void delete_images_from_directory(StringArray *images) {
    for (int i = 0; i < images->size; ++i) {
        free(images->items[i]);
    }

    free(images->items);
}
