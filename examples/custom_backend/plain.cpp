
#include <cronch/deserialize.hpp>
#include <cronch/concepts.hpp> 
#include <cronch/metatypes.hpp> // Provides meta::nameof<int>() support i.e serializable<int> is satisfied

#include <boost/lexical_cast.hpp>

#include <iostream>


class plain_backend {
public:
    using document_type = std::string;

    explicit plain_backend(std::string doc) : document_{std::move(doc)} {}

    static void serialize_to(document_type& doc, const auto& v) {
        doc += boost::lexical_cast<std::string>(v);
    }

    template<typename V>
    void deserialize_to(V& v) const {
        v = boost::lexical_cast<std::decay_t<V>>(document_);
    }

    static auto to_string(const document_type& doc) -> std::string {
        return doc;
    }
private:
    std::string document_;


};

static_assert(cronch::concepts::backend<plain_backend>, "must be a backend");

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Invalid number of arguments, requires 1\n";
        return EXIT_FAILURE;
    }

    const auto v = cronch::deserialize<int>(plain_backend{argv[1]});

    std::cout << "You passed an int: " << v << '\n';
}

