#include "database.h"

#include <fstream>
#include <sstream>
#include <string>

namespace {

void raw_line_append(Database* db, const std::string& text) {
    RawLine* node = new RawLine{text, nullptr};

    if (db->raw_lines == nullptr) {
        db->raw_lines = node;
        return;
    }

    RawLine* current = db->raw_lines;
    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = node;
}

bool write_set_value(const std::string& value, void* context) {
    std::ostream* out = static_cast<std::ostream*>(context);
    *out << ' ' << value;
    return static_cast<bool>(*out);
}

} // namespace

void db_init(Database* db) {
    db->sets = nullptr;
    db->raw_lines = nullptr;
}

Set* db_find_set(Database* db, const std::string& name) {
    if (db == nullptr) {
        return nullptr;
    }

    NamedSet* current = db->sets;

    while (current != nullptr) {
        if (current->name == name) {
            return current->value;
        }

        current = current->next;
    }

    return nullptr;
}

Set* db_get_or_create_set(Database* db, const std::string& name) {
    Set* existing = db_find_set(db, name);

    if (existing != nullptr) {
        return existing;
    }

    NamedSet* node = new NamedSet;
    node->name = name;
    node->value = set_create();
    node->next = db->sets;
    db->sets = node;

    return node->value;
}

bool db_load(Database* db, const std::string& filename) {
    std::ifstream input(filename.c_str());

    // Если файла ещё нет, это новая.
    if (!input.is_open()) {
        return true;
    }

    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream row(line);
        std::string type;
        row >> type;

        if (type == "E") {
            std::string set_name;

            if (!(row >> set_name)) {
                raw_line_append(db, line);
                continue;
            }

            // Если множество встречается в нескольких строках,
            // все его элементы объединяются в одно множество.
            Set* set = db_get_or_create_set(db, set_name);

            std::string value;
            while (row >> value) {
                set_add(set, value);
            }
        } else {
            raw_line_append(db, line);
        }
    }

    return true;
}

bool db_save(const Database* db, const std::string& filename) {
    std::ofstream output(filename.c_str());

    if (!output.is_open()) {
        return false;
    }

    // Сохраняем неизвестные этой программе записи без изменений.
    for (RawLine* line = db->raw_lines;
         line != nullptr;
         line = line->next) {
        output << line->text << '\n';
    }

    for (NamedSet* item = db->sets;
         item != nullptr;
         item = item->next) {
        output << "E " << item->name;

        if (!set_for_each(item->value, write_set_value, &output)) {
            return false;
        }

        output << '\n';
    }

    return static_cast<bool>(output);
}

void db_destroy(Database* db) {
    if (db == nullptr) {
        return;
    }

    while (db->sets != nullptr) {
        NamedSet* next = db->sets->next;
        set_destroy(db->sets->value);
        delete db->sets;
        db->sets = next;
    }

    while (db->raw_lines != nullptr) {
        RawLine* next = db->raw_lines->next;
        delete db->raw_lines;
        db->raw_lines = next;
    }

}

bool db_replace_set(Database* db, const std::string& name, Set* replacement) {
    if (db == nullptr || replacement == nullptr) {
        return false;
    }

    NamedSet* current = db->sets;

    while (current != nullptr) {
        if (current->name == name) {
            set_destroy(current->value);
            current->value = replacement;
            return true;
        }

        current = current->next;
    }

    NamedSet* node = new NamedSet;
    node->name = name;
    node->value = replacement;
    node->next = db->sets;
    db->sets = node;

    return true;
}

