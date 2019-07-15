/* 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   Router.h
 * Author: Peter G. Jensen <root@petergjoel.dk>
 *
 * Created on June 24, 2019, 11:22 AM
 */

#ifndef ROUTER_H
#define ROUTER_H

#include <limits>
#include <vector>
#include <string>
#include <memory>

#include <ptrie_map.h>

#include "RoutingTable.h"
class Interface {
public:
    Interface(size_t id, Router* target, bool virt = false) : _id(id), _target(target), _virtual(virt) {};
    Router* target() const { return _target; }
    size_t id() const { return _id; }
    int routing_table() const { return _table; }
private:
    size_t _id = std::numeric_limits<size_t>::max();
    Router* _target = nullptr;
    size_t _table = -1; // lets just chose the ingoing RT nondet for now.
    bool _virtual = false;
};

class Router {
public:
    Router(size_t id);
    Router(const Router& orig) = default;
    virtual ~Router() = default;
    size_t index() const { return _index; }
    void add_name(const std::string& name);
    const std::string& name() const;
    bool has_config() const { return _has_config; }
    bool parse_adjacency(std::istream& data, std::vector<std::unique_ptr<Router>>& routers, ptrie::map<Router*>& mapping);
    bool parse_routing(std::istream& data, std::istream& indirect);
    void print_dot(std::ostream& out);
    Interface* get_interface(std::string iface, Router* expected = nullptr);
    Interface* interface_no(size_t i) const {
        return _interfaces[i].get();
    }
    std::unique_ptr<char[]> interface_name(size_t i);
    
private:
    size_t _index = std::numeric_limits<size_t>::max();
    std::vector<std::string> _names;
    std::vector<std::unique_ptr<Interface>> _interfaces;
    std::vector<RoutingTable> _tables;
    ptrie::map<Interface*> _interface_map;
    size_t _inamelength = 0; // for printing
    bool _has_config = false;
    bool _inferred = false;
};

#endif /* ROUTER_H */

