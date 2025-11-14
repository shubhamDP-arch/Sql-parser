
#include "binder/types.h"
#include <cstddef>
#include <string>
#include <vector>
struct ColumnInfo{
  std::string name;
  DataType type;
  size_t column_id;
  bool nullable;
  size_t max_length;

  ColumnInfo(std::string n, DataType t, size_t id, bool nullable = true, size_t len = 0);
};  

class TableInfo{
  public:
    std::string name;
    size_t table_id;
    std::vector<ColumnInfo>columns;

    TableInfo(std::string n , size_t id);

    void addColumn(const ColumnInfo& col);

    const ColumnInfo* getColumn(const std::string& col_name)const;

    
};