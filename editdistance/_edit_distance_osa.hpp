#ifndef EDIT_DISTANCE_OSA_HPP
#define EDIT_DISTANCE_OSA_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>


enum EditopName {
    INSERT,
    DELETE,
    REPLACE,
    TRANSPOSE
};

struct Editop {
    EditopName name;
    int src_idx;
    int dst_idx;
    double cost;
    std::string output_string;
    
    Editop() : name(INSERT), src_idx(0), dst_idx(0), cost(0.0), output_string("") {}
    Editop(EditopName n, int si, int di, double c, const std::string& os)
        : name(n), src_idx(si), dst_idx(di), cost(c), output_string(os) {}
};


std::vector<std::vector<double>> compute_dp_table(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
);


double cpp_compute_distance(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
);


std::vector<std::vector<Editop>> backtrack_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map,
    const std::vector<std::vector<double>>& dp, 
    int i, 
    int j, 
    std::vector<Editop>& current_path
);


std::vector<std::vector<Editop>> cpp_compute_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
);


void cpp_print_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
);


std::string editop_name_to_string(EditopName name);
std::ostream& operator<<(std::ostream& os, const Editop& op);

#endif // EDIT_DISTANCE_OSA_HPP