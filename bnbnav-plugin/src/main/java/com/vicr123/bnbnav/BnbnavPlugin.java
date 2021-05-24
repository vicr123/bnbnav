package com.vicr123.bnbnav;

import com.vicr123.bnbnav.commands.AddRoadNodeCommand;
import kong.unirest.Unirest;
import kong.unirest.UnirestInstance;
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

        getLogger().info("bnbnav is ready!");
    }

    @Override
    public void onDisable() {
    unirest.shutDown();
    }
}
