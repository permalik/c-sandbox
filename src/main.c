#include <assert.h>
#ifdef __APPLE__
	#include <dirent.h>
#else
	#include <dirent.h>
	#include <sys/dirent.h>
#endif
#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <time.h>
#include <unistd.h>

struct Files {
    char *file;
};

int random_number_string(char *buffer) {
    const int MAX_STRING_LENGTH = 10;
    int dataset[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i < MAX_STRING_LENGTH; i++) {
        int index = rand() % (MAX_STRING_LENGTH - i);
        buffer[i] = '0' + dataset[index];

        for (int j = index; j < MAX_STRING_LENGTH - i - 1; j++) {
            dataset[j] = dataset[j + 1];
        }
    }
    return 0;
}

int unlink_callback(const char *file_path, const struct stat *stat_info,
                    int typeflag, struct FTW *ftwbuffer) {
    if (typeflag == FTW_DP) {
        return rmdir(file_path);
    }
    return unlink(file_path);
}

int rimraf(const char *dir_path) {
    return nftw(dir_path, unlink_callback, 50, FTW_DEPTH | FTW_PHYS);
}

int init_directory(const char **dir_path) {
    const int RWXRXRX = 0755;
    struct stat dir_stat;
    if (stat(*dir_path, &dir_stat) == 0) {
        if (S_ISDIR(dir_stat.st_mode)) {
            if (rimraf(*dir_path) != 0) {
                perror("Error recursively removing directory.\n");
                return 1;
            }
            mkdir(*dir_path, RWXRXRX);
        } else {
            printf("Path exists but is not directory.\n");
            return 1;
        }
    } else {
        if (errno == ENOENT) {
            mkdir(*dir_path, RWXRXRX);
        } else {
            perror("Failed to stat directory.\n");
            return 1;
        }
    }
    return 0;
}

int populate_source(const char **dir_path, const char *source_file_names[],
                    int *file_names_length) {
    int is_ok = EXIT_SUCCESS;
    for (int i = 0; i < *file_names_length; i++) {
        size_t path_length =
            strlen(*dir_path) + strlen(source_file_names[i]) + 2;
        char *file_path = malloc(path_length);
        if (file_path == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for %s\n",
                    source_file_names[i]);
            is_ok = EXIT_FAILURE;
            continue;
        }

        snprintf(file_path, path_length, "%s/%s", *dir_path,
                 source_file_names[i]);
        FILE *fp = fopen(file_path, "w+");
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

int sort_numbers(const char **source_dir_path, const char **dest_dir_path) {
    // TODO: is_ok
    struct dirent *source_entry;
    DIR *source_dir = opendir(*source_dir_path);
    if (!source_dir) {
        perror("Failed open source dir.\n");
        return 1;
    }
    DIR *dest_dir = opendir(*dest_dir_path);
    if (!dest_dir) {
        perror("Error: Failed open dest dir.\n");
        return 1;
    }
    srand(time(NULL));
    while ((source_entry = readdir(source_dir)) != NULL) {
        size_t source_path_length =
            strlen(*source_dir_path) + strlen(source_entry->d_name) + 2;
        char *source_file_path = malloc(source_path_length);
        snprintf(source_file_path, source_path_length, "%s/%s",
                 *source_dir_path, source_entry->d_name);

        struct stat s;
        if (stat(source_file_path, &s) == 0) {
            if (S_ISREG(s.st_mode)) {
                int c;
                char chars[10] = {0};
                FILE *fp = fopen(source_file_path, "r");
                int source_char_counter = 0;
                while ((c = fgetc(fp)) != EOF) {
                    if (c == '\n') {
                        continue;
                    }
                    chars[source_char_counter] = c;
                    source_char_counter++;
                    if (source_char_counter == 10) {
                        for (int i = 1; i < 10; i++) {
                            int key = chars[i];
                            int j = i - 1;
                            while (j >= 0 && chars[j] > key) {
                                chars[j + 1] = chars[j];
                                j--;
                            }
                            chars[j + 1] = key;
                        }
                        time_t t = time(NULL);
                        struct tm tm_info;
                        localtime_r(&t, &tm_info);
                        char timestamp[13];
                        strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M",
                                 &tm_info);

                        size_t dest_path_length =
                            strlen(*dest_dir_path) + strlen(timestamp) +
                            strlen(source_entry->d_name) + 3;
                        char *dest_file_path = malloc(dest_path_length);
                        snprintf(dest_file_path, dest_path_length, "%s/%s.%s",
                                 *dest_dir_path, timestamp,
                                 source_entry->d_name);
                        FILE *dest_fp = fopen(dest_file_path, "a");
                        char number_string[11] = {0};
                        for (int j = 0; j < 10; j++) {
                            number_string[j] = chars[j];
                        }
                        fwrite(number_string, 1, 10, dest_fp);
                        fwrite("\n", 1, 1, dest_fp);
                        fclose(dest_fp);
                        free(dest_file_path);
                        source_char_counter = 0;
                    }
                }

                if (ferror(fp)) {
                    puts("I/O error while reading.");
                } else if (feof(fp)) {
                    // puts("Successfully read to EOF.");
                }

                fclose(fp);
                free(source_file_path);
                continue;
            } else if (strcmp(source_entry->d_name, ".") == 0 ||
                       strcmp(source_entry->d_name, "..") == 0) {
                printf("Skipping . and .. directories.\n");
                free(source_file_path);
                continue;
            } else {
                printf("Path exists but is not regular file.\n");
                free(source_file_path);
                closedir(dest_dir);
                closedir(source_dir);
                return 1;
            }
        } else {
            if (errno == ENOENT) {
                perror("Error: File does not exist.\n");
                free(source_file_path);
                closedir(dest_dir);
                closedir(source_dir);
                return 1;
            } else {
                perror("Error: Failed to stat file.\n");
                free(source_file_path);
                closedir(dest_dir);
                closedir(source_dir);
                return 1;
            }
        }
        free(source_file_path);
    }
    closedir(source_dir);
    return 0;
}

int main() {
    const char *SOURCEDIRECTORY =
        "/Users/tymalik/Docs/Git/sandbox/c_sandbox/src/data";
    const char *DESTDIRECTORY =
        "/Users/tymalik/Docs/Git/sandbox/c_sandbox/src/dest";
    int is_source_initialized = init_directory(&SOURCEDIRECTORY);
    assert((is_source_initialized == 0) && "Source must be initialized.\n");
    int is_dest_initialized = init_directory(&DESTDIRECTORY);
    assert((is_dest_initialized == 0) && "Dest must be initialized.\n");

    const char *filenames[] = {
        "one.txt",          "two.txt",          "three.txt",
        "four.txt",         "five.txt",         "six.txt",
        "seven.txt",        "eight.txt",        "nine.txt",
        "ten.txt",          "eleven.txt",       "twelve.txt",
        "thirteen.txt",     "fourteen.txt",     "fifteen.txt",
        "sixteen.txt",      "seventeen.txt",    "eighteen.txt",
        "nineteen.txt",     "twenty.txt",       "twenty_one.txt",
        "twenty_two.txt",   "twenty_three.txt", "twenty_four.txt",
        "twenty_five.txt",  "twenty_six.txt",   "twenty_seven.txt",
        "twenty_eight.txt", "twenty_nine.txt",  "thirty.txt",
        "thirty_one.txt",   "thirty_two.txt",   "thirty_three.txt",
        "thirty_four.txt",  "thirty_five.txt",  "thirty_six.txt",
        "thirty_seven.txt", "thirty_eight.txt", "thirty_nine.txt",
        "forty.txt",        "forty_one.txt",    "forty_two.txt",
        "forty_three.txt",  "forty_four.txt",   "forty_five.txt",
        "forty_six.txt",    "forty_seven.txt",  "forty_eight.txt",
        "forty_nine.txt",   "fifty.txt"};
    int filenames_length = sizeof(filenames) / sizeof(filenames[0]);
    assert(filenames_length > 0 && "Filenames cannot be empty.\n");

    int is_populate_configured =
        populate_source(&SOURCEDIRECTORY, filenames, &filenames_length);
    assert((is_populate_configured == 0) &&
           "Population assets must be configured.\n");

    int is_sort_configured = sort_numbers(&SOURCEDIRECTORY, &DESTDIRECTORY);
    assert((is_sort_configured == 0) && "Source assets must be configured.\n");

    return 0;
}
