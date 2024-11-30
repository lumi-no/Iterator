#include <iostream>
#include <vector>
#include <string>
#include <memory>

template <typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual T Current() const = 0;
    virtual bool Next() = 0;
    virtual bool HasMore() const = 0;
};

template <typename T>
class IterableCollection {
public:
    virtual ~IterableCollection() = default;
    virtual std::unique_ptr<Iterator<T>> CreateIterator() const = 0;
};


template <typename T>
class VectorIterator : public Iterator<T> {
private:
    const std::vector<T>& collection_;
    size_t position_;

public:
    explicit VectorIterator(const std::vector<T>& collection)
        : collection_(collection), position_(0) {}

    T Current() const override {
        return collection_[position_];
    }

    bool Next() override {
        if (HasMore()) {
            ++position_;
            return true;
        }
        return false;
    }

    bool HasMore() const override {
        return position_ < collection_.size();
    }
};

template <typename T>
class VectorCollection : public IterableCollection<T> {
private:
    std::vector<T> items_;

public:
    void Add(const T& item) {
        items_.push_back(item);
    }

    std::unique_ptr<Iterator<T>> CreateIterator() const override {
        return std::make_unique<VectorIterator<T>>(items_);
    }
};


void ClientCode(const IterableCollection<std::string>& collection) {
    auto iterator = collection.CreateIterator();
    while (iterator->HasMore()) {
        std::cout << iterator->Current() << std::endl;
        iterator->Next();
    }
}

int main() {
    VectorCollection<std::string> collection;
    collection.Add("Елемент 1");
    collection.Add("Елемент 2");
    collection.Add("Елемент 3");

    std::cout << "Обхід елементів колекції:\n";
    ClientCode(collection);

    return 0;
}
