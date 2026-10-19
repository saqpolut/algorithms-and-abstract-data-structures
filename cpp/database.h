#ifndef DATABASE_H
#define DATABASE_H

#include "set.h"

#include <string>

struct NamedSet {
    std::string name;
    Set* value;
    NamedSet* next;
};

struct RawLine {
    std::string text;
    RawLine* next;
};

struct Database {
    NamedSet* sets;
    RawLine* raw_lines;
};

void db_init(Database* db);
bool db_load(Database* db, const std::string& filename);
bool db_save(const Database* db, const std::string& filename);
void db_destroy(Database* db);

Set* db_find_set(Database* db, const std::string& name);
Set* db_get_or_create_set(Database* db, const std::string& name);

//замена множества
bool db_replace_set(Database* db, const std::string& name, Set* replacement);


#endif
