package com.vicr123.bnbnav;

import kong.unirest.UnirestInstance;
import org.bukkit.event.EventHandler;
import org.bukkit.event.Listener;
import org.bukkit.event.player.PlayerQuitEvent;

public class EventListener implements Listener {
    UnirestInstance unirest;

    EventListener(UnirestInstance unirest) {
        this.unirest = unirest;
    }

    @EventHandler
    public void quitEvent(PlayerQuitEvent quitEvent) {
        unirest.delete("/player/{player}")
                .routeParam("player", quitEvent.getPlayer().getName())
                .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(null))
                .asEmptyAsync();
    }
}
