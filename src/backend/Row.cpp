#include "Row.h"

#include <variant>

// Set a column value
void Row::setColumn(const std::string& columnName, ColumnValue value)
{
    columns[columnName] = value;
}

// Get a column value
Row::ColumnValue Row::getColumn(const std::string& columnName) const
{
    auto it = columns.find(columnName);
    if (it != columns.end())
    {
        return it->second;
    }
    else
    {
        throw std::invalid_argument("Column not found");
    }
}

// Get the primary key (must be an integer)
int Row::getPrimaryKey() const {
    // Find the "id" column
    auto it = columns.find("ID");

    // Ensure the "id" column exists and is an integer
    if (it != columns.end() && std::holds_alternative<int>(it->second)) {
        return std::get<int>(it->second);
    }
    throw std::invalid_argument("Primary key 'id' is missing or is not an integer.");
}