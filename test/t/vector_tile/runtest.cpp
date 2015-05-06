
#include <test.hpp>

#include <string>
#include <vector>

// Input data.vector.pbf is encoded according to
// https://github.com/mapbox/mapnik-vector-tile/blob/master/proto/vector_tile.proto

static std::vector<std::string> expected_layer_names = {
    "landuse", "waterway", "water", "aeroway", "barrier_line", "building",
    "landuse_overlay", "tunnel", "road", "bridge", "admin",
    "country_label_line", "country_label", "marine_label", "state_label",
    "place_label", "water_label", "area_label", "rail_station_label",
    "airport_label", "road_label", "waterway_label", "building_label"
};

namespace {

std::string get_name(mapbox::util::pbf layer) { // copy!
    while (layer.next(1)) { // required string name
        return layer.string();
    }
    return "";
}

} // anon namespace

TEST_CASE("reading vector tiles") {

    SECTION("iterate over message using next()") {
        std::string buffer = get_file_data("./test/t/vector_tile/data.vector.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        std::vector<std::string> layer_names;
        while (item.next()) {
            if (item.tag == 3) { // repeated message Layer
                mapbox::util::pbf layer { item.message() };
                while (layer.next()) {
                    switch (layer.tag) {
                        case 1: // required string name
                            layer_names.push_back(layer.string());
                            break;
                        default:
                            layer.skip();
                    }
                }
            } else {
                item.skip();
            }
        }

        REQUIRE(layer_names == expected_layer_names);
    }

    SECTION("iterate over message using next(type)") {
        std::string buffer = get_file_data("./test/t/vector_tile/data.vector.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        std::vector<std::string> layer_names;
        while (item.next(3)) { // repeated message Layer
            mapbox::util::pbf layermsg { item.message() };
            while (layermsg.next(1)) { // required string name
                layer_names.push_back(layermsg.string());
            }
        }

        REQUIRE(layer_names == expected_layer_names);
    }

    SECTION("iterate over features in road layer") {
        std::string buffer = get_file_data("./test/t/vector_tile/data.vector.pbf");

        mapbox::util::pbf item(buffer.data(), buffer.size());

        int n=0;
        while (item.next(3)) { // repeated message Layer
            mapbox::util::pbf layer { item.message() };
            std::string name = get_name(layer);
            if (name == "road") {
                while (layer.next(2)) { // repeated Feature
                    ++n;
                    mapbox::util::pbf feature { layer.message() };
                    while (feature.next()) {
                        switch (feature.tag) {
                            case 1: { // optional uint64 id
                                auto id = feature.varint<uint64_t>();
                                REQUIRE(id >= 1);
                                REQUIRE(id <= 504);
                                break;
                            }
                            case 3: { // optional GeomType
                                auto geom_type = feature.varint<uint32_t>();
                                REQUIRE(geom_type >= 1);
                                REQUIRE(geom_type <= 3);
                                break;
                            }
                            default:
                                feature.skip();
                        }
                    }
                }
            }
        }

        REQUIRE(n == 502);
    }

}

