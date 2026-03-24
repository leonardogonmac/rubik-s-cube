#ifndef PDB_API_H

#define PDB_API_H

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <stdexcept>

class NibblePDB {
public:
    NibblePDB(const std::string& filename, std::uint64_t numEntries, std::size_t headerBytes = 0);
    // Get the raw 4-bit value at index (0 .. numEntries-1)
    std::uint8_t getRaw(std::uint64_t index) const;

    // Interpret the raw nibble as a distance.
    // NOTE: Depending on how the DB was generated, this might be d or d+1.
    // Start by returning it directly, then calibrate using a few test states.
    std::uint8_t getDistance(std::uint64_t index) const;

    std::uint64_t size() const;

private:
    std::vector<std::uint8_t> data;
    std::uint64_t             numEntries;
    std::size_t               headerBytes;
};

#endif