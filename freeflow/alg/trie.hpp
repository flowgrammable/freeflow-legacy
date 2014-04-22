#ifndef FREEFLOW_TRIE_HPP
#define FREEFLOW_TRIE_HPP

namespace freeflow {

struct Key
{
  uint8_t value;
  uint8_t mask;
}

template<std::size_t byte>
  class Trie
  {

    Trie();
    ~Trie();

    void insert(const &key[T]);
    void remove();
    bool lookup();
    
  };

}

#include "trie.ipp"

#endif
