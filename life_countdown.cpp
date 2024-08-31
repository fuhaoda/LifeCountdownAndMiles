#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

// Function to check if a year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to calculate the total number of days in a year
int daysInYear(int year) {
    return isLeapYear(year) ? 366 : 365;
}

// Function to calculate the difference in days between two dates
int daysBetweenDates(std::tm start, std::tm end) {
    std::time_t start_time = std::mktime(&start);
    std::time_t end_time = std::mktime(&end);
    double difference = std::difftime(end_time, start_time) / (60 * 60 * 24);
    return static_cast<int>(difference);
}

// Function to calculate the weeks left until a certain age
int weeksLeftUntilAge(int birthYear, int birthMonth, int birthDay, int targetAge) {
    std::tm birthDate = {0, 0, 0, birthDay, birthMonth - 1, birthYear - 1900};
    std::tm targetDate = {0, 0, 0, birthDay, birthMonth - 1, birthYear + targetAge - 1900};

    int totalDays = daysBetweenDates(birthDate, targetDate);
    return totalDays / 7; // Convert days to weeks
}

// Main function to calculate life countdown and miles to run
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <birthdate in YYYYMMDD> <miles goal>" << std::endl;
        return 1;
    }

    // Parse input arguments
    std::string birthdateStr = argv[1];
    int milesGoal = std::stoi(argv[2]);

    int birthYear, birthMonth, birthDay;
    std::istringstream(birthdateStr.substr(0, 4)) >> birthYear;
    std::istringstream(birthdateStr.substr(4, 2)) >> birthMonth;
    std::istringstream(birthdateStr.substr(6, 2)) >> birthDay;

    // Get the current date
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);

    // Calculate total and remaining weeks
    int totalWeeks = weeksLeftUntilAge(birthYear, birthMonth, birthDay, 85);
    int weeksLived = weeksLeftUntilAge(birthYear, birthMonth, birthDay, now->tm_year + 1900 - birthYear);
    int weeksLeft = totalWeeks - weeksLived;

    // Calculate the miles needed to run per week
    std::tm startOfYear = {0, 0, 0, 1, 0, now->tm_year}; // January 1st of the current year
    std::tm endOfYear = {0, 0, 0, 31, 11, now->tm_year}; // December 31st of the current year

    int daysSinceStartOfYear = daysBetweenDates(startOfYear, *now);
    int daysLeftInYear = daysBetweenDates(*now, endOfYear);

    int weeksLeftInYear = daysLeftInYear / 7;
    
    // Calculate the miles run so far
    double proportionOfYearPassed = static_cast<double>(daysSinceStartOfYear) / daysInYear(now->tm_year + 1900);
    double milesRunSoFar = proportionOfYearPassed * milesGoal;

    // Output results with formatting
    std::cout << "If you die at 85 years old, in your left, you will have total " << totalWeeks << " weeks, "
              << "and now you only have \033[1;31m" << weeksLeft << "\033[0m weeks left!" << std::endl;
    std::cout << "To achieve your goal to run " << milesGoal << " miles in " << (now->tm_year + 1900) << ", You should have already run approximately \033[1;31m" << std::fixed << std::setprecision(1) << milesRunSoFar << "\033[0m miles this year." << std::endl;
   
    return 0;
}
