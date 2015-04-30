
#include <test.hpp>

TEST_CASE("reading vector tiles") {

    std::string buffer = get_file_data("./test/t/vector_tile/14_8716_8015.vector.pbf");

    mapbox::util::pbf item(buffer.data(), buffer.size());

    std::vector<std::string> layer_names;
    while (item.next()) {
        // it's a layer according to
        // https://github.com/mapbox/mapnik-vector-tile/blob/master/proto/vector_tile.proto
        if (item.tag == 3) {
            mapbox::util::pbf layermsg { item.message() };
            while (layermsg.next()) {
                if (layermsg.tag == 1) {
                    layer_names.push_back(layermsg.string());
                } else {
                    layermsg.skip();
                }
            }
        } else {
            item.skip();
        }
    }

    std::vector<std::string> expected_layer_names = {
        "landuse", "waterway", "water", "aeroway", "barrier_line", "building",
        "landuse_overlay", "tunnel", "road", "bridge", "admin",
        "country_label_line", "country_label", "marine_label", "state_label",
        "place_label", "water_label", "area_label", "rail_station_label",
        "airport_label", "road_label", "waterway_label", "building_label"
    };

    REQUIRE(layer_names == expected_layer_names);
}

