#include "pdb_api.h"

NibblePDB::NibblePDB(const std::string& filename, std::uint64_t numEntries, std::size_t headerBytes): 
            numEntries(numEntries), headerBytes(headerBytes){
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open PDB file: " + filename);
    }

    in.seekg(0, std::ios::end);
    std::streamsize fileSize = in.tellg();
    in.seekg(0, std::ios::beg);

    if (fileSize <= static_cast<std::streamsize>(headerBytes)) {
        throw std::runtime_error("PDB file too small: " + filename);
    }

    data.resize(static_cast<std::size_t>(fileSize));
    if (!in.read(reinterpret_cast<char*>(data.data()), fileSize)) {
        throw std::runtime_error("Failed to read PDB file: " + filename);
    }

    // Sanity check: we need at least ceil(numEntries/2) bytes of data after the header.
    std::uint64_t neededBytes = (numEntries + 1) / 2;
    if (static_cast<std::uint64_t>(fileSize) < headerBytes + neededBytes) {
        throw std::runtime_error("PDB file does not contain enough data for numEntries.");
    }
}
    // Get the raw 4-bit value at index (0 .. numEntries-1)
    std::uint8_t NibblePDB::getRaw(std::uint64_t index) const {
        if (index >= numEntries) {
            throw std::out_of_range("PDB index out of range");
        }

        std::uint64_t nibbleIndex = index; // 0-based
        std::uint64_t byteIndex   = headerBytes + nibbleIndex / 2;
        bool useHighNibble        = (nibbleIndex & 1u) != 0;

        std::uint8_t byte = data[static_cast<std::size_t>(byteIndex)];
        return useHighNibble ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
    }

    // Interpret the raw nibble as a distance.
    // NOTE: Depending on how the DB was generated, this might be d or d+1.
    // Start by returning it directly, then calibrate using a few test states.
    std::uint8_t NibblePDB::getDistance(std::uint64_t index) const {
        std::uint8_t v = getRaw(index);
        if (v == 0xF) {
            throw std::runtime_error("PDB entry is unreachable/uninitialized");
        }

        if (v == 0) {
            throw std::runtime_error("Invalid PDB entry: 0 cannot occur when stored as d+1");
        }

        return v - 1;

    }

    std::uint64_t NibblePDB::size() const { return numEntries; }