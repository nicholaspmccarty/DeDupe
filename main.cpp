/**
 * A simple program to dedupe a list of users, IP-addresses, and ports
 * to retain either the latest two or just the 3rd (cummulatively)
 * entry for each user.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
// Synonyms to make the code a bit more readable.

/** The list of input lines read from a given data file.  This list is
 *  maintained in-memory to facilitate de-duping and returning the
 *  values.
 */
using DataList = std::vector<std::string>;

/** A temporary unordered map that is used to ease looking-up repeated
 * user ID's in the data file. The key to this map is the user ID and
 * value is the index position in the DataList vector. For example,
 * if DataList has the following 4 lines in it: 
 * {"bob 1.1.1 20", "may 2.2.2 30", "bob 3.3.3. 40", "doe 4.4.4. 50"}
 * then, OccursList would be {{"bob", {0, 2}}, {"may", {2}}, {"doe", {3}}.
 * This way if we encounter another line for "bob" (that is the 3rd entry
 * for user bob), we can clear out lines at index positions 0 and 2 to
 * de-dupe the data.
 */
using OccursList = std::unordered_map<std::string, std::vector<size_t>>;

/**
 * Method that returns a de-duped list of data from the given input
 * stream.  The input stream is assumed to have data in the format:
 * userID IP-address port.  This method dedupes based on userID
 * only. If the same userID occurs for the 3rd time, then this method
 * clears out the previous two entries.
 *
 * \param[in] is The input stream from where the data is to be read.
 *
 * \return This method returns a vector that contains the de-deuped
 * information, with duplicates replaced by an empty string.
 */
DataList dedupe(std::istream& is) {
    // The vector that contains de-duped list of lines (read from the
    // input stream is) to be returned back to the caller
    DataList data;

    // The following unordered map tracks number of occurrences for
    // each user. See documentaiton on the OccursList (above) for
    // additional details.
    OccursList occurs;

    size_t index = 0;
    std::string name, ip, port;
    // Keep processing triplets of data from the input stream.
    for (std::string line; std::getline(is, line);) {
        
        // Grab the line and split it up to name, ip, port
        std::istringstream iss(line);
        iss >> name >> ip >> port;
        
        // If does not exist, add to map.
        if (occurs.find(name) == occurs.end()) {
            occurs[name] = std::vector<size_t>(); // Assign an empty vector
    }

    // AI developed this logic until line 82.
    if (occurs[name].size() == 2) {
            // Clear the previous two entries by setting them to empty strings
            data[occurs[name][0]] = ""; // First occurrence
            data[occurs[name][1]] = ""; // Second occurrence

            // Remove the old indices from the vector
            occurs[name].clear(); 
        }

        // Add the current line to the data vector and store its index
        data.push_back(line);
        occurs[name].push_back(index); // Add the index of this line to the user's entry list

        ++index;



    }
    // Return the de-duped list of entries
    return data;
}

//-------------------------------------------------------------------
//  DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//-------------------------------------------------------------------


/**
 * The main method that just acts as a test for the dedupe method in
 * this program.
 *
 * \param[in] argc The number of command-line arguments.  This program
 * requires at least 1.
 *
 * \param[in] argv The actual command-line argument. The first
 * command-line argument is assumed to be the data file to be
 * processed.
 */
int main(int argc, char *argv[]) {
    // Check to ensure we have a data file specified.
    if (argc != 2) {
        std::cout << "Specify data file as command-line argument.\n";
        return 1;
    }
    // Open the data file and ensure it is readable.
    std::ifstream is(argv[1]);
    if (!is.good()) {
        std::cout << "Error reading data from " << argv[1] << std::endl;
        return 2;
    }
    // Now have the dedupe method do the core processing
    const auto result = dedupe(is);
    // Finally, print the deduped list
    for (size_t i = 0; (i < result.size()); i++) {
        std::cout << i << ": " << result[i] << std::endl;
    }
}
