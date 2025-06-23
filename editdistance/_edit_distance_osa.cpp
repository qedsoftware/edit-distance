#include "_edit_distance_osa.hpp"
#include <algorithm>
#include <cmath>

std::vector<std::vector<double>> compute_dp_table(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
) {
    int len_a = a.length();
    int len_b = b.length();
    std::vector<std::vector<double>> dp(len_a + 1, std::vector<double>(len_b + 1, 0.0));
    
    for (int i = 0; i <= len_a; ++i) {
        dp[i][0] = i * delete_weight;
    }
    for (int j = 0; j <= len_b; ++j) {
        dp[0][j] = j * insert_weight;
    }
    
    for (int i = 1; i <= len_a; ++i) {
        for (int j = 1; j <= len_b; ++j) {
            if (a[i-1] == b[j-1]) {
                dp[i][j] = dp[i-1][j-1]; // match, no cost
                continue; // skip swap and other ops, match is optimal
            }
            double deletion = dp[i-1][j] + delete_weight;
            double insertion = dp[i][j-1] + insert_weight;
            double substitution = dp[i-1][j-1] + replace_weight;
            dp[i][j] = std::min({deletion, insertion, substitution});
            if (i > 1 && j > 1 &&
                a[i-1] == b[j-2] && a[i-2] == b[j-1]) {
                dp[i][j] = std::min(dp[i][j],
                                    dp[i-2][j-2] + swap_weight);
            }
        }
    }
    
    return dp;
}

double cpp_compute_distance(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
) {
    auto dp = compute_dp_table(a, b, replace_weight, insert_weight, delete_weight, swap_weight);
    return dp[a.length()][b.length()];
}

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
) {
    if (i == 0 && j == 0) {
        std::vector<CppEditop> reversed_path = current_path;
        std::reverse(reversed_path.begin(), reversed_path.end());
        return {reversed_path};
    }
    
    std::vector<std::vector<CppEditop>> all_paths;
    double current_cost = dp[i][j];
    const double tol = 1e-6;

    if (i > 0 && std::abs((dp[i-1][j] + delete_weight) - current_cost) < tol) {
        CppEditop op(DELETE, i-1, i-1, delete_weight, std::string(1, a[i-1]));
        current_path.push_back(op);
        auto paths = backtrack_all_paths(a, b, dp, i-1, j, current_path, replace_weight, insert_weight, delete_weight, swap_weight);
        all_paths.insert(all_paths.end(), paths.begin(), paths.end());
        current_path.pop_back();
    }
    
    if (j > 0 && std::abs((dp[i][j-1] + insert_weight) - current_cost) < tol) {
        CppEditop op(INSERT, i, i, insert_weight, std::string(1, b[j-1]));
        current_path.push_back(op);
        auto paths = backtrack_all_paths(a, b, dp, i, j-1, current_path, replace_weight, insert_weight, delete_weight, swap_weight);
        all_paths.insert(all_paths.end(), paths.begin(), paths.end());
        current_path.pop_back();
    }
    
    if (i > 0 && j > 0) {
        double sub_cost = (a[i-1] == b[j-1]) ? 0.0 : replace_weight;
        if (std::abs((dp[i-1][j-1] + sub_cost) - current_cost) < tol) {
            std::string out_char = (sub_cost == 0.0) ? std::string(1, a[i-1]) : std::string(1, b[j-1]);
            CppEditop op(REPLACE, i-1, j-1, sub_cost, out_char);
            current_path.push_back(op);
            auto paths = backtrack_all_paths(a, b, dp, i-1, j-1, current_path, replace_weight, insert_weight, delete_weight, swap_weight);
            all_paths.insert(all_paths.end(), paths.begin(), paths.end());
            current_path.pop_back();
        }
    }
    
    if (i > 1 && j > 1 &&
        a[i-1] == b[j-2] && a[i-2] == b[j-1] &&
        std::abs((dp[i-2][j-2] + swap_weight) - current_cost) < tol) {
        std::string swap_str = std::string(1, b[j-2]) + std::string(1, b[j-1]);
        CppEditop op(SWAP, i-2, j-2, swap_weight, swap_str);
        current_path.push_back(op);
        auto paths = backtrack_all_paths(a, b, dp, i-2, j-2, current_path, replace_weight, insert_weight, delete_weight, swap_weight);
        all_paths.insert(all_paths.end(), paths.begin(), paths.end());
        current_path.pop_back();
    }
    
    return all_paths;
}

std::vector<std::vector<CppEditop>> cpp_compute_all_paths(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
) {
    auto dp = compute_dp_table(a, b, replace_weight, insert_weight, delete_weight, swap_weight);
    std::vector<CppEditop> current_path;
    return backtrack_all_paths(a, b, dp, a.length(), b.length(), current_path, replace_weight, insert_weight, delete_weight, swap_weight);
}

void cpp_print_all_paths(
    const std::string& a, 
    const std::string& b,
    double replace_weight,
    double insert_weight,
    double delete_weight,
    double swap_weight
) {
    auto paths = cpp_compute_all_paths(a, b, replace_weight, insert_weight, delete_weight, swap_weight);
    double distance = cpp_compute_distance(a, b, replace_weight, insert_weight, delete_weight, swap_weight);
    
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

std::string editop_name_to_string(CppEditopName name) {
    switch (name) {
        case INSERT: return "INSERT";
        case DELETE: return "DELETE";
        case REPLACE: return "REPLACE";
        case SWAP: return "SWAP";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const CppEditop& op) {
    os << "CppEditop(name=" << editop_name_to_string(op.name)
       << ", src_idx=" << op.src_idx
       << ", dst_idx=" << op.dst_idx
       << ", cost=" << op.cost
       << ", output_string='" << op.output_string << "')";
    return os;
}
