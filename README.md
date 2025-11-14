# C++ Database System

A relational database management system (RDBMS) built from scratch in C++ for educational purposes. This project implements the core components of a SQL database engine, following the architecture of production systems like PostgreSQL and MySQL.

---

## 🎯 Project Overview

This database system currently implements the **front-end** and **middle-end** of a database engine:

### ✅ Implemented Components

1. **Lexer** - Tokenizes SQL strings into keywords, identifiers, operators, and literals
2. **Parser** - Builds Abstract Syntax Trees (AST) from tokens using recursive descent parsing
3. **Catalog** - Stores and manages database schema (tables, columns, data types)
4. **Binder** - Performs semantic analysis, name resolution, and type checking

### Architecture Flow

```
SQL Query String
      ↓
   LEXER (Tokenization)
      ↓
   Tokens [SELECT, name, FROM, users, ...]
      ↓
   PARSER (Syntax Analysis)
      ↓
   Parser AST (Unresolved)
      ↓
   BINDER (Semantic Analysis)
      ↓
   Bound AST (Resolved + Typed)
```

---

## 🚀 Features

### Lexer
- Tokenizes SQL keywords (SELECT, FROM, WHERE, INSERT, etc.)
- Recognizes identifiers, numbers, strings, and operators
- Handles single-line and multi-line SQL
- Case-insensitive keyword parsing

### Parser
- **Supported SQL Statements:**
  - `SELECT columns FROM table WHERE condition`
  - `INSERT INTO table (columns) VALUES (values)`
  - `INSERT INTO table VALUES (values)`
- **Expression Support:**
  - Binary operators: `=`, `!=`, `<`, `>`, `<=`, `>=`
  - Logical operators: `AND`, `OR`
  - Arithmetic operators: `+`, `-`, `*`, `/`
  - Parenthesized expressions
- **Operator Precedence:**
  - Logical: `OR` < `AND`
  - Comparison: `=`, `!=`, `<`, `>`, etc.

### Catalog
- Table metadata storage (name, ID, columns)
- Column metadata (name, type, position, nullable, max length)
- Supported data types: `INTEGER`, `FLOAT`, `VARCHAR`, `BOOLEAN`, `DATE`
- Case-insensitive table/column lookups

### Binder
- **Name Resolution:**
  - Verifies tables exist in the catalog
  - Verifies columns exist in their respective tables
  - Resolves `SELECT *` to all table columns
- **Type Checking:**
  - Infers types for literals (integers, floats, strings)
  - Validates operator compatibility (e.g., can't compare INTEGER with VARCHAR)
  - Ensures WHERE clauses return BOOLEAN
  - Checks INSERT value types match column types
- **Type Inference:**
  - Numeric type promotion (INTEGER + FLOAT → FLOAT)
  - Comparison result types (any comparison → BOOLEAN)
  - Logical operator types (AND/OR require BOOLEAN operands)

---

## 📁 Project Structure

```
database_project/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   ├── parser/
│   │   ├── lexer.cpp
│   │   ├── parser.cpp
│   │   └── ast.cpp
│   └── binder/
│       ├── types.cpp
│       ├── catalog.cpp
│       ├── bound_ast.cpp
│       └── binder.cpp
├── include/
│   ├── parser/
│   │   ├── lexer.h
│   │   ├── parser.h
│   │   ├── token.h
│   │   └── ast.h
│   └── binder/
│       ├── types.h
│       ├── catalog.h
│       ├── bound_ast.h
│       └── binder.h
├── tests/
│   ├── test_main.cpp
│   ├── lexer_test.cpp
│   ├── parser_test.cpp
│   └── binder_test.cpp
└── build/
```

---

## 🛠️ Building the Project

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- Google Test (for running tests)

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd database_project

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make

# Run the demo
./database

# Run tests
./tests/run_tests
```

---

## 💻 Usage Example

```cpp
#include "parser/lexer.h"
#include "parser/parser.h"
#include "binder/catalog.h"
#include "binder/binder.h"

int main() {
    // Create catalog and define schema
    Catalog catalog;
    auto* users = catalog.createTable("users");
    users->addColumn(ColumnInfo("id", DataType::INTEGER, 0, false));
    users->addColumn(ColumnInfo("name", DataType::VARCHAR, 1, false, 50));
    users->addColumn(ColumnInfo("age", DataType::INTEGER, 2, true));
    
    // Parse SQL
    std::string sql = "SELECT name, age FROM users WHERE age > 18";
    Lexer lexer(sql);
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    auto ast = parser.parse();
    
    // Bind (semantic analysis)
    Binder binder(catalog);
    auto* select_stmt = dynamic_cast<SelectStatement*>(ast.get());
    auto bound_stmt = binder.bindSelect(*select_stmt);
    
    // Print bound AST
    std::cout << bound_stmt->toString() << std::endl;
    
    return 0;
}
```

**Output:**
```
BoundSELECT
  Columns:
    users.name [t0.c1:VARCHAR]
    users.age [t0.c2:INTEGER]
  FROM: users (table_id=0)
  WHERE: ((users.age [t0.c2:INTEGER] > 18:INTEGER):BOOLEAN)
```

---

## 🧪 Running Tests

The project includes comprehensive unit tests for all components:

```bash
cd build

# Run all tests
./tests/run_tests

# Run specific test suite
./tests/run_tests --gtest_filter=LexerTest.*
./tests/run_tests --gtest_filter=ParserTest.*
./tests/run_tests --gtest_filter=BinderTest.*

# Run with verbose output
./tests/run_tests --gtest_verbose
```

### Test Coverage

- **Lexer Tests**: Tokenization of keywords, identifiers, numbers, strings, operators
- **Parser Tests**: SQL parsing, operator precedence, error handling
- **Binder Tests**: Name resolution, type checking, error detection

---

## 📚 Supported SQL

### SELECT Statements
```sql
SELECT * FROM users;
SELECT name FROM users;
SELECT name, age, email FROM users;
SELECT name FROM users WHERE age > 18;
SELECT name FROM users WHERE age > 18 AND balance > 1000.0;
SELECT name FROM users WHERE age < 20 OR age > 60;
```

### INSERT Statements
```sql
INSERT INTO users VALUES (1, 'Alice', 25, 'alice@example.com');
INSERT INTO users (name, age) VALUES ('Bob', 30);
```

### Data Types
- `INTEGER` - 64-bit signed integer
- `FLOAT` - Double-precision floating point
- `VARCHAR(n)` - Variable-length string (max length n)
- `BOOLEAN` - True/false values
- `DATE` - Date values (not fully implemented)

### Operators
- **Comparison**: `=`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical**: `AND`, `OR`
- **Arithmetic**: `+`, `-`, `*`, `/`

---

## 🎓 Learning Resources

This project follows principles taught in:
- **CMU 15-445/645**: Database Systems (Andy Pavlo)
- **"Database System Concepts"** by Silberschatz, Korth, and Sudarshan
- **PostgreSQL** and **MySQL** source code architecture

### Key Design Patterns Used
- **Compiler Pipeline Pattern**: Multi-phase translation (Lexer → Parser → Binder)
- **Visitor Pattern**: AST traversal and transformation
- **Composite Pattern**: Tree-structured AST nodes
- **Builder Pattern**: Incremental AST construction

---

## 🔧 VS Code Configuration

### Recommended Extensions
- **C/C++** (Microsoft) - IntelliSense and debugging
- **CMake Tools** - CMake integration
- **clangd** - Fast code completion
- **C++ TestMate** - Test runner UI

### Generate `compile_commands.json`
```bash
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
ln -s build/compile_commands.json compile_commands.json
```

---

## 🐛 Error Handling

The system provides clear error messages for:

### Syntax Errors
```
✗ ERROR: Expected FROM keyword
✗ ERROR: Expected table name
✗ ERROR: Expected closing parenthesis
```

### Semantic Errors
```
✗ ERROR: Table 'nonexistent' does not exist
✗ ERROR: Column 'xyz' does not exist in table 'users'
✗ ERROR: Type mismatch: cannot compare INTEGER with VARCHAR
✗ ERROR: WHERE clause must evaluate to BOOLEAN, got INTEGER
```

---

## 🚧 Known Limitations

### Not Yet Implemented
- ❌ Query execution (no executor)
- ❌ Data storage (no disk manager or buffer pool)
- ❌ CREATE TABLE statements
- ❌ UPDATE and DELETE statements
- ❌ JOIN operations
- ❌ Aggregate functions (COUNT, SUM, AVG, etc.)
- ❌ GROUP BY and HAVING clauses
- ❌ ORDER BY clause
- ❌ Subqueries
- ❌ Indexes (B+ trees)
- ❌ Transactions (ACID properties)
- ❌ Concurrency control (locking)

---

## 📈 Future Roadmap

1. **Optimizer** - Cost-based query optimization and plan generation
2. **Executor** - Query execution engine with iterator model
3. **Storage Engine** - Disk manager, buffer pool, and page management
4. **Indexing** - B+ tree implementation for fast lookups
5. **Transactions** - ACID properties with write-ahead logging
6. **Advanced SQL** - JOINs, aggregation, subqueries, ORDER BY

---

## 🤝 Contributing

This is an educational project. Contributions, suggestions, and improvements are welcome!

### Areas for Improvement
- Additional SQL features (LIMIT, DISTINCT, etc.)
- Better error messages with line/column numbers
- Performance optimizations
- More comprehensive test coverage
- Documentation improvements

---

## 📝 License

This project is created for educational purposes. Feel free to use and modify for learning.

---

## 👨‍💻 Author

Created as a learning project to understand database internals by building a database from scratch in C++.

---

## 🙏 Acknowledgments

- Carnegie Mellon University's Database Systems course (15-445/645)
- PostgreSQL and MySQL open-source communities
- "Database System Concepts" textbook authors
- Andy Pavlo's database systems lectures

---

## 📞 Support

For questions, issues, or suggestions:
- Open an issue on GitHub
- Check the test files for usage examples
- Review the CMU 15-445 course materials

---

**Current Status**: ✅ Lexer, Parser, and Binder fully implemented and tested.

**Next Milestone**: Implement Query Executor for actual query execution.
