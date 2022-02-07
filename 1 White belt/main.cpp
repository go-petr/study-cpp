#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <iomanip>
#include "test_runner.h"

using namespace std;

class Date {
public:
    Date(const int &new_year, const int &new_month, const int &new_day) {
        if (new_month < 1 || new_month > 12) {
            throw runtime_error("Month value is invalid: " + to_string(new_month));
        }
        if (new_day < 1 || new_day > 31) {
            throw runtime_error("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const {
        return year;
    };

    int GetMonth() const {
        return month;
    };

    int GetDay() const {
        return day;
    };

private:
    int year, month, day;
};

Date ParseDate(const string &date) {
    istringstream sstream(date);

    int year_sign = 1;
    if (sstream.peek() == '-') {
        year_sign = -1;
        sstream.ignore(1);
    }

    string year_str, month_str, day_str;
    getline(sstream, year_str, '-');
    getline(sstream, month_str, '-');
    getline(sstream, day_str, '-');


    int year, month, day;
    try {
        year = stoi(isdigit(year_str.back()) ? year_str : "invalid end") * year_sign;
        month = stoi(isdigit(month_str.back()) ? month_str : "invalid end");
        day = stoi(isdigit(day_str.back()) ? day_str : "invalid end");
    } catch (exception &e) {
        throw runtime_error("Wrong date format: " + date);
    }

    return Date(year, month, day);
}

ostream &operator<<(ostream &stream, const Date &date) {
    stream << setfill('0') << setw(4);
    if (date.GetYear() < 0) {
        stream << "" + to_string(date.GetYear());
    } else {
        stream << to_string(date.GetYear());
    }
    stream << "-" << setw(2) << to_string(date.GetMonth())
           << "-" << setw(2) << to_string(date.GetDay());
    return stream;
}

bool operator<(const Date &lhs, const Date &rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        } else {
            return lhs.GetMonth() < rhs.GetMonth();
        }
    } else {
        return lhs.GetYear() < rhs.GetYear();
    }
};


class Database {
public:
    void AddEvent(const Date &date, const string &event);

    bool DeleteEvent(const Date &date, const string &event);

    int DeleteDate(const Date &date);

    string Find(const Date &date) const;

    void Print() const;
};

void ProcessQuery(Database &db, const string &query) {
    // ignore empty commands
    if (!query.empty()) {
        istringstream sstream(query);
        string command;
        sstream >> command;
        if (command == "Add") {
            string date_input, event;
            sstream >> date_input >> event;
            const Date date = ParseDate(date_input);

        } else if (command == "Find") {
            string date_input;
            sstream >> date_input;
            const Date date = ParseDate(date_input);

        } else if (command == "Del") {
            string date, event;
            sstream >> date >> event;

        } else if (command == "Print") {

        } else {
            throw runtime_error("Unknown command: " + command);
        }
    }
}

/* --------------------------------------TESTING----------------------------------------------
 * Below are the tests that use test_runner.h, a self-developed unit test framework.
 * This framework was developed during the second course of the specialization ("Yellow belt").
 * It is based only on the students' c++ language knowledge after the first two courses.
 */

string ProcessQueryException(Database &db, const string &query) {
    // Return string from the exception thrown in the ProcessQuery function.
    // If no exception was thrown, return string "no exception".
    try {
        ProcessQuery(db, query);
    } catch (exception &e) {
        return static_cast<string>(e.what());
    }
    return "no exception";
}

void TestProcessQuery() {
    Database db;
    string input, expected;

    expected = "Unknown command: ";

    input = "XXX";
    AssertEqual(ProcessQueryException(db, input), expected + input, "Unknown command");

    input = "";
    AssertEqual(ProcessQueryException(db, input), "no exception", "Empty command");
}

string ParseDateException(const string &date) {
    // Return string from the exception thrown in the ParseDate function.
    // If no exception was thrown, return string "no exception".
    try {
        Date bad_date = ParseDate(date);
    } catch (exception &e) {
        return static_cast<string>(e.what());
    }
    return "no exception";
}

bool operator==(const Date &lhs, const Date &rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            if (lhs.GetDay() == rhs.GetDay()) {
                return true;
            }
        }
    }
    return false;
};

void TestDateParser() {
    Database db;
    string input, expected;

    // Test "Wrong date format" type exceptions

    expected = "Wrong date format: ";

    input = "X-1-1";
    AssertEqual(ParseDateException(input), expected + input, "Wrong year format");

    input = "1-X-1";
    AssertEqual(ParseDateException(input), expected + input, "Wrong month format");

    input = "1-1-X";
    AssertEqual(ParseDateException(input), expected + input, "Wrong day format");


    input = "*1-1-1";
    AssertEqual(ParseDateException(input), expected + input, "Wrong first char format");

    input = "1*1-1";
    AssertEqual(ParseDateException(input), expected + input, "Wrong second char format");

    input = "1-1*1";
    AssertEqual(ParseDateException(input), expected + input, "Wrong third char format");

    input = "1 1 1";
    AssertEqual(ParseDateException(input), expected + input, "Char absence");


    input = "-1-1";
    AssertEqual(ParseDateException(input), expected + input, "Year absence");

    input = "1--1";
    AssertEqual(ParseDateException(input), expected + input, "Month absence");

    input = "1-1-";
    AssertEqual(ParseDateException(input), expected + input, "Day absence");

    input = "1-20-";
    AssertEqual(ParseDateException(input), expected + input, "Wrong date format and month");


    input = "-100-1-1";
    AssertEqual(ParseDate(input), Date(-100, 1, 1), "Negative year");

    input = "+100-1-1";
    AssertEqual(ParseDate(input), Date(100, 1, 1), "+ sign before year");

    input = "1-+1-1";
    AssertEqual(ParseDate(input), Date(1, 1, 1), "+ sign before month");

    input = "1-1-+1";
    AssertEqual(ParseDate(input), Date(1, 1, 1), "+ sign before day");

    input = "1+-1-1";
    AssertEqual(ParseDateException(input), expected + input, "+ sign after year");

    input = "1-1+-1";
    AssertEqual(ParseDateException(input), expected + input, "+ sign after");


    // Test month and day values

    input = "1-20-1";
    AssertEqual(ParseDateException(input), "Month value is invalid: 20", "");

    input = "1-1-100";
    AssertEqual(ParseDateException(input), "Day value is invalid: 100", "");

    input = "1-20-100";
    AssertEqual(ParseDateException(input), "Month value is invalid: 20", "");

}

void TestAll() {
    TestRunner runner = TestRunner();
    runner.RunTest(TestProcessQuery, "TestInvalidCommand");
    runner.RunTest(TestDateParser, "TestDateParser");

}


int main() {
    TestAll();


    Database db;

    string query;
    while (getline(cin, query)) {
        try {
            ProcessQuery(db, query);
        } catch (exception &e) {
            cout << e.what();
            return 0;
        }
    }

    return 0;
}