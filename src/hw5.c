#include "hw5.h"

int is_word_boundary(char c);
int match_prefix(const char *str, const char *prefix);
int match_suffix(const char *str, const char *suffix);

/**
 * @brief Counts number of lines in a file
 *
 * @param filename Name of the file
 * @return int Number of lines
 */
int countLines(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        return -1;
    }

    int count = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }

    fclose(file);

    // Account for files that don't end with a newline
    if (ch != '\n' && count != 0)
    {
        count++;
    }

    return count;
}

/**
 * @brief Checks if argv contains a specific flag
 *
 * @param argc Number of commandline arguments
 * @param argv Commandline arguments
 * @param flag Flag to check
 * @return int 1 if flag exists 0 if doesn't
 */
int contains(int argc, const char *argv[], char *flag)
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], flag) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Checks if a word is space or punctuation or at boundry
 *
 * @param c Character to check
 * @return int 1 if it is and 0 if it is not
 */
int is_word_boundary(char c)
{
    return isspace((unsigned char)c) || ispunct((unsigned char)c) || c == '\0';
}

/**
 * @brief Checks for specific prefix in a word
 *
 * @param str word to check
 * @param prefix prefix to look for
 * @return int if found then 1 or else 0
 */
int match_prefix(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix) - 1) == 0;
}

/**
 * @brief Checks for specific suffix in a word
 *
 * @param str word to check
 * @param suffix suffic to look for
 * @return int if found then 1 or else 0
 */
int match_suffix(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix) - 1;
    return str_len >= suffix_len && strcmp(str + str_len - suffix_len, suffix + 1) == 0;
}

/**
 * @brief Copies non-word characters from the word start pointer to the output pointer.
 * @param output Pointer to a pointer of the destination where non-word characters will be copied to.
 * @param word_start Pointer to a pointer of the source where non-word characters are copied from.
 */
void copy_non_word_characters(char **output, char **word_start)
{
    while (**word_start && is_word_boundary(**word_start))
    {
        *(*output)++ = *(*word_start)++; // Copy non-word characters
    }
}

/**
 * @brief Temporarily terminates a word by placing a null terminator at the word end.
 * @param word_end Pointer to the end of the word which will be temporarily null-terminated.
 * @return The original character that was at the word end before null-termination.
 */

char temporarily_terminate_word(char *word_end)
{
    char temp = *word_end;
    *word_end = '\0';
    return temp;
}

/**
 * @brief Restores the original character at the end of a word after a temporary null termination.
 *
 * @param word_end Pointer to the end of the word where the original character is to be restored.
 * @param original_char The original character to be restored at the word end.
 */

void restore_original_character(char *word_end, char original_char)
{
    *word_end = original_char;
}

/**
 * @brief Performs the replacement of a word by copying the replacement text to the output.
 *
 * @param output Pointer to a pointer of the destination where the replacement text will be copied.
 * @param word_start Pointer to the start of the word in the original text (unused in this function).
 * @param replacement The replacement text that will be copied to the output.
 */

void perform_replacement(char **output, const char *word_start, const char *replacement)
{
    *output += sprintf(*output, "%s", replacement); // Replace word
}

/**
 * @brief Checks for a wildcard and then replaces specified word with a particular word
 *
 * @param line Line to search word in
 * @param search_text Word to search for
 * @param replacement Word to replace the search word with
 */
void wildcard_replace(char *line, const char *search_text, const char *replacement)
{
    const char *wildcard_pos = strchr(search_text, '*');
    int prefix_search = (wildcard_pos == search_text + strlen(search_text) - 1);
    int suffix_search = (wildcard_pos == search_text);
    char buffer[1024];
    char *output = buffer;
    char *word_start = line;
    char *word_end;

    while (*word_start)
    {
        copy_non_word_characters(&output, &word_start);

        if (*word_start == '\0')
            break; // End of line

        word_end = word_start;
        while (*word_end && !is_word_boundary(*word_end))
        {
            word_end++;
        }

        char temp = temporarily_terminate_word(word_end);

        if ((prefix_search && match_prefix(word_start, search_text)) ||
            (suffix_search && match_suffix(word_start, search_text)))
        {
            perform_replacement(&output, word_start, replacement);
        }
        else
        {
            output += sprintf(output, "%s", word_start); // Copy word unchanged
        }

        restore_original_character(word_end, temp);
        word_start = word_end; // Move to the next word
    }

    *output = '\0';       // Null-terminate the buffer
    strcpy(line, buffer); // Now, copy the modified line back to the original line buffer
}

/**
 * @brief Normal replacing a specific word with a particular word
 *
 * @param source Line
 * @param substring Substring to checl
 * @param with Word to replace with
 * @return char* The string after the word is replaced
 */
char *string_replace(char *source, char *substring, char *with)
{
    char *substring_source = strstr(source, substring);
    if (substring_source == NULL)
    {
        return NULL;
    }

    memmove(
        substring_source + strlen(with),
        substring_source + strlen(substring),
        strlen(substring_source) - strlen(substring) + 1);

    memcpy(substring_source, with, strlen(with));
    return substring_source + strlen(with);
}

int main(int argc, char *argv[])
{

    // Error handling....
    int opt;
    char *sArguments = NULL;
    char *rArguments = NULL;
    char *lValue = NULL;
    char *inputFile = NULL;
    char *outputFile = NULL;
    long longArguments[2] = {0};

    if (argc < 7)
        exit(MISSING_ARGUMENT);

    while ((opt = getopt(argc, argv, "s:r:l:")) != -1)
    {
        switch (opt)
        {
        case 's':
            if (sArguments)
                exit(DUPLICATE_ARGUMENT);
            if (optarg[0] == '-')
                exit(S_ARGUMENT_MISSING);
            sArguments = optarg;
            break;
        case 'r':
            if (rArguments)
                exit(DUPLICATE_ARGUMENT);
            if (optarg[0] == '-')
                exit(R_ARGUMENT_MISSING);
            rArguments = optarg;
            break;
        case 'l':
            if (lValue)
                exit(DUPLICATE_ARGUMENT);
            if (optarg[0] == '-')
                exit(L_ARGUMENT_INVALID);
            lValue = optarg;
            char *token = strtok(lValue, ",");
            int counter = 0;
            while (token != NULL && counter < 2)
            {
                char *endValue;
                long num = strtol(token, &endValue, 10);
                if (endValue == token || num == 0)
                {
                    exit(L_ARGUMENT_INVALID);
                }
                longArguments[counter++] = num;
                token = strtok(NULL, ",");
            }
            if (counter != 2 || longArguments[0] > longArguments[1])
            {
                exit(L_ARGUMENT_INVALID);
            }
            break;
        default:
            // Handle unexpected options and ignoring
            break;
        }
    }

    // Check for the mandatory arguments (-s and -r)
    if (!sArguments)
        exit(S_ARGUMENT_MISSING);
    if (!rArguments)
        exit(R_ARGUMENT_MISSING);

    if (optind < argc)
    {
        inputFile = argv[optind++];
    }
    if (optind < argc)
    {
        outputFile = argv[optind++];
    }

    // Check for the filenames
    if (!inputFile || !outputFile)
    {
        exit(MISSING_ARGUMENT);
    }

    FILE *fptr = fopen(inputFile, "r");
    if (fptr == NULL)
    {
        exit(INPUT_FILE_MISSING);
    }

    FILE *optr = fopen(outputFile, "w");
    if (optr == NULL)
    {
        exit(OUTPUT_FILE_UNWRITABLE);
    }

    // WildCard_Error
    if (contains(argc, argv, "-w") == 1)
    {
        int lengthOfsArgument = strlen(sArguments);

        if (sArguments[0] != '*' && sArguments[lengthOfsArgument - 1] != '*')
            exit(WILDCARD_INVALID);

        if (sArguments[0] == '*' && sArguments[lengthOfsArgument - 1] == '*')
            exit(WILDCARD_INVALID);

        if (sArguments[0] != '*')
        {
            // Read the file line by line
            char bfr[1000] = {0};
            int lines = 1;
            int minimum = -1;
            int maximum = countLines(inputFile) + 1;

            if (contains(argc, argv, "-l") == 1)
            {
                minimum = longArguments[0];
                maximum = longArguments[1];
            }

            while (fgets(bfr, 1000, fptr) != NULL)
            {
                if (lines >= minimum && lines <= maximum)
                {
                    wildcard_replace(bfr, sArguments, rArguments);
                }

                fputs(bfr, optr);
                lines++;
            }
            fclose(optr);
        }

        else
        {
            // Read the file line by line
            char bfr[1000] = {0};
            int lines = 1;
            int minimum = -1;
            int maximum = countLines(inputFile) + 1;

            if (contains(argc, argv, "-l") == 1)
            {
                minimum = longArguments[0];
                maximum = longArguments[1];
            }

            while (fgets(bfr, 1000, fptr) != NULL)
            {
                if (lines >= minimum && lines <= maximum)
                    wildcard_replace(bfr, sArguments, rArguments);

                fputs(bfr, optr);
                lines++;
            }
            fclose(optr);
        }
    }
    else
    {
        // Read the file line by line
        char bfr[1000] = {0};
        int lines = 1;
        int minimum = -1;
        int maximum = countLines(inputFile) + 1;

        if (contains(argc, argv, "-l") == 1)
        {
            minimum = longArguments[0];
            maximum = longArguments[1];
        }

        while (fgets(bfr, 1000, fptr) != NULL)
        {
            if (lines >= minimum && lines <= maximum)
            {
                while (string_replace(bfr, sArguments, rArguments))
                    ;
            }

            fputs(bfr, optr);
            lines++;
        }
        fclose(optr);
    }
}
