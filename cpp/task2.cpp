#include "database.h"

#include <iostream>
#include <sstream>
#include <string>

struct PrintContext {
    bool first;
};

bool print_set_value(const std::string& value, void* context) {
    PrintContext* data = static_cast<PrintContext*>(context);

    if (!data->first) {
        std::cout << ", ";
    }

    std::cout << value;
    data->first = false;
    return true;
}

void print_set(const Set* set) {
    std::cout << "-> {";

    PrintContext context{true};
    set_for_each(set, print_set_value, &context);

    std::cout << "}\n";
}

bool execute_basic_query(
    Database* db,
    const std::string& command,
    const std::string& set_name,
    const std::string& value,
    bool* modified
) {
    if (command == "SETADD") {
        Set* set = db_get_or_create_set(db, set_name);

        if (set_add(set, value)) {
            std::cout << "-> " << value << '\n';
            *modified = true;
        } else {
            std::cout << "-> ALREADY_EXISTS\n";
        }

        return true;
    }

    if (command == "SETDEL") {
        Set* set = db_find_set(db, set_name);

        if (set != nullptr && set_del(set, value)) {
            std::cout << "-> " << value << '\n';
            *modified = true;
        } else {
            std::cout << "-> NOT_FOUND\n";
        }

        return true;
    }

    if (command == "SET_AT") {
        Set* set = db_find_set(db, set_name);

        if (set != nullptr && set_at(set, value)) {
            std::cout << "-> TRUE\n";
        } else {
            std::cout << "-> FALSE\n";
        }

        return true;
    }

    return false;
}

//обработчик операця над множествами
bool execute_set_operation(
    Database* db,
    const std::string& command,
    const std::string& result_name,
    const std::string& first_name,
    const std::string& second_name,
    bool* modified
) {
    Set* first = db_find_set(db, first_name);
    Set* second = db_find_set(db, second_name);

    if (first == nullptr || second == nullptr) {
        std::cerr << "Error: source set not found\n";
        return false;
    }

    Set* result = nullptr;

    if (command == "SETUNION") {
        result = set_union_new(first, second);
    } else if (command == "SETINTERSECTION") {
        result = set_intersection_new(first, second);
    } else if (command == "SETDIFF") {
        result = set_difference_new(first, second);
    } else {
        return false;
    }

    if (result == nullptr) {
        std::cerr << "Error: cannot create result set\n";
        return false;
    }

    if (!db_replace_set(db, result_name, result)) {
        set_destroy(result);
        std::cerr << "Error: cannot save result set\n";
        return false;
    }

    *modified = true;
    print_set(result);
    return true;
}

int main(int argc, char* argv[]) {
    std::string filename;
    std::string query;

    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];

        if (argument == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (argument == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (filename.empty() || query.empty()) {
        std::cerr
            << "Usage: " << argv[0]
            << " --file <file.data> --query '<command>'\n";
        return 1;
    }

    Database db;
    db_init(&db);

    if (!db_load(&db, filename)) {
        std::cerr << "Error: cannot load database\n";
        db_destroy(&db);
        return 1;
    }

    std::istringstream input(query);
    std::string command;
    input >> command;

    bool modified = false;
    bool success = false;

    if (command == "SETUNION" ||
        command == "SETINTERSECTION" ||
        command == "SETDIFF") {
        std::string result_name;
        std::string first_name;
        std::string second_name;

        if (!(input >> result_name >> first_name >> second_name)) {
            std::cerr
                << "Usage: " << command
                << " <result_set> <setA> <setB>\n";
            db_destroy(&db);
            return 1;
        }

        success = execute_set_operation(
            &db,
            command,
            result_name,
            first_name,
            second_name,
            &modified
        );
    } else {
        std::string set_name;
        std::string value;

        if (!(input >> set_name >> value)) {
            std::cerr
                << "Usage: SETADD|SETDEL|SET_AT <set_name> <value>\n";
            db_destroy(&db);
            return 1;
        }

        success = execute_basic_query(
            &db,
            command,
            set_name,
            value,
            &modified
        );

        if (!success) {
            std::cerr << "Unknown command: " << command << '\n';
        }
    }

    if (success && modified && !db_save(&db, filename)) {
        std::cerr << "Error: cannot save database\n";
        db_destroy(&db);
        return 1;
    }

    db_destroy(&db);
    return success ? 0 : 1;
}
