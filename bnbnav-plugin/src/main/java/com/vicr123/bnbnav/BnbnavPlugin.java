package com.vicr123.bnbnav;

import com.vicr123.bnbnav.commands.AddRoadNodeCommand;
import com.vicr123.bnbnav.commands.EditNavCommand;
import github.scarsz.discordsrv.DiscordSRV;
import kong.unirest.Unirest;
import kong.unirest.UnirestInstance;
import org.bukkit.Location;
import org.bukkit.entity.Player;
import org.bukkit.plugin.java.JavaPlugin;

public class BnbnavPlugin extends JavaPlugin {
    UnirestInstance unirest;
    BluemapIntegration bluemap;

    public static final String API_BASE = "http://localhost:5813/api";
    public static final int SERVER_API_VERSION = 2;

    @Override
    public void onEnable() {
        unirest = Unirest.spawnInstance();

        DiscordSRV.getPlugin().getAccountLinkManager();

        unirest.config()
                .defaultBaseUrl(API_BASE)
                .setDefaultHeader("X-Bnbnav-Api-Version", String.valueOf(SERVER_API_VERSION));

        getServer().getPluginManager().registerEvents(new EventListener(this, unirest), this);
        this.getCommand("addroadnode").setExecutor(new AddRoadNodeCommand(unirest));
        this.getCommand("editnav").setExecutor(new EditNavCommand());

        getServer().getScheduler().scheduleSyncRepeatingTask(this, this::detectPlayers, 0, 1);

        bluemap = new BluemapIntegration(this, unirest);

        getLogger().info("bnbnav is ready!");
    }

    void detectPlayers() {
        for (Player player : getServer().getOnlinePlayers()) {
            Location loc = player.getLocation();
            String body = "{\"id\": \"%s\", \"x\": %s, \"y\": %s, \"z\": %s, \"world\": \"%s\"}".formatted(player.getName(), loc.getX(), loc.getY(), loc.getZ(), loc.getWorld().getName());
            unirest.post("/player/{player}")
                    .routeParam("player", player.getName())
                    .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(null, true))
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
