package com.vicr123.bnbnav;

import org.bukkit.plugin.java.JavaPlugin;

public class BnbnavPlugin extends JavaPlugin {
    private static BnbnavPlugin plugin;

    public static BnbnavPlugin getPlugin() { // getter for the static plugin instance
        return plugin;
    }

    @Override
    public void onEnable() {
        plugin = getPlugin(BnbnavPlugin.class);
        getLogger().info("bnbnav is ready!");
    }

    @Override
    public void onDisable() {

    }
}
