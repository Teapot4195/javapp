#include "Enum.h"

#include <iostream>
#include <stacktrace>

#include "String.h"

struct enum_info {
    shared<Enum> self;
    shared<String> name;
    int ordinal;
};

struct enum_descriptor {
    std::vector<enum_info> informations;
    int allocated = -1;
};

std::unordered_map<std::type_index, enum_descriptor> global_enum_table;

struct enum_lateinit : virtual internals::lateinit_frame {
    shared<String> name;
    int ordinal;

    enum_lateinit(shared<String> name, const int ordinal) : name(std::move(name)), ordinal(ordinal) {}

    ~enum_lateinit() override = default;
};

std::type_index Enum::get_type_index_checked() {
    auto index = std::type_index(typeid(*this));

    if (!global_enum_table.contains(index) || self_data == nullptr) {
        std::cerr << "[FATAL] It appears this enum variant was never registered to the global enum table." << std::endl;
        std::cerr << "[NOTE] The current type is \"" << typeid(*this).name() << "\"" << std::endl;
        std::cerr << "[NOTE] Although probably not useful, the stack trace is as follows" << std::endl;
        auto current_stacktrace = std::stacktrace::current();
        for (auto& entry : current_stacktrace)
            std::cerr << entry << std::endl;
        std::cerr << "[FATAL] This is an unanticipated case, the program will now die" << std::endl;
        std::terminate();
    }

    return index;
}

std::shared_ptr<Object> Enum::clone() {
    throw std::runtime_error("THROW CLONENOTSUPPORTEDEXCPETION!!!!");
}

Enum::Enum(const shared<String>& name, int ordinal) {
    _G_stack.put<enum_lateinit>(name, ordinal);
}

Enum::Enum(const std::string &name) {
    _G_stack.put<enum_lateinit>(alloc<String>(name), std::numeric_limits<int>::min());
}

bool check_get(const enum_descriptor& table, const shared<String> &target) {
    for (const enum_info& entry : table.informations) {
        if (entry.name->equals(target.get())) {
            return true;
        }
    }

    return false;
}

void Enum::lateinit() {
    auto data = _G_stack.pop<enum_lateinit>();

    auto& table_entry = global_enum_table[std::type_index(typeid(*this))];

    if (check_get(table_entry, data->name)) {
        // multiple definitions
        std::cerr << "[FATAL] multiple definitions of enum type " << typeid(*this).name() << "::" << data->name->get_backing() << std::endl;
        std::cerr << "[NOTE] the current stack is as follows" << std::endl;
        auto current_stacktrace = std::stacktrace::current();
        for (auto& entry : current_stacktrace) {
            std::cerr << entry << std::endl;
        }
        std::cerr << "[FATAL] the program will now die, as the singleton guarantee for enums has been violated" << std::endl;
        std::terminate();
    }

    if (data->ordinal == std::numeric_limits<int>::min()) {
        data->ordinal = ++table_entry.allocated;
    }

    table_entry.informations.emplace_back(std::dynamic_pointer_cast<Enum>(shared_from_this()), data->name, data->ordinal);

    this->self_data = &table_entry.informations.back();

    Object::lateinit();
}

int Enum::compareTo(shared<Object> o) {
    const auto obj = std::dynamic_pointer_cast<Enum>(o);
    if (this->ordinal() < obj->ordinal())
        return -1;
    if (this->ordinal() > obj->ordinal())
        return 1;
    return 0;
}

bool Enum::equals(Object *obj) {
    get_type_index_checked();

    if (!obj->instanceof<Enum>())
        return false;

    auto other = dynamic_cast<Enum*>(obj);

    // enums are easy to compare, if they have the same object self_data, then they're the same variant, otherwise
    // they're not the same
    if (other->self_data == this->self_data)
        return true;
    return false;
}

int Enum::hashCode() {
    // the type index's hash and the ordinal should be enough to uniquely identify each enum variant.
    int hash = 17;

    hash = hash * 31 + this->get_type_index_checked().hash_code();
    hash = hash * 31 + this->self_data->ordinal;

    return hash;
}

shared<String> Enum::name() const {
    return self_data->name;
}

int Enum::ordinal() {
    return self_data->ordinal;
}

std::shared_ptr<String> Enum::toString() {
    return self_data->name;
}

shared<Object> Enum::valueOf(std::type_index enumClass, shared<String> name) {
    if (name == nullptr)
        throw std::runtime_error("THROW NULLPTREXCEPTION");

    if (!global_enum_table.contains(enumClass)) {
        std::cerr << "[FATAL] It appears this enum variant was never registered to the global enum table." << std::endl;
        std::cerr << "[NOTE] The current type is \"" << enumClass.name() << "\"" << std::endl;
        std::cerr << "[NOTE] Although probably not useful, the stack trace is as follows" << std::endl;
        auto current_stacktrace = std::stacktrace::current();
        for (auto& entry : current_stacktrace)
            std::cerr << entry << std::endl;
        std::cerr << "[FATAL] This is an unanticipated case, the program will now die" << std::endl;
        std::terminate();
    }

    auto entry = global_enum_table[enumClass];

    for (const auto& option : entry.informations) {
        if (option.name->equals(name)) {
            return option.self;
        }
    }

    throw std::runtime_error("THROW ILLEGALARGUMENTEXCEPTION");
}

shared<Array<>> Enum::values(std::type_index enumClass) {

    if (!global_enum_table.contains(enumClass)) {
        std::cerr << "[FATAL] It appears this enum variant was never registered to the global enum table." << std::endl;
        std::cerr << "[NOTE] The current type is \"" << enumClass.name() << "\"" << std::endl;
        std::cerr << "[NOTE] Although probably not useful, the stack trace is as follows" << std::endl;
        auto current_stacktrace = std::stacktrace::current();
        for (auto& entry : current_stacktrace)
            std::cerr << entry << std::endl;
        std::cerr << "[FATAL] This is an unanticipated case, the program will now die" << std::endl;
        std::terminate();
    }

    auto entry = global_enum_table[enumClass];

    std::vector<shared<Object>> result_vec;

    for (const auto& option : entry.informations)
        result_vec.push_back(option.self);

    return alloc<Array<>>(result_vec);
}
