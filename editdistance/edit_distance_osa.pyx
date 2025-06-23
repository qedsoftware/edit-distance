# distutils: language = c++
# distutils: sources = ./editdistance/_edit_distance_osa.cpp

from libcpp.map cimport map
from libcpp.string cimport string
from libcpp.vector cimport vector

from enum import Enum


cdef extern from "_edit_distance_osa.hpp":
    cdef enum CppEditopName:
        INSERT
        DELETE
        REPLACE
        SWAP
    
    cdef struct CppEditop:
        CppEditopName name
        int src_idx
        int dst_idx
        double cost
        string output_string
    
    vector[vector[CppEditop]] cpp_compute_all_paths(const string& a, const string& b, const map[CppEditopName, double]& cost_map)
    void cpp_print_all_paths(const string& a, const string& b, const map[CppEditopName, double]& cost_map)
    double cpp_compute_distance(const string& a, const string& b, const map[CppEditopName, double]& cost_map)


class EditopName(Enum):
    INSERT = 0
    DELETE = 1
    REPLACE = 2
    SWAP = 3


cdef class Editop:
    cdef readonly object name
    cdef readonly int src_idx
    cdef readonly int dst_idx
    cdef readonly double cost
    cdef readonly str output_string

    def __init__(self, name, src_idx, dst_idx, cost, output_string):
        self.name = name
        self.src_idx = src_idx
        self.dst_idx = dst_idx
        self.cost = cost
        self.output_string = output_string

    def __repr__(self):
        return f"Editop(name={self.name}, src_idx={self.src_idx}, dst_idx={self.dst_idx}, cost={self.cost}, output_string='{self.output_string}')"


cdef map[CppEditopName, double] _convert_cost_map(dict cost_map):
    cdef map[CppEditopName, double] cpp_cost_map
    if EditopName.INSERT in cost_map:
        cpp_cost_map[INSERT] = cost_map[EditopName.INSERT]
    if EditopName.DELETE in cost_map:
        cpp_cost_map[DELETE] = cost_map[EditopName.DELETE]
    if EditopName.REPLACE in cost_map:
        cpp_cost_map[REPLACE] = cost_map[EditopName.REPLACE]
    if EditopName.SWAP in cost_map:
        cpp_cost_map[SWAP] = cost_map[EditopName.SWAP]
    return cpp_cost_map


def get_all_paths(
    str a,
    str b,
    double replace_weight=1.0,
    double insert_weight=1.0,
    double delete_weight=1.0,
    double swap_weight=1.0
):
    cdef dict cost_map = {
        EditopName.REPLACE: replace_weight,
        EditopName.INSERT: insert_weight,
        EditopName.DELETE: delete_weight,
        EditopName.SWAP: swap_weight
    }
    cdef string cpp_a = a.encode("utf-8")
    cdef string cpp_b = b.encode("utf-8")
    cdef map[CppEditopName, double] cpp_cost_map = _convert_cost_map(cost_map)
    cdef vector[vector[CppEditop]] cpp_paths = cpp_compute_all_paths(cpp_a, cpp_b, cpp_cost_map)
    python_paths = []
    cdef vector[CppEditop] cpp_path
    cdef CppEditop cpp_op
    for cpp_path in cpp_paths:
        python_path = []
        for cpp_op in cpp_path:
            if cpp_op.cost == 0:
                continue
            if cpp_op.name == INSERT:
                py_name = EditopName.INSERT
            elif cpp_op.name == DELETE:
                py_name = EditopName.DELETE
            elif cpp_op.name == REPLACE:
                py_name = EditopName.REPLACE
            elif cpp_op.name == SWAP:
                py_name = EditopName.SWAP
            else:
                py_name = None
            python_path.append(Editop(
                py_name,
                cpp_op.src_idx,
                cpp_op.dst_idx,
                cpp_op.cost,
                cpp_op.output_string.decode("utf-8")
            ))
        python_paths.append(python_path)
    return python_paths


def print_all_paths(
    str a,
    str b,
    double replace_weight=1.0,
    double insert_weight=1.0,
    double delete_weight=1.0,
    double swap_weight=1.0
):
    cdef dict cost_map = {
        EditopName.REPLACE: replace_weight,
        EditopName.INSERT: insert_weight,
        EditopName.DELETE: delete_weight,
        EditopName.SWAP: swap_weight
    }
    cdef string cpp_a = a.encode("utf-8")
    cdef string cpp_b = b.encode("utf-8")
    cdef map[CppEditopName, double] cpp_cost_map = _convert_cost_map(cost_map)
    cpp_print_all_paths(cpp_a, cpp_b, cpp_cost_map)


def compute_distance(
    str a,
    str b,
    double replace_weight=1.0,
    double insert_weight=1.0,
    double delete_weight=1.0,
    double swap_weight=1.0
):
    cdef dict cost_map = {
        EditopName.REPLACE: replace_weight,
        EditopName.INSERT: insert_weight,
        EditopName.DELETE: delete_weight,
        EditopName.SWAP: swap_weight
    }
    cdef string cpp_a = a.encode("utf-8")
    cdef string cpp_b = b.encode("utf-8")
    cdef map[CppEditopName, double] cpp_cost_map = _convert_cost_map(cost_map)
    return cpp_compute_distance(cpp_a, cpp_b, cpp_cost_map)
