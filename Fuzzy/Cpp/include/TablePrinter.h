#ifndef TABLE_PRINTER_H
#define TABLE_PRINTER_H

#include <vector>
#include <string>
#include <sstream>

class TablePrinter {
public:
  TablePrinter(
    const std::vector<std::string>& headers, 
    const std::vector<int>& widths
  );

  void printHeader() const;

  template <typename... Args>
  void printRow(
    Args... args
  ) const;

private:
  std::vector<std::string> headers;
  std::vector<int> columnWidths;

  int totalWidth() const;

  template <typename T, typename... Args>
  void printRowHelper(
    size_t index, 
    T value, 
    Args... args
  ) const;

  void printRowHelper(
    size_t
  ) const;

  template <typename T>
  std::string toString(const T& value) const;

  std::string truncateAndPad(
    const std::string& str,
    int width
  ) const;
};

template <typename... Args>
void TablePrinter::printRow(
  Args... args
) const
{
  printRowHelper(0, args...);
  std::cout << std::endl;
}

template <typename T, typename... Args>
void TablePrinter::printRowHelper(
  size_t index,
  T value,
  Args... args
) const
{
  if (index < columnWidths.size()) {
    std::cout << std::left << std::setw(columnWidths[index]) << truncateAndPad(toString(value), columnWidths[index]);
    printRowHelper(index + 1, args...);
  }
}

template <typename T>
std::string TablePrinter::toString(const T& value) const {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

#endif