#include <tile_to_merc.hpp>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("test single tile")
{
    sse_example::tile t{1, 0, 1};
    auto bbox = box_from_tile(t);
    CHECK(bbox.min.x == Approx(-20037508.3427892439));
    CHECK(bbox.min.y == Approx(-20037508.3427892439));
    CHECK(bbox.max.x == Approx(0.0));
    CHECK(bbox.max.y == Approx(0.0));
}

TEST_CASE("test tile set")
{
    sse_example::tile_vector tiles;
    tiles.push_back({1, 0, 0});
    tiles.push_back({1, 0, 1});
    tiles.push_back({1, 1, 0});
    tiles.push_back({1, 1, 1});
    auto bboxs = box_from_tile(tiles);
    REQUIRE(bboxs.size() == 4);
    CHECK(bboxs[0].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[0].min.y == Approx(0.0));
    CHECK(bboxs[0].max.x == Approx(0.0));
    CHECK(bboxs[0].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[1].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[1].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[1].max.x == Approx(0.0));
    CHECK(bboxs[1].max.y == Approx(0.0));
    CHECK(bboxs[2].min.x == Approx(0.0));
    CHECK(bboxs[2].min.y == Approx(0.0));
    CHECK(bboxs[2].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[2].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[3].min.x == Approx(0.0));
    CHECK(bboxs[3].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[3].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[3].max.y == Approx(0.0));
}

TEST_CASE("test tile set float")
{
    sse_example::tile_vector tiles;
    tiles.push_back({1, 0, 0});
    tiles.push_back({1, 0, 1});
    tiles.push_back({1, 1, 0});
    tiles.push_back({1, 1, 1});
    auto bboxs = box_from_tile_float(tiles);
    REQUIRE(bboxs.size() == 4);
    CHECK(bboxs[0].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[0].min.y == Approx(0.0));
    CHECK(bboxs[0].max.x == Approx(0.0));
    CHECK(bboxs[0].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[1].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[1].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[1].max.x == Approx(0.0));
    CHECK(bboxs[1].max.y == Approx(0.0));
    CHECK(bboxs[2].min.x == Approx(0.0));
    CHECK(bboxs[2].min.y == Approx(0.0));
    CHECK(bboxs[2].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[2].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[3].min.x == Approx(0.0));
    CHECK(bboxs[3].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[3].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[3].max.y == Approx(0.0));
}

TEST_CASE("test tiles sse double")
{
    sse_example::tile_vector tiles;
    tiles.push_back({1, 0, 0});
    tiles.push_back({1, 0, 1});
    tiles.push_back({1, 1, 0});
    tiles.push_back({1, 1, 1});
    auto bboxs = box_from_tile_sse_double(tiles);
    REQUIRE(bboxs.size() == 4);
    CHECK(bboxs[0].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[0].min.y == Approx(0.0));
    CHECK(bboxs[0].max.x == Approx(0.0));
    CHECK(bboxs[0].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[1].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[1].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[1].max.x == Approx(0.0));
    CHECK(bboxs[1].max.y == Approx(0.0));
    CHECK(bboxs[2].min.x == Approx(0.0));
    CHECK(bboxs[2].min.y == Approx(0.0));
    CHECK(bboxs[2].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[2].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[3].min.x == Approx(0.0));
    CHECK(bboxs[3].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[3].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[3].max.y == Approx(0.0));
}

TEST_CASE("test tiles sse float")
{
    sse_example::tile_vector tiles;
    tiles.push_back({1, 0, 0});
    tiles.push_back({1, 0, 1});
    tiles.push_back({1, 1, 0});
    tiles.push_back({1, 1, 1});
    auto bboxs = box_from_tile_sse_float(tiles);
    REQUIRE(bboxs.size() == 4);
    CHECK(bboxs[0].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[0].min.y == Approx(0.0));
    CHECK(bboxs[0].max.x == Approx(0.0));
    CHECK(bboxs[0].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[1].min.x == Approx(-20037508.3427892439));
    CHECK(bboxs[1].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[1].max.x == Approx(0.0));
    CHECK(bboxs[1].max.y == Approx(0.0));
    CHECK(bboxs[2].min.x == Approx(0.0));
    CHECK(bboxs[2].min.y == Approx(0.0));
    CHECK(bboxs[2].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[2].max.y == Approx(20037508.3427892439));
    CHECK(bboxs[3].min.x == Approx(0.0));
    CHECK(bboxs[3].min.y == Approx(-20037508.3427892439));
    CHECK(bboxs[3].max.x == Approx(20037508.3427892439));
    CHECK(bboxs[3].max.y == Approx(0.0));
}
