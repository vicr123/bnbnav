package com.vicr123.bnbnav;

import kong.unirest.Unirest;
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
        unirest.delete("/players/{player}")
                .routeParam("player", quitEvent.getPlayer().getName())
                .asEmptyAsync();
    }
}
