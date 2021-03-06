#ifndef EC_H
#define EC_H

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;
class Entity_manager;

using component_id = std::size_t;
using group        = std::size_t;

inline component_id get_new_component_type_id() {
    static component_id last_id = 0u;
    return last_id++;
}

template <typename T>
inline component_id get_component_type_id() noexcept {
    static component_id type_id = get_new_component_type_id();
    return type_id;
}

constexpr std::size_t max_components = 32;
constexpr std::size_t max_groups     = 32;

using component_bitset = std::bitset<max_components>;
using group_bitset     = std::bitset<max_groups>;

using component_array = std::array<Component*, max_components>;

// Component class

class Component {

   public:
    Entity*      parent_entity;
    virtual void init();
    virtual void quick_update();
    virtual void update();
    virtual void late_update();
    virtual void draw();

    virtual ~Component();
};

// Entity class

class Entity {

   private:
    bool                                    active;
    std::vector<std::unique_ptr<Component>> compopnents_vector;
    component_array                         components_array;
    component_bitset                        components_bitset;
    group_bitset                            groups_bitset;

   public:
    // TODO: resolve publicity ot manager
    Entity_manager& manager;
    Entity(Entity_manager& _manager);
    void quick_update();
    void update();
    void late_update();
    void draw();
    bool is_active();
    void set_inactive();

    template <typename T>
    bool has_component() const {
        return components_bitset[get_component_type_id<T>()];
    }

    template <typename T, typename... T_args>
    T& add_component(T_args&&... args) {

        T* c(new T(std::forward<T_args>(args)...));

        c->parent_entity = this;

        std::unique_ptr<Component> temp_ptr{c};

        compopnents_vector.emplace_back(std::move(temp_ptr));

        components_array[get_component_type_id<T>()]  = c;
        components_bitset[get_component_type_id<T>()] = true;

        c->init();

        return *c;
    }

    template <typename T>
    T& get_component() const {
        auto ptr(components_array[get_component_type_id<T>()]);
        return *static_cast<T*>(ptr);
    }

    bool has_group(group group_bitset);

    void add_group(group new_group);
    void remove_group(group to_rmv);
};

// Entity_manger class

class Entity_manager {

   private:
    std::vector<std::unique_ptr<Entity>>         entities_vector;
    std::array<std::vector<Entity*>, max_groups> grouped_entities;

   public:
    void                  quick_update();
    void                  update();
    void                  late_update();
    void                  draw();
    void                  remove_inactive();
    void                  add_to_group(Entity* entity, group grp);
    std::vector<Entity*>& get_group(group grp);
    Entity&               add_entity();
    void                  clear();
};

#endif