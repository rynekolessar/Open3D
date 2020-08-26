// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include <cstring>
#include <string>

#include "open3d/Macro.h"
#include "open3d/core/Dispatch.h"
#include "open3d/utility/Console.h"

namespace open3d {
namespace core {

class OPEN3D_API Dtype {
public:
    static const Dtype Undefined;
    static const Dtype Float32;
    static const Dtype Float64;
    static const Dtype Int32;
    static const Dtype Int64;
    static const Dtype UInt8;
    static const Dtype UInt16;
    static const Dtype Bool;

public:
    enum class DtypeCode {
        Undefined,
        Bool,  // Needed to distinguish bool from uint8_t.
        Int,
        UInt,
        Float,
        Object,
    };

    Dtype() : Dtype(DtypeCode::Undefined, 1, "Undefined") {}

    Dtype(DtypeCode dtype_code, int64_t byte_size, const std::string &name)
        : dtype_code_(dtype_code), byte_size_(byte_size) {
        (void)dtype_code_;
        (void)byte_size_;
        if (name.size() > 15) {
            utility::LogError("Name {} must be shorter.", name);
        } else {
            strcpy_s(name_, 16, name.c_str());
        }
    }

    /// Convert from C++ types to Dtype. Known types are explicitly specialized,
    /// e.g. FromType<float>(). Unsupported type results in an exception.
    template <typename T>
    static inline const Dtype FromType() {
        utility::LogError("Unsupported data type");
        return Dtype::Undefined;
    }

    int64_t ByteSize() const { return byte_size_; }

    std::string ToString() const { return name_; }

    bool operator==(const Dtype &other) const {
        return dtype_code_ == other.dtype_code_ &&
               byte_size_ == other.byte_size_ && name_ == other.name_;
    }

    bool operator!=(const Dtype &other) const { return !(*this == other); }

private:
    DtypeCode dtype_code_;
    int64_t byte_size_;
    char name_[16];  // Avoids MSVC warning on exporting std::string to DLL.
};

template <>
inline const Dtype Dtype::FromType<float>() {
    return Dtype::Float32;
}

template <>
inline const Dtype Dtype::FromType<double>() {
    return Dtype::Float64;
}

template <>
inline const Dtype Dtype::FromType<int32_t>() {
    return Dtype::Int32;
}

template <>
inline const Dtype Dtype::FromType<int64_t>() {
    return Dtype::Int64;
}

template <>
inline const Dtype Dtype::FromType<uint8_t>() {
    return Dtype::UInt8;
}

template <>
inline const Dtype Dtype::FromType<uint16_t>() {
    return Dtype::UInt16;
}

template <>
inline const Dtype Dtype::FromType<bool>() {
    return Dtype::Bool;
}

}  // namespace core
}  // namespace open3d
