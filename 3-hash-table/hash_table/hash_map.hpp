//
// Created by Burenin Artem on 09.10.2023.
//

#include <vector>
#include <functional>


namespace burenotti::hash_map {

    namespace details {
        template<class K, class V, class Hash = std::hash<K>>
        struct Node {
        public:

            Node() {
                _used = false;
            }

            explicit Node(K key) : Node(key, V{}, Hash{}(key)) {
            }

            Node(K key, V value, size_t hash) : _key(std::move(key)), _hash(hash), _value(std::move(value)),
                                                _used(true) {
            }

            Node(const Node& source) {
                _key = source._key;
                _value = source._value;
                _hash = source._hash;
                if (source._overflow) {
                    _overflow = new Node(*source._overflow);
                }
            }

            Node(Node&& source) noexcept {
                std::swap(*this, source);
            }

            Node& operator=(const Node& right) {
                if (this == &right) {
                    return *this;
                }

                _key = right._key;
                _value = right._value;
                _hash = right._hash;
                if (right._overflow) {
                    _overflow = new Node(*right._overflow);
                }

                return *this;
            }

            Node& operator=(Node&& right) noexcept {
                std::swap(*this, right);
                return *this;
            }

            ~Node() {
                delete _overflow;
            }


        public:
            // node, created
            std::tuple<Node*, bool> find_or_create(const K& key) {
                if (!_used) {
                    _used = true;
                    _key = key;
                    _hash = Hash{}(key);
                    return make_tuple(this, true);
                }

                if (_key == key) {
                    return make_tuple(this, false);
                }
                if (_overflow == nullptr) {
                    _overflow = new Node<K, V, Hash>{key};
                    return make_tuple(_overflow, true);
                }
                return _overflow->find_or_create(key);
            }

            Node* find(const K& key) {
                if (_used && _key == key) {
                    return this;
                }
                if (_overflow) {
                    return this->_overflow->find(key);
                }
                return nullptr;
            }

        public:
            bool _used = false;
            K _key;
            V _value;
            size_t _hash = 0;
            Node* _overflow = nullptr;
        };

    }

    template<class K, class V, class Hash = std::hash<K>>
    class HashMap {
    private:
        constexpr static size_t DEFAULT_RESERVE_SIZE = 16;
        constexpr static size_t GROW_FACTOR = 2;
        constexpr static size_t LOAD_FACTOR = 2;

    public:

        HashMap() : HashMap(DEFAULT_RESERVE_SIZE) {
        }

        explicit HashMap(size_t capacity) : _size(0), _hasher(Hash{}) {
            _data.resize(capacity);
        }

        HashMap(const HashMap& source) = default;

        HashMap(HashMap&& source) noexcept = default;

        HashMap& operator=(const HashMap& right) = default;

        HashMap& operator=(HashMap&& right) noexcept = default;

        ~HashMap() = default;

    public:

        [[nodiscard]] size_t size() const {
            return _size;
        }

        [[nodiscard]] size_t capacity() const {
            return _data.size();
        }

    public:

        V& operator[](const K& key) {
            if (_data.size() < LOAD_FACTOR * (_size + 1)) {
                grow_up();
            }
            auto [node, created] = find_or_create(_data, key);
            if (created) {
                _size++;
            }
            return node->_value;
        }


        bool contains(const K& key) {
            return find(key);
        }

        void erase(const K& key) {
            auto* node = find(key);
            if (node) {
                node->_used = false;
                _size--;
            }
        }

        void visit_all(std::function<bool(const K&, V&)> visitor) {
            for (auto& node: _data) {
                if (node._used) {
                    bool ok = visitor(node._key, node._value);
                    if (!ok) {
                        return;
                    }
                }
                auto* overflow = node._overflow;
                while (overflow) {
                    if (overflow->_used) {
                        bool ok = visitor(node._key, node._value);
                        if (!ok) {
                            return;
                        }
                    }
                    overflow = overflow->_overflow;
                }
            }
        }


    private:

        void grow_up() {
            grow_up((!_data.empty()) ? _data.size() * GROW_FACTOR : DEFAULT_RESERVE_SIZE);
        }

        void grow_up(size_t new_size) {
            std::vector<details::Node<K, V, Hash>> new_data(new_size);
            std::vector<details::Node<K, V, Hash>*> stack;
            for (auto& node: _data) {
                if (node._used) {
                    stack.push_back(&node);
                }
                auto* overflow = node._overflow;
                while (overflow != nullptr) {
                    if (overflow->_used) {
                        stack.push_back(overflow);
                    }

                    overflow = overflow->_overflow;
                }
            }

            while (!stack.empty()) {
                auto* node = stack.back();
                stack.pop_back();
                auto [node_dest, _] = find_or_create(new_data, std::move(node->_key));
                node_dest->_value = std::move(node->_value);
            }

            _data = std::move(new_data);
        }

        std::tuple<details::Node<K, V, Hash>*, bool>
        find_or_create(std::vector<details::Node<K, V, Hash>>& data, const K& key) {
            auto hash = _hasher(key);
            auto index = hash % data.size();
            return data[index].find_or_create(key);
        }

        details::Node<K, V, Hash>* find(const K& key) {
            auto hash = _hasher(key);
            auto index = hash % _data.size();
            auto* node = _data[index].find(key);
            return node;
        }

    private:
        Hash _hasher;
        size_t _size = 0;
        std::vector<details::Node<K, V, Hash>> _data;
    };

}
