package com.vicr123.bnbnav.commands;

import kong.unirest.Unirest;
import org.bukkit.Location;
import org.bukkit.command.Command;
import org.bukkit.command.CommandExecutor;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;

public class AddRoadNodeCommand implements CommandExecutor {
    @Override
    public boolean onCommand(CommandSender commandSender, Command command, String s, String[] strings) {
        if (!(commandSender instanceof Player)) {
            commandSender.sendMessage("addroadnode can only be used with a player.");
            return false;
        }

        Location location = ((Player) commandSender).getLocation();
        String body = "{\"x\": " + location.getBlockX() + ", \"y\": " + location.getBlockY()  +", \"z\": " + location.getBlockZ() + "}";

//        String body = "{}";
        Unirest.post("/nodes/add")
                .contentType("application/json")
                .body(body)
                .asStringAsync((response) -> {

                    if (response.isSuccess()) {
                        commandSender.sendMessage("A road node was added at (" + location.getBlockX() + ", " + location.getBlockY() + ", " + location.getBlockZ() + ")");
                    } else {
                        commandSender.sendMessage("The road node could not be added to the map.");
                    }
                });

        return false;
    }
}
