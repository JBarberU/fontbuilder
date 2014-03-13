#include <iostream>
#include <vector>
#include <cstring>

#include <DistanceField.h>

#define DISTANCE_FIELD_DISTANCE 20

void showHelp(const char *msg)
{
    if (msg)
        std::cout << msg << "\n\n";

    std::cout << ("\nUsage:\n"
               "\t-h Help\n"
               "\t-i The input file\n"
               "\t-o The output file\n"
               "\t-m The maximum measurement of the output file\n"
               "\t-d The distance (spread) to use\n"
               "\t-c The number of threads to use\n"
               "\nCopyright (C) 2014 John Barbero Unenge\n\n");
}

int main(int argc, char **argv)
{
    if (argc != 11)
    {
        if (argc < 2)
            showHelp(0);
        else
            showHelp("Wrong number of arguments!");
        return 0;
    }

    std::string in_file, out_file;

    int maxSize{0}, distance{0}, cores{0};
    for (int i = 0; i < argc - 1; ++i)
    {
        if (!strcmp(argv[i], "-i"))
            in_file = std::string(argv[i+1]);
        else if (!strcmp(argv[i], "-o"))
            out_file = std::string(argv[i+1]);
        else if (!strcmp(argv[i], "-m"))
            maxSize = std::atoi(argv[i+1]);
        else if (!strcmp(argv[i], "-d"))
            distance = std::atoi(argv[i+1]);
        else if (!strcmp(argv[i], "-c"))
            cores = std::atoi(argv[i+1]);
    }

    if (in_file.empty() || out_file.empty() || maxSize == 0 || distance == 0)
    {
        showHelp("You have to specify an input file, an output file, a maximum size and a distance!");
        return 0;
    }

    if (maxSize <= 0)
    {
        showHelp("The maximum output size has to be a value > 0");
        return 0;
    }

    if (distance <= 0)
    {
        showHelp("The distance has to be a value > 0");
        return 0;
    }

    if (cores <= 0)
    {
        showHelp("The number of cores has to be a value > 0");
        return 0;
    }

    dfgenerator::DistanceField df{in_file, size_t(maxSize)};
    df.getDfImage(distance, cores).saveImage(out_file);

    std::cout << "Done!" << std::endl;
    return 0;
}
