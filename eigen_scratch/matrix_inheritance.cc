// Goal: See how easy it is to extend a Matrix's constructor
// Purpose: See if we can permit the semantics in `cpp_quick/composition_ctor.cc`
#include "cpp_quick/name_trait.h"

#include <string>
#include <iostream>

#include <Eigen/Dense>

using std::cout;
using std::endl;
using std::string;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;

// Define toy inheritance

class MyMat : public MatrixXd {
public:
    // Specialize
    // NOTE: Must use greedy match to combat Eigen's template ctor
    // Can't use rvalue? If I specify T&&, then it will resort to Matrix(const T&)
    template<typename T, typename Cond =
        typename std::enable_if<std::is_convertible<T, string>::value>::type>
    MyMat(const T& name)
        : MatrixXd() {
        string s(name);
        cout << "string: " << name << endl;

        MatrixXd& value = *this;
        value.resize(s.size(), 1);
        for (int i = 0; i < rows(); ++i) {
            value(i) = s[i];
        }
    }

    using MatrixXd::MatrixXd;
};

int main() {
    MyMat x("bob");

    cout << x.transpose() << endl;
    MatrixXd y = x / 2;
    cout << y.transpose() << endl;

    return 0;
}