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
 * File:   RoutingTable.h
 * Author: Peter G. Jensen <root@petergjoel.dk>
 *
 * Created on July 2, 2019, 3:29 PM
 */

#include <rapidxml.hpp>
#include <string>
#include <vector>

#include <ptrie_map.h>

#include "Router.h"

#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

class Router;
class Interface;

class RoutingTable {
public:
    RoutingTable(const RoutingTable& orig) = default;
    virtual ~RoutingTable() = default;
    RoutingTable(RoutingTable&&) = default;
    static RoutingTable parse(rapidxml::xml_node<char>* node, ptrie::map<std::pair<std::string,std::string>>& indirect, Router* parent);

    RoutingTable& operator=(const RoutingTable&) = default;
    RoutingTable& operator=(RoutingTable&&) = default;
    bool empty() const { return _entries.empty(); }
    bool overlaps(const RoutingTable& other, Router& parent) const;
private:
    RoutingTable();

    enum type_t {DISCARD, RECIEVE, ROUTE, MPLS};
    enum op_t {PUSH, POP, SWAP};
    struct action_t {
        op_t _op = POP;
        int64_t _label = std::numeric_limits<int64_t>::min();
        std::ostream& operator<<(std::ostream& s) const;
    };
    struct forward_t {
        type_t _type = MPLS;
        std::vector<action_t> _ops;
        Interface* _via = nullptr;
        size_t _weight = 0;
    };
    struct entry_t {
        int64_t _top_label = std::numeric_limits<int64_t>::min();
        bool _decreasing = false;
        std::vector<forward_t> _rules;
        bool operator==(const entry_t& other) const
        {
            return _decreasing == other._decreasing && _top_label == other._top_label;
        }
        
        bool operator<(const entry_t& other) const
        {
            if(other._decreasing == _decreasing)
                return _top_label < other._top_label;
            return _decreasing < other._decreasing;
        }
    };
    
    std::string _name;
    std::vector<entry_t> _entries;
};

#endif /* ROUTINGTABLE_H */

