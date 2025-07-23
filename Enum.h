#pragma once

#include "Array.h"
#include "Comparable.h"
#include "Object.h"
#include <java/io/Serializable.h>

class Enum : public virtual Object, public virtual Comparable, public virtual java::io::Serializable {
    struct enum_info* self_data{nullptr};

    std::type_index get_type_index_checked();

protected:
    std::shared_ptr<Object> clone() final;

public:
    ~Enum() override = default;

    Enum(const shared<String>& name, int ordinal);

    /**
     * @brief One of the default enum constructors for subclasses, this one automatically creates zero indexed
     * enums based on type.
     * @param name the declaration name for this enum, used for some uses.
     */
    explicit Enum(const std::string& name);

    void lateinit() override;

    int compareTo(shared<Object> o) override;

    bool equals(Object *obj) final;

    DEFINE_SHARED_EQUALS

    int hashCode() final;

    shared<String> name() const;

    int ordinal();

    shared<String> toString() override;

    static shared<Object> valueOf(std::type_index enumClass, shared<String> name);

    static shared<Array<>> values(std::type_index enumClass);
};
