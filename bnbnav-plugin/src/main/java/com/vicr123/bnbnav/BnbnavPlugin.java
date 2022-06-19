package com.vicr123.bnbnav;

import com.vicr123.bnbnav.commands.AddRoadNodeCommand;
import com.vicr123.bnbnav.commands.EditNavCommand;
import kong.unirest.Unirest;
import kong.unirest.UnirestInstance;
import org.bukkit.Location;
import org.bukkit.entity.Player;
import org.bukkit.plugin.java.JavaPlugin;

public class BnbnavPlugin extends JavaPlugin {
    UnirestInstance unirest;

    public static final String API_BASE = "http://localhost:5813/api";

    @Override
    public void onEnable() {
        unirest = Unirest.spawnInstance();

        unirest.config()
                .defaultBaseUrl(API_BASE);

        getServer().getPluginManager().registerEvents(new EventListener(unirest), this);
        this.getCommand("addroadnode").setExecutor(new AddRoadNodeCommand(unirest));
        this.getCommand("editnav").setExecutor(new EditNavCommand());

        getServer().getScheduler().scheduleSyncRepeatingTask(this, this::detectPlayers, 0, 1);

        getLogger().info("bnbnav is ready!");
    }

    void detectPlayers() {
        for (Player player : getServer().getOnlinePlayers()) {
            Location loc = player.getLocation();
            String body = "{\"id\": \"" + player.getName() + "\", \"x\": " + loc.getX() + ", \"y\": " + loc.getY() + ", \"z\": " + loc.getZ() + "}";
            unirest.post("/player/{player}")
                    .routeParam("player", player.getName())
                    .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(null))
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
