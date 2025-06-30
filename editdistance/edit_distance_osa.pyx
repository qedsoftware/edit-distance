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
    
    vector[vector[CppEditop]] cpp_compute_all_paths(
        const string& a, const string& b,
        double replace_weight, double insert_weight, double delete_weight, double swap_weight)
    void cpp_print_all_paths(
        const string& a, const string& b,
        double replace_weight, double insert_weight, double delete_weight, double swap_weight)
    double cpp_compute_distance(
        const string& a, const string& b,
        double replace_weight, double insert_weight, double delete_weight, double swap_weight)


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


def get_all_paths(
    str a,
    str b,
    double replace_weight=1.0,
    double insert_weight=1.0,
    double delete_weight=1.0,
    double swap_weight=1.0
):
    cdef string cpp_a = a.encode("utf-8")
    cdef string cpp_b = b.encode("utf-8")
    cdef vector[vector[CppEditop]] cpp_paths = cpp_compute_all_paths(
        cpp_a, cpp_b, replace_weight, insert_weight, delete_weight, swap_weight)
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

def apply_editops(src, dst, editops):
    src_idx = 0
    s = ""
    for op in editops:
        while src_idx < op.src_idx:
            s += src[src_idx]
            src_idx += 1
        if op.name == EditopName.INSERT:
            s += dst[op.dst_idx]
        elif op.name == EditopName.DELETE:
            src_idx += 1
        elif op.name == EditopName.REPLACE:
            s += dst[op.dst_idx]
            src_idx += 1
        elif op.name == EditopName.SWAP:
            s += src[op.src_idx + 1]        
            s += src[op.src_idx]
            src_idx += 2
    while src_idx < len(src):
        s += src[src_idx]
        src_idx += 1
    return s


def print_all_paths(
    str a,
    str b,
    double replace_weight=1.0,
    double insert_weight=1.0,
    double delete_weight=1.0,
    double swap_weight=1.0
):
    cdef string cpp_a = a.encode("utf-8")
    cdef string cpp_b = b.encode("utf-8")
    cpp_print_all_paths(
        cpp_a, cpp_b, replace_weight, insert_weight, delete_weight, swap_weight)


def compute_distance(
    str a,
    str b,
    double replace_weight=1.0,
    double insert_weight=1.0,
    double delete_weight=1.0,
    double swap_weight=1.0
):
    cdef string cpp_a = a.encode("utf-8")
    cdef string cpp_b = b.encode("utf-8")
    return cpp_compute_distance(
        cpp_a, cpp_b, replace_weight, insert_weight, delete_weight, swap_weight)
