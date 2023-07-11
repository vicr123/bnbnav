package com.vicr123.bnbnav.commands;

import com.vicr123.bnbnav.JwtProvisioning;
import kong.unirest.UnirestInstance;
import org.bukkit.Location;
import org.bukkit.command.Command;
import org.bukkit.command.CommandExecutor;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;

public class AddRoadNodeCommand implements CommandExecutor {
    UnirestInstance unirest;

    public AddRoadNodeCommand(UnirestInstance unirest) {
        this.unirest = unirest;
    }

    @Override
    public boolean onCommand(CommandSender commandSender, Command command, String s, String[] strings) {
        if (!(commandSender instanceof Player)) {
            commandSender.sendMessage("addroadnode can only be used with a player.");
            return false;
        }

        Location location = ((Player) commandSender).getLocation();
        String body = String.format("{\"x\": %d, \"y\": %d, \"z\": %d, \"world\": \"%s\", \"player\": \"%s\", \"uuid\": \"%s\"}", location.getBlockX(), location.getBlockY(), location.getBlockZ(), location.getWorld().getName(), commandSender.getName(), ((Player) commandSender).getUniqueId());

        unirest.post("/nodes/add")
                .contentType("application/json")
                .header("Authorization", "Bearer " + JwtProvisioning.JwtFor(commandSender, true))
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
