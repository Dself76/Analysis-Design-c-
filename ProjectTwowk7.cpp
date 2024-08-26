#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <limits> // Include this for std::numeric_limits

// Define a structure to store course information.
struct Course {
    std::string courseNumber;    // Unique identifier for the course.
    std::string name;    // Descriptive name of the course.
    std::vector<std::string> prerequisites;   // List of prerequisites as course numbers.
};

// Function to load courses from a CSV file.
std::vector<Course> LoadCoursesFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<Course> courses;
    std::unordered_set<std::string> courseNumbers;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return courses;
    }

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string courseNumber, name, prerequisite;
        Course course;

        std::getline(stream, courseNumber, ',');
        std::getline(stream, name, ',');

        course.courseNumber = courseNumber;
        course.name = name;

        while (std::getline(stream, prerequisite, ',')) {
            course.prerequisites.push_back(prerequisite);
        }

        if (!courseNumbers.insert(course.courseNumber).second) {
            std::cerr << "Error: Duplicate course number found -> " << course.courseNumber << std::endl;
        }
        else {
            courses.push_back(course);
        }
    }

    file.close();
    return courses;
}

// Function to print information about a single course.
void PrintCourseInfo(const Course& course) {
    std::cout << "Course Number: " << course.courseNumber << std::endl;
    std::cout << "Name: " << course.name << std::endl;
    if (!course.prerequisites.empty()) {
        std::cout << "Prerequisites: ";
        for (const auto& prereq : course.prerequisites) {
            std::cout << prereq << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "No prerequisites." << std::endl;
    }
}

// Main function to drive the program.
int main() {
    std::cout << "Welcome to the course planner.\n";
    std::vector<Course> courses;
    std::string fileName, courseNumber;

    int option;
    do {
        std::cout << "\n1. Input Name of Course or Number.\n";
        std::cout << "2. Print Course List.\n";
        std::cout << "3. Print Course.\n";
        std::cout << "9. Exit\n";
        std::cout << "What would you like to do? ";
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (option) {
        case 1:
            std::cout << "Enter the file name containing course data: ";
            std::cin >> fileName;
            courses = LoadCoursesFromFile(fileName);
            std::cout << "Data loaded from file.\n";
            break;
        case 2:
            std::sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
                return a.courseNumber < b.courseNumber;
                });
            std::cout << "Here is a sample schedule:\n";
            for (const auto& course : courses) {
                std::cout << course.courseNumber << ", " << course.name << std::endl;
            }
            break;
        case 3: {
            std::cout << "What course do you want to know about? ";
            std::cin >> courseNumber;
            auto it = std::find_if(courses.begin(), courses.end(), [&courseNumber](const Course& c) {
                return c.courseNumber == courseNumber;
                });
            if (it != courses.end()) {
                PrintCourseInfo(*it);
            }
            else {
                std::cout << "Course not found." << std::endl;
            }
            break;
        }
        case 9:
            std::cout << "Thank you for using the course planner!" << std::endl;
            break;
        default:
            std::cout << option << " is not a valid option.\n";
            break;
        }
    } while (option != 9);

    return 0;
}
