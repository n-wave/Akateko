/*
  ==============================================================================

    Akateko.cpp
    Created: 1 May 2018 12:56:31pm
    Author:  mario

  ==============================================================================
*/

#include "Akateko.h"

namespace akateko {
    bool nameSortAscending(PresetRow const &lhs, PresetRow const &rhs){
        //Ignore case Comparison
        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? false : true;
    }

    bool nameSortDescending(PresetRow const &lhs, PresetRow const &rhs){
        //Ignore case Comparison
        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? true : false;
    }


    bool categorySortAscending(PresetRow const &lhs, PresetRow const &rhs){
        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? false : true;
        }

        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? false : true;
        }

        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? false : true;
    }

    bool categorySortDescending(const PresetRow &lhs, const PresetRow &rhs){
        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? true : false;
        }

        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? true : false;
        }

        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? true : false;
    }

    bool authorSortAscending(PresetRow const &lhs, PresetRow const &rhs){
        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? false : true;
        }

        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? false : true;
        }

        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? false : true;
    }

    bool authorSortDescending(const PresetRow &lhs, const PresetRow &rhs){
        if(!lhs.author.equalsIgnoreCase(rhs.author)){
            return (lhs.author.compareIgnoreCase(rhs.author) == 1) ? true : false;
        }

        if(!lhs.category.equalsIgnoreCase(rhs.category)){
            return (lhs.category.compareIgnoreCase(rhs.category) == 1) ? true : false;
        }
        return (lhs.name.compareIgnoreCase(rhs.name) == 1) ? true : false;
    }
}
