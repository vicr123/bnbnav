package com.vicr123.bnbnav;

import com.vicr123.bnbnav.commands.AddRoadNodeCommand;
import kong.unirest.Unirest;
import kong.unirest.UnirestInstance;
import org.bukkit.Location;
import org.bukkit.entity.Player;
import org.bukkit.plugin.java.JavaPlugin;

public class BnbnavPlugin extends JavaPlugin {
    private static BnbnavPlugin plugin;

    UnirestInstance unirest;

    public static BnbnavPlugin getPlugin() { // getter for the static plugin instance
        return plugin;
    }

    public static final String API_BASE = "http://localhost:5813/api";
    public static final String AUTH_TOKEN = "%NJJ8r5Dv5s8^qhurgFAXSCJS!%7#BQ$";

    @Override
    public void onEnable() {
        plugin = getPlugin(BnbnavPlugin.class);

        unirest = Unirest.spawnInstance();

        unirest.config()
                .defaultBaseUrl(API_BASE)
                .setDefaultHeader("Authorization", "Bearer " + AUTH_TOKEN);

        this.getCommand("addroadnode").setExecutor(new AddRoadNodeCommand(unirest));

        getServer().getScheduler().scheduleSyncRepeatingTask(getPlugin(), this::detectPlayers, 20, 1);

        getLogger().info("bnbnav is ready!");
    }

    void detectPlayers() {
        for (Player player : getServer().getOnlinePlayers()) {
            Location loc = player.getLocation();
            String body = "{\"id\": \"" + player.getName() + "\", \"x\": " + loc.getX() + ", \"y\": " + loc.getY() + ", \"z\": " + loc.getZ() + "}";
            unirest.post("/player/" + player.getName())
                    .contentType("application/json")
                    .body(body)
                    .asEmptyAsync();
        }
    }

    @Override
    public void onDisable() {
        unirest.shutDown();
    }
}
