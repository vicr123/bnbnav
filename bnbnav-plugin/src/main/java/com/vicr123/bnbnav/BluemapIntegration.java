package com.vicr123.bnbnav;

import com.google.gson.Gson;
import de.bluecolored.bluemap.api.BlueMapAPI;
import de.bluecolored.bluemap.api.markers.HtmlMarker;
import de.bluecolored.bluemap.api.markers.MarkerSet;
import kong.unirest.UnirestInstance;
import org.apache.commons.lang.StringEscapeUtils;
import org.bukkit.Bukkit;

import java.util.Arrays;

public class BluemapIntegration {

    private static class Label {
        private String name;
        private String type;
        private int x;
        private int y;
        private int z;
        private String id;
        private String world;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getType() {
            return type;
        }

        public void setType(String type) {
            this.type = type;
        }

        public int getX() {
            return x;
        }

        public void setX(int x) {
            this.x = x;
        }

        public int getY() {
            return y;
        }

        public void setY(int y) {
            this.y = y;
        }

        public int getZ() {
            return z;
        }

        public void setZ(int z) {
            this.z = z;
        }

        public String getId() {
            return id;
        }

        public void setId(String id) {
            this.id = id;
        }

        public String getWorld() {
            return world;
        }

        public void setWorld(String world) {
            this.world = world;
        }
    }

    final String CITY_LABEL = "<div style=\"font-size: 20pt; color: white; transform: translate(-50%%, -50%%);\">%s</div>";
    final String COUNTRY_LABEL = "<div style=\"font-size: 40pt; font-variant: small-caps; color: white; transform: translate(-50%%, -50%%);\">%s</div>";

    private final BnbnavPlugin plugin;
    private final UnirestInstance unirest;

    BluemapIntegration(BnbnavPlugin plugin, UnirestInstance unirest) {
        this.plugin = plugin;
        this.unirest = unirest;
        BlueMapAPI.onEnable(this::updateBluemap);
        BlueMapAPI.getInstance().ifPresent(this::updateBluemap);

        var interval = 20 * 60 * 5;
        Bukkit.getServer().getScheduler().scheduleSyncRepeatingTask(plugin, () -> BlueMapAPI.getInstance().ifPresent(this::updateBluemap), interval, interval);
    }

    void updateBluemap(BlueMapAPI api) {
        plugin.getLogger().info("bnbnav is updating bluemap");
        // Request the data from the server
        unirest.get("/data/labels").asStringAsync(response -> {
            if (!response.isSuccess()) {
                plugin.getLogger().warning("Unable to retrieve bnbnav data from the server");
                return;
            }

            var labels = new Gson().fromJson(response.getBody(), Label[].class);
            var worlds = Arrays.stream(labels).map(Label::getWorld).distinct();
            for (var world : worlds.toArray()) {
                var citySet = MarkerSet.builder().label("Cities").build();
                var countrySet = MarkerSet.builder().label("Countries").build();

                for (var label : labels) {
                    if (!label.getWorld().equals(world)) continue;

                    var cleanLabel = StringEscapeUtils.escapeHtml(label.getName());

                    var markerBuilder = HtmlMarker.builder()
                            .position(label.getX(), label.getY(), (double) label.getZ())
                            .label(label.getName());

                    if (label.getType().equals("label-city")) {
                        citySet.getMarkers().put(label.getId(), markerBuilder
                                .html(String.format(CITY_LABEL, cleanLabel))
                                .maxDistance(1000)
                                .build());
                    } else if (label.getType().equals("label-country")) {
                        countrySet.getMarkers().put(label.getId(), markerBuilder
                                .html(String.format(COUNTRY_LABEL, cleanLabel))
                                .minDistance(1000)
                                .build());
                    } else {
                        plugin.getLogger().warning("No handler for label \"%s\"".formatted(label.getType()));
                    }
                }

                api.getWorlds().stream().filter(w -> w.getSaveFolder().equals(world)).findFirst().ifPresentOrElse(bluemapWorld -> {
                    for (var map : bluemapWorld.getMaps()) {
                        map.getMarkerSets().put("bnbnav-country", countrySet);
                        map.getMarkerSets().put("bnbnav-city", citySet);
                    }
                }, () -> plugin.getLogger().warning("No world called \"%s\"".formatted(world)));

            }

        });
    }
}
