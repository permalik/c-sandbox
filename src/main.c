#include <stdio.h>

struct Files {
    char *file;
};

int main() {
    const char *SOURCEDIRECTORY = "data";
    const char *DESTDIRECTORY = "dest";
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

    // TODO: assert inputFiles is not empty
    // TODO: remove stale dirs
    // TODO: make new dirs
    // TODO: make files if source

    printf("%s%s", SOURCEDIRECTORY, "\n___\n");
    printf("%s%s", DESTDIRECTORY, "\n___\n");
    return 0;
}
