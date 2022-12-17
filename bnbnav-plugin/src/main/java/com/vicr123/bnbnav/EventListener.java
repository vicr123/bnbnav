package com.vicr123.bnbnav;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;
import github.scarsz.discordsrv.DiscordSRV;
import kong.unirest.UnirestInstance;
import org.bukkit.entity.Player;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.player.AsyncPlayerChatEvent;
import org.bukkit.event.player.PlayerJoinEvent;
import org.bukkit.event.player.PlayerQuitEvent;

public class EventListener implements Listener {
    UnirestInstance unirest;

    EventListener(UnirestInstance unirest) {
        this.unirest = unirest;
    }

    @EventHandler
    public void quitEvent(PlayerQuitEvent quitEvent) {
        var discordId = DiscordSRV.getPlugin().getAccountLinkManager().getDiscordId(quitEvent.getPlayer().getUniqueId());
        String body = String.format("{\"discordId\": %s}", discordId == null ? "null" : "\"" + discordId + "\"");

        unirest.delete("/player/{player}")
                .routeParam("player", quitEvent.getPlayer().getName())
                .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(null))
                .contentType("application/json")
                .body(body)
                .asEmptyAsync();
    }

    @EventHandler
    public void joinEvent(PlayerJoinEvent joinEvent) {
        var discordId = DiscordSRV.getPlugin().getAccountLinkManager().getDiscordId(joinEvent.getPlayer().getUniqueId());
        String body = String.format("{\"discordId\": %s}", discordId == null ? "null" : "\"" + discordId + "\"");

        unirest.post("/player/{player}/join")
                .routeParam("player", joinEvent.getPlayer().getName())
                .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(null))
                .contentType("application/json")
                .body(body)
                .asEmptyAsync();
    }

    @EventHandler
    public void chatEvent(AsyncPlayerChatEvent chatEvent) {
        var discordId = DiscordSRV.getPlugin().getAccountLinkManager().getDiscordId(chatEvent.getPlayer().getUniqueId());
        Gson gson = new Gson();

        var root = new JsonObject();
        if (discordId == null) {
            root.add("discordId", JsonNull.INSTANCE);
        } else {
            root.addProperty("discordId", discordId + "");
        }
        root.addProperty("message", chatEvent.getMessage());

        unirest.post("/chat/{player}")
                .routeParam("player", chatEvent.getPlayer().getName())
                .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(null))
                .contentType("application/json")
                .body(gson.toJson(root))
                .asEmptyAsync();
    }
}
