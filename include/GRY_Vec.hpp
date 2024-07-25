#pragma once
#include <cmath>

#ifndef uint
#define uint unsigned int
#endif

template<typename T, uint N>
struct GRY_Vec {
private:
    T data[N];
public:
    T& operator[](const uint i) { return data[i]; }
    const T& operator[](const uint i) const { return data[i]; }
};

template<typename T>
struct GRY_Vec<T,3> {
    /* UB */
    union {
        struct { T x, y, z; };
        struct { T r, g, b; };
        T data[3];
    };
    
    GRY_Vec() {}
    GRY_Vec(T x, T y, T z) {
        data[0] = x; data[1] = y; data[2] = z;
    }
    T& operator[](const uint i) { return data[i]; }
    const T& operator[](const uint i) const { return data[i]; }
};

template<typename T>
struct GRY_Vec<T,2> {
    /* UB */
    union {
        struct { T x, y; };
        T data[2];
    };
    
    GRY_Vec() {}
    GRY_Vec(T x, T y) {
        data[0] = x; data[1] = y;
    }
    T& operator[](const uint i) { return data[i]; }
    const T& operator[](const uint i) const { return data[i]; }
};

typedef GRY_Vec<float,3> Vec3f;
typedef GRY_Vec<int, 3> Vec3i;
typedef GRY_Vec<float, 2> Vec2f;
typedef GRY_Vec<int, 2> Vec2i;

template<typename T, uint N>
GRY_Vec<T,N> operator+(const GRY_Vec<T,N>& lhs, const GRY_Vec<T,N>& rhs) {
    GRY_Vec<T,N> ret;
    for (uint i = 0; i < N; i++) { ret[i] = lhs[i] + rhs[i]; }
    return ret;
}

template<typename T, uint N>
GRY_Vec<T,N> operator+=(GRY_Vec<T,N>& lhs, const GRY_Vec<T,N>& rhs) {
    for (uint i = 0; i < N; i++) { lhs[i] += rhs[i]; }
}

template<typename T, uint N>
GRY_Vec<T,N> operator-(const GRY_Vec<T,N>& lhs, const GRY_Vec<T,N>& rhs) {
    GRY_Vec<T,N> ret;
    for (uint i = 0; i < N; i++) { ret[i] = lhs[i] - rhs[i]; }
    return ret;
}

template<typename T, uint N>
GRY_Vec<T,N> operator-=(GRY_Vec<T,N>& lhs, const GRY_Vec<T,N>& rhs) {
    for (uint i = 0; i < N; i++) { lhs[i] -= rhs[i]; }
}

template<typename T, typename K, uint N>
GRY_Vec<T,N> operator*(const GRY_Vec<T,N>& vec, const K& scalar) {
    GRY_Vec<T,N> ret;
    for (uint i = 0; i < N; i++) { ret[i] = vec[i] * scalar; }
    return ret;
}

template<typename T, typename K, uint N>
void operator*=(GRY_Vec<T,N>& vec, const K& scalar) {
    for (uint i = 0; i < N; i++) { vec[i] *= scalar; }
}

template<typename T, uint N>
T GRY_VecLengthSq(const GRY_Vec<T,N>& vec) {
    T sum = 0;
    for (uint i = 0; i < N; i++) { sum += vec[i] * vec[i]; }
    return sum;
}

template<typename T, uint N>
T GRY_VecDistanceSq(const GRY_Vec<T,N>& lhs, const GRY_Vec<T,N>& rhs) {
    T sum = 0;
    for (uint i = 0; i < N; i++) {
        sum += (lhs[i] - rhs[i]) * (lhs[i] - rhs[i]);
    }
    return sum;
}

template<typename T, uint N>
T GRY_VecDot(const GRY_Vec<T,N>& lhs, const GRY_Vec<T,N>& rhs) {
    T sum = 0;
    for (uint i = 0; i < N; i++) { sum += lhs[i] * rhs[i]; }
    return sum;
}

template<typename T>
GRY_Vec<T,3> GRY_VecCross(const GRY_Vec<T,3>& lhs, const GRY_Vec<T,3>& rhs) {
    GRY_Vec<T,3> ret;
    ret[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    ret[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];
    return ret;
}

template<uint N>
GRY_Vec<float,N> GRY_VecNormalize(const GRY_Vec<float,N>& vec) {
    return vec * (1.0f / sqrtf(GRY_VecLengthSq(vec)));
}

#undef uint
