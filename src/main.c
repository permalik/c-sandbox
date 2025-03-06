#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <time.h>
#include <unistd.h>

struct Files {
    char* file;
};

int random_number_string(char* buffer) {
    int dataset[] = {0,1,2,3,4,5,6,7,8,9};
    int n = 10;

    for (int i = 0; i < n; i++) {
        int index = rand() % (n - i);
        buffer[i] = '0' + dataset[index];

        for (int j = index; j < n - i - 1; j++) {
            dataset[j] = dataset[j + 1];
        }
    }
    return 0;
}

int unlink_callback(const char* file_path, const struct stat* stat_info, int typeflag, struct FTW* ftwbuffer) {
    if (typeflag == FTW_DP) {
        return rmdir(file_path);
    }
    return unlink(file_path);
}

int rimraf(const char* dir_path) {
    return nftw(dir_path, unlink_callback, 50, FTW_DEPTH | FTW_PHYS);
}

int init_directory(const char** dir_path) {
    struct stat s;
    if (stat(*dir_path, &s) == 0) {
        if (S_ISDIR(s.st_mode)) {
            if (rimraf(*dir_path) != 0) {
                perror("Error recursively removing directory.\n");
                return 1;
            }
            mkdir(*dir_path, 0755);
        } else {
            printf("Path exists but is not directory.\n");
            return 1;
        }
    } else {
        if (errno == ENOENT) {
            mkdir(*dir_path, 0755);
        } else {
            perror("Failed to stat directory.\n");
            return 1;
        }
    }
    return 0;
}

int populate_source(const char** dir_path, const char* source_file_names[], int* file_names_length) {
    int is_ok = EXIT_SUCCESS;
    for (int i = 0; i < *file_names_length; i++) {
        size_t path_length = strlen(*dir_path) + strlen(source_file_names[i]) + 2;
        char* file_path = malloc(path_length);
        if (file_path == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for %s\n", source_file_names[i]);
            is_ok = EXIT_FAILURE;
            continue;
        }

        snprintf(file_path, path_length, "%s/%s", *dir_path, source_file_names[i]);
        FILE* fp = fopen(file_path, "w+");
        if (!fp) {
            perror("Error: File opening failed.\n");
            is_ok = EXIT_FAILURE;
            continue;
        }

        for (int j = 0; j < 100; j++) {
            char random_number[11];
            random_number_string(random_number);
            fwrite(random_number, 1, 10, fp);
            fwrite("\n", 1, 1, fp);
        }
        rewind(fp);
        fclose(fp);
        free(file_path);
    }

    if (is_ok == EXIT_FAILURE) {
        fprintf(stderr, "Memory allocations failed.\n");
        return 1;
    }
    return 0;
}

int sort_numbers(const char** source_dir_path, const char** dest_dir_path) {
    // TODO: is_ok
    struct dirent* entry;
    DIR* dir = opendir(*source_dir_path);
    if (!dir) {
        perror("Failed opendir.\n");
        return 1;
    }
    srand(time(NULL));
    while ((entry = readdir(dir)) != NULL) {
        size_t path_length = strlen(*source_dir_path) + strlen(entry->d_name) + 2;
        char* file_path = malloc(path_length);
        snprintf(file_path , path_length, "%s/%s", *source_dir_path, entry->d_name);

        struct stat s;
        if (stat(file_path, &s) == 0) {
            if (S_ISREG(s.st_mode)) {
                // int c;
                // char str[11] = "";
                // int offset = 0;
                // FILE* fp = fopen(file_path, "r");
                // while ((c = fgetc(fp)) != EOF) {
                //     if (c == '\n') {
                //         continue;
                //     }

                //     if (offset < 10) {
                //         str[offset++] = c;
                //     }
                // }
                // str[offset] = '\0';
                // puts(str);
                // offset = 0;
                int c;
                char chars[10];
                FILE* fp = fopen(file_path, "r");
                int counter = 0;
                while ((c = fgetc(fp)) != EOF) {
                    if (c == '\n') {
                        continue;
                    }
                    chars[counter] = c;
                    counter++;
                    if (counter == 10) {
                        for (int i = 1; i < 10; i++) {
                            int key = chars[i];
                            int j = i - 1;
                            while (j >= 0 && chars[j] > key) {
                                chars[j + 1] = chars[j];
                                j--;
                            }
                            chars[j + 1] = key;
                        }
                        for (int j = 0; j < 10; j++) {
                            printf("%c\n", chars[j]);
                        }
                        counter = 0;
                    }
                }

                if (ferror(fp)) {
                    puts("I/O error while reading.");
                } else if (feof(fp)) {
                    puts("Successfully read to EOF.");
                }

                fclose(fp);
                free(file_path);
                continue;
            } else if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                printf("Skipping . and .. directories.\n");
                free(file_path);
                continue;
            } else {
                printf("Path exists but is not regular file.\n");
                free(file_path);
                return 1;
            }
        } else {
            if (errno == ENOENT) {
                perror("Error: File does not exist.\n");
                free(file_path);
                return 1;
            } else {
                perror("Error: Failed to stat file.\n");
                free(file_path);
                return 1;
            }
        }
        free(file_path);
    }

    time_t t = time(NULL);
    struct tm tm_info;
    localtime_r(&t, &tm_info);
    char timestamp[13];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M", &tm_info);

    // TODO: iterate file lines
    // TODO: read line to buffer
    // TODO: add char to array, converting to int
    // TODO: sort array
    // TODO: convert int arr to chars
    // TODO: write string from arr to dest file
    return 0;
}

int main() {
    const char* SOURCEDIRECTORY = "/Users/tymalik/Docs/Git/construct/c_construct/src/data";
    const char* DESTDIRECTORY = "/Users/tymalik/Docs/Git/construct/c_construct/src/dest";
    int is_source_initialized = init_directory(&SOURCEDIRECTORY);
    assert((is_source_initialized == 0) && "Source must be initialized.\n");
    int is_dest_initialized = init_directory(&DESTDIRECTORY);
    assert((is_dest_initialized == 0) && "Dest must be initialized.\n");

    const char* filenames[] = {
        "one.txt", "two.txt", "three.txt", "four.txt", "five.txt",
        "six.txt", "seven.txt", "eight.txt", "nine.txt", "ten.txt",
        "eleven.txt", "twelve.txt", "thirteen.txt", "fourteen.txt", "fifteen.txt",
        "sixteen.txt", "seventeen.txt", "eighteen.txt", "nineteen.txt", "twenty.txt",
        "twenty_one.txt", "twenty_two.txt", "twenty_three.txt", "twenty_four.txt", "twenty_five.txt",
        "twenty_six.txt", "twenty_seven.txt", "twenty_eight.txt", "twenty_nine.txt", "thirty.txt",
        "thirty_one.txt", "thirty_two.txt", "thirty_three.txt", "thirty_four.txt", "thirty_five.txt",
        "thirty_six.txt", "thirty_seven.txt", "thirty_eight.txt", "thirty_nine.txt", "forty.txt",
        "forty_one.txt", "forty_two.txt", "forty_three.txt", "forty_four.txt", "forty_five.txt",
        "forty_six.txt", "forty_seven.txt", "forty_eight.txt", "forty_nine.txt", "fifty.txt"
    };
    int filenames_length = sizeof(filenames) / sizeof(filenames[0]);
    assert(filenames_length > 0 && "Filenames cannot be empty.\n");

    int is_populate_configured = populate_source(&SOURCEDIRECTORY, filenames, &filenames_length);
    assert((is_populate_configured == 0) && "Population assets must be configured.\n");

    int is_sort_configured = sort_numbers(&SOURCEDIRECTORY, &DESTDIRECTORY);
    assert((is_sort_configured == 0) && "Source assets must be configured.\n");

    return 0;
}
