#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <unistd.h>

struct Files {
    char *file;
};

int init_directory(const char **dir_path/*,char *filenames[]*/) {
    // TODO: remove stale dirs
    struct stat s;
    if (stat(*dir_path, &s) == 0) {
        if (S_ISDIR(s.st_mode)) {
            printf("Found directory: %s", *dir_path);
            return 1;
        } else {
            printf("Path exists but is not directory.\n");
        }
    } else {
        if (errno == ENOENT) {
            printf("Directory does not exist\n");
        } else {
            perror("Failed to stat.\n");
        }
    }
    // TODO: make new dirs
    // TODO: make files if source
    return 0;
}

int main() {
    const char *filenames[] = {
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
    assert(filenames_length > 0 && "filenames cannot be empty");

    for (int i = 0; i < filenames_length; i++) {
        printf("%s\n", filenames[i]);
    }

    const char *SOURCEDIRECTORY = "/Users/tymalik/Docs/Git/construct/c_construct/src/data";
    const char *DESTDIRECTORY = "dest";
    int is_source_initialized = init_directory(&SOURCEDIRECTORY);
    assert((is_source_initialized == 1) && "Source must be initialized.");

    printf("%s%s", SOURCEDIRECTORY, "\n___\n");
    printf("%s%s", DESTDIRECTORY, "\n___\n");
    return 0;
}
