package com.vicr123.bnbnav.commands;

import com.vicr123.bnbnav.JwtProvisioning;
import net.md_5.bungee.api.ChatColor;
import net.md_5.bungee.api.chat.ClickEvent;
import net.md_5.bungee.api.chat.TextComponent;
import org.bukkit.command.Command;
import org.bukkit.command.CommandExecutor;
import org.bukkit.command.CommandSender;
import org.bukkit.entity.Player;

public class EditNavCommand implements CommandExecutor {
    @Override
    public boolean onCommand(CommandSender commandSender, Command command, String s, String[] strings) {
        if (!(commandSender instanceof Player)) {
            commandSender.sendMessage("editnav can only be used with a player.");
            return false;
        }

        if (!commandSender.hasPermission("bnbnav.edit")) {
            commandSender.sendMessage("You do not have permission to edit bnbnav.");
            return false;
        }

        TextComponent component = new TextComponent("Click to type your token in chat.");
        component.setColor(ChatColor.GREEN);
        component.setUnderlined(true);
        component.setClickEvent(new ClickEvent(ClickEvent.Action.SUGGEST_COMMAND, JwtProvisioning.JwtFor(commandSender, false)));
        commandSender.spigot().sendMessage(component);

        return false;
    }
}
