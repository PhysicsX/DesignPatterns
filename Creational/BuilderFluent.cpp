#include <iostream>
#include <sstream>

// fluent builder pattern example

// forwared decleration
class ProductBuilder;

class Product
{
        Product() = default; // disable constructor

        std::string mName;
        std::string mDateOfProd;
        std::string mVersion;
    
    public:
        friend class ProductBuilder; // ProductBuilder can access to private members

        static ProductBuilder build();
    
        std::ostream& print(std::ostream &out) const 
        {
            out << mName <<" : "<< mDateOfProd<<" : "<<mVersion;
            return out;
        }
};


class ProductBuilder
{
    Product mProduct;

    public:

    ProductBuilder& name(const std::string& name)
    {
        mProduct.mName = name;
        return *this;
    }

    ProductBuilder& date(const std::string& date)
    {
        mProduct.mDateOfProd = date;
        return *this;
    }

    ProductBuilder& version(const std::string& version)
    {
        mProduct.mVersion = version;
        return *this;
    }

    operator Product&&() {
        return std::move(mProduct);
    }

};

ProductBuilder Product::build()
{
    return ProductBuilder();
}

std::ostream& operator <<(std::ostream &out, const Product& product) {

return product.print(out);

}

int main() {

    Product pr = Product::build().name("dummy")
                                 .date("11.11.11")
                                 .version("0.0.1");

    std::cout<<pr<<std::endl;
}