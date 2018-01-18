#include <mapbox/geometry/box.hpp>

// sse
#include <x86intrin.h>

// std
#include <vector>
#include <cmath>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

namespace sse_example {

constexpr double EARTH_RADIUS = 6378137.0;
constexpr double HALF_OF_EQUATOR = M_PI * EARTH_RADIUS;

struct tile
{
    std::int32_t z;
    std::int32_t x;
    std::int32_t y;
};

using tile_vector = std::vector<tile>;

template <typename T>
using box_vector = std::vector<mapbox::geometry::box<T>>;

mapbox::geometry::box<double> box_from_tile(tile const& t)
{

    const double tile_size = 2.0 * HALF_OF_EQUATOR / static_cast<float>(1ul << t.z);
    double minx = -HALF_OF_EQUATOR + static_cast<double>(t.x) * tile_size;
    double miny = HALF_OF_EQUATOR - (static_cast<double>(t.y) + 1.0) * tile_size;
    double maxx = -HALF_OF_EQUATOR + (static_cast<double>(t.x) + 1.0) * tile_size;
    double maxy = HALF_OF_EQUATOR - static_cast<double>(t.y) * tile_size;
    return {{minx, miny}, {maxx, maxy}};
}

mapbox::geometry::box<float> box_from_tile_float(tile const& t)
{

    const float tile_size = 2.0f * static_cast<float>(HALF_OF_EQUATOR) / static_cast<float>(1ul << t.z);
    float minx = -static_cast<float>(HALF_OF_EQUATOR) + static_cast<float>(t.x) * tile_size;
    float miny = static_cast<float>(HALF_OF_EQUATOR) - (static_cast<float>(t.y) + 1.0f) * tile_size;
    float maxx = -static_cast<float>(HALF_OF_EQUATOR) + (static_cast<float>(t.x) + 1.0f) * tile_size;
    float maxy = static_cast<float>(HALF_OF_EQUATOR) - static_cast<float>(t.y) * tile_size;
    return {{minx, miny}, {maxx, maxy}};
}

box_vector<double> box_from_tile(tile_vector const& tiles)
{

    box_vector<double> output;
    output.reserve(tiles.size());
    for (auto const& t : tiles)
    {
        output.push_back(box_from_tile(t));
    }
    return output;
}

box_vector<float> box_from_tile_float(tile_vector const& tiles)
{

    box_vector<float> output;
    output.reserve(tiles.size());
    for (auto const& t : tiles)
    {
        output.push_back(box_from_tile_float(t));
    }
    return output;
}

box_vector<double> box_from_tile_sse_double(tile_vector const& tiles)
{

    box_vector<double> output;
    const std::size_t t_size = tiles.size();
    output.reserve(t_size);

    const __m128d HALF_OF_EQUATOR_M128D = _mm_set1_pd(HALF_OF_EQUATOR);
    const __m128d EQUATOR_M128D = _mm_set1_pd(HALF_OF_EQUATOR * 2.0);

    std::size_t i = 0;
    for (; i + 1 < t_size; i += 2)
    {
        __m128d x = _mm_cvtepi32_pd(_mm_set_epi32(0, 0, tiles[i + 1].x, tiles[i].x));
        __m128d y = _mm_cvtepi32_pd(_mm_set_epi32(0, 0, tiles[i + 1].y, tiles[i].y));
        __m128d shifted_z = _mm_cvtepi32_pd(_mm_sllv_epi32(_mm_set1_epi32(1), _mm_set_epi32(0, 0, tiles[i + 1].z, tiles[i].z)));
        // If no AVX2 use this below
        //__m128d shifted_z = _mm_set_pd(static_cast<double>(1ul << tiles[i+1].z), static_cast<double>(1ul << tiles[i].z));

        __m128d tile_size = _mm_div_pd(EQUATOR_M128D, shifted_z);
        __m128d minx = _mm_sub_pd(_mm_mul_pd(x, tile_size), HALF_OF_EQUATOR_M128D);
        __m128d miny = _mm_sub_pd(HALF_OF_EQUATOR_M128D, _mm_mul_pd(_mm_add_pd(y, _mm_set1_pd(1.0)), tile_size));
        __m128d maxx = _mm_sub_pd(_mm_mul_pd(_mm_add_pd(x, _mm_set1_pd(1.0)), tile_size), HALF_OF_EQUATOR_M128D);
        __m128d maxy = _mm_sub_pd(HALF_OF_EQUATOR_M128D, _mm_mul_pd(y, tile_size));

        output.emplace_back(mapbox::geometry::point<double>(minx[0], miny[0]), mapbox::geometry::point<double>(maxx[0], maxy[0]));
        output.emplace_back(mapbox::geometry::point<double>(minx[1], miny[1]), mapbox::geometry::point<double>(maxx[1], maxy[1]));
    }
    for (; i < t_size; i++)
    {
        output.push_back(box_from_tile(tiles[i]));
    }
    return output;
}

box_vector<float> box_from_tile_sse_float(tile_vector const& tiles)
{

    box_vector<float> output;
    const std::size_t t_size = tiles.size();
    output.reserve(t_size);

    const __m128 HALF_OF_EQUATOR_M128 = _mm_set1_ps(static_cast<float>(HALF_OF_EQUATOR));
    const __m128 EQUATOR_M128 = _mm_set1_ps(static_cast<float>(HALF_OF_EQUATOR * 2.0));

    std::size_t i = 0;
    for (; i + 3 < t_size; i += 4)
    {
        __m128 x = _mm_cvtepi32_ps(_mm_set_epi32(tiles[i + 3].x, tiles[i + 2].x, tiles[i + 1].x, tiles[i].x));
        __m128 y = _mm_cvtepi32_ps(_mm_set_epi32(tiles[i + 3].y, tiles[i + 2].y, tiles[i + 1].y, tiles[i].y));
        __m128 shifted_z = _mm_cvtepi32_ps(_mm_sllv_epi32(_mm_set1_epi32(1), _mm_set_epi32(tiles[i + 3].z, tiles[i + 2].z, tiles[i + 1].z, tiles[i].z)));

        // If no AVX2 use below
        //__m128 shifted_z = _mm_set_ps(static_cast<float>(1ul << tiles[i+3].z), static_cast<float>(1ul << tiles[i+2].z), static_cast<float>(1ul << tiles[i+1].z), static_cast<float>(1ul << tiles[i].z));

        __m128 tile_size = _mm_div_ps(EQUATOR_M128, shifted_z);
        __m128 minx = _mm_sub_ps(_mm_mul_ps(x, tile_size), HALF_OF_EQUATOR_M128);
        __m128 miny = _mm_sub_ps(HALF_OF_EQUATOR_M128, _mm_mul_ps(_mm_add_ps(y, _mm_set1_ps(1.0)), tile_size));
        __m128 maxx = _mm_sub_ps(_mm_mul_ps(_mm_add_ps(x, _mm_set1_ps(1.0)), tile_size), HALF_OF_EQUATOR_M128);
        __m128 maxy = _mm_sub_ps(HALF_OF_EQUATOR_M128, _mm_mul_ps(y, tile_size));

        output.emplace_back(mapbox::geometry::point<float>(minx[0], miny[0]), mapbox::geometry::point<float>(maxx[0], maxy[0]));
        output.emplace_back(mapbox::geometry::point<float>(minx[1], miny[1]), mapbox::geometry::point<float>(maxx[1], maxy[1]));
        output.emplace_back(mapbox::geometry::point<float>(minx[2], miny[2]), mapbox::geometry::point<float>(maxx[2], maxy[2]));
        output.emplace_back(mapbox::geometry::point<float>(minx[3], miny[3]), mapbox::geometry::point<float>(maxx[3], maxy[3]));
    }
    for (; i < t_size; i++)
    {
        output.push_back(box_from_tile_float(tiles[i]));
    }
    return output;
}
}
