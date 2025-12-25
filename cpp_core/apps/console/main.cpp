#include "matrix/matrix.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace matrixlib;

static std::string out_path_for(const std::string& in_path) {
    // write out.txt next to input file
    auto pos = in_path.find_last_of("/\\");
    if (pos == std::string::npos) return "out.txt";
    return in_path.substr(0, pos + 1) + "out.txt";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: matrix_console <input_file>\n";
        return 1;
    }
    std::string path = argv[1];
    std::ifstream fin(path);
    if (!fin) {
        std::cerr << "failed to open file: " << path << "\n";
        return 1;
    }

    try {
        Matrix A = read_matrix_from_stream(fin);

        // allow optional whitespace between matrices
        // if second matrix not present, create empty
        Matrix B;
        // peek if there is another matrix header
        // skip whitespace
        fin >> std::ws;
        if (fin.peek() != EOF) {
            B = read_matrix_from_stream(fin);
        }

        std::cout << "loaded matrix A: " << A.rows() << "x" << A.cols() << "\n";
        if (!B.empty()) std::cout << "loaded matrix B: " << B.rows() << "x" << B.cols() << "\n";

        std::cout << "\nchoose operation:\n"
                  << "1) add (A+B)\n"
                  << "2) sub (A-B)\n"
                  << "3) scalar (A*k)\n"
                  << "4) mul (A*B)\n"
                  << "5) transpose (A^T)\n"
                  << "6) trace (tr(A))\n"
                  << "7) det (det(A), n<=10)\n"
                  << "8) solve (A x = b), b is matrix B (n x 1)\n"
                  << "your choice: ";

        int op = 0;
        std::cin >> op;

        std::ofstream fout(out_path_for(path));
        if (!fout) throw std::runtime_error("failed to open out.txt for writing");

        if (op == 1) {
            if (B.empty()) throw std::runtime_error("operation requires matrix B");
            Matrix R = add(A, B);
            write_matrix_to_stream(std::cout, R);
            write_matrix_to_stream(fout, R);
        } else if (op == 2) {
            if (B.empty()) throw std::runtime_error("operation requires matrix B");
            Matrix R = sub(A, B);
            write_matrix_to_stream(std::cout, R);
            write_matrix_to_stream(fout, R);
        } else if (op == 3) {
            float k;
            std::cout << "enter scalar k: ";
            std::cin >> k;
            Matrix R = scalar_mul(A, k);
            write_matrix_to_stream(std::cout, R);
            write_matrix_to_stream(fout, R);
        } else if (op == 4) {
            if (B.empty()) throw std::runtime_error("operation requires matrix B");
            Matrix R = multiply(A, B);
            write_matrix_to_stream(std::cout, R);
            write_matrix_to_stream(fout, R);
        } else if (op == 5) {
            Matrix R = transpose(A);
            write_matrix_to_stream(std::cout, R);
            write_matrix_to_stream(fout, R);
        } else if (op == 6) {
            double t = trace(A);
            std::cout << "trace(A) = " << t << "\n";
            fout << "trace(A) = " << t << "\n";
        } else if (op == 7) {
            double d = det_small(A);
            std::cout << "det(A) = " << d << "\n";
            fout << "det(A) = " << d << "\n";
        } else if (op == 8) {
            if (B.empty()) throw std::runtime_error("solve requires matrix B as b (n x 1)");
            LUResult lu = lu_decompose(A);
            Matrix x = lu_solve(lu, B);
            std::cout << "solution x:\n";
            write_matrix_to_stream(std::cout, x);
            write_matrix_to_stream(fout, x);
        } else {
            throw std::runtime_error("unknown operation");
        }

        std::cout << "\nwritten result to: " << out_path_for(path) << "\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 2;
    }
}
