#ifndef EDIT_DISTANCE_OSA_HPP
#define EDIT_DISTANCE_OSA_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>


enum CppEditopName {
    INSERT,
    DELETE,
    REPLACE,
    SWAP
};

struct CppEditop {
    CppEditopName name;
    int src_idx;
    int dst_idx;
    double cost;
    std::string output_string;
    
    CppEditop() : name(INSERT), src_idx(0), dst_idx(0), cost(0.0), output_string("") {}
    CppEditop(CppEditopName n, int si, int di, double c, const std::string& os)
        : name(n), src_idx(si), dst_idx(di), cost(c), output_string(os) {}
};


std::vector<std::vector<double>> compute_dp_table(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
);


double cpp_compute_distance(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
);


std::vector<std::vector<CppEditop>> backtrack_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::vector<std::vector<double>>& dp, 
    int i, 
    int j, 
    std::vector<CppEditop>& current_path,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
);


std::vector<std::vector<CppEditop>> cpp_compute_all_paths(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
);


void cpp_print_all_paths(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
);


std::string editop_name_to_string(CppEditopName name);
std::ostream& operator<<(std::ostream& os, const CppEditop& op);

#endif // EDIT_DISTANCE_OSA_HPP