#include "TablePrinter.h"

#include <iostream>
#include <iomanip>

TablePrinter::TablePrinter(
  const std::vector<std::string>& headers, 
  const std::vector<int>& widths
) : 
  headers(headers), 
  columnWidths(widths) 
{
}

void TablePrinter::printHeader() const 
{
  for (size_t i = 0; i < headers.size(); ++i) {
    std::cout << std::left << std::setw(columnWidths[i]) << headers[i];
  }
  std::cout << std::endl;
  std::cout << std::string(totalWidth(), '-') << std::endl;
}

int TablePrinter::totalWidth() const 
{
  int sum = 0;
  for (int w : columnWidths) sum += w;
  return sum;
}

void TablePrinter::printRowHelper(size_t i) const 
{
}

std::string TablePrinter::truncateAndPad(
  const std::string& str,
  int width
) const {
  if (str.size() > static_cast<size_t>(width)) {
    return str.substr(0, width - 3) + "...";
  }
  else {
    return str + std::string(width - str.size(), ' ');
  }
}