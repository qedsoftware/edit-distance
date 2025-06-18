#include "_edit_distance_osa.hpp"
#include <algorithm>
#include <cmath>

std::vector<std::vector<double>> compute_dp_table(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
) {
    int len_a = a.length();
    int len_b = b.length();
    std::vector<std::vector<double>> dp(len_a + 1, std::vector<double>(len_b + 1, 0.0));
    
    for (int i = 0; i <= len_a; ++i) {
        dp[i][0] = i * cost_map.at(DELETE);
    }
    for (int j = 0; j <= len_b; ++j) {
        dp[0][j] = j * cost_map.at(INSERT);
    }
    
    for (int i = 1; i <= len_a; ++i) {
        for (int j = 1; j <= len_b; ++j) {
            double deletion = dp[i-1][j] + cost_map.at(DELETE);
            double insertion = dp[i][j-1] + cost_map.at(INSERT);
            double substitution_cost = (a[i-1] == b[j-1]) ? 0.0 : cost_map.at(REPLACE);
            double substitution = dp[i-1][j-1] + substitution_cost;
            
            dp[i][j] = std::min({deletion, insertion, substitution});
            
            if (i > 1 && j > 1 &&
                a[i-1] == b[j-2] && a[i-2] == b[j-1]) {
                dp[i][j] = std::min(dp[i][j],
                                    dp[i-2][j-2] + cost_map.at(TRANSPOSE));
            }
        }
    }
    
    return dp;
}


double cpp_compute_distance(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
) {
    auto dp = compute_dp_table(a, b, cost_map);
    return dp[a.length()][b.length()];
}

std::vector<std::vector<Editop>> backtrack_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map,
    const std::vector<std::vector<double>>& dp, 
    int i, 
    int j, 
    std::vector<Editop>& current_path
) {
    if (i == 0 && j == 0) {
        std::vector<Editop> reversed_path = current_path;
        std::reverse(reversed_path.begin(), reversed_path.end());
        return {reversed_path};
    }
    
    std::vector<std::vector<Editop>> all_paths;
    double current_cost = dp[i][j];
    const double tol = 1e-6;


    if (i > 0 && std::abs((dp[i-1][j] + cost_map.at(DELETE)) - current_cost) < tol) {
        Editop op(DELETE, i-1, i-1, cost_map.at(DELETE), std::string(1, a[i-1]));
        current_path.push_back(op);
        auto paths = backtrack_all_paths(a, b, cost_map, dp, i-1, j, current_path);
        all_paths.insert(all_paths.end(), paths.begin(), paths.end());
        current_path.pop_back();
    }
    
    if (j > 0 && std::abs((dp[i][j-1] + cost_map.at(INSERT)) - current_cost) < tol) {
        Editop op(INSERT, i, i, cost_map.at(INSERT), std::string(1, b[j-1]));
        current_path.push_back(op);
        auto paths = backtrack_all_paths(a, b, cost_map, dp, i, j-1, current_path);
        all_paths.insert(all_paths.end(), paths.begin(), paths.end());
        current_path.pop_back();
    }
    

    if (i > 0 && j > 0) {
        double sub_cost = (a[i-1] == b[j-1]) ? 0.0 : cost_map.at(REPLACE);
        if (std::abs((dp[i-1][j-1] + sub_cost) - current_cost) < tol) {
            std::string out_char = (sub_cost == 0.0) ? std::string(1, a[i-1]) : std::string(1, b[j-1]);
            Editop op(REPLACE, i-1, j-1, sub_cost, out_char);
            current_path.push_back(op);
            auto paths = backtrack_all_paths(a, b, cost_map, dp, i-1, j-1, current_path);
            all_paths.insert(all_paths.end(), paths.begin(), paths.end());
            current_path.pop_back();
        }
    }
    

    if (i > 1 && j > 1 &&
        a[i-1] == b[j-2] && a[i-2] == b[j-1] &&
        std::abs((dp[i-2][j-2] + cost_map.at(TRANSPOSE)) - current_cost) < tol) {
        std::string transpose_str = std::string(1, b[j-2]) + std::string(1, b[j-1]);
        Editop op(TRANSPOSE, i-2, j-2, cost_map.at(TRANSPOSE), transpose_str);
        current_path.push_back(op);
        auto paths = backtrack_all_paths(a, b, cost_map, dp, i-2, j-2, current_path);
        all_paths.insert(all_paths.end(), paths.begin(), paths.end());
        current_path.pop_back();
    }
    
    return all_paths;
}


std::vector<std::vector<Editop>> cpp_compute_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
) {
    auto dp = compute_dp_table(a, b, cost_map);
    std::vector<Editop> current_path;
    return backtrack_all_paths(a, b, cost_map, dp, a.length(), b.length(), current_path);
}


void cpp_print_all_paths(
    const std::string& a, 
    const std::string& b, 
    const std::map<EditopName, double>& cost_map
) {
    auto paths = cpp_compute_all_paths(a, b, cost_map);
    double distance = cpp_compute_distance(a, b, cost_map);
    
    std::cout << "OSA Distance from '" << a << "' to '" << b << "': " << distance << std::endl;
    std::cout << "Number of optimal edit sequences: " << paths.size() << std::endl;
    std::cout << std::endl;
    
    for (size_t i = 0; i < paths.size(); ++i) {
        std::cout << "Path " << (i + 1) << ":" << std::endl;
        for (const auto& op : paths[i]) {
            std::cout << "  " << op << std::endl;
        }
        std::cout << std::endl;
    }
}

std::string editop_name_to_string(EditopName name) {
    switch (name) {
        case INSERT: return "INSERT";
        case DELETE: return "DELETE";
        case REPLACE: return "REPLACE";
        case TRANSPOSE: return "TRANSPOSE";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const Editop& op) {
    os << "Editop(name=" << editop_name_to_string(op.name)
       << ", src_idx=" << op.src_idx
       << ", dst_idx=" << op.dst_idx
       << ", cost=" << op.cost
       << ", output_string='" << op.output_string << "')";
    return os;
}
