#ifndef ROW_H
#define ROW_H

#include <string>
#include <unordered_map>
#include <variant>
#include <stdexcept>

class Row
{
public:
    // Define a variant type to hold integer, string, or boolean values
    using ColumnValue = std::variant<int, std::string, bool>;

    // Map column names to their respective values
    std::unordered_map<std::string, ColumnValue> columns;

    // Default constructor
    Row() = default;

    // Method to set a column value
    void setColumn(const std::string& columnName, ColumnValue value);

    // Method to get a column value
    ColumnValue getColumn(const std::string& columnName) const;

    // Method to get the primary key
    int getPrimaryKey() const;

    // Destructor
    ~Row() = default;
};

#endif // ROW_H
    