#include <csv.h>
#include <sys/stat.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

/**
 * @brief Read the CSV file and store the data in the vectors
 *
 * @param filename The name of the file to read
 * @param x The vector to store the x data
 * @param y1 The vector to store the y1 data
 * @param y2 The vector to store the y2 data
 *
 * @return 0 if successful, 1 otherwise
 *
 * @note The CSV file should have the following format:
 * `<group>;<timestamp>;<computed_steering>;<ground_steering>;`
 */
int readCSV(const std::string& filename, std::vector<int64_t>& x, std::vector<double>& y1, std::vector<double>& y2) {

    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;

        return 1;
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        // Skip the first column (group column)
        if (!std::getline(iss, token, ';')) continue;

        try {
            // timestamp (x)
            int64_t x_val;
            if (!(iss >> x_val)) continue;

            if (!std::getline(iss, token, ';')) continue;

            // computed_steering (y1)
            double y1_val;
            if (!(iss >> y1_val)) continue;

            if (!std::getline(iss, token, ';')) continue;

            // ground_steering (y2)
            double y2_val;
            if (!(iss >> y2_val)) continue;

            // Store the data in the vectors
            x.push_back(x_val);
            y1.push_back(y1_val);
            y2.push_back(y2_val);
        }

        catch (...) {
            std::cerr << "Error reading file: " << filename << std::endl;

            return 1;
        }
    }

    return 0;
}

/**
 * @brief Print the match percentage of the two vectors
 *
 * @param filename The name of the file
 * @param x The x data
 * @param y1 The y1 data
 * @param y2 The y2 data
 *
 * @return The match percentage
 */
double print_match_percentage(const std::string& filename, const std::vector<int64_t>& x, const std::vector<double>& y1, const std::vector<double>& y2) {
    int count = 0;
    int valid_points = 0;
    for (size_t i = 0; i < y1.size(); ++i) {
        if (y2[i] == 0) continue;  // Skip if y2 is 0

        valid_points++;
        if (std::abs(y1[i] - y2[i]) <= 0.25 * std::abs(y2[i])) {
            count++;
        }
    }

    if (valid_points == 0) {
        std::cout << "No valid data points found." << std::endl;
        return 0.0;
    }

    double percentage = (static_cast<double>(count) / valid_points) * 100;
    std::cout << filename
              << "   | "
              << std::fixed
              << std::setprecision(2)
              << percentage
              << "%"
              << std::endl;

    return percentage;
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file1.csv> [<file2.csv> ...]" << std::endl;
        return 1;
    }

    // Check if the output directory exists, if not create it
    struct stat info;
    if (stat("output", &info) != 0 || !(info.st_mode & S_IFDIR)) {
        // The directory does not exist, create it
        mkdir("output", 0755);
    }

    // Variable to store the sum of match percentages
    double sum_match_percentage = 0;

    std::string base_path = "./csv/";
    std::string file_suffix = ".csv";

    // Print full line
    std::cout << "------------------------------------------------------------" << std::endl;

    // Print the header
    std::cout << "Filename | Match Percentage" << std::endl;

    // Print full line
    std::cout << "------------------------------------------------------------" << std::endl;

    for (int i = 1; i < argc; ++i) {
        std::vector<int64_t> x_data;
        std::vector<double> y_data1, y_data2;

        // Construct the full file path
        std::string full_path = base_path + argv[i] + file_suffix;

        if (readCSV(full_path, x_data, y_data1, y_data2)) {
            continue;
        }

        // Plot the data using matplotlibcpp.h
        plt::figure_size(1200, 780);
        plt::plot(x_data, y_data1, "b");
        plt::plot(x_data, y_data2, "r");
        plt::xlabel("Time (microseconds)");
        plt::ylabel("Computed Steering (Blue) | Ground Steering (Red)");
        std::string plot_filename = "output/" + std::string(argv[i]) + ".png";
        plt::save(plot_filename);

        // Print the information
        sum_match_percentage += print_match_percentage(argv[i], x_data, y_data1, y_data2);
    }

    // Divide by argc - 1 to get the average match percentage
    sum_match_percentage / (argc - 1);

    // Print full line
    std::cout << "------------------------------------------------------------" << std::endl;

    // Print the average match percentage
    std::cout << "Average match percentage: " << sum_match_percentage / (argc - 1) << "%" << std::endl;

    // Print full line
    std::cout << "------------------------------------------------------------" << std::endl;

    return 0;
}
