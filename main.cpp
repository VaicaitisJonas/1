//160 uzd
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>

using namespace std;

const int MAX_PRIME = 25; // maximum number of prime numbers to consider
const int primes[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97 };   // list of the first 25 prime numbers

// class for computing prime factorizations
class PrimeFactorization {
    private:
        vector<long long> factors; // vector of prime factors

    public:
        PrimeFactorization() {
            factors.assign(MAX_PRIME + 1, 0); // constructor initializes factors to zero
        }

        // function to compute prime factors of n
        void compute_factors(long long n) {
            long long i, j, count;
            while (n != 1)   // loop until all prime factors have been found
            {
                i = n;
                j = 0;

                while (i > 1)   // loop through each prime number and count its occurrence in n
                {
                    count = 0;
                    while (i % primes[j] == 0)
                    {
                        i /= primes[j];
                        count++;
                    }
                    factors[j] += count;   // add the count to the vector of prime factors
                    j++;
                }

                n--;
            }
        }

        // function to convert prime factors to a string
        string to_string() const {
            stringstream ss;
            long long count = 0;
            ss << setw(3) << factors[0] << " ";   // add the first prime factor to the string
            count++;
            for (int i = 1; i <= MAX_PRIME; i++) {          // loop through the remaining prime factors
                if (factors[i] != 0) {                     // if the prime factor appears in the vector
                    ss << setw(3) << factors[i] << " ";   // add the count of the prime factor to the string
                    count++;
                }
                if (count == 15) {                     // if 15 prime factors have been added to the string
                    ss << endl << "       ";          // add a new line and indentation to the string
                    count = 0;                       // reset the prime factor count
                }
            }
            return ss.str();                        // return the final string of prime factors
        }
};

// class for reading integers from a file
class FileReader {
    private:
        ifstream file;          // input file stream

    public:
        FileReader(const string& filename) {              // constructor opens the file
            file.open(filename);
        }

        bool is_open() const {                        // function to check if the file is open
            return file.is_open();
        }

        long long read_int() {                    // function to read a long long integer from the file
            long long n;
            file >> n;
            return n;
        }

        bool eof() {                            // function to check if the end of the file has been reached
            return file.eof();
        }
};

// class for writing strings to
class FileWriter {
    private:
        ofstream file;                            // output file stream

    public:
        FileWriter(const string& filename) {         // constructor opens the file
            file.open(filename);
        }

        bool is_open() const {                   // function to check if the file is open
            return file.is_open();
        }

        void write_line(const string& line) {     // function to write data into file
            file << line << endl;
        }
};

int main() {
    FileReader reader("input.txt");
    FileWriter writer("output.txt");

    if (!reader.is_open() || !writer.is_open()) {         // Check if both files are open, if not, print error message and exit program
        cout << "Error opening file." << endl;
        return 1;
    }

    PrimeFactorization pf;                               // Create PrimeFactorization object 
    
    auto total_duration = std::chrono::microseconds::zero();
    
    while (!reader.eof()) {                              // Read input from file until end of file is reached
        long long n = reader.read_int();                 // Read a long long integer from the file
        if (n == 0) {                                    // If input is 0, exit the loop
            break;
        }

        auto start = std::chrono::high_resolution_clock::now(); // Start the timer

        pf.compute_factors(n);                          // Compute prime factorization of input number

        auto stop = std::chrono::high_resolution_clock::now(); // Stop the timer

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); // Calculate the duration in microseconds
        total_duration += duration;

        stringstream ss;                               // Create a stringstream to format output string
        ss << setw(3) << n << "! = " << pf.to_string();

        writer.write_line(ss.str());                   // Write output string to file using FileWriter object

        pf = PrimeFactorization();                     // Reset the PrimeFactorization object for next iteration
    }

    stringstream duration_ss;
    duration_ss << "Total elapsed time: " << total_duration.count() << " microseconds";
    writer.write_line(duration_ss.str());

    return 0;
}
